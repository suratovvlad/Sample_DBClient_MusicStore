#ifndef SQLQUERYMANAGER_H
#define SQLQUERYMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class SqlQueryManager : public QObject
{
	Q_OBJECT
private:
	QSqlQuery* m_sqlQuery;

public:
	explicit SqlQueryManager(const QSqlDatabase& database, QObject *parent = 0);
	virtual ~SqlQueryManager();

signals:

public slots:

};

#endif // SQLQUERYMANAGER_H
