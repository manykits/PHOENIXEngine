// PX2GeneralAlloctor.hpp

#ifndef PX2GENERALALLOCTOR_HPP
#define PX2GENERALALLOCTOR_HPP

#include "PX2CorePre.hpp"
#include "PX2Mutex.hpp"

namespace PX2
{

	// 模仿C++ SGISTL的二级内存分配法则，线程安全的内存分配器
	/*
	* 通常用来分配平凡分配释放的小块内存。
	* 把512字节以下的内存分成128份，以4个字节递增。于是，空闲链表一共有128个节
	* 点，每个节点指向一个用于服务该节点所代表内存大小的空闲链表。
	*
	*	a) 检查numBytes子节对应的空闲链表中有没有
	*	b) 如果没有，看看后备区里面，能不能分配
	*	c) 如果后备区域也没有了，才问系统要
	*
	*/
	class PX2_ENGINE_ITEM GeneralAlloctor
	{
	public:
		static void *Allocate (size_t numBytes);
		static void Deallocate (void *buffer, size_t numBytes);
		static void *Reallocate (void *buffer, size_t oldSize, size_t newSize);

	private:
		static const int Align = 4;
		static const int MaxAllowAlloctedBytes = 512;
		static const int NumFreelists = 128;

		union AllocObj
		{
			union AllocObj *ObjLink;
			char ChientData[1];
		};
		static AllocObj* volatile msFreeList[NumFreelists]; 

		// 如果numBytes=11, (11 + 4-1)/4 -1 = 2, 2就是入口位置
		static  size_t FreeListIndex(size_t numBytes) ;

		// 考虑一下这样的情景: 申请者要一块大小为11字节的内存，由于11不属于我
		// 们预先切好的4的倍数，所以先做一个RoundUp,去试图分配12字节大小的内存
		// (11 + 3) & (~11) = 12
		static size_t RoundUp (size_t numBytes);

		// 返回一个大小为numBytes的内存
		/* 
		* 内部可以建立多个numBytes物体的链表，如果这个链表中有未使用的，直接从
		* 中挑选一个。
		*/
		static void* ReFill (size_t numBytes);
		static char* ChunkAlloc (size_t numBytesPerObj, int &numObjs);

		static char* msStartFree;
		static char* msEndFree;
		static size_t msHeapSize;
		static Mutex msMutex;
	};

#include "PX2GeneralAlloctor.inl"

}

#endif