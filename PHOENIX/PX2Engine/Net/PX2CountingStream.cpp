// PX2CountingStream.cpp

#include "PX2CountingStream.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
CountingStreamBuf::CountingStreamBuf() :
_pIstr(0),
_pOstr(0),
_chars(0),
_lines(0),
_pos(0)
{
}
//----------------------------------------------------------------------------
CountingStreamBuf::CountingStreamBuf(std::istream& istr) :
_pIstr(&istr),
_pOstr(0),
_chars(0),
_lines(0),
_pos(0)
{
}
//----------------------------------------------------------------------------
CountingStreamBuf::CountingStreamBuf(std::ostream& ostr) :
_pIstr(0),
_pOstr(&ostr),
_chars(0),
_lines(0),
_pos(0)
{
}
//----------------------------------------------------------------------------
CountingStreamBuf::~CountingStreamBuf()
{
}
//----------------------------------------------------------------------------
int CountingStreamBuf::ReadFromDevice()
{
	if (_pIstr)
	{
		int c = _pIstr->get();
		if (c != -1)
		{
			++_chars;
			if (_pos++ == 0) ++_lines;
			if (c == '\n') _pos = 0;
		}
		return c;
	}
	return -1;
}
//----------------------------------------------------------------------------
int CountingStreamBuf::WriteToDevice(char c)
{
	++_chars;
	if (_pos++ == 0) ++_lines;
	if (c == '\n') _pos = 0;
	if (_pOstr) _pOstr->put(c);
	return charToInt(c);
}
//----------------------------------------------------------------------------
void CountingStreamBuf::reset()
{
	_chars = 0;
	_lines = 0;
	_pos = 0;
}
//----------------------------------------------------------------------------
void CountingStreamBuf::SetCurrentLineNumber(int line)
{
	_lines = line;
}
//----------------------------------------------------------------------------
void CountingStreamBuf::AddChars(int chars)
{
	_chars += chars;
}
//----------------------------------------------------------------------------
void CountingStreamBuf::AddLines(int lines)
{
	_lines += lines;
}
//----------------------------------------------------------------------------
void CountingStreamBuf::AddPos(int pos)
{
	_pos += pos;
}
//----------------------------------------------------------------------------
CountingIOS::CountingIOS()
{
	//poco_ios_init(&_buf);
}
//----------------------------------------------------------------------------
CountingIOS::CountingIOS(std::istream& istr) : _buf(istr)
{
	//poco_ios_init(&_buf);
}
//----------------------------------------------------------------------------
CountingIOS::CountingIOS(std::ostream& ostr) : _buf(ostr)
{
	//poco_ios_init(&_buf);
}
//----------------------------------------------------------------------------
CountingIOS::~CountingIOS()
{
}
//----------------------------------------------------------------------------
void CountingIOS::reset()
{
	_buf.reset();
}
//----------------------------------------------------------------------------
void CountingIOS::SetCurrentLineNumber(int line)
{
	_buf.SetCurrentLineNumber(line);
}
//----------------------------------------------------------------------------
void CountingIOS::AddChars(int chars)
{
	_buf.AddChars(chars);
}
//----------------------------------------------------------------------------
void CountingIOS::AddLines(int lines)
{
	_buf.AddLines(lines);
}
//----------------------------------------------------------------------------
void CountingIOS::AddPos(int pos)
{
	_buf.AddPos(pos);
}
//----------------------------------------------------------------------------
CountingStreamBuf* CountingIOS::rdbuf()
{
	return &_buf;
}
//----------------------------------------------------------------------------
CountingInputStream::CountingInputStream(std::istream& istr) : 
CountingIOS(istr), std::istream(&_buf)
{
}
//----------------------------------------------------------------------------
CountingInputStream::~CountingInputStream()
{
}
//----------------------------------------------------------------------------
CountingOutputStream::CountingOutputStream() : std::ostream(&_buf)
{
}
//----------------------------------------------------------------------------
CountingOutputStream::CountingOutputStream(std::ostream& ostr) :
CountingIOS(ostr), std::ostream(&_buf)
{
}
//----------------------------------------------------------------------------
CountingOutputStream::~CountingOutputStream()
{
}
//----------------------------------------------------------------------------