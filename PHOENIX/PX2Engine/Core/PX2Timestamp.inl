// PX2Timestamp.inl

//----------------------------------------------------------------------------
inline bool Timestamp::operator == (const Timestamp& ts) const
{
	return mTimeVal == ts.mTimeVal;
}
//----------------------------------------------------------------------------
inline bool Timestamp::operator != (const Timestamp& ts) const
{
	return mTimeVal != ts.mTimeVal;
}
//----------------------------------------------------------------------------
inline bool Timestamp::operator >  (const Timestamp& ts) const
{
	return mTimeVal > ts.mTimeVal;
}
//----------------------------------------------------------------------------
inline bool Timestamp::operator >= (const Timestamp& ts) const
{
	return mTimeVal >= ts.mTimeVal;
}
//----------------------------------------------------------------------------
inline bool Timestamp::operator <  (const Timestamp& ts) const
{
	return mTimeVal < ts.mTimeVal;
}
//----------------------------------------------------------------------------
inline bool Timestamp::operator <= (const Timestamp& ts) const
{
	return mTimeVal <= ts.mTimeVal;
}
//----------------------------------------------------------------------------
inline Timestamp Timestamp::operator + (Timestamp::TimeDiff d) const
{
	return Timestamp(mTimeVal + d);
}
//----------------------------------------------------------------------------
inline Timestamp Timestamp::operator - (Timestamp::TimeDiff d) const
{
	return Timestamp(mTimeVal - d);
}
//----------------------------------------------------------------------------
inline Timestamp::TimeDiff Timestamp::operator - (const Timestamp& ts) const
{
	return mTimeVal - ts.mTimeVal;
}
//----------------------------------------------------------------------------
inline Timestamp& Timestamp::operator += (Timestamp::TimeDiff d)
{
	mTimeVal += d;
	return *this;
}
//----------------------------------------------------------------------------
inline Timestamp& Timestamp::operator -= (Timestamp::TimeDiff d)
{
	mTimeVal -= d;
	return *this;
}
//----------------------------------------------------------------------------
inline std::time_t Timestamp::EpochTime() const
{
	return std::time_t(mTimeVal/Resolution());
}
//----------------------------------------------------------------------------
inline Timestamp::UtcTimeVal Timestamp::UTCTime() const
{
	return mTimeVal*10 + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}
//----------------------------------------------------------------------------
inline Timestamp::TimeVal Timestamp::EpochMicroseconds() const
{
	return mTimeVal;
}
//----------------------------------------------------------------------------
inline Timestamp::TimeDiff Timestamp::Elapsed() const
{
	Timestamp now;
	return now - *this;
}
//----------------------------------------------------------------------------
inline bool Timestamp::IsElapsed(Timestamp::TimeDiff interval) const
{
	Timestamp now;
	Timestamp::TimeDiff diff = now - *this;
	return diff >= interval;
}
//----------------------------------------------------------------------------
inline Timestamp::TimeVal Timestamp::Resolution()
{
	return 1000000;
}
//----------------------------------------------------------------------------