#ifndef DATABASEIMPL_H
#define DATABASEIMPL_H

#include <QString>
#include <QSqlDatabase>

class DatabaseImpl
{
public:
  DatabaseImpl(void);
  bool connect (void);
  QString getConnectionName ();
  bool update ();
  ~DatabaseImpl(void);

protected:

private:
  QString getCfgEntry (QString arg);
  QString getDbVersion (QString rConnName);
  bool updateDevToDbVer_1_0 ();
  QString mUser;
  QSqlDatabase mDb;
};

#endif // DATABASEIMPL_H
