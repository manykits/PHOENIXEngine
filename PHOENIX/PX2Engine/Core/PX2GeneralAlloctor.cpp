// PX2GeneralAlloctor.cpp

#include "PX2GeneralAlloctor.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
char *GeneralAlloctor::msStartFree = 0;
char *GeneralAlloctor::msEndFree = 0;
size_t GeneralAlloctor::msHeapSize = 0;
Mutex GeneralAlloctor::msMutex;
GeneralAlloctor::AllocObj* volatile GeneralAlloctor::msFreeList[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//----------------------------------------------------------------------------
void *GeneralAlloctor::Allocate (size_t numBytes)
{
	void *ret = 0;

	if (0 == numBytes)
		return 0;

	if (numBytes > (size_t)MaxAllowAlloctedBytes) 
	{
		// 一级分配
		ret = malloc(numBytes);
	}
	else 
	{
		AllocObj* volatile * myFreeList = msFreeList + FreeListIndex(numBytes);

		ScopedCS cs(&msMutex);

		AllocObj *retTemp = *myFreeList;

		if (0 == retTemp)
		{
			// 如果对应的节点上没有可用的内存, 去后备区或者系统拿内存
			ret = ReFill(RoundUp(numBytes));
		}
		else
		{
			// 对应节点上有存货，取一个返回给客户，并且修改指针指向下一个可用内
			// 存块
			*myFreeList = retTemp->ObjLink;
			ret = retTemp;
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
void GeneralAlloctor::Deallocate (void *buffer, size_t numBytes)
{
	if (0==buffer || 0==numBytes)
		return;

	if (numBytes > (size_t)MaxAllowAlloctedBytes)
	{
		free(buffer);
	}
	else 
	{
		AllocObj* volatile *myFreeList = msFreeList + FreeListIndex(numBytes);

		AllocObj *temp = (AllocObj*)buffer;

		ScopedCS cs(&msMutex);

		temp->ObjLink = *myFreeList;

		*myFreeList = temp;
	}
}
//----------------------------------------------------------------------------
void *GeneralAlloctor::Reallocate (void *buffer, size_t oldSize, size_t newSize)
{
	void* result;
	size_t copySize;

	if (oldSize>(size_t)MaxAllowAlloctedBytes && newSize>(size_t)MaxAllowAlloctedBytes)
	{
		return realloc(buffer, newSize);
	}

	if (RoundUp(oldSize) == RoundUp(newSize))
		return(buffer);

	result = Allocate(newSize);
	copySize = newSize > oldSize? oldSize : newSize;
	memcpy(result, buffer, copySize);
	Deallocate(buffer, oldSize);

	return(result);
}
//----------------------------------------------------------------------------
void* GeneralAlloctor::ReFill (size_t numBytes)
{
	// 尝试分配20个
	int numObjs = 20;
	char *chunk = ChunkAlloc(numBytes, numObjs);

	AllocObj* volatile* myFreeList = 0;
	AllocObj* result = 0;
	AllocObj* curObj = 0;
	AllocObj* nextObj = 0;
	int i = 0;

	if (1 == numObjs)
		return chunk; // 只拿到一个，没有多余的内存插入链表了，直接给申请者

	// 把多余的区块加入空闲链表

	myFreeList = msFreeList + FreeListIndex(numBytes);

	result = (AllocObj*)chunk;

	*myFreeList = nextObj = (AllocObj*)(chunk + numBytes);
	for (i = 1; ; i++)
	{
		curObj = nextObj;
		nextObj = (AllocObj*)((char*)nextObj + numBytes);
		if (numObjs - 1 == i) // the last one
		{
			curObj->ObjLink = 0;
			break;
		} 
		else
		{
			curObj->ObjLink = nextObj;
		}
	}

	return(result);
}
//----------------------------------------------------------------------------
/*
* ChunkAlloc是被ReFill调用的，也就是说是在对应的空闲链表上没有存货时被调用，
* SGI的设计者会优先考虑从后备区域中拿内存。
* static char* msStartFree; static char* msEndFree;是用来标志后备区的开始和
* 结束的。在程序已开始都被初始化为0，也就是一开始后备区里面什么也没有。假设程序
* 已开始，用户申请一块32字节的内存，察看对应的空闲链表，发现没有存货，试图使用
* ReFill来提取内存，我们的良好愿望是拿20*32字节，把32字节返回，ReFill调用
* ChunkAlloc来拿这640字节，检查后备区发现祖上什么也没有留下。只能自己伸手问系统
* 要了！本着每次要都多要一点的指导精神，我们问系统要 2倍的需求，就是
* 2* 20 * 32 = 2* 640字节。如果能成，我们把640字节中的32字节返回给申请人，余下
* 的640-32字节链入对应的空闲链表。多拿的640字节做后备区使用。好了，现在用户又来
* 申请一块64字节的内存，察看对应空闲链表，发现没有存货，调用refill，我们指望拿
* 到20*64=1280字节，检查后备区，只有640字节阿，能给多少给多少吧，640字节全给他，
* 相当去640/64 = 10个要求的内存块。把1个返回给客户，把剩下的9个(640-64)字节链入
* 相应的空闲链表。
*/
char* GeneralAlloctor::ChunkAlloc (size_t numBytesPerObj, int &numObjs)
{
	char *result = 0;
	size_t totalNumBytes = numBytesPerObj * numObjs;
	size_t bytesLeft = msEndFree - msStartFree;

	if (bytesLeft >= totalNumBytes)
	{
		result = msStartFree;
		msStartFree += totalNumBytes;
		return result;
	} 
	else if (bytesLeft >= numBytesPerObj) 
	{
		numObjs = (int)(bytesLeft/numBytesPerObj);
		totalNumBytes = numBytesPerObj * numObjs;
		result = msStartFree;
		msStartFree += totalNumBytes;
		return(result);
	}
	else 
	{
		size_t bytesToGet = 2*totalNumBytes + RoundUp(msHeapSize>>4);

		if (bytesLeft > 0) 
		{
			// 利用剩余的内存,先把后备区域中剩下的内存,链接到相应的空闲链表里面去

			AllocObj* volatile* myFreeList = msFreeList + FreeListIndex(bytesLeft);

			((AllocObj*)msStartFree)->ObjLink = *myFreeList;

			*myFreeList = (AllocObj*)msStartFree;
		}

		msStartFree = (char*)malloc(bytesToGet);

		if (0 == msStartFree)
		{   
			// 极端情况，系统没有内存了。
			// 想办法从各个空闲链表里面挖一点内存出来，一旦能挖到足够的内存，调
			// 用ChunkAlloc再试一次并返回
			size_t i;
			AllocObj* volatile* myFreeList;
			AllocObj *p = 0;

			for (i=numBytesPerObj; i<=(size_t)MaxAllowAlloctedBytes; i+=(size_t)Align) 
			{				
				myFreeList = msFreeList + FreeListIndex(i);
				p = *myFreeList;
				if (0 != p) 
				{
					// 该更大的空闲链表里面尚有可用区块，卸下一块给后备

					*myFreeList = p->ObjLink;
					msStartFree = (char*)p;
					msEndFree = msStartFree + i;

					 // 此时的后备区一定能够供应至少一块需求区块的内存
					return ChunkAlloc(numBytesPerObj, numObjs);
				}
			}

			 // 程序能运行到这里，真的是山穷水尽
			msEndFree = 0;
			msStartFree = (char*)malloc(bytesToGet);
		}

		// 已经成功的从系统中要到所需要的内存,把申请到的内存拨给后备区使用,分配
		msHeapSize += bytesToGet;
		msEndFree = msStartFree + bytesToGet;
		return ChunkAlloc(numBytesPerObj, numObjs);
	}
}
//----------------------------------------------------------------------------