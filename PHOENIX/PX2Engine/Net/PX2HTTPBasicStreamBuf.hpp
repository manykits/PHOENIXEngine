// PX2HTTPBasicStreamBuf.hpp

#ifndef PX2HTTPBASICSTREAMBUF_HPP
#define PX2HTTPBASICSTREAMBUF_HPP

#include "PX2BufferedStreamBuf.hpp"
#include "PX2HTTPBufferAllocator.hpp"

namespace PX2
{

	typedef BasicBufferedStreamBuf<char, std::char_traits<char>, HTTPBufferAllocator> HTTPBasicStreamBuf;

}

#endif