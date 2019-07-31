// PX2MemoryPool.hpp

#ifndef PX2MEMORYPOOL_HPP
#define PX2MEMORYPOOL_HPP

#include "PX2CorePre.hpp"
#include "PX2Mutex.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM MemoryPool
	{
	public:
		MemoryPool(std::size_t blockSize, int preAlloc = 0, int maxAlloc = 0);
		~MemoryPool();

		void* Get();
		void Release(void* ptr);

		std::size_t GetBlockSize() const;
		int GetNumAllocated() const;
		int Available() const;

	private:
		MemoryPool();
		MemoryPool(const MemoryPool&);
		MemoryPool& operator = (const MemoryPool&);

		void Clear();

		enum
		{
			BLOCK_RESERVE = 128
		};

		typedef std::vector<char*> BlockVec;

		std::size_t mBlockSize;
		int mNumMaxAlloc;
		int mNumAllocated;
		BlockVec mBlocks;
		Mutex mMutex;
	};



}

#endif