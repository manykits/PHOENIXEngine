// PX2DataBase_SQLite.hpp

#ifndef PX2DATABASE_SQLITE_HPP
#define PX2DATABASE_SQLITE_HPP

#include "PX2DataBase.hpp"
#include "PX2Mutex.hpp"

struct sqlite3;
struct sqlite3_stmt;

namespace PX2
{

	class PX2_ENGINE_ITEM DatabaseSQLite : public DatabaseDriver
	{
	public:
		DatabaseSQLite(const std::string &db);
		virtual ~DatabaseSQLite();

		virtual bool BeginTransaction();
		virtual bool Rollback();
		virtual bool Commit();

		virtual uint64_t GetLastInsertedRowID();

		virtual std::string EscapeString(const std::string &s);
		virtual std::string EscapeBlob(const char* s, uint32_t length);


	protected:
		virtual bool InternalQuery(const std::string &query);
		virtual DBResult *InternalSelectQuery(const std::string &query);

		std::string _parse(const std::string &s);

		Mutex mSqliteMutex;
		sqlite3* mHandle;
	};

	class PX2_ENGINE_ITEM SQLiteResult : public DBResult
	{
		friend class DatabaseSQLite;

	public:
		virtual int32_t GetDataInt(const std::string &s);
		virtual uint32_t GetDataUInt(const std::string &s);
		virtual int64_t GetDataLong(const std::string &s);
		virtual std::string GetDataString(const std::string &s);
		virtual const char* GetDataStream(const std::string &s, unsigned long &size);

		virtual DBResult *Advance();
		virtual bool IsEmpty();

	protected:
		SQLiteResult(sqlite3_stmt* stmt);
		virtual ~SQLiteResult();

		typedef std::map<const std::string, uint32_t> listNames_t;
		listNames_t mListNames;

		bool mIsRowAvailable;
		sqlite3_stmt* mHandle;
	};


}

#endif