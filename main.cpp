#include <QCoreApplication>
#include "databaseimpl.h"
#include <QString>
#include <QSqlQuery>
#include <QVariant>
#include <iostream>
#include <QSettings>

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  DatabaseImpl database;

  if (true == database.connect())
  {
    database.update();
  }
  return a.exec();
  //return 0;
}
