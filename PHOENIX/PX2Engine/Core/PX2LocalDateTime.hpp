// PX2LocalDateTime.hpp

#ifndef PX2LOCALDATETIME_HPP
#define PX2LOCALDATETIME_HPP

#include "PX2CorePre.hpp"
#include "PX2DateTime.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LocalDateTime
	{
	public:
		LocalDateTime ();
		LocalDateTime (int year, int month, int day, int hour = 0, 
			int minute = 0, int second = 0, int millisecond = 0, 
			int microsecond = 0);
		LocalDateTime (int tzd, int year, int month, int day, int hour, 
			int minute, int second, int millisecond, int microsecond);
		LocalDateTime (const DateTime& dateTime);
		LocalDateTime (int tzd, const DateTime& dateTime);
		LocalDateTime (int tzd, const DateTime& dateTime, bool adjust);
		LocalDateTime (double julianDay);
		LocalDateTime (int tzd, double julianDay);
		LocalDateTime (const LocalDateTime& dateTime);
		~LocalDateTime();

		LocalDateTime& operator = (const LocalDateTime& dateTime);
		//LocalDateTime& operator = (const Timestamp &timestamp);
		LocalDateTime& operator = (double julianDay);
		
		LocalDateTime& Assign (int year, int month, int day, int hour = 0,
			int minute = 0, int second = 0, int millisecond = 0,
			int microseconds = 0);
		LocalDateTime& Assign (int tzd, int year, int month, int day, int hour,
			int minute, int second, int millisecond, int microseconds);
		LocalDateTime& Assign(int tzd, double julianDay);
		
		void Swap (LocalDateTime& dateTime);
		int Year () const;
		int Month () const;  // 1-12
		int Week (int firstDayOfWeek = DateTime::MONDAY) const;
		int Day () const; // 1-31
		int DayOfWeek () const; // 0-6 (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
		
		/// 返回在一年当中的第几天
		/**
		* 例如，1月1号是第一天，2月1号是第32天
		*/
		int DayOfYear () const;

		int Hour () const; // 0-23
		int HourAMPM () const; // 0-12
		bool IsAM () const; // < 12; 
		bool IsPM () const; // >= 12
		int Minute () const; // 0-59
		int Second () const; // 0-59
		int Millisecond () const; // 0-999
		int Microsecond () const; // 0-999

		double JulianDay () const;
		int TZD () const;
		DateTime UTC () const;
		Timestamp GetTimestamp () const;
		Timestamp::UtcTimeVal UTCTime () const;

		bool operator == (const LocalDateTime& dateTime) const;	
		bool operator != (const LocalDateTime& dateTime) const;	
		bool operator <  (const LocalDateTime& dateTime) const;	
		bool operator <= (const LocalDateTime& dateTime) const;	
		bool operator >  (const LocalDateTime& dateTime) const;	
		bool operator >= (const LocalDateTime& dateTime) const;	

		LocalDateTime  operator +  (const Timespan& span) const;
		LocalDateTime  operator -  (const Timespan& span) const;
		Timespan       operator -  (const LocalDateTime& dateTime) const;
		LocalDateTime& operator += (const Timespan& span);
		LocalDateTime& operator -= (const Timespan& span);

	protected:
		LocalDateTime (Timestamp::UtcTimeVal utcTime, Timestamp::TimeDiff diff,
			int tzd);

		void DetermineTZD (bool adjust = false);
		void AdjustForTZD();

		std::time_t DstOffset(int& DstOffset) const;

	private:
		DateTime mDateTime;
		int mTZD;

		friend class DateTimeFormatter;
		friend class DateTimeParser;
	};

#include "PX2LocalDateTime.inl"

}

#endif