// PX2RandomStream.hpp

#ifndef PX2RANDOMSTREAM_HPP
#define PX2RANDOMSTREAM_HPP

#include "PX2CorePre.hpp"
#include "PX2BufferedStreamBuf.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM RandomBuf : public BasicBufferedStreamBuf<char, std::char_traits<char> >
	{
	public:
		RandomBuf();
		~RandomBuf();
		int readFromDevice(char* buffer, std::streamsize length);
	};

	class PX2_ENGINE_ITEM RandomIOS : public virtual std::ios
	{
	public:
		RandomIOS();
		~RandomIOS();
		RandomBuf* rdbuf();

	protected:
		RandomBuf _buf;
	};

	class PX2_ENGINE_ITEM RandomInputStream : public RandomIOS, public std::istream
	{
	public:
		RandomInputStream();
		~RandomInputStream();
	};

}

#endif