// PX2StreamCopier.cpp

#include "PX2StreamCopier.hpp"
#include "PX2DataBuffer.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
std::streamsize StreamCopier::CopyStream(std::istream& istr,
	std::ostream& ostr, std::size_t bufferSize)
{
	assertion(bufferSize > 0, "");

	DataBuffer<char> buffer(bufferSize);
	std::streamsize len = 0;
	istr.read(buffer.begin(), bufferSize);
	std::streamsize n = istr.gcount();
	while (n > 0)
	{
		len += n;
		ostr.write(buffer.begin(), n);
		if (istr && ostr)
		{
			istr.read(buffer.begin(), bufferSize);
			n = istr.gcount();
		}
		else n = 0;
	}
	return len;
}
//----------------------------------------------------------------------------
std::streamsize StreamCopier::CopyStreamUnbuffered(std::istream& istr,
	std::ostream& ostr)
{
	char c = 0;
	std::streamsize len = 0;
	istr.get(c);
	while (istr && ostr)
	{
		++len;
		ostr.put(c);
		istr.get(c);
	}
	return len;
}
//----------------------------------------------------------------------------
std::streamsize StreamCopier::CopyToString(std::istream& istr,
	std::string& str, std::size_t bufferSize)
{
	assertion(bufferSize > 0, "");

	DataBuffer<char> buffer(bufferSize);
	std::streamsize len = 0;
	istr.read(buffer.begin(), bufferSize);
	std::streamsize n = istr.gcount();
	while (n > 0)
	{
		len += n;
		str.append(buffer.begin(), static_cast<std::string::size_type>(n));
		if (istr)
		{
			istr.read(buffer.begin(), bufferSize);
			n = istr.gcount();
		}
		else n = 0;
	}
	return len;
}
//----------------------------------------------------------------------------