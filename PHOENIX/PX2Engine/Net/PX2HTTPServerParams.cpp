// PX2HTTPServerParams.cpp

#include "PX2HTTPServerParams.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HTTPServerParams::HTTPServerParams() :
mTimeout(60000000),
mIsKeepAlive(true),
mMaxKeepAliveRequests(0),
mKeepAliveTimeout(15000000)
{
}
//----------------------------------------------------------------------------
HTTPServerParams::~HTTPServerParams()
{
}
//----------------------------------------------------------------------------
void HTTPServerParams::SetServerName(const std::string& serverName)
{
	mServerName = serverName;
}
//----------------------------------------------------------------------------
void HTTPServerParams::SetSoftwareVersion(const std::string& softwareVersion)
{
	mSoftwareVersion = softwareVersion;
}
//----------------------------------------------------------------------------
void HTTPServerParams::SetTimeout(const Timespan& timeout)
{
	mTimeout = timeout;
}
//----------------------------------------------------------------------------
void HTTPServerParams::SetKeepAlive(bool keepAlive)
{
	mIsKeepAlive = keepAlive;
}
//----------------------------------------------------------------------------
void HTTPServerParams::SetKeepAliveTimeout(const Timespan& timeout)
{
	mKeepAliveTimeout = timeout;
}
//----------------------------------------------------------------------------
void HTTPServerParams::SetMaxKeepAliveRequests(int maxKeepAliveRequests)
{
	assert(maxKeepAliveRequests >= 0);
	mMaxKeepAliveRequests = maxKeepAliveRequests;
}
//----------------------------------------------------------------------------
const std::string& HTTPServerParams::GetServerName() const
{
	return mServerName;
}
//----------------------------------------------------------------------------
const std::string& HTTPServerParams::GetSoftwareVersion() const
{
	return mSoftwareVersion;
}
//----------------------------------------------------------------------------
const Timespan& HTTPServerParams::GetTimeout() const
{
	return mTimeout;
}
//----------------------------------------------------------------------------
bool HTTPServerParams::GetKeepAlive() const
{
	return mIsKeepAlive;
}
//----------------------------------------------------------------------------
int HTTPServerParams::GetMaxKeepAliveRequests() const
{
	return mMaxKeepAliveRequests;
}
//----------------------------------------------------------------------------
const Timespan& HTTPServerParams::GetKeepAliveTimeout() const
{
	return mKeepAliveTimeout;
}
//----------------------------------------------------------------------------