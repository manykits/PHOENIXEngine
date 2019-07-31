// PX2DataBase_SQLite.cpp

#include "PX2DataBase_SQLite.hpp"
#include "PX2Log.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;
#include <sqlite3.h>

#if SQLITE_VERSION_NUMBER < 3003009
#define OTS_SQLITE3_PREPARE sqlite3_prepare
#else
#define OTS_SQLITE3_PREPARE sqlite3_prepare_v2
#endif

//----------------------------------------------------------------------------
DatabaseSQLite::DatabaseSQLite(const std::string &db)
{
	mIsSupportMultiLine = false;
	mIsConnected = false;
	mDatabaseType = DatabaseDriver::DBT_SQLITE;

	std::fstream fin(db.c_str(), std::ios::in | std::ios::binary);
	if (fin.fail())
	{
		PX2_LOG_ERROR("Failed to initialize %s SQLite connection.", db.c_str());
		return;
	}
	fin.close();

	if (sqlite3_open(db.c_str(), &mHandle) != SQLITE_OK)
	{
		PX2_LOG_ERROR("Failed to initialize SQLite connection.");

		sqlite3_close(mHandle);
	}
	else
	{
		mIsConnected = true;
	}
}
//----------------------------------------------------------------------------
DatabaseSQLite::~DatabaseSQLite()
{
	sqlite3_close(mHandle);
}
//----------------------------------------------------------------------------
bool DatabaseSQLite::BeginTransaction()
{
	return ExecuteQuery("BEGIN");
}
//----------------------------------------------------------------------------
bool DatabaseSQLite::Rollback()
{
	return ExecuteQuery("ROLLBACK");
}
//----------------------------------------------------------------------------
bool DatabaseSQLite::Commit()
{
	return ExecuteQuery("COMMIT");
}
//----------------------------------------------------------------------------
std::string DatabaseSQLite::_parse(const std::string &s)
{
	std::string query = "";

	query.reserve(s.size());
	bool inString = false;
	uint8_t ch;
	for (uint32_t a = 0; a < s.length(); a++)
	{
		ch = s[a];

		if (ch == '\'')
		{
			if (inString && s[a + 1] != '\'')
				inString = false;
			else
				inString = true;
		}

		if (ch == '`' && !inString)
			ch = '"';

		query += ch;
	}

	return query;
}
//----------------------------------------------------------------------------
bool DatabaseSQLite::InternalQuery(const std::string &query)
{
	ScopedCS cs(&mSqliteMutex);

	if (!mIsConnected)
		return false;

#ifdef __DEBUG_SQL__
	PX2_LOG_ERROR("SQLITE QUERY:%s", query.c_str());
#endif

	std::string buf = _parse(query);
	sqlite3_stmt* stmt;
	// prepares statement
	if (OTS_SQLITE3_PREPARE(mHandle, buf.c_str(), (int)buf.length(), 
		&stmt, NULL) != SQLITE_OK)
	{
		sqlite3_finalize(stmt);
		PX2_LOG_ERROR("OTS_SQLITE3_PREPARE(): SQLITE ERROR: %s,(%s)",
			sqlite3_errmsg(mHandle), buf.c_str());
		return false;
	}

	// executes it once
	int ret = sqlite3_step(stmt);
	if (ret != SQLITE_OK && ret != SQLITE_DONE && ret != SQLITE_ROW)
	{
		sqlite3_finalize(stmt);
		PX2_LOG_ERROR("sqlite3_step(): SQLITE ERROR:%s, %s",
			sqlite3_errmsg(mHandle), buf.c_str());

		return false;
	}

	// closes statement
	// at all not sure if it should be debugged - query was executed correctly...
	sqlite3_finalize(stmt);

	return true;
}
//----------------------------------------------------------------------------
DBResult *DatabaseSQLite::InternalSelectQuery(const std::string &query)
{
	ScopedCS cs(&mSqliteMutex);

	if (!mIsConnected)
		return 0;

#ifdef __DEBUG_SQL__
	PX2_LOG_ERROR("SQLITE QUERY:%s", query.c_str());
#endif

	std::string buf = _parse(query);

	sqlite3_stmt* stmt;
	// prepares statement
	if (OTS_SQLITE3_PREPARE(mHandle, buf.c_str(), (int)buf.length(), 
		&stmt, NULL) != SQLITE_OK)
	{
		sqlite3_finalize(stmt);

		PX2_LOG_ERROR("OTS_SQLITE3_PREPARE(): SQLITE ERROR:%s (%s)",
			sqlite3_errmsg(mHandle), buf.c_str());

		return 0;
	}

	DBResult *ret = new0 SQLiteResult(stmt);
	if (ret->Advance())
	{
		return ret;
	}
	delete0(ret);

	return 0;
}
//----------------------------------------------------------------------------
uint64_t DatabaseSQLite::GetLastInsertedRowID()
{
	return (uint64_t)sqlite3_last_insert_rowid(mHandle);
}
//----------------------------------------------------------------------------
std::string DatabaseSQLite::EscapeString(const std::string &s)
{
	if (s.empty())
		return std::string("''");

	char* output = new char[s.length() * 2 + 3];
	sqlite3_snprintf((int)s.length() * 2 + 3, output, "%Q", s.c_str());
	std::string r(output);
	delete[] output;

	return r;
}
//----------------------------------------------------------------------------
std::string DatabaseSQLite::EscapeBlob(const char* s, uint32_t length)
{
	std::string buf = "x'";

	char* hex = new char[2 + 1]; //need one extra byte for null-character

	for (uint32_t i = 0; i < length; i++)
	{
		sprintf(hex, "%02x", ((unsigned char)s[i]));
		buf += hex;
	}

	delete[] hex;

	buf += "'";
	return buf;
}
//----------------------------------------------------------------------------
int32_t SQLiteResult::GetDataInt(const std::string &s)
{
	listNames_t::iterator it = mListNames.find(s);
	if (it != mListNames.end())
		return sqlite3_column_int(mHandle, it->second);

	std::cout << "Error during GetDataInt(" << s << ")." << std::endl;
	return 0; // Failed
}
//----------------------------------------------------------------------------
uint32_t SQLiteResult::GetDataUInt(const std::string &s)
{
	listNames_t::iterator it = mListNames.find(s);
	if (it != mListNames.end())
		return (uint32_t)sqlite3_column_int64(mHandle, it->second);

	PX2_LOG_ERROR("Error during GetDataInt(%s)", s.c_str());

	return 0; // Failed
}
//----------------------------------------------------------------------------
int64_t SQLiteResult::GetDataLong(const std::string &s)
{
	listNames_t::iterator it = mListNames.find(s);
	if (it != mListNames.end())
		return sqlite3_column_int64(mHandle, it->second);

	PX2_LOG_ERROR("Error during GetDataLong(%s)", s.c_str());

	return 0; // Failed
}
//----------------------------------------------------------------------------
std::string SQLiteResult::GetDataString(const std::string &s)
{
	listNames_t::iterator it = mListNames.find(s);
	if (it != mListNames.end())
	{
		std::string value = (const char*)sqlite3_column_text(mHandle,
			it->second);
		return value;
	}

	PX2_LOG_ERROR("Error during GetDataLong(%s)", s.c_str());

	return std::string("");
}
//----------------------------------------------------------------------------
const char* SQLiteResult::GetDataStream(const std::string &s,
	unsigned long &size)
{
	listNames_t::iterator it = mListNames.find(s);
	if (it != mListNames.end()){
		const char* value = (const char*)sqlite3_column_blob(mHandle, it->second);
		size = sqlite3_column_bytes(mHandle, it->second);
		return value;
	}

	PX2_LOG_ERROR("Error during GetDataStream(%s)", s.c_str());

	return 0;
}
//----------------------------------------------------------------------------
DBResult *SQLiteResult::Advance()
{
	mIsRowAvailable = (sqlite3_step(mHandle) == SQLITE_ROW);
	return mIsRowAvailable ? this : 0;
}
//----------------------------------------------------------------------------
bool SQLiteResult::IsEmpty()
{
	return !mIsRowAvailable;
}
//----------------------------------------------------------------------------
SQLiteResult::SQLiteResult(sqlite3_stmt* stmt)
{
	mHandle = stmt;
	mIsRowAvailable = false;
	mListNames.clear();

	int32_t fields = sqlite3_column_count(mHandle);
	for (int32_t i = 0; i < fields; i++)
	{
		mListNames[sqlite3_column_name(mHandle, i)] = i;
	}
}
//----------------------------------------------------------------------------
SQLiteResult::~SQLiteResult()
{
	sqlite3_finalize(mHandle);
}
//----------------------------------------------------------------------------