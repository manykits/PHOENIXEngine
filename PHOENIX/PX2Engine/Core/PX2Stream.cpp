// PX2Stream.cpp

#include "PX2Stream.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Stream::Stream () :
mStreamType(ST_MAX_TYPE)
{
}
//----------------------------------------------------------------------------
Stream::~Stream ()
{
}
//----------------------------------------------------------------------------
int Stream::GetStreamingSize (const std::string& input)
{
	// string被写入的硬盘，首先写入字符串长度，其次写入字符集，不需要写入
	// "\0"终结符.写入的字符串被垫到4字节的倍数大小.
    int length = (int)strlen(input.c_str());
    int padding = 0;
    if (length > 0)
    {
        padding = (length % 4);
        if (padding > 0)
        {
            padding = 4 - padding;
        }
    }
    return sizeof(int) + length*sizeof(char) + padding;
}
//----------------------------------------------------------------------------