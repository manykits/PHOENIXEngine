// PX2DateTime.inl

//----------------------------------------------------------------------------
inline Timestamp DateTime::GetTimestamp() const
{
	return Timestamp::FromUtcTime(mUtcTime);
}
//----------------------------------------------------------------------------
inline Timestamp::UtcTimeVal DateTime::UTCTime() const
{
	return mUtcTime;
}
//----------------------------------------------------------------------------
inline int DateTime::Year() const
{
	return mYear;
}
//----------------------------------------------------------------------------
inline int DateTime::Month() const
{
	return mMonth;
}
//----------------------------------------------------------------------------
inline int DateTime::Day() const
{
	return mDay;
}
//----------------------------------------------------------------------------
inline int DateTime::Hour() const
{
	return mHour;
}
//----------------------------------------------------------------------------
inline int DateTime::HourAMPM() const
{
	if (mHour < 1)
		return 12;
	else if (mHour > 12)
		return mHour - 12;

	return mHour;
}
//----------------------------------------------------------------------------
inline bool DateTime::IsAM() const
{
	return mHour < 12;
}
//----------------------------------------------------------------------------
inline bool DateTime::IsPM() const
{
	return mHour >= 12;
}
//----------------------------------------------------------------------------
inline int DateTime::Minute() const
{
	return mMinute;
}
//----------------------------------------------------------------------------
inline int DateTime::Second() const
{
	return mSecond;
}
//----------------------------------------------------------------------------
inline int DateTime::Millisecond() const
{
	return mMillisecond;
}
//----------------------------------------------------------------------------
inline int DateTime::Microsecond() const
{
	return mMicrosecond;
}
//----------------------------------------------------------------------------
inline bool DateTime::operator == (const DateTime& dateTime) const
{
	return mUtcTime == dateTime.mUtcTime;
}
//----------------------------------------------------------------------------
inline bool DateTime::operator != (const DateTime& dateTime) const	
{
	return mUtcTime != dateTime.mUtcTime;
}
//----------------------------------------------------------------------------
inline bool DateTime::operator <  (const DateTime& dateTime) const	
{
	return mUtcTime < dateTime.mUtcTime;
}
//----------------------------------------------------------------------------
inline bool DateTime::operator <= (const DateTime& dateTime) const
{
	return mUtcTime <= dateTime.mUtcTime;
}
//----------------------------------------------------------------------------
inline bool DateTime::operator >  (const DateTime& dateTime) const
{
	return mUtcTime > dateTime.mUtcTime;
}
//----------------------------------------------------------------------------
inline bool DateTime::operator >= (const DateTime& dateTime) const	
{
	return mUtcTime >= dateTime.mUtcTime;
}
//----------------------------------------------------------------------------
inline bool DateTime::IsLeapYear(int year)
{
	return (year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0);
}
//----------------------------------------------------------------------------
inline double DateTime::ToJulianDay(Timestamp::UtcTimeVal UTCTime)
{
	double utcDays = double(UTCTime)/864000000000.0;
	return utcDays + 2299160.5; // first day of Gregorian reform (Oct 15 1582)
}
//----------------------------------------------------------------------------
inline Timestamp::UtcTimeVal DateTime::ToUtcTime(double JulianDay)
{
	return Timestamp::UtcTimeVal((JulianDay - 2299160.5)*864000000000.0);
}
//----------------------------------------------------------------------------