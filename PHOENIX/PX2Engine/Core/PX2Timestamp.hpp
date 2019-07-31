// PX2Timestamp.hpp

#ifndef PX2TIMESTAMP_HPP
#define PX2TIMESTAMP_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Timestamp
	{
	public:
		typedef int64_t TimeVal;    /// UTC time value in microsecond Resolution
		typedef int64_t UtcTimeVal; /// UTC time value in 100 nanosecond Resolution
		typedef int64_t TimeDiff;   /// 两个Timestamp间隔的微秒数

		Timestamp();
		Timestamp(TimeVal tv);
		Timestamp(const Timestamp& other);
		~Timestamp();

		Timestamp& operator = (const Timestamp& other);
		Timestamp& operator = (TimeVal tv);
		void Swap(Timestamp& timestamp);

		void Update();

		bool operator == (const Timestamp& ts) const;
		bool operator != (const Timestamp& ts) const;
		bool operator >  (const Timestamp& ts) const;
		bool operator >= (const Timestamp& ts) const;
		bool operator <  (const Timestamp& ts) const;
		bool operator <= (const Timestamp& ts) const;

		Timestamp  operator +  (TimeDiff d) const;
		Timestamp  operator -  (TimeDiff d) const;
		TimeDiff   operator -  (const Timestamp& ts) const;
		Timestamp& operator += (TimeDiff d);
		Timestamp& operator -= (TimeDiff d);

		std::time_t EpochTime() const;
		UtcTimeVal UTCTime() const;
		TimeVal EpochMicroseconds() const;
		TimeDiff Elapsed() const;
		bool IsElapsed(TimeDiff interval) const;

		static Timestamp FromEpochTime(std::time_t t);
		static Timestamp FromUtcTime(UtcTimeVal val);
		static TimeVal Resolution();

#if defined(_WIN32)
		static Timestamp FromFileTimeNP(uint32_t fileTimeLow, uint32_t fileTimeHigh);
		void ToFileTimeNP(uint32_t& fileTimeLow, uint32_t& fileTimeHigh) const;
#endif

	private:
		TimeVal mTimeVal;
	};

#include "PX2Timestamp.inl"

}

#endif