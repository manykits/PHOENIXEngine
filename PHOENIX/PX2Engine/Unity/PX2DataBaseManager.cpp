// PX2DataBaseManager.cpp

#include "PX2DataBaseManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
DataBaseManager::DataBaseManager()
{

}
//----------------------------------------------------------------------------
DataBaseManager::~DataBaseManager()
{
}
//----------------------------------------------------------------------------
void DataBaseManager::AddDatabase(DatabaseDriver *database)
{
	DatabaseDriver::DatabaseType dbt = database->GetDatabaseType();
	if (DatabaseDriver::DBT_MYSQL == dbt)
	{
		mDatabaseDriverMySQL = database;
	}
	else if (DatabaseDriver::DBT_SQLITE == dbt)
	{
		mDatabaseDriverSQLite = database;
	}
}
//----------------------------------------------------------------------------
DatabaseDriver *DataBaseManager::GetDatabase(DatabaseDriver::DatabaseType dbt)
{
	if (DatabaseDriver::DBT_MYSQL == dbt)
	{
		return mDatabaseDriverMySQL;
	}
	else if (DatabaseDriver::DBT_SQLITE == dbt)
	{
		return  mDatabaseDriverSQLite ;
	}

	return 0;
}
//----------------------------------------------------------------------------