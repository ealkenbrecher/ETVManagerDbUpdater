#include <QCoreApplication>
#include "databaseimpl.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    DatabaseImpl database;
    database.connect();

    return a.exec();
}
