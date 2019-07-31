// PX2Exception.cpp

#include "PX2Exception.hpp"
#include <typeinfo>
using namespace PX2;

//----------------------------------------------------------------------------
Exception::Exception(int code) : _pNested(0), _code(code)
{
}
//----------------------------------------------------------------------------
Exception::Exception(const std::string& msg, int code) :
_msg(msg), _pNested(0), _code(code)
{
}
//----------------------------------------------------------------------------
Exception::Exception(const std::string& msg, const std::string& arg, int code) :
_msg(msg), _pNested(0), _code(code)
{
	if (!arg.empty())
	{
		_msg.append(": ");
		_msg.append(arg);
	}
}
//----------------------------------------------------------------------------
Exception::Exception(const std::string& msg, const Exception& nested, int code) :
_msg(msg), _pNested(nested.clone()), _code(code)
{
}
//----------------------------------------------------------------------------
Exception::Exception(const Exception& exc) :
std::exception(exc),
_msg(exc._msg),
_code(exc._code)
{
	_pNested = exc._pNested ? exc._pNested->clone() : 0;
}
//----------------------------------------------------------------------------
Exception::~Exception() throw()
{
	delete _pNested;
}
//----------------------------------------------------------------------------
Exception& Exception::operator = (const Exception& exc)
{
	if (&exc != this)
	{
		Exception* newPNested = exc._pNested ? exc._pNested->clone() : 0;
		delete _pNested;
		_msg = exc._msg;
		_pNested = newPNested;
		_code = exc._code;
	}
	return *this;
}
//----------------------------------------------------------------------------
const char* Exception::name() const throw()
{
	return "Exception";
}
//----------------------------------------------------------------------------
const char* Exception::className() const throw()
{
	return typeid(*this).name();
}
//----------------------------------------------------------------------------
const char* Exception::what() const throw()
{
	return name();
}
//----------------------------------------------------------------------------
std::string Exception::displayText() const
{
	std::string txt = name();
	if (!_msg.empty())
	{
		txt.append(": ");
		txt.append(_msg);
	}
	return txt;
}
//----------------------------------------------------------------------------
void Exception::extendedMessage(const std::string& arg)
{
	if (!arg.empty())
	{
		if (!_msg.empty()) _msg.append(": ");
		_msg.append(arg);
	}
}
//----------------------------------------------------------------------------
Exception* Exception::clone() const
{
	return new Exception(*this);
}
//----------------------------------------------------------------------------
void Exception::rethrow() const
{
	throw *this;
}
//----------------------------------------------------------------------------
PX2_IMPLEMENT_EXCEPTION(RuntimeException, Exception, "Runtime exception")
PX2_IMPLEMENT_EXCEPTION(NotFoundException, RuntimeException, "Not found")
PX2_IMPLEMENT_EXCEPTION(TimeoutException, RuntimeException, "Time out")
PX2_IMPLEMENT_EXCEPTION(IOException, RuntimeException, "I/O error")
PX2_IMPLEMENT_EXCEPTION(DataException, RuntimeException, "Data error")
PX2_IMPLEMENT_EXCEPTION(DataFormatException, DataException, "Bad data format")
PX2_IMPLEMENT_EXCEPTION(SyntaxException, DataException, "Syntax error")
PX2_IMPLEMENT_EXCEPTION(OutOfMemoryException, RuntimeException, "Out of memory")


PX2_IMPLEMENT_EXCEPTION(URISyntaxException, SyntaxException, "Bad URI syntax")
PX2_IMPLEMENT_EXCEPTION(PathSyntaxException, SyntaxException, "Bad path syntax")
PX2_IMPLEMENT_EXCEPTION(FileException, IOException, "File access error")
PX2_IMPLEMENT_EXCEPTION(FileExistsException, FileException, "File exists")
PX2_IMPLEMENT_EXCEPTION(FileNotFoundException, FileException, "File not found")
PX2_IMPLEMENT_EXCEPTION(PathNotFoundException, FileException, "Path not found")
PX2_IMPLEMENT_EXCEPTION(FileReadOnlyException, FileException, "File is read-only")
PX2_IMPLEMENT_EXCEPTION(FileAccessDeniedException, FileException, "Access to file denied")
PX2_IMPLEMENT_EXCEPTION(CreateFileException, FileException, "Cannot create file")
PX2_IMPLEMENT_EXCEPTION(OpenFileException, FileException, "Cannot open file")
PX2_IMPLEMENT_EXCEPTION(WriteFileException, FileException, "Cannot write file")
PX2_IMPLEMENT_EXCEPTION(ReadFileException, FileException, "Cannot read file")
