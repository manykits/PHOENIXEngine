// PX2Timespan.cpp

#include "PX2Timespan.hpp"
using namespace PX2;

const Timespan::TimeDiff Timespan::MILLISECONDS = 1000;
const Timespan::TimeDiff Timespan::SECONDS      = 1000*Timespan::MILLISECONDS;
const Timespan::TimeDiff Timespan::MINUTES      =   60*Timespan::SECONDS;
const Timespan::TimeDiff Timespan::HOURS        =   60*Timespan::MINUTES;
const Timespan::TimeDiff Timespan::DAYS         =   24*Timespan::HOURS;
//----------------------------------------------------------------------------
Timespan::Timespan() :
mSpan(0)
{
}
//----------------------------------------------------------------------------
Timespan::Timespan(TimeDiff microseconds)
	:
mSpan(microseconds)
{
}
//----------------------------------------------------------------------------
Timespan::Timespan(long seconds, long microseconds)
	:
mSpan(TimeDiff(seconds)*SECONDS + microseconds)
{
}
//----------------------------------------------------------------------------
Timespan::Timespan(int days, int hours, int minutes, int seconds, 
	int microseconds) :
mSpan(TimeDiff(microseconds) + TimeDiff(seconds)*SECONDS 
	+ TimeDiff(minutes)*MINUTES + TimeDiff(hours)*HOURS + TimeDiff(days)*DAYS)
{
}
//----------------------------------------------------------------------------
Timespan::Timespan(const Timespan& timespan)
	:
mSpan(timespan.mSpan)
{
}
//----------------------------------------------------------------------------
Timespan::~Timespan()
{
}
//----------------------------------------------------------------------------
Timespan& Timespan::operator = (const Timespan& timespan)
{
	mSpan = timespan.mSpan;
	return *this;
}
//----------------------------------------------------------------------------
Timespan& Timespan::operator = (TimeDiff microseconds)
{
	mSpan = microseconds;
	return *this;
}
//----------------------------------------------------------------------------
Timespan& Timespan::Assign(int days, int hours, int minutes, int seconds, 
	int microseconds)
{
	mSpan = TimeDiff(microseconds) + TimeDiff(seconds)*SECONDS 
		+ TimeDiff(minutes)*MINUTES + TimeDiff(hours)*HOURS
		+ TimeDiff(days)*DAYS;
	return *this;
}
//----------------------------------------------------------------------------
Timespan& Timespan::Assign(long seconds, long microseconds)
{
	mSpan = TimeDiff(seconds)*SECONDS + TimeDiff(microseconds);
	return *this;
}
//----------------------------------------------------------------------------
void Timespan::Swap(Timespan& timespan)
{
	std::swap(mSpan, timespan.mSpan);
}
//----------------------------------------------------------------------------
Timespan Timespan::operator + (const Timespan& d) const
{
	return Timespan(mSpan + d.mSpan);
}
//----------------------------------------------------------------------------
Timespan Timespan::operator - (const Timespan& d) const
{
	return Timespan(mSpan - d.mSpan);
}
//----------------------------------------------------------------------------
Timespan& Timespan::operator += (const Timespan& d)
{
	mSpan += d.mSpan;
	return *this;
}
//----------------------------------------------------------------------------
Timespan& Timespan::operator -= (const Timespan& d)
{
	mSpan -= d.mSpan;
	return *this;
}
//----------------------------------------------------------------------------
Timespan Timespan::operator + (TimeDiff microseconds) const
{
	return Timespan(mSpan + microseconds);
}
//----------------------------------------------------------------------------
Timespan Timespan::operator - (TimeDiff microseconds) const
{
	return Timespan(mSpan - microseconds);
}
//----------------------------------------------------------------------------
Timespan& Timespan::operator += (TimeDiff microseconds)
{
	mSpan += microseconds;
	return *this;
}
//----------------------------------------------------------------------------
Timespan& Timespan::operator -= (TimeDiff microseconds)
{
	mSpan -= microseconds;
	return *this;
}
//----------------------------------------------------------------------------