// PX2FixMemoryPool.inl

//----------------------------------------------------------------------------
template <typename T>
FixMemoryPool<T>::FixMemoryPool (int size)
	:
mBlockSize(size),
mBlockHeader(0),
mFreeNodeHeader(0)
{
	AllocBlocks();
}
//----------------------------------------------------------------------------
template <typename T>
FixMemoryPool<T>::~FixMemoryPool ()
{
	while (mBlockHeader != 0)
	{
		MemBlock *tmp = mBlockHeader;
		mBlockHeader = tmp->Prev;
		free(tmp->Node);
		free(tmp);
	}
}
//----------------------------------------------------------------------------
template<typename T>
void* FixMemoryPool<T>::AllocBlock (size_t size)
{
	if (size != sizeof(T))
	{
		return malloc(size);
	}

	ScopedCS sc(&mMutex);

	if (0 == mFreeNodeHeader)
	{
		ReallocBlocks();
	}

	void *buf = mFreeNodeHeader;
	mFreeNodeHeader = mFreeNodeHeader->Prev;

	return buf;
}
//----------------------------------------------------------------------------
template<typename T>
void  FixMemoryPool<T>::FreeBlock(void *memBlock , size_t size)
{
	if (0 == memBlock)
		return;

	if (sizeof(T) != size)
	{
		free(memBlock);
	}
	else
	{
		ScopedCS cs(&mMutex);

		MemNode *node = (MemNode*)memBlock;
		node->Prev = mFreeNodeHeader;
		mFreeNodeHeader = node;
	}
}
//----------------------------------------------------------------------------
template<typename T>
void FixMemoryPool<T>::AllocBlocks()
{
	MemBlock *newBlock = (MemBlock*)malloc(sizeof(MemBlock));

	newBlock->Node = (MemNode*)malloc(sizeof(MemNode)* mBlockSize);

	newBlock->Node->Prev = 0;
	for (int i = 1; i < mBlockSize; ++i)
	{
		newBlock->Node[i].Prev = &(newBlock->Node[i-1]);
	}
	mFreeNodeHeader = &newBlock->Node[mBlockSize - 1];

	newBlock->Prev = mBlockHeader;
	mBlockHeader = newBlock;
}
//----------------------------------------------------------------------------
template<typename T>
void FixMemoryPool<T>::ReallocBlocks()
{
	mBlockSize *= 2;
	AllocBlocks();
}
//----------------------------------------------------------------------------