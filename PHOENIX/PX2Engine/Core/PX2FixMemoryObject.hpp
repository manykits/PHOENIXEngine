// PX2FixMemoryObject.hpp

#ifndef PX2FIXMEMORYOBJECT_HPP
#define PX2FIXMEMORYOBJECT_HPP

#include "PX2CorePre.hpp"
#include "PX2FixMemoryPool.hpp"

namespace PX2
{

	template <typename Obj>
	class FixMemoryObject
	{
	public:
		static void *operator new (size_t numBytes)
		{
			return msMemPool.AllocBlock(numBytes);
		}

		static void operator delete (void *buffer, size_t numBytes)
		{
			msMemPool.FreeBlock(buffer, numBytes);
		}

		static FixMemoryPool<Obj> msMemPool;
	};

	template <typename Obj>
	FixMemoryPool<Obj> FixMemoryObject<Obj>::msMemPool;
		
}

#endif