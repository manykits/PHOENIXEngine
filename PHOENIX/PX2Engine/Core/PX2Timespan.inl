// PX2Timespan.inl

//----------------------------------------------------------------------------
inline int Timespan::Days() const
{
	return int(mSpan/DAYS);
}
//----------------------------------------------------------------------------
inline int Timespan::Hours() const
{
	return int((mSpan/HOURS) % 24);
}
//----------------------------------------------------------------------------
inline int Timespan::TotalHours() const
{
	return int(mSpan/HOURS);
}
//----------------------------------------------------------------------------
inline int Timespan::Minutes() const
{
	return int((mSpan/MINUTES) % 60);
}
//----------------------------------------------------------------------------
inline int Timespan::TotalMinutes() const
{
	return int(mSpan/MINUTES);
}
//----------------------------------------------------------------------------
inline int Timespan::Seconds() const
{
	return int((mSpan/SECONDS) % 60);
}
//----------------------------------------------------------------------------
inline int Timespan::TotalSeconds() const
{
	return int(mSpan/SECONDS);
}
//----------------------------------------------------------------------------
inline int Timespan::Milliseconds() const
{
	return int((mSpan/MILLISECONDS) % 1000);
}
//----------------------------------------------------------------------------
inline Timespan::TimeDiff Timespan::TotalMilliseconds() const
{
	return mSpan/MILLISECONDS;
}
//----------------------------------------------------------------------------
inline int Timespan::Microseconds() const
{
	return int(mSpan % 1000);
}
//----------------------------------------------------------------------------
inline int Timespan::Useconds() const
{
	return int(mSpan % 1000000);
}
//----------------------------------------------------------------------------
inline Timespan::TimeDiff Timespan::TotalMicroseconds() const
{
	return mSpan;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator == (const Timespan& ts) const
{
	return mSpan == ts.mSpan;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator != (const Timespan& ts) const
{
	return mSpan != ts.mSpan;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator >  (const Timespan& ts) const
{
	return mSpan > ts.mSpan;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator >= (const Timespan& ts) const
{
	return mSpan >= ts.mSpan;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator <  (const Timespan& ts) const
{
	return mSpan < ts.mSpan;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator <= (const Timespan& ts) const
{
	return mSpan <= ts.mSpan;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator == (TimeDiff microseconds) const
{
	return mSpan == microseconds;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator != (TimeDiff microseconds) const
{
	return mSpan != microseconds;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator >  (TimeDiff microseconds) const
{
	return mSpan > microseconds;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator >= (TimeDiff microseconds) const
{
	return mSpan >= microseconds;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator <  (TimeDiff microseconds) const
{
	return mSpan < microseconds;
}
//----------------------------------------------------------------------------
inline bool Timespan::operator <= (TimeDiff microseconds) const
{
	return mSpan <= microseconds;
}
//----------------------------------------------------------------------------