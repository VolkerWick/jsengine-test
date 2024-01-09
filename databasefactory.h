#ifndef DATABASEFACTORY_H
#define DATABASEFACTORY_H

#include <QObject>
#include <QJSValue>

// https://stackoverflow.com/a/37575936/5359437

class QJSEngine;

class DatabaseFactory : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseFactory(QJSEngine* jsEngine) : jsEngine(jsEngine) {}
    Q_INVOKABLE QJSValue createInstance();

signals:

private:
    QJSEngine* jsEngine;
};

#endif // DATABASEFACTORY_H
