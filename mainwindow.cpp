#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "databasefactory.h"

#include <QDateTime>
#include <QPushButton>
#include <QSettings>
#include <QJSEngine>
#include <QShortcut>

const char* windowGeometry = "window/geometry";
const char* javascriptText = "javascript/text";

static QDebug operator<<(QDebug d, const QJSValue& value) {
    return d << value.toString();
}

static QPlainTextEdit* consoleOutput = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , jsEngine(new QJSEngine(this))
{
    ui->setupUi(this);
    setWindowTitle(QApplication::applicationName());
    setWindowFlag(Qt::WindowStaysOnTopHint);

    ::consoleOutput = ui->consoleOutput;
    qInstallMessageHandler(&MainWindow::customMessageHandler);

    qInfo() << "Settings file:" << QSettings().fileName();

    jsEngine->installExtensions(QJSEngine::AllExtensions);
    auto databaseFactory = jsEngine->newQObject(new DatabaseFactory(jsEngine));
    jsEngine->globalObject().setProperty("_databaseFactory", databaseFactory);

    jsEngine->evaluate(R"(function Database() { return _databaseFactory.createInstance();})");

    QSettings settings;
    if (settings.contains(windowGeometry)) {
        setGeometry(settings.value(windowGeometry).toRect());
    }

    if (settings.contains(javascriptText)) {
        ui->scriptEdit->setPlainText(settings.value(javascriptText).toString());
    }

    auto runButton = new QPushButton(tr("run"), this);
    connect(runButton, &QPushButton::pressed, ui->consoleOutput, &QPlainTextEdit::clear);

    connect(runButton, &QPushButton::pressed, this, [this]() {
        QString script = ui->scriptEdit->toPlainText();
        QSettings().setValue(javascriptText, script);

        QJSValue result = jsEngine->evaluate(script);

        if (result.isError()) {
            qCritical() << "Error" << result;
        }
    });

    runButton->setShortcut(QKeySequence(Qt::Key_F5));
    ui->statusbar->addWidget(runButton);

    auto lineDeleteShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_K), this);
    connect(lineDeleteShortcut, &QShortcut::activated, this, [this]() {
        auto cursor = ui->scriptEdit->textCursor();
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.removeSelectedText();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!::consoleOutput) {
        qInstallMessageHandler(nullptr);
        qCritical() << "Unable to use console output. Restoring message handler";
        return;
    }

    QString s;
    QTextStream(&s) << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << " " << msg;

    ::consoleOutput->appendPlainText(s);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings;
    settings.setValue(windowGeometry, geometry());
    settings.setValue(javascriptText, ui->scriptEdit->toPlainText());

    QMainWindow::closeEvent(event);
}
