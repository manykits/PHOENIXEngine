// PX2DateTime.hpp

#ifndef PX2DATETIME_HPP
#define PX2DATETIME_HPP

#include "PX2CorePre.hpp"
#include "PX2Timestamp.hpp"
#include "PX2Timespan.hpp"

namespace PX2
{

	// Coordinated Universal Time（UTC）：世界标准时间 
	// 就是大家所熟知的格林威治标准时间（Greenwich Mean Time，GMT）：比如，中国
	// 内地的时间与UTC的时差为+8，也就是UTC+8。美国是UTC-5。
	
	// Calendar Time：日历时间 
	// 是“从一个标准时间点到此时的时间经过的 (秒数)”来表示的时间。
	// （例如：1970年1月1日0时0分0秒)。这个标准时间点对不同的编译器来说会有所不
	// 同，但对一个编译系统来说，这个标准时间点是不变的，该编译系统中的时间对应
	// 的日历时间都通过该标准时间点来衡量，所以可以说日历时间是“相对时间”，但
	// 是无论你在哪一个时区，在同一时刻对同一个标准时间点来说，日历时间都是一样
	// 的。 
	
	// Epoch：时间点 
	// 时间点在标准C/C++中是一个整数，它用此时的时间和标准时间点相差的秒数（即
	// 日历时间）来表示。

	class PX2_ENGINE_ITEM DateTime
	{
	public:
		enum Months
		{
			JANUARY = 1,
			FEBRUARY,
			MARCH,
			APRIL,
			MAY,
			JUNE,
			JULY,
			AUGUST,
			SEPTEMBER,
			OCTOBER,
			NOVEMBER,
			DECEMBER
		};

		enum DaysOfWeek
		{
			SUNDAY = 0,
			MONDAY,
			TUESDAY,
			WEDNESDAY,
			THURSDAY,
			FRIDAY,
			SATURDAY
		};

		DateTime();
		DateTime(const Timestamp& timestamp);
		DateTime(int year, int month, int day, int hour = 0, int minute = 0,
			int second = 0, int millisecond = 0, int microsecond = 0);
		DateTime(double julianDay);
		DateTime(Timestamp::UtcTimeVal UTCTime, Timestamp::TimeDiff diff);
		DateTime(const DateTime& dateTime);
		~DateTime();

		DateTime& operator = (const DateTime& dateTime);
		DateTime& operator = (const Timestamp& timestamp);
		DateTime& operator = (double julianDay);
		DateTime& Assign(int year, int month, int day, int hour = 0, int minute = 0, 
			int second = 0, int millisecond = 0, int microseconds = 0);

		void Swap(DateTime& dateTime);

		int Year() const;
		int Month() const; //< 1-12
		int Week(int firstDayOfWeek = MONDAY) const;
		int Day() const; //< 1-31
		int DayOfWeek() const; //< 0-6 (0 = Sunday, 1 = Monday, ..., 6 = Saturday)

		/// 返回在一年当中的第几天
		/**
		* 例如，1月1号是第一天，2月1号是第32天
		*/
		int DayOfYear() const;

		int Hour() const; //< 0-23

		int HourAMPM() const; //< 0-12

		bool IsAM() const; //< hour < 12;

		bool IsPM() const; //< hour >= 12

		int Minute() const; //< 0-59

		int Second() const; // 0-59

		int Millisecond() const; // 0-999

		int Microsecond() const; // 0-999

		double JulianDay() const;
		/// Returns the julian day for the date and time.

		Timestamp GetTimestamp() const;
		/// Returns the date and time expressed as a Timestamp.

		Timestamp::UtcTimeVal UTCTime() const;
		/// Returns the date and time expressed in UTC-based
		/// time. UTC base time is midnight, October 15, 1582.
		/// Resolution is 100 nanoseconds.

		bool operator == (const DateTime& dateTime) const;	
		bool operator != (const DateTime& dateTime) const;	
		bool operator <  (const DateTime& dateTime) const;	
		bool operator <= (const DateTime& dateTime) const;	
		bool operator >  (const DateTime& dateTime) const;	
		bool operator >= (const DateTime& dateTime) const;	

		DateTime  operator +  (const Timespan& span) const;
		DateTime  operator -  (const Timespan& span) const;
		Timespan  operator -  (const DateTime& dateTime) const;
		DateTime& operator += (const Timespan& span);
		DateTime& operator -= (const Timespan& span);

		void MakeUTC (int tzd);
		/// Converts a local time into UTC, by applying the given time zone differential.

		void MakeLocal (int tzd);
		/// Converts a UTC time into a local time, by applying the given time zone differential.

		static bool IsLeapYear (int year);
		/// Returns true if the given year is a leap year;
		/// false otherwise.

		static int DaysOfMonth(int year, int month);
		/// Returns the number of days in the given month
		/// and year. Month is from 1 to 12.

		static bool IsValid(int year, int month, int day, int hour = 0, 
			int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);
		/// Checks if the given date and time is valid
		/// (all arguments are within a proper range).
		///
		/// Returns true if all arguments are valid, false otherwise.

	protected:	
		static double ToJulianDay(Timestamp::UtcTimeVal UTCTime);
		static double ToJulianDay(int year, int month, int day, int hour = 0,
			int minute = 0, int second = 0, int millisecond = 0,
			int microsecond = 0);
		static Timestamp::UtcTimeVal ToUtcTime(double julianDay);

		void ComputeGregorian(double julianDay);

		void ComputeDaytime();
		/// Extracts the daytime (hours, minutes, seconds, etc.) from the stored UTCTime.

	private:
		void CheckLimit(int& lower, int& higher, int limit);
		void Normalize();
		///utility functions used to correct the overflow in computeGregorian

		Timestamp::UtcTimeVal mUtcTime;
		int  mYear;
		int  mMonth;
		int  mDay;
		int  mHour;
		int  mMinute;
		int  mSecond;
		int  mMillisecond;
		int  mMicrosecond;
	};
	
	inline void Swap(DateTime& d1, DateTime& d2)
	{
		d1.Swap(d2);
	}

#include "PX2DateTime.inl"

}

#endif