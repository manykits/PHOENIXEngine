// PX2DataBase.hpp

#ifndef PX2DATABSE_HPP
#define PX2DATABSE_HPP

#include "PX2UnityPre.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2Mutex.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class DatabaseDriver;
	class DBQuery;

	class PX2_ENGINE_ITEM DBResult
	{
	public:
		DBResult();
		virtual ~DBResult();

		virtual int32_t GetDataInt(const std::string &s);
		virtual uint32_t GetDataUInt(const std::string &s);
		virtual int64_t GetDataLong(const std::string &s);
		virtual std::string GetDataString(const std::string &s);
		virtual const char* GetDataStream(const std::string &s, unsigned long &size);
		virtual bool IsEmpty();
		virtual DBResult *Advance();
	};
	typedef Pointer0<DBResult> DBResultPtr;

	class PX2_ENGINE_ITEM DatabaseDriver
	{
	public:
		DatabaseDriver();
		virtual ~DatabaseDriver();

		enum DatabaseType
		{
			DBT_MYSQL,
			DBT_SQLITE,
			DBT_MAX_TYPE
		};
		DatabaseType GetDatabaseType() const;

		bool IsConnected() const;
		bool IsSupportMultiLine() const;

	protected:
		friend class DBTransaction;

		virtual bool BeginTransaction() = 0;
		virtual bool Rollback() = 0;
		virtual bool Commit() = 0;

	public:
		bool ExecuteQuery(const std::string &query);
		bool ExecuteQuery(DBQuery &query);
		virtual uint64_t GetLastInsertedRowID() = 0;
		DBResult *StoreQuery(const std::string &query);
		DBResult *StoreQuery(DBQuery &query);
		virtual std::string EscapeString(const std::string &s) = 0;
		virtual std::string EscapeBlob(const char* s, uint32_t length) = 0;

	protected:
		virtual bool InternalQuery(const std::string &query) = 0;
		virtual DBResult *InternalSelectQuery(const std::string &query) = 0;

		bool mIsConnected;
		bool mIsSupportMultiLine;
		DatabaseType mDatabaseType;
	};
	typedef Pointer0<DatabaseDriver> DatabaseDriverPtr;

	class PX2_ENGINE_ITEM DBQuery : public std::ostringstream
	{
		friend class DatabaseDriver;

	public:
		DBQuery();
		~DBQuery();

		void Reset() { str(""); }

	protected:
		static Mutex msDatabaseMutex;
	};
	typedef Pointer0<DBQuery> DBQueryPtr;

	class PX2_ENGINE_ITEM DBInsert
	{
	public:
		DBInsert(DatabaseDriver* db);
		~DBInsert();

		void SetQuery(const std::string& query);
		bool AddRow(const std::string& row);
		bool AddRowAndReset(std::ostringstream& row);
		bool Execute();
		uint64_t GetInsertID();

	protected:
		DatabaseDriver* mDataDriver;
		bool mIsSupportMultiLine;
		uint32_t mRows;
		std::string mQuery;
		std::ostringstream mBuf;
	};
	typedef Pointer0<DBInsert> DBInsertPtr;

	class PX2_ENGINE_ITEM DBTransaction
	{
	public:
		DBTransaction(DatabaseDriver* database)
		{
			mDatabase = database;
			mState = STATE_NO_START;
		}

		~DBTransaction()
		{
			if (mState == STATE_START){
				mDatabase->Rollback();
			}
		}

		bool Begin()
		{
			mState = STATE_START;
			return mDatabase->BeginTransaction();
		}

		bool Commit()
		{
			if (mState == STATE_START)
			{
				mState = STEATE_COMMIT;
				return mDatabase->Commit();
			}
			else
			{
				return false;
			}
		}

	private:
		enum TransactionStates_t{
			STATE_NO_START,
			STATE_START,
			STEATE_COMMIT
		};
		TransactionStates_t mState;
		DatabaseDriver* mDatabase;
	};

#include "PX2DataBase.inl"

}

#endif