// PX2HTTPBufferAllocator.cpp

#include "PX2HTTPBufferAllocator.hpp"
#include "PX2Memory.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
char* HTTPBufferAllocator::Allocate(std::streamsize size)
{
	assert(size == BUFFER_SIZE);

	return new1<char>((int)size);
}
//----------------------------------------------------------------------------
void HTTPBufferAllocator::Deallocate(char* ptr, std::streamsize size)
{
	assert(size == BUFFER_SIZE);
	delete1(ptr);
}
//----------------------------------------------------------------------------