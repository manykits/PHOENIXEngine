// PX2DataBase.cpp

#include "PX2DataBase.hpp"
using namespace PX2;

#ifdef PX2USE_MYSQL__
#include "database_driver_mysql.h"
#endif
#ifdef PX2USE_SQLITE__
#include "database_driver_sqlite.h"
#endif

//----------------------------------------------------------------------------
DBResult::DBResult() 
{
}
//----------------------------------------------------------------------------
DBResult::~DBResult() 
{
}
//----------------------------------------------------------------------------
int32_t DBResult::GetDataInt(const std::string &s) 
{
	PX2_UNUSED(s);
	return 0;
}
//----------------------------------------------------------------------------
uint32_t DBResult::GetDataUInt(const std::string &s) 
{
	PX2_UNUSED(s);
	return 0; 
}
//----------------------------------------------------------------------------
int64_t DBResult::GetDataLong(const std::string &s)
{
	PX2_UNUSED(s);
	return 0;
}
//----------------------------------------------------------------------------
std::string DBResult::GetDataString(const std::string &s)
{ 
	PX2_UNUSED(s);
	return "''";
}
//----------------------------------------------------------------------------
const char* DBResult::GetDataStream(const std::string &s, unsigned long &size)
{ 
	PX2_UNUSED(s);
	PX2_UNUSED(size);
	return 0; 
}
//----------------------------------------------------------------------------
bool DBResult::IsEmpty()
{
	return true; 
}
//----------------------------------------------------------------------------
DBResult *DBResult::Advance() 
{
	return 0; 
}
//----------------------------------------------------------------------------
DatabaseDriver::DatabaseDriver() :
mIsConnected(false),
mIsSupportMultiLine(false),
mDatabaseType(DBT_MAX_TYPE)
{
}
//----------------------------------------------------------------------------
DatabaseDriver::~DatabaseDriver()
{
}
//----------------------------------------------------------------------------
bool DatabaseDriver::ExecuteQuery(DBQuery &query)
{
	return InternalQuery(query.str());
}
//----------------------------------------------------------------------------
bool DatabaseDriver::ExecuteQuery(const std::string &query)
{
	return InternalQuery(query);
}
//----------------------------------------------------------------------------
DBResult *DatabaseDriver::StoreQuery(const std::string &query)
{
	return InternalSelectQuery(query);
}
//----------------------------------------------------------------------------
DBResult *DatabaseDriver::StoreQuery(DBQuery &query)
{
	return StoreQuery(query.str());
}
//----------------------------------------------------------------------------
Mutex DBQuery::msDatabaseMutex;
//----------------------------------------------------------------------------
DBQuery::DBQuery()
{
	msDatabaseMutex.Enter();
}
//----------------------------------------------------------------------------
DBQuery::~DBQuery()
{
	msDatabaseMutex.Leave();
}
//----------------------------------------------------------------------------
DBInsert::DBInsert(DatabaseDriver* db)
{
	mDataDriver = db;
	mRows = 0;

	mIsSupportMultiLine = mDataDriver->IsSupportMultiLine();
}
//----------------------------------------------------------------------------
DBInsert::~DBInsert()
{
}
//----------------------------------------------------------------------------
void DBInsert::SetQuery(const std::string& query)
{
	mQuery = query;
	mBuf.str("");
	mRows = 0;
}
//----------------------------------------------------------------------------
bool DBInsert::AddRow(const std::string& row)
{
	if (mIsSupportMultiLine)
	{
		mRows++;
		size_t size = (size_t)mBuf.tellp();

		// adds new row to buffer
		if (size == 0)
		{
			mBuf << "(" << row << ")";
		}
		else if (size > 8192)
		{
			if (!Execute())
				return false;

			mBuf << "(" << row << ")";
		}
		else
		{
			mBuf << ",(" + row + ")";
		}

		return true;
	}
	else
	{
		// executes INSERT for current row
		return mDataDriver->ExecuteQuery(mQuery + "(" + row + ")");
	}
}
//----------------------------------------------------------------------------
bool DBInsert::AddRowAndReset(std::ostringstream& row)
{
	bool ret = AddRow(row.str());
	row.str("");
	return ret;
}
//----------------------------------------------------------------------------
bool DBInsert::Execute()
{
	if (mIsSupportMultiLine && mBuf.tellp() > 0)
	{
		if (mRows == 0)
		{
			//no rows to execute
			return true;
		}
		// executes buffer
		bool res = mDataDriver->ExecuteQuery(mQuery + mBuf.str());

		// Reset counters
		mRows = 0;
		mBuf.str("");
		return res;
	}
	else{
		// INSERTs were executed on-fly
		return true;
	}
}
//----------------------------------------------------------------------------