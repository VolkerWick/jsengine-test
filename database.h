#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);

    Q_INVOKABLE bool open(const QString& fileName);
    Q_INVOKABLE void close();
    Q_INVOKABLE bool isOpen() const { return database.isOpen(); }
    Q_INVOKABLE bool isValid() const { return database.isValid(); }

    Q_INVOKABLE QVector<QVariant> execute(const QString& query) const;
    Q_INVOKABLE QSqlError lastError() const { return database.lastError(); }

signals:

private:
    QSqlDatabase database;
};

#endif // DATABASE_H
