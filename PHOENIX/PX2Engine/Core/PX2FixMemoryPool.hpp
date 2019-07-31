// PX2FixMemoryPool.hpp

#ifndef PX2FIXMEMORYPOOL_HPP
#define PX2FIXMEMORYPOOL_HPP

#include "PX2CorePre.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Memory.hpp"

namespace PX2
{

	template <typename T>
	class FixMemoryPool
	{
	public:
		FixMemoryPool (int size=1);
		~FixMemoryPool ();

		void *AllocBlock (size_t size);
		void FreeBlock (void *memBlock , size_t size );

	private:
		void AllocBlocks ();
		void ReallocBlocks ();

		struct MemNode
		{
			MemNode *Prev;
			char Data[sizeof(T) - sizeof(MemNode*)];
		};

		struct MemBlock
		{
			MemBlock *Prev;
			MemNode *Node;
		};

		MemBlock *mBlockHeader;
		MemNode *mFreeNodeHeader;
		int mBlockSize;
		Mutex mMutex;
	};

#include "PX2FixMemoryPool.inl"

}

#endif