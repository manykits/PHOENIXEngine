// PX2DateTimeFormatter.cpp

#include "PX2DateTimeFormatter.hpp"
#include "PX2DateTimeFormat.hpp"
#include "PX2NumberFormatter.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void DateTimeFormatter::append(std::string& str, const LocalDateTime& dateTime, const std::string& fmt)
{
	DateTimeFormatter::append(str, dateTime.mDateTime, fmt, dateTime.TZD());
}
//----------------------------------------------------------------------------
void DateTimeFormatter::append(std::string& str, const DateTime& dateTime, const std::string& fmt, int timeZoneDifferential)
{
	std::string::const_iterator it = fmt.begin();
	std::string::const_iterator end = fmt.end();
	while (it != end)
	{
		if (*it == '%')
		{
			if (++it != end)
			{
				switch (*it)
				{
				case 'w': str.append(DateTimeFormat::WEEKDAY_NAMES[dateTime.DayOfWeek()], 0, 3); break;
				case 'W': str.append(DateTimeFormat::WEEKDAY_NAMES[dateTime.DayOfWeek()]); break;
				case 'b': str.append(DateTimeFormat::MONTH_NAMES[dateTime.Month() - 1], 0, 3); break;
				case 'B': str.append(DateTimeFormat::MONTH_NAMES[dateTime.Month() - 1]); break;
				case 'd': NumberFormatter::append0(str, dateTime.Day(), 2); break;
				case 'e': NumberFormatter::append(str, dateTime.Day()); break;
				case 'f': NumberFormatter::append(str, dateTime.Day(), 2); break;
				case 'm': NumberFormatter::append0(str, dateTime.Month(), 2); break;
				case 'n': NumberFormatter::append(str, dateTime.Month()); break;
				case 'o': NumberFormatter::append(str, dateTime.Month(), 2); break;
				case 'y': NumberFormatter::append0(str, dateTime.Year() % 100, 2); break;
				case 'Y': NumberFormatter::append0(str, dateTime.Year(), 4); break;
				case 'H': NumberFormatter::append0(str, dateTime.Hour(), 2); break;
				case 'h': NumberFormatter::append0(str, dateTime.HourAMPM(), 2); break;
				case 'a': str.append(dateTime.IsAM() ? "am" : "pm"); break;
				case 'A': str.append(dateTime.IsAM() ? "AM" : "PM"); break;
				case 'M': NumberFormatter::append0(str, dateTime.Minute(), 2); break;
				case 'S': NumberFormatter::append0(str, dateTime.Second(), 2); break;
				case 's': NumberFormatter::append0(str, dateTime.Second(), 2);
					str += '.';
					NumberFormatter::append0(str, dateTime.Millisecond() * 1000 + dateTime.Microsecond(), 6);
					break;
				case 'i': NumberFormatter::append0(str, dateTime.Millisecond(), 3); break;
				case 'c': NumberFormatter::append(str, dateTime.Millisecond() / 100); break;
				case 'F': NumberFormatter::append0(str, dateTime.Millisecond() * 1000 + dateTime.Microsecond(), 6); break;
				case 'z': TzdISO(str, timeZoneDifferential); break;
				case 'Z': TzdRFC(str, timeZoneDifferential); break;
				default:  str += *it;
				}
				++it;
			}
		}
		else str += *it++;
	}
}
//----------------------------------------------------------------------------
void DateTimeFormatter::append(std::string& str, const Timespan& timespan, const std::string& fmt)
{
	std::string::const_iterator it = fmt.begin();
	std::string::const_iterator end = fmt.end();
	while (it != end)
	{
		if (*it == '%')
		{
			if (++it != end)
			{
				switch (*it)
				{
				case 'd': NumberFormatter::append(str, timespan.Days()); break;
				case 'H': NumberFormatter::append0(str, timespan.Hours(), 2); break;
				case 'h': NumberFormatter::append(str, timespan.TotalHours()); break;
				case 'M': NumberFormatter::append0(str, timespan.Minutes(), 2); break;
				case 'm': NumberFormatter::append(str, timespan.TotalMinutes()); break;
				case 'S': NumberFormatter::append0(str, timespan.Seconds(), 2); break;
				case 's': NumberFormatter::append(str, timespan.TotalSeconds()); break;
				case 'i': NumberFormatter::append0(str, timespan.Milliseconds(), 3); break;
				case 'c': NumberFormatter::append(str, timespan.Milliseconds() / 100); break;
				case 'F': NumberFormatter::append0(str, timespan.Milliseconds() * 1000 + timespan.Microseconds(), 6); break;
				default:  str += *it;
				}
				++it;
			}
		}
		else str += *it++;
	}
}
//----------------------------------------------------------------------------
void DateTimeFormatter::TzdISO(std::string& str, int timeZoneDifferential)
{
	if (timeZoneDifferential != UTC)
	{
		if (timeZoneDifferential >= 0)
		{
			str += '+';
			NumberFormatter::append0(str, timeZoneDifferential / 3600, 2);
			str += ':';
			NumberFormatter::append0(str, (timeZoneDifferential % 3600) / 60, 2);
		}
		else
		{
			str += '-';
			NumberFormatter::append0(str, -timeZoneDifferential / 3600, 2);
			str += ':';
			NumberFormatter::append0(str, (-timeZoneDifferential % 3600) / 60, 2);
		}
	}
	else str += 'Z';
}
//----------------------------------------------------------------------------
void DateTimeFormatter::TzdRFC(std::string& str, int timeZoneDifferential)
{
	if (timeZoneDifferential != UTC)
	{
		if (timeZoneDifferential >= 0)
		{
			str += '+';
			NumberFormatter::append0(str, timeZoneDifferential / 3600, 2);
			NumberFormatter::append0(str, (timeZoneDifferential % 3600) / 60, 2);
		}
		else
		{
			str += '-';
			NumberFormatter::append0(str, -timeZoneDifferential / 3600, 2);
			NumberFormatter::append0(str, (-timeZoneDifferential % 3600) / 60, 2);
		}
	}
	else str += "GMT";
}
//----------------------------------------------------------------------------