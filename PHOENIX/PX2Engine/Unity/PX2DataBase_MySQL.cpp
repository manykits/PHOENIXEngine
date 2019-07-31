// PX2DataBase_MySQL.cpp

#include "PX2DataBase_MySQL.hpp"

#if defined PX2USEDATABASE_MYSQL
#include "PX2Log.hpp"
#include "errmsg.h"
using namespace PX2;

//----------------------------------------------------------------------------
DatabaseMySQL::DatabaseMySQL(const std::string &host, const std::string &user,
	const std::string &passwd, const std::string &db, int port) 
{
	mIsSupportMultiLine = true;
	mIsConnected = false;

	// connection handle initialization
	if (!mysql_init(&mHandle))
	{
		PX2_LOG_ERROR("Failed to initialize MySQL connection handle.");
		return;
	}

	// automatic reconnect
	my_bool reconnect = true;
	mysql_options(&mHandle, MYSQL_OPT_RECONNECT, &reconnect);

	unsigned int timeout = 5;
	int ret = mysql_options(&mHandle, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&timeout);
	if (0 != ret)
	{
		PX2_LOG_ERROR("set options MYSQL_OPT_CONNECT_TIMEOUT error");
	}

	const char *info = mysql_info(&mHandle);
	PX2_LOG_ERROR("Info %s", info);

	// connects to database
	if (!mysql_real_connect(&mHandle, host.c_str(), user.c_str(),
		passwd.c_str(), db.c_str(), port, 0, 0))
	{
		PX2_LOG_ERROR("Failed to connect to database. MYSQL ERROR: %s", mysql_error(&mHandle));
		return;
	}

	if (MYSQL_VERSION_ID < 50019)
	{
		// mySQL servers < 5.0.19 has a bug where MYSQL_OPT_RECONNECT is 
		// (incorrectly) reset by mysql_real_connect calls
		// See http://dev.mysql.com/doc/refman/5.0/en/mysql-options.html for more information.
		mysql_options(&mHandle, MYSQL_OPT_RECONNECT, &reconnect);
		PX2_LOG_ERROR("[Warning] Outdated mySQL server detected. Consider upgrading to a newer version.");
	}

	mIsConnected = true;

	bool isStorageBinary = true;
	if (isStorageBinary)
	{
		DBQuery query;
		query << "SHOW variables LIKE 'max_allowed_packet';";

		DBResultPtr result;
		if ((result = StoreQuery(query.str())))
		{
			int32_t max_query = result->GetDataInt("Value");

			if (max_query < 16777216)
			{
				std::cout << std::endl << "[Warning] max_allowed_packet might be set too low for binary map storage." << std::endl;
				std::cout << "Use the following query to raise max_allow_packet: " << std::endl;
				std::cout << "\tSET GLOBAL max_allowed_packet = 16777216;" << std::endl;
			}
		}
	}
}
//----------------------------------------------------------------------------
DatabaseMySQL::~DatabaseMySQL()
{
	mysql_close(&mHandle);
}
//----------------------------------------------------------------------------
bool DatabaseMySQL::BeginTransaction()
{
	return ExecuteQuery("BEGIN");
}
//----------------------------------------------------------------------------
bool DatabaseMySQL::Rollback()
{
	if (!mIsConnected)
		return false;

#ifdef __DEBUG_SQL__
	PX2_LOG_INFO("ROLLBACK");
#endif

	if (mysql_rollback(&mHandle) != 0)
	{
		PX2_LOG_ERROR("mysql_rollback(): MYSQL ERROR: %s",
			mysql_error(&mHandle));
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool DatabaseMySQL::Commit()
{
	if (!mIsConnected)
		return false;

#ifdef __DEBUG_SQL__
	PX2_LOG_INFO("COMMIT");
#endif
	if (mysql_commit(&mHandle) != 0)
	{
		PX2_LOG_ERROR("mysql_commit(): MYSQL ERROR: %s",
			mysql_error(&mHandle));
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool DatabaseMySQL::InternalQuery(const std::string &query)
{
	if (!mIsConnected)
		return false;

#ifdef __DEBUG_SQL__
	PX2_LOG_INFO("MYSQL QUERY:" + query);
#endif

	bool state = true;

	// executes the query
	if (mysql_real_query(&mHandle, query.c_str(), (unsigned long)query.length()) != 0)
	{
		PX2_LOG_ERROR("mysql_real_query(): %s : MYSQL ERROR: %s", query.substr(0, 256),
			mysql_error(&mHandle));

		int error = mysql_errno(&mHandle);

		if (error == CR_SERVER_LOST || error == CR_SERVER_GONE_ERROR)
		{
			mIsConnected = false;
		}

		state = false;
	}

	// we should call that every time as someone would call ExecuteQuery('SELECT...')
	// as it is described in MySQL manual: "it doesn't hurt" :P
	MYSQL_RES* m_res = mysql_store_result(&mHandle);

	if (m_res)
		mysql_free_result(m_res);

	return state;
}
//----------------------------------------------------------------------------
DBResult *DatabaseMySQL::InternalSelectQuery(const std::string &query)
{
	if (!mIsConnected)
		return 0;

#ifdef __DEBUG_SQL__
	PX2_LOG_INFO("MYSQL QUERY:" + query);
#endif

	// executes the query
	if (mysql_real_query(&mHandle, query.c_str(),
		(unsigned long)query.length()) != 0)
	{
		PX2_LOG_ERROR("mysql_real_query():%s : MYSQL ERROR: ", query,
			mysql_error(&mHandle));

		int error = mysql_errno(&mHandle);

		if (error == CR_SERVER_LOST || error == CR_SERVER_GONE_ERROR)
{
			mIsConnected = false;
		}

	}

	MYSQL_RES* m_res = mysql_store_result(&mHandle);

	// error occured
	if (!m_res)
	{
		std::cout << "mysql_store_result(): " << query.substr(0, 256) << ": MYSQL ERROR: " << mysql_error(&mHandle) << std::endl;
		int error = mysql_errno(&mHandle);

		if (error == CR_SERVER_LOST || error == CR_SERVER_GONE_ERROR){
			mIsConnected = false;
		}

		return 0;
	}

	// retriving results of query
	DBResult *res = new0 MySQLResult(m_res);
	if (res->Advance())
	{
		return res;
	}
	delete0(res);
	
	return 0;
}
//----------------------------------------------------------------------------
uint64_t DatabaseMySQL::GetLastInsertedRowID()
{
	return (uint64_t)mysql_insert_id(&mHandle);
}
//----------------------------------------------------------------------------
std::string DatabaseMySQL::EscapeString(const std::string &s)
{
	return EscapeBlob(s.c_str(), (uint32_t)s.length());
}
//----------------------------------------------------------------------------
std::string DatabaseMySQL::EscapeBlob(const char* s, uint32_t length)
{
	// remember about quoiting even an empty string!
	if (!s)
		return std::string("''");

	// the worst case is 2n + 1
	char* output = new char[length * 2 + 1];

	// quotes escaped string and frees temporary buffer
	mysql_real_escape_string(&mHandle, output, s, length);
	std::string r = "'";
	r += output;
	r += "'";
	delete[] output;
	return r;
}
//----------------------------------------------------------------------------
int32_t MySQLResult::GetDataInt(const std::string &s)
{
	listNames_t::iterator it = mListNames.find(s);
	if (it != mListNames.end())
	{
		if (mRow[it->second] == 0)
		{
			return 0;
		}
		else
		{
			return atoi(mRow[it->second]);
		}
	}

	PX2_LOG_ERROR("Error during GetDataInt(%s).", s.c_str());

	return 0; // Failed
}
//----------------------------------------------------------------------------
uint32_t MySQLResult::GetDataUInt(const std::string &s)
{
	listNames_t::iterator it = mListNames.find(s);
	if (it != mListNames.end())
	{
		if (mRow[it->second] == 0)
		{
			return 0;
		}
		else
		{
			std::istringstream os(mRow[it->second]);
			uint32_t res;
			os >> res;
			return res;
		}
	}

	PX2_LOG_ERROR("Error during GetDataInt(%s).", s.c_str());

	return 0; // Failed
}
//----------------------------------------------------------------------------
int64_t MySQLResult::GetDataLong(const std::string &s)
{
	listNames_t::iterator it = mListNames.find(s);
	if (it != mListNames.end())
	{
		if (mRow[it->second] == 0)
		{
			return 0;
		}
		else
		{
			return atoll(mRow[it->second]);
		}
	}

	PX2_LOG_ERROR("Error during GetDataLong(%s).", s.c_str());

	return 0; // Failed
}
//----------------------------------------------------------------------------
std::string MySQLResult::GetDataString(const std::string &s)
{
	listNames_t::iterator it = mListNames.find(s);
	if (it != mListNames.end())
	{
		if (mRow[it->second] == 0)
			return std::string("");
		else
			return std::string(mRow[it->second]);
	}

	PX2_LOG_ERROR("Error during GetDataString(%s).", s.c_str());

	return std::string(""); // Failed
}
//----------------------------------------------------------------------------
const char* MySQLResult::GetDataStream(const std::string &s, unsigned long &size)
{
	listNames_t::iterator it = mListNames.find(s);
	if (it != mListNames.end())
	{
		if (mRow[it->second] == 0)
		{
			size = 0;
			return 0;
		}
		else{
			size = mysql_fetch_lengths(mHandle)[it->second];
			return mRow[it->second];
		}
	}

	std::cout << "Error during GetDataStream(" << s << ")." << std::endl;
	size = 0;
	return 0;
}
//----------------------------------------------------------------------------
DBResult *MySQLResult::Advance()
{
	mRow = mysql_fetch_row(mHandle);
	return mRow != 0 ? this : 0;
}
//----------------------------------------------------------------------------
bool MySQLResult::IsEmpty()
{
	return mRow == 0;
}
//----------------------------------------------------------------------------
MySQLResult::MySQLResult(MYSQL_RES* res)
{
	mHandle = res;
	mListNames.clear();

	MYSQL_FIELD* field;
	int32_t i = 0;
	while ((field = mysql_fetch_field(mHandle))){
		mListNames[field->name] = i;
		i++;
	}
}
//----------------------------------------------------------------------------
MySQLResult::~MySQLResult()
{
	mysql_free_result(mHandle);
}
//----------------------------------------------------------------------------

#endif