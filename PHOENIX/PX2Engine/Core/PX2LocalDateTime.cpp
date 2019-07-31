// PX2LocalDateTime.cpp

#include "PX2LocalDateTime.hpp"
#include "PX2TimeZone.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
LocalDateTime::LocalDateTime()
{
	DetermineTZD(true);
}
//----------------------------------------------------------------------------
LocalDateTime::LocalDateTime(int year, int month, int day, int hour, 
	int minute, int second, int millisecond, int microsecond)
	:
mDateTime(year, month, day, hour, minute, second, millisecond, microsecond)
{
	DetermineTZD();
}
//----------------------------------------------------------------------------
LocalDateTime::LocalDateTime(int tzd, int year, int month, int day, int hour,
	int minute, int second, int millisecond, int microsecond)
	:
mDateTime(year, month, day, hour, minute, second, millisecond, microsecond),
	mTZD(tzd)
{
}
//----------------------------------------------------------------------------
LocalDateTime::LocalDateTime(double julianDay):
mDateTime(julianDay)
{
	DetermineTZD(true);
}
//----------------------------------------------------------------------------
LocalDateTime::LocalDateTime(int tzd, double julianDay):
mDateTime(julianDay),
	mTZD(tzd)
{
	AdjustForTZD();
}
//----------------------------------------------------------------------------
LocalDateTime::LocalDateTime(const DateTime& dateTime):
mDateTime(dateTime)
{
	DetermineTZD(true);
}
//----------------------------------------------------------------------------
LocalDateTime::LocalDateTime(int tzd, const DateTime& dateTime):
mDateTime(dateTime),
	mTZD(tzd)
{
	AdjustForTZD();
}
//----------------------------------------------------------------------------
LocalDateTime::LocalDateTime(int tzd, const DateTime& dateTime, bool adjust):
mDateTime(dateTime),
	mTZD(tzd)
{
	if (adjust)
		AdjustForTZD();
}
//----------------------------------------------------------------------------
LocalDateTime::LocalDateTime(const LocalDateTime& dateTime):
mDateTime(dateTime.mDateTime),
	mTZD(dateTime.mTZD)
{
}
//----------------------------------------------------------------------------
LocalDateTime::LocalDateTime(Timestamp::UtcTimeVal UTCTime, Timestamp::TimeDiff diff, int tzd):
mDateTime(UTCTime, diff),
	mTZD(tzd)
{
	AdjustForTZD();
}
//----------------------------------------------------------------------------
LocalDateTime::~LocalDateTime()
{
}
//----------------------------------------------------------------------------
LocalDateTime& LocalDateTime::operator = (const LocalDateTime& dateTime)
{
	if (&dateTime != this)
	{
		mDateTime = dateTime.mDateTime;
		mTZD = dateTime.mTZD;
	}
	return *this;
}
////----------------------------------------------------------------------------
//LocalDateTime& LocalDateTime::operator = (const Timestamp& timestamp)
//{
//	if (timestamp != this->timestamp())
//	{
//		mDateTime = timestamp;
//		DetermineTZD(true);
//	}
//	return *this;
//}
//----------------------------------------------------------------------------
LocalDateTime& LocalDateTime::operator = (double julianDay)
{
	mDateTime = julianDay;
	DetermineTZD(true);
	return *this;
}
//----------------------------------------------------------------------------
LocalDateTime& LocalDateTime::Assign(int year, int month, int day, int hour, 
	int minute, int second, int millisecond, int microseconds)
{
	mDateTime.Assign(year, month, day, hour, minute, second, millisecond, microseconds);
	DetermineTZD(false);
	return *this;
}
//----------------------------------------------------------------------------
LocalDateTime& LocalDateTime::Assign(int tzd, int year, int month, int day,
	int hour, int minute, int second, int millisecond, int microseconds)
{
	mDateTime.Assign(year, month, day, hour, minute, second, millisecond, 
		microseconds);
	mTZD = tzd;
	return *this;
}
//----------------------------------------------------------------------------
LocalDateTime& LocalDateTime::Assign(int tzd, double julianDay)
{
	mTZD      = tzd;
	mDateTime = julianDay;
	AdjustForTZD();
	return *this;
}
//----------------------------------------------------------------------------
void LocalDateTime::Swap(LocalDateTime& dateTime)
{
	mDateTime.Swap(dateTime.mDateTime);
	std::swap(mTZD, dateTime.mTZD);
}
//----------------------------------------------------------------------------
DateTime LocalDateTime::UTC() const
{
	return DateTime(mDateTime.UTCTime(),
		-((Timestamp::TimeDiff)mTZD)*Timespan::SECONDS);
}
//----------------------------------------------------------------------------
bool LocalDateTime::operator == (const LocalDateTime& dateTime) const
{
	return UTCTime() == dateTime.UTCTime();
}
//----------------------------------------------------------------------------
bool LocalDateTime::operator != (const LocalDateTime& dateTime) const	
{
	return UTCTime() != dateTime.UTCTime();
}
//----------------------------------------------------------------------------
bool LocalDateTime::operator <  (const LocalDateTime& dateTime) const	
{
	return UTCTime() < dateTime.UTCTime();
}
//----------------------------------------------------------------------------
bool LocalDateTime::operator <= (const LocalDateTime& dateTime) const	
{
	return UTCTime() <= dateTime.UTCTime();
}
//----------------------------------------------------------------------------
bool LocalDateTime::operator >  (const LocalDateTime& dateTime) const	
{
	return UTCTime() > dateTime.UTCTime();
}
//----------------------------------------------------------------------------
bool LocalDateTime::operator >= (const LocalDateTime& dateTime) const	
{
	return UTCTime() >= dateTime.UTCTime();
}
//----------------------------------------------------------------------------
LocalDateTime LocalDateTime::operator + (const Timespan& span) const
{
	DateTime tmp(UTCTime(), span.TotalMicroseconds());
	return LocalDateTime(tmp);
}
//----------------------------------------------------------------------------
LocalDateTime LocalDateTime::operator - (const Timespan& span) const
{
	DateTime tmp(UTCTime(), -span.TotalMicroseconds());
	return LocalDateTime(tmp);
}
//----------------------------------------------------------------------------
Timespan LocalDateTime::operator - (const LocalDateTime& dateTime) const
{
	return Timespan((UTCTime() - dateTime.UTCTime())/10);
}
//----------------------------------------------------------------------------
LocalDateTime& LocalDateTime::operator += (const Timespan& span)
{
	*this = DateTime(UTCTime(), span.TotalMicroseconds());
	return *this;
}
//----------------------------------------------------------------------------
LocalDateTime& LocalDateTime::operator -= (const Timespan& span)
{
	*this = DateTime(UTCTime(), -span.TotalMicroseconds());
	return *this;
}
//----------------------------------------------------------------------------
void LocalDateTime::DetermineTZD(bool adjust)
{
	if (adjust)
	{
		std::time_t epochTime = mDateTime.GetTimestamp().EpochTime();

#if defined(_WIN32) || defined(WIN32)
		std::tm* broken = std::localtime(&epochTime);
		if (!broken)
		{
			assertion(false, "Cannot get local time.");
		}

		mTZD = (TimeZone::UTCOffset() + ((broken->tm_isdst == 1) ? 3600 : 0));
#else
		std::tm broken;
		if (!localtime_r(&epochTime, &broken))
		{
			assertion(false, "Cannot get local time.");
		}

		mTZD = (TimeZone::UTCOffset() + ((broken.tm_isdst == 1) ? 3600 : 0));
#endif

		AdjustForTZD();
	}
	else
	{
		int dst;
		DstOffset(dst);
		mTZD = (TimeZone::UTCOffset() + dst);
	}
}
//----------------------------------------------------------------------------
std::time_t LocalDateTime::DstOffset(int& DstOffset) const
{
	std::time_t local;
	std::tm broken;

	broken.tm_year  = (mDateTime.Year() - 1900);
	broken.tm_mon   = (mDateTime.Month() - 1);
	broken.tm_mday  = mDateTime.Day();
	broken.tm_hour  = mDateTime.Hour();
	broken.tm_min   = mDateTime.Minute();
	broken.tm_sec   = mDateTime.Second();
	broken.tm_isdst = -1;

	// 将用tm结构表示的时间转化为日历时间。
	local = std::mktime(&broken);

	DstOffset = (broken.tm_isdst == 1) ? 3600 : 0;

	return local;
}
//----------------------------------------------------------------------------