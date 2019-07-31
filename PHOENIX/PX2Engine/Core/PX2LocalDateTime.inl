// PX2LocalDateTime.inl

//----------------------------------------------------------------------------
inline int LocalDateTime::Year() const
{
	return mDateTime.Year();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::Month() const
{
	return mDateTime.Month();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::Week(int firstDayOfWeek) const
{
	return mDateTime.Week(firstDayOfWeek);
}
//----------------------------------------------------------------------------
inline int LocalDateTime::Day() const
{
	return mDateTime.Day();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::DayOfWeek() const
{
	return mDateTime.DayOfWeek();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::DayOfYear() const
{
	return mDateTime.DayOfYear();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::Hour() const
{
	return mDateTime.Hour();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::HourAMPM() const
{
	return mDateTime.HourAMPM();
}
//----------------------------------------------------------------------------
inline bool LocalDateTime::IsAM() const
{
	return mDateTime.IsAM();
}
//----------------------------------------------------------------------------
inline bool LocalDateTime::IsPM() const
{
	return mDateTime.IsPM();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::Minute() const
{
	return mDateTime.Minute();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::Second() const
{
	return mDateTime.Second();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::Millisecond() const
{
	return mDateTime.Millisecond();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::Microsecond() const
{
	return mDateTime.Microsecond();
}
//----------------------------------------------------------------------------
inline double LocalDateTime::JulianDay() const
{
	return mDateTime.JulianDay();
}
//----------------------------------------------------------------------------
inline int LocalDateTime::TZD() const
{
	return mTZD;
}
//----------------------------------------------------------------------------
inline Timestamp LocalDateTime::GetTimestamp() const
{
	return Timestamp::FromUtcTime(mDateTime.UTCTime());
}
//----------------------------------------------------------------------------
inline Timestamp::UtcTimeVal LocalDateTime::UTCTime() const
{
	return mDateTime.UTCTime() - ((Timestamp::TimeDiff)mTZD)*10000000;
}
//----------------------------------------------------------------------------
inline void LocalDateTime::AdjustForTZD()
{
	mDateTime += Timespan(((Timestamp::TimeDiff)mTZD)*Timespan::SECONDS);
}
//----------------------------------------------------------------------------