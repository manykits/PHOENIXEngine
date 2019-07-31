// PX2DataBaseManager.hpp

#ifndef PX2DATABSEMANAGER_HPP
#define PX2DATABSEMANAGER_HPP

#include "PX2DataBase_MySQL.hpp"
#include "PX2DataBase_SQLite.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2DataBase.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM DataBaseManager : public Singleton<DataBaseManager>
	{
	public:
		DataBaseManager();
		virtual ~DataBaseManager();

		void AddDatabase(DatabaseDriver *database);
		DatabaseDriver *GetDatabase(DatabaseDriver::DatabaseType dbt);

	protected:
		DatabaseDriverPtr mDatabaseDriverMySQL;
		DatabaseDriverPtr mDatabaseDriverSQLite;
	};

#define PX2_DATABASE DataBaseManager::GetSingleton()
#define PX2_DATABASE_MYSQL DataBaseManager::GetSingleton().GetDatabase(DatabaseDriver::DBT_MYSQL)
#define PX2_DATABASE_SQLITE DataBaseManager::GetSingleton().GetDatabase(DatabaseDriver::DBT_SQLITE)

}

#endif