// PX2Timestamp.cpp

#include "PX2Timestamp.hpp"
using namespace PX2;

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

#if defined __APPLE__
#include <sys/time.h>
#elif defined __LINUX__
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>
#else
#include <time.h>
#include <sys/timeb.h>
#endif

//----------------------------------------------------------------------------
Timestamp::Timestamp()
	:
mTimeVal(0)
{
	Update();
}
//----------------------------------------------------------------------------
Timestamp::Timestamp(TimeVal tv)
{
	mTimeVal = tv;
}
//----------------------------------------------------------------------------
Timestamp::Timestamp(const Timestamp& other)
{
	mTimeVal = other.mTimeVal;
}
//----------------------------------------------------------------------------
Timestamp::~Timestamp()
{
}
//----------------------------------------------------------------------------
Timestamp& Timestamp::operator = (const Timestamp& other)
{
	mTimeVal = other.mTimeVal;
	return *this;
}
//----------------------------------------------------------------------------
Timestamp& Timestamp::operator = (TimeVal tv)
{
	mTimeVal = tv;
	return *this;
}
//----------------------------------------------------------------------------
void Timestamp::Swap(Timestamp& timestamp)
{
	std::swap(mTimeVal, timestamp.mTimeVal);
}
//----------------------------------------------------------------------------
Timestamp Timestamp::FromEpochTime(std::time_t t)
{
	return Timestamp(TimeVal(t)*Resolution());
}
//----------------------------------------------------------------------------
Timestamp Timestamp::FromUtcTime(UtcTimeVal val)
{
	val -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
	val /= 10;
	return Timestamp(val);
}
//----------------------------------------------------------------------------
void Timestamp::Update()
{
#if defined(_WIN32) || defined(WIN32)
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart  = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.LowPart  = ft.dwLowDateTime;
	ts.HighPart = ft.dwHighDateTime;
	ts.QuadPart -= epoch.QuadPart;
	mTimeVal = ts.QuadPart/10;
#else
	struct timeval tv;
	gettimeofday(&tv, 0);
	mTimeVal = TimeVal(tv.tv_sec)*Resolution() + tv.tv_usec;
#endif
}
//----------------------------------------------------------------------------
#if defined(_WIN32)
Timestamp Timestamp::FromFileTimeNP(uint32_t fileTimeLow, uint32_t fileTimeHigh)
{
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.LowPart = fileTimeLow;
	ts.HighPart = fileTimeHigh;
	ts.QuadPart -= epoch.QuadPart;

	return Timestamp(ts.QuadPart / 10);
}
//----------------------------------------------------------------------------
void Timestamp::ToFileTimeNP(uint32_t& fileTimeLow, uint32_t& fileTimeHigh) const
{
	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in 
						 // Windows NT FILETIME
	epoch.LowPart = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.QuadPart = mTimeVal * 10;
	ts.QuadPart += epoch.QuadPart;
	fileTimeLow = ts.LowPart;
	fileTimeHigh = ts.HighPart;
}
//----------------------------------------------------------------------------
#endif