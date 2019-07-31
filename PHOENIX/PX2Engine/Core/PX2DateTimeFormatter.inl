// PX2DateTimeFormatter.inl

//----------------------------------------------------------------------------
inline std::string DateTimeFormatter::format(const Timestamp& timestamp, const std::string& fmt, int timeZoneDifferential)
{
	DateTime dateTime(timestamp);
	return format(dateTime, fmt, timeZoneDifferential);
}
//----------------------------------------------------------------------------
inline std::string DateTimeFormatter::format(const DateTime& dateTime, const std::string& fmt, int timeZoneDifferential)
{
	std::string result;
	result.reserve(64);
	append(result, dateTime, fmt, timeZoneDifferential);
	return result;
}
//----------------------------------------------------------------------------
inline std::string DateTimeFormatter::format(const LocalDateTime& dateTime, const std::string& fmt)
{
	return format(dateTime.mDateTime, fmt, dateTime.TZD());
}
//----------------------------------------------------------------------------
inline std::string DateTimeFormatter::format(const Timespan& timespan, const std::string& fmt)
{
	std::string result;
	result.reserve(32);
	append(result, timespan, fmt);
	return result;
}
//----------------------------------------------------------------------------
inline void DateTimeFormatter::append(std::string& str, const Timestamp& timestamp, const std::string& fmt, int timeZoneDifferential)
{
	DateTime dateTime(timestamp);
	append(str, dateTime, fmt, timeZoneDifferential);
}
//----------------------------------------------------------------------------
inline std::string DateTimeFormatter::TzdISO(int timeZoneDifferential)
{
	std::string result;
	result.reserve(8);
	TzdISO(result, timeZoneDifferential);
	return result;
}
//----------------------------------------------------------------------------
inline std::string DateTimeFormatter::TzdRFC(int timeZoneDifferential)
{
	std::string result;
	result.reserve(8);
	TzdRFC(result, timeZoneDifferential);
	return result;
}
//----------------------------------------------------------------------------