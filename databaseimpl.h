#ifndef DATABASEIMPL_H
#define DATABASEIMPL_H

#include <QString>
#include <QSqlDatabase>

class DatabaseImpl
{
public:
  DatabaseImpl(void);
  void connect (void);
  QString getConnectionName ();
  ~DatabaseImpl(void);

protected:

private:
  QString mUser;
  QSqlDatabase mDb;
};

#endif // DATABASEIMPL_H
