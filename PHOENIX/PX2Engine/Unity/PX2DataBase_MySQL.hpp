// PX2DataBase_MySQL.hpp

#ifndef PX2DATABASE_MYSQL_HPP
#define PX2DATABASE_MYSQL_HPP

//#define PX2USEDATABASE_MYSQL
#if defined PX2USEDATABASE_MYSQL

#include "PX2DataBase.hpp"

#if defined _WIN32 || defined(WIN32)
#include<winsock2.h>
#endif
#include <mysql.h>

namespace PX2
{
	
	class PX2_ENGINE_ITEM DatabaseMySQL : public DatabaseDriver
	{
	public:
		DatabaseMySQL(const std::string &host, const std::string &user,
			const std::string &passwd, const std::string &db, int port);
		virtual ~DatabaseMySQL();

		virtual bool BeginTransaction();
		virtual bool Rollback();
		virtual bool Commit();

		virtual uint64_t GetLastInsertedRowID();

		virtual std::string EscapeString(const std::string &s);
		virtual std::string EscapeBlob(const char* s, uint32_t length);

	protected:
		virtual bool InternalQuery(const std::string &query);
		virtual DBResult *InternalSelectQuery(const std::string &query);

		MYSQL mHandle;
	};

	class PX2_ENGINE_ITEM MySQLResult : public DBResult
	{
		friend class DatabaseMySQL;

	public:
		virtual int32_t GetDataInt(const std::string &s);
		virtual uint32_t GetDataUInt(const std::string &s);
		virtual int64_t GetDataLong(const std::string &s);
		virtual std::string GetDataString(const std::string &s);
		virtual const char* GetDataStream(const std::string &s, unsigned long &size);

		virtual DBResult *Advance();
		virtual bool IsEmpty();

	protected:
		MySQLResult(MYSQL_RES* res);
		virtual ~MySQLResult();

		typedef std::map<const std::string, uint32_t> listNames_t;
		listNames_t mListNames;

		MYSQL_RES* mHandle;
		MYSQL_ROW mRow;
	};

}

#endif

#endif