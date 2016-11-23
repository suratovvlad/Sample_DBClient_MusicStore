#include "SqlQueryManager.h"

SqlQueryManager::SqlQueryManager(const QSqlDatabase& database, QObject *parent) :
	QObject(parent)
{
	m_sqlQuery = new QSqlQuery(database);
}

SqlQueryManager::~SqlQueryManager()
{
}
