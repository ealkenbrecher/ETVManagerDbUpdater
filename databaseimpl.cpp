#include "databaseimpl.h"
#include <QSettings>
#include <QString>
#include <QSqlDatabase>
#include <QApplication>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

DatabaseImpl::DatabaseImpl(void)
{
  mUser = "";
}

void DatabaseImpl::connect (void)
{
  QString path = QApplication::applicationDirPath();
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

  qDebug () << "dbUser: " << dbUser;

  mDb = QSqlDatabase::addDatabase(dbType, dbUser);
  mDb.setDatabaseName(dbName);
  mDb.setHostName(dbHost);
  mDb.setPort(dbPort);

  qDebug () << "dbName von DB: " << mDb.connectionName();

  if(!mDb.open())  // open it and keep it opened
  {
    QString errMsg = "Fehlerbeschreibung: \n";
    errMsg.append(mDb.lastError().text());
    QMessageBox::critical(0, QObject::tr("Datenbankfehler"), errMsg);
  }

  mUser = dbUser;
}

QString DatabaseImpl::getConnectionName ()
{
  return mUser;
}

DatabaseImpl::~DatabaseImpl(void)
{

}
