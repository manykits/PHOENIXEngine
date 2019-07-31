// PX2MemoryPool.cpp

#include "PX2MemoryPool.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Exception.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
MemoryPool::MemoryPool(std::size_t blockSize, int preAlloc, int maxAlloc) :
mBlockSize(blockSize),
mNumMaxAlloc(maxAlloc),
mNumAllocated(preAlloc)
{
	assert(maxAlloc == 0 || maxAlloc >= preAlloc);
	assert(preAlloc >= 0 && maxAlloc >= 0);

	int r = BLOCK_RESERVE;
	if (preAlloc > r)
		r = preAlloc;
	if (maxAlloc > 0 && maxAlloc < r)
		r = maxAlloc;
	mBlocks.reserve(r);

	try
	{
		for (int i = 0; i < preAlloc; ++i)
		{
			mBlocks.push_back(new char[mBlockSize]);
		}
	}
	catch (...)
	{
		Clear();
		throw;
	}
}
//----------------------------------------------------------------------------
MemoryPool::~MemoryPool()
{
	Clear();
}
//----------------------------------------------------------------------------
void MemoryPool::Clear()
{
	for (BlockVec::iterator it = mBlocks.begin(); it != mBlocks.end(); ++it)
	{
		delete[] * it;
	}
	mBlocks.clear();
}
//----------------------------------------------------------------------------
void* MemoryPool::Get()
{
	ScopedCS lock(&mMutex);

	if (mBlocks.empty())
	{
		if (mNumMaxAlloc == 0 || mNumAllocated < mNumMaxAlloc)
		{
			++mNumAllocated;
			return new char[mBlockSize];
		}
		else
		{
			throw OutOfMemoryException("MemoryPool exhausted");
		}
	}
	else
	{
		char* ptr = mBlocks.back();
		mBlocks.pop_back();
		return ptr;
	}
}
//----------------------------------------------------------------------------
void MemoryPool::Release(void* ptr)
{
	ScopedCS lock(&mMutex);

	try
	{
		mBlocks.push_back(reinterpret_cast<char*>(ptr));
	}
	catch (...)
	{
		delete[] reinterpret_cast<char*>(ptr);
	}
}
//----------------------------------------------------------------------------
std::size_t MemoryPool::GetBlockSize() const
{
	return mBlockSize;
}
//----------------------------------------------------------------------------
int MemoryPool::GetNumAllocated() const
{
	return mNumAllocated;
}
//----------------------------------------------------------------------------
int MemoryPool::Available() const
{
	return (int)mBlocks.size();
}
//----------------------------------------------------------------------------