// PX2MemoryStream.cpp

#include "PX2MemoryStream.hpp"

namespace PX2
{

	//----------------------------------------------------------------------------
	MemoryIOS::MemoryIOS(char* pBuffer, std::streamsize bufferSize) :
		_buf(pBuffer, bufferSize)
	{
	}
	//----------------------------------------------------------------------------
	MemoryIOS::~MemoryIOS()
	{
	}
	//----------------------------------------------------------------------------
	MemoryInputStream::MemoryInputStream(const char* pBuffer, std::streamsize bufferSize) :
		MemoryIOS(const_cast<char*>(pBuffer), bufferSize),
		std::istream(&_buf)
	{
		}
	//----------------------------------------------------------------------------
	MemoryInputStream::~MemoryInputStream()
	{
	}
	//----------------------------------------------------------------------------
	MemoryOutputStream::MemoryOutputStream(char* pBuffer, std::streamsize bufferSize) :
		MemoryIOS(pBuffer, bufferSize),
		std::ostream(&_buf)
	{
	}
	//----------------------------------------------------------------------------
	MemoryOutputStream::~MemoryOutputStream()
	{
	}
	//----------------------------------------------------------------------------

}