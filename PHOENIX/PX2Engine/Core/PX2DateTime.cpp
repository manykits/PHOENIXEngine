// PX2DateTime.cpp

#include "PX2DateTime.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
DateTime::DateTime()
{
	Timestamp now;
	mUtcTime = now.UTCTime();
	ComputeGregorian(JulianDay());
	ComputeDaytime();
}
//----------------------------------------------------------------------------
DateTime::DateTime(const Timestamp& timestamp)
	:
mUtcTime(timestamp.UTCTime())
{
	ComputeGregorian(JulianDay());
	ComputeDaytime();
}
//----------------------------------------------------------------------------
DateTime::DateTime(int year, int month, int day, int hour, int minute, 
	int second, int millisecond, int microsecond)
	:
mYear(year),
	mMonth(month),
	mDay(day),
	mHour(hour),
	mMinute(minute),
	mSecond(second),
	mMillisecond(millisecond),
	mMicrosecond(microsecond)
{
	assertion(year >= 0 && year <= 9999, "should in right range.\n");
	assertion (month >= 1 && month <= 12, "should in right range.\n");
	assertion (day >= 1 && day <= DaysOfMonth(year, month),
		"should in right range.\n");
	assertion (hour >= 0 && hour <= 23, "should in right range.\n");
	assertion (minute >= 0 && minute <= 59, "should in right range.\n");
	assertion (second >= 0 && second <= 59, "should in right range.\n");
	assertion (millisecond >= 0 && millisecond <= 999, 
		"should in right range.\n");
	assertion (microsecond >= 0 && microsecond <= 999, 
		"should in right range.\n");

	mUtcTime = ToUtcTime(ToJulianDay(year, month, day))
		+ 10*(hour*Timespan::HOURS 
		+ minute*Timespan::MINUTES
		+ second*Timespan::SECONDS
		+ millisecond*Timespan::MILLISECONDS + microsecond);
}
//----------------------------------------------------------------------------
DateTime::DateTime(double julianDay)
	:
mUtcTime(ToUtcTime(julianDay))
{
	ComputeGregorian(julianDay);
}
//----------------------------------------------------------------------------
DateTime::DateTime(Timestamp::UtcTimeVal UTCTime, Timestamp::TimeDiff diff)
	:
mUtcTime(UTCTime + diff*10)
{
	ComputeGregorian(JulianDay());
	ComputeDaytime();
}
//----------------------------------------------------------------------------
DateTime::DateTime(const DateTime& dateTime)	
	:
