#include "database.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

Database::Database(QObject *parent)
    : QObject{parent}
{
   static bool initialized = false;
    if (!initialized) {
        initialized = true;
        database = QSqlDatabase::addDatabase("QSQLITE");
    } else {
        database = QSqlDatabase::database();
    }
}

bool Database::open(const QString &fileName)
{
    database.setDatabaseName(fileName);
    auto result = database.open();

    if (!result) {
        qCritical() << "Unable to open database" << database.databaseName() << database.lastError();
    }

    return result;
}

void Database::close()
{
    if (database.isOpen()) {
        database.close();
    }
}

QVector<QVariant> Database::execute(const QString &query) const
{
    QVector<QVariant> result;

    QSqlQuery sqlQuery = database.exec(query);

    while (sqlQuery.next()) {
        QVector<QVariant> row;
        for (int i = 0; i < sqlQuery.record().count(); i++) {
            row << sqlQuery.record().value(i);
        }
        result << QVariant(row);
    }

    return result;
}
