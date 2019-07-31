// PX2TimeZone.cpp

#include "PX2TimeZone.hpp"
#include "PX2Assert.hpp"

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#else

#if defined(__APPLE__)
#include <sys/time.h>
#else
#include <sys/timeb.h>
#endif

class TimeZoneInfo
{
public:
	TimeZoneInfo()
	{
		tzset();
	}

	int GetTimeZone()
	{
#if defined(__APPLE__)  || defined(__ANDROID__) || defined (__LINUX__)
		std::time_t now = std::time(NULL);
		struct std::tm t;
		gmtime_r(&now, &t);
		std::time_t utc = std::mktime(&t);
		return now - utc;
#endif
		return 0;
	}

	const char* name(bool dst)
	{
		return tzname[dst ? 1 : 0];
	}
};

static TimeZoneInfo gTimeZone;

#endif

using namespace PX2;

//----------------------------------------------------------------------------
int TimeZone::TZD()
{
	return UTCOffset() + Dst();
}
//----------------------------------------------------------------------------
int TimeZone::UTCOffset()
{
#if defined(_WIN32) || defined(WIN32)
	TIME_ZONE_INFORMATION gTimeZone;
	DWORD dstFlag = GetTimeZoneInformation(&gTimeZone);
	PX2_UNUSED(dstFlag);
	return -gTimeZone.Bias*60;
#else
	return gTimeZone.GetTimeZone();
#endif
}
//----------------------------------------------------------------------------
int TimeZone::Dst()
{
#if defined(_WIN32) || defined(WIN32)
	TIME_ZONE_INFORMATION gTimeZone;
	DWORD dstFlag = GetTimeZoneInformation(&gTimeZone);
	return (dstFlag == TIME_ZONE_ID_DAYLIGHT)? -gTimeZone.DaylightBias*60 : 0;
#else
	std::time_t now = std::time(0);
	struct std::tm t;
	if (!localtime_r(&now, &t))
	{
		assertion(false, "cannot get local time DST offset.");
	}
	return t.tm_isdst == 1 ? 3600 : 0;
#endif
}
//----------------------------------------------------------------------------
bool TimeZone::IsDst(const Timestamp& timestamp)
{
#if defined(_WIN32) || defined(WIN32)
	std::time_t time = timestamp.EpochTime();
	struct std::tm* tms = std::localtime(&time);
	if (!tms)
	{
		assertion(false, "cannot get local time DST flag");
	}
	return tms->tm_isdst > 0;
#else
	std::time_t time = timestamp.EpochTime();
	struct std::tm* tms = std::localtime(&time);
	if (!tms)
	{
		assertion(false, "cannot get local time DST flag");
	}
	return tms->tm_isdst > 0;
#endif
}
//----------------------------------------------------------------------------