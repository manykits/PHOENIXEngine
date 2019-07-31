// PX2Timespan.hpp

#ifndef PX2TIMESPAN_HPP
#define PX2TIMESPAN_HPP

#include "PX2CorePre.hpp"
#include "PX2Timestamp.hpp"

namespace PX2
{

	/**
	* 将时间转换为微秒(一百万分之一秒)，计算时间的跨度
	*/
	class PX2_ENGINE_ITEM Timespan
	{
	public:
		typedef Timestamp::TimeDiff TimeDiff;

		Timespan ();
		Timespan (TimeDiff microseconds);
		Timespan (long seconds, long microseconds);
		Timespan (int days, int hours, int minutes, int seconds,
			int microseconds);
		Timespan (const Timespan& timespan);
		~Timespan ();

		Timespan& operator = (const Timespan& timespan);
		Timespan& operator = (TimeDiff microseconds);
		Timespan& Assign(int days, int hours, int minutes, int seconds,
			int microseconds);
		Timespan& Assign(long seconds, long microseconds);
		void Swap(Timespan& timespan);

		bool operator == (const Timespan& ts) const;
		bool operator != (const Timespan& ts) const;
		bool operator >  (const Timespan& ts) const;
		bool operator >= (const Timespan& ts) const;
		bool operator <  (const Timespan& ts) const;
		bool operator <= (const Timespan& ts) const;

		bool operator == (TimeDiff microseconds) const;
		bool operator != (TimeDiff microseconds) const;
		bool operator >  (TimeDiff microseconds) const;
		bool operator >= (TimeDiff microseconds) const;
		bool operator <  (TimeDiff microseconds) const;
		bool operator <= (TimeDiff microseconds) const;

		Timespan operator + (const Timespan& d) const;
		Timespan operator - (const Timespan& d) const;
		Timespan& operator += (const Timespan& d);
		Timespan& operator -= (const Timespan& d);

		Timespan operator + (TimeDiff microseconds) const;
		Timespan operator - (TimeDiff microseconds) const;
		Timespan& operator += (TimeDiff microseconds);
		Timespan& operator -= (TimeDiff microseconds);

		int Days() const;

		int Hours() const; //< 0-23
		int TotalHours() const;

		int Minutes() const; //< 0-59
		int TotalMinutes() const;

		int Seconds() const; //< 0-59
		int TotalSeconds() const;

		int Milliseconds() const; //< 0-999
		TimeDiff TotalMilliseconds() const;

		int Microseconds() const; //< 0-999
		int Useconds() const; //< 0-999999
		TimeDiff TotalMicroseconds() const;

		static const TimeDiff MILLISECONDS; //< 1毫秒为 MILLISECONDS个微秒数
		static const TimeDiff SECONDS;      //< 1秒为	SECONDS		个微秒数
		static const TimeDiff MINUTES;      //< 1分钟为 MINUTES		个微秒数
		static const TimeDiff HOURS;        //< 1小时为 HOURS		个微秒数
		static const TimeDiff DAYS;         //< 1天为	DAYS		个微秒数

	private:
		TimeDiff mSpan;
	};

	inline void Swap(Timespan& s1, Timespan& s2)
	{
		s1.Swap(s2);
	}

#include "PX2Timespan.inl"

}

#endif