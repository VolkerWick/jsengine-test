#include "databasefactory.h"
#include "database.h"

#include <QJSEngine>

QJSValue DatabaseFactory::createInstance()
{
    return jsEngine->newQObject(new Database);
}