mUtcTime(dateTime.mUtcTime),
	mYear(dateTime.mYear),
	mMonth(dateTime.mMonth),
	mDay(dateTime.mDay),
	mHour(dateTime.mHour),
	mMinute(dateTime.mMinute),
	mSecond(dateTime.mSecond),
	mMillisecond(dateTime.mMillisecond),
	mMicrosecond(dateTime.mMicrosecond)
{
}
//----------------------------------------------------------------------------
DateTime::~DateTime()
{
}
//----------------------------------------------------------------------------
DateTime& DateTime::operator = (const DateTime& dateTime)
{
	if (&dateTime != this)
	{
		mUtcTime     = dateTime.mUtcTime;
		mYear        = dateTime.mYear;
		mMonth       = dateTime.mMonth;
		mDay         = dateTime.mDay;
		mHour        = dateTime.mHour;
		mMinute      = dateTime.mMinute;
		mSecond      = dateTime.mSecond;
		mMillisecond = dateTime.mMillisecond;
		mMicrosecond = dateTime.mMicrosecond;
	}
	return *this;
}
//----------------------------------------------------------------------------
DateTime& DateTime::operator = (const Timestamp& timestamp)
{
	mUtcTime = timestamp.UTCTime();
	ComputeGregorian(JulianDay());
	ComputeDaytime();
	return *this;
}
//----------------------------------------------------------------------------
DateTime& DateTime::operator = (double julianDay)
{
	mUtcTime = ToUtcTime(julianDay);
	ComputeGregorian(julianDay);
	return *this;
}
//----------------------------------------------------------------------------
DateTime& DateTime::Assign(int year, int month, int day, int hour, int minute,
	int second, int millisecond, int microsecond)
{
	assertion (year >= 0 && year <= 9999, "should be in right range.");
	assertion (month >= 1 && month <= 12, "should be in right range.");
	assertion (day >= 1 && day <= DaysOfMonth(year, month), 
		"should be in right range.");
	assertion (hour >= 0 && hour <= 23, "should be in right range.");
	assertion (minute >= 0 && minute <= 59, "should be in right range.");
	assertion (second >= 0 && second <= 59, "should be in right range.");
	assertion (millisecond >= 0 && millisecond <= 999,
		 "should be in right range.");
	assertion (microsecond >= 0 && microsecond <= 999,
		 "should be in right range.");

	mUtcTime     = ToUtcTime(ToJulianDay(year, month, day)) 
		+ 10*(
		hour*Timespan::HOURS
		+ minute*Timespan::MINUTES 
		+ second*Timespan::SECONDS
		+ millisecond*Timespan::MILLISECONDS
		+ microsecond);
	mYear        = year;
	mMonth       = month;
	mDay         = day;
	mHour        = hour;
	mMinute      = minute;
	mSecond      = second;
	mMillisecond = millisecond;
	mMicrosecond = microsecond;

	return *this;
}
//----------------------------------------------------------------------------
void DateTime::Swap(DateTime& dateTime)
{
	std::swap(mUtcTime, dateTime.mUtcTime);
	std::swap(mYear, dateTime.mYear);
	std::swap(mMonth, dateTime.mMonth);
	std::swap(mDay, dateTime.mDay);
	std::swap(mHour, dateTime.mHour);
	std::swap(mMinute, dateTime.mMinute);
	std::swap(mSecond, dateTime.mSecond);
	std::swap(mMillisecond, dateTime.mMillisecond);
	std::swap(mMicrosecond, dateTime.mMicrosecond);
}
//----------------------------------------------------------------------------
int DateTime::DayOfWeek() const
{
	return int((std::floor(JulianDay() + 1.5))) % 7;
}
//----------------------------------------------------------------------------
int DateTime::DayOfYear() const
{
	int doy = 0;
	for (int month = 1; month < mMonth; ++month)
		doy += DaysOfMonth(mYear, month);
	doy += mDay;
	return doy;
}
//----------------------------------------------------------------------------
int DateTime::DaysOfMonth(int year, int month)
{
	assertion (month >= 1 && month <= 12, "month should be in right range.\n");

	static int daysOfMonthTable[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month == 2 && IsLeapYear(year))
		return 29;
	else
		return daysOfMonthTable[month];
}
//----------------------------------------------------------------------------
bool DateTime::IsValid(int year, int month, int day, int hour, int minute,
	int second, int millisecond, int microsecond)
{
	return
		(year >= 0 && year <= 9999) &&
		(month >= 1 && month <= 12) &&
		(day >= 1 && day <= DaysOfMonth(year, month)) &&
		(hour >= 0 && hour <= 23) &&
		(minute >= 0 && minute <= 59) &&
		(second >= 0 && second <= 59) &&
		(millisecond >= 0 && millisecond <= 999) &&
		(microsecond >= 0 && microsecond <= 999);
}
//----------------------------------------------------------------------------
int DateTime::Week(int firstDayOfWeek) const
{
	assertion (firstDayOfWeek >= 0 && firstDayOfWeek <= 6, 
		"should be in right range.\n");

	/// find the first firstDayOfWeek.
	int baseDay = 1;
	while (DateTime(mYear, 1, baseDay).DayOfWeek() != firstDayOfWeek)
		++baseDay;

	int doy  = DayOfYear();
	int offs = baseDay <= 4 ? 0 : 1; 
	if (doy < baseDay)
		return offs;
	else
		return (doy - baseDay)/7 + 1 + offs;
}
//----------------------------------------------------------------------------
double DateTime::JulianDay() const
{
	return ToJulianDay(mUtcTime);
}
//----------------------------------------------------------------------------
DateTime DateTime::operator + (const Timespan& span) const
{
	return DateTime(mUtcTime, span.TotalMicroseconds());
}
//----------------------------------------------------------------------------
DateTime DateTime::operator - (const Timespan& span) const
{
	return DateTime(mUtcTime, -span.TotalMicroseconds());
}
//----------------------------------------------------------------------------
Timespan DateTime::operator - (const DateTime& dateTime) const
{
	return Timespan((mUtcTime - dateTime.mUtcTime)/10);
}
//----------------------------------------------------------------------------
DateTime& DateTime::operator += (const Timespan& span)
{
	mUtcTime += span.TotalMicroseconds()*10;
	ComputeGregorian(JulianDay());
	ComputeDaytime();
	return *this;
}
//----------------------------------------------------------------------------
DateTime& DateTime::operator -= (const Timespan& span)
{
	mUtcTime -= span.TotalMicroseconds()*10;
	ComputeGregorian(JulianDay());
	ComputeDaytime();
	return *this;
}
//----------------------------------------------------------------------------
void DateTime::MakeUTC(int tzd)
{
	operator -= (Timespan(((Timestamp::TimeDiff) tzd)*Timespan::SECONDS));
}
//----------------------------------------------------------------------------
void DateTime::MakeLocal(int tzd)
{
	operator += (Timespan(((Timestamp::TimeDiff) tzd)*Timespan::SECONDS));
}
//----------------------------------------------------------------------------
double DateTime::ToJulianDay(int year, int month, int day, int hour, 
	int minute, int second, int millisecond, int microsecond)
{
	// lookup table for (153*month - 457)/5 - note that 3 <= month <= 14.
	static int lookup[] = {-91, -60, -30, 0, 31, 61, 92, 122, 153, 184, 214, 
		245, 275, 306, 337};

	// day to double
	double dday = double(day)
		+ ((double((hour*60 + minute)*60 + second)*1000 + millisecond)*1000 
		+ microsecond)/86400000000.0;
	if (month < 3)
	{
		month += 12;
		--year;
	}
	double dyear = double(year);
	return dday + lookup[month] + 365*year + std::floor(dyear/4) 
		- std::floor(dyear/100) + std::floor(dyear/400) + 1721118.5;
}
//----------------------------------------------------------------------------
void DateTime::CheckLimit(int& lower, int& higher, int limit)
{
	if (lower >= limit)
	{
		higher += short(lower / limit);
		lower   = short(lower % limit);
	}
}
//----------------------------------------------------------------------------
void DateTime::Normalize()
{
	CheckLimit(mMicrosecond, mMillisecond, 1000);
	CheckLimit(mMillisecond, mSecond, 1000);
	CheckLimit(mSecond, mMinute, 60);
	CheckLimit(mMinute, mHour, 60);
	CheckLimit(mHour, mDay, 24);

	if (mDay > DaysOfMonth(mYear, mMonth))
	{
		mDay -= DaysOfMonth(mYear, mMonth);
		if (++mMonth > 12)
		{
			++mYear;
			mMonth -= 12;
		}
	}
}
//----------------------------------------------------------------------------
void DateTime::ComputeGregorian(double julianDay)
{
	double z    = std::floor(julianDay - 1721118.5);
	double r    = julianDay - 1721118.5 - z;
	double g    = z - 0.25;
	double a    = std::floor(g / 36524.25);
	double b    = a - std::floor(a/4);
	mYear       = short(std::floor((b + g)/365.25));
	double c    = b + z - std::floor(365.25*mYear);
	mMonth      = short(std::floor((5*c + 456)/153));
	double dday = c - std::floor((153.0*mMonth - 457)/5) + r;
	mDay        = short(dday);
	if (mMonth > 12)
	{
		++mYear;
		mMonth -= 12;
	}
	r      *= 24;
	mHour   = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 60;
	mMinute = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 60;
	mSecond = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 1000;
	mMillisecond = short(std::floor(r));
	r      -= std::floor(r);
	r      *= 1000;
	mMicrosecond = short(r + 0.5);

	Normalize();

	assertion (mMonth >= 1 && mMonth <= 12, "should be in right range.\n");
	assertion (mDay >= 1 && mDay <= DaysOfMonth(mYear, mMonth),
		"should be in right range.\n");
	assertion (mHour >= 0 && mHour <= 23, "should be in right range.\n");
	assertion (mMinute >= 0 && mMinute <= 59, "should be in right range.\n");
	assertion (mSecond >= 0 && mSecond <= 59, "should be in right range.\n");
	assertion (mMillisecond >= 0 && mMillisecond <= 999,
		 "should be in right range.\n");
	assertion (mMicrosecond >= 0 && mMicrosecond <= 999,
		 "should be in right range.\n");
}
//----------------------------------------------------------------------------
void DateTime::ComputeDaytime()
{
	Timespan span(mUtcTime/10);
	mHour        = span.Hours();
	mMinute      = span.Minutes();
	mSecond      = span.Seconds();
	mMillisecond = span.Milliseconds();
	mMicrosecond = span.Microseconds();
}
//----------------------------------------------------------------------------