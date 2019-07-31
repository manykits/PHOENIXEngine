// PX2Exception.inl

//----------------------------------------------------------------------------
inline const Exception* Exception::nested() const
{
	return _pNested;
}
//----------------------------------------------------------------------------
inline const std::string& Exception::message() const
{
	return _msg;
}
//----------------------------------------------------------------------------
inline void Exception::message(const std::string& msg)
{
	_msg = msg;
}
//----------------------------------------------------------------------------
inline int Exception::code() const
{
	return _code;
}
//----------------------------------------------------------------------------