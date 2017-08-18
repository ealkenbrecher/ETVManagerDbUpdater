#include "databaseimpl.h"
#include <QSettings>
#include <QString>
#include <QSqlDatabase>
#include <QCoreApplication>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <iostream>

DatabaseImpl::DatabaseImpl(void)
{
  mUser = "";
}

QString DatabaseImpl::getCfgEntry (QString arg)
{
  QString path = QCoreApplication::applicationDirPath();
  path.append("/cfg/config.ini");

  QSettings settings (path, QSettings::IniFormat);
  QString retVal = settings.value(arg, "").toString();

  return retVal;
}

bool DatabaseImpl::connect (void)
{
  bool retVal = false;

  QString path = QCoreApplication::applicationDirPath();
  path.append("/cfg/config.ini");

  QSettings settings (path, QSettings::IniFormat);

  QString dbType ("");
  QString dbUser ("");
  QString dbName ("");
  QString dbHost ("");
  int dbPort = 0;

  dbType = settings.value("database/type", "").toString();
  dbUser = settings.value("database/user", "").toString();
  dbName = settings.value("database/name", "").toString();
  dbHost = settings.value("database/host", "").toString();
  dbPort = settings.value("database/port", "").toInt();

  mDb = QSqlDatabase::addDatabase(dbType, dbUser);
  mDb.setDatabaseName(dbName);
  mDb.setHostName(dbHost);
  mDb.setPort(dbPort);

  if(!mDb.open())  // open it and keep it opened
  {
    QString errMsg = "Fehlerbeschreibung: \n";
    errMsg.append(mDb.lastError().text());
  }
  else
  {
    std::cout << "********************************\n";
    std::cout << "Datenbank erfolgreich geoeffnet.\n";
    std::cout << "********************************\n";
    retVal = true;
  }

  mUser = dbUser;

  return retVal;
}

QString DatabaseImpl::getDbVersion (QString rConnName)
{
  std::cout << "**********************************\n";
  std::cout << "Pruefe Datenbankversion\n";

  //aktuelle Db-Version checken
  QSqlQuery query (QSqlDatabase::database(rConnName));
  query.prepare("SELECT dbVersion FROM DbInfo");
  QString dbVersion ("");

  if (query.exec())
  {
    dbVersion = query.value(1).toString();
  }
  else
  {
    std::cout << "Datenbankversion nicht hinterlegt.\n";
  }

  return dbVersion;
}

QString DatabaseImpl::getConnectionName ()
{
  return mUser;
}

bool DatabaseImpl::update ()
{
  QString currentDbVer = getDbVersion (getConnectionName());
  QString cfgDbVer ("") ;
  bool retVal = false;

  cfgDbVer = getCfgEntry("config/currentDbVersion");

  if ("" == cfgDbVer)
  {
    std::cout << "Configeintrag 'config/currentDbVersion' fehlt. Abbruch!\n";
  }
  else
  {
    if (cfgDbVer != currentDbVer)
    {
      QString msg ("");
      msg.append("Aktuelle Datenbankversion: ");
      msg.append(currentDbVer);
      msg.append(".\nZiel Datenbankversion: ");
      msg.append(cfgDbVer);
      msg.append("\n");
      std::cout << msg.toStdString();

      int maxTries = 0;

      bool ok = true;
      while (currentDbVer != cfgDbVer && maxTries < 10 && ok)
      {
        maxTries++;

        if ("" == currentDbVer)
        {
          ok = updateDevToDbVer_1_0 ();
        }
        currentDbVer = getDbVersion(getConnectionName());
      }
      retVal = ok;

      //max iterations reached, db update was not successful -> err Msg
      if (maxTries == 10)
      {
        QString err ("");
        QString tries = QString::number(maxTries);
        err.append("Datenbankupdate nach ");
        err.append(tries);
        err.append(" Iterationen nicht erfolgreich.\n");
        std::cout << err.toStdString();
      }
    }
    else
    {
      QString msg ("");
      msg.append("Aktuelle Datenbankversion entspricht Ziel Datenbankversion: ");
      msg.append(currentDbVer);
      msg.append("\n");
      std::cout << msg.toStdString();
      retVal = true;
    }
  }
  return retVal;
}

bool DatabaseImpl::updateDevToDbVer_1_0 ()
{
  QSqlQuery query (QSqlDatabase::database(getConnectionName ()));
  bool retVal = false;

  //retVal = query.exec ("CREATE TABLE IF NOT EXISTS dbInfo (id integer PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, dbVersion BLOB)");
  retVal = query.exec ("CREATE TABLE IF NOT EXISTS dbInfo (dbVersion BLOB)");

  if (retVal)
  {
    std::cout << "Tabelle zum Ablegen der aktuellen Datenbankversion angelegt.\n";
    query.prepare("UPDATE dbInfo SET dbVersion = :version");
    query.bindValue(":version", "1.0");
    query.exec();

    if (retVal)
    {
      std::cout << "Datenbankversion 1.0 in Datenbank gespeichert.\n";
    }
    else
      std::cout << "Datenbankversion konnte nicht in Datenbank abgelegt werden.\n";

  }
  else
  {
    std::cout << "Create Table fail\n";
    std::cout << query.lastError().databaseText().toStdString();
    std::cout << query.lastError().driverText().toStdString();
  }

  return retVal;
}

DatabaseImpl::~DatabaseImpl(void)
{

}
