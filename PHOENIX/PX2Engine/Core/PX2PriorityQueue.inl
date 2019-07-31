// PX2PriorityQueue.inl

//----------------------------------------------------------------------------
// PriorityQ
//----------------------------------------------------------------------------
template<class T>
PriorityQ<T>::PriorityQ(int maxSize)
	:
mMaxSize(maxSize),
mSize(0)
{
	mHeap.assign(maxSize+1, T());
}
//----------------------------------------------------------------------------
template<class T>
PriorityQ<T>::~PriorityQ ()
{
}
//----------------------------------------------------------------------------
template<class T>
bool PriorityQ<T>::IsEmpty() const
{
	return (mSize==0); 
}
//----------------------------------------------------------------------------
template<class T>
void PriorityQ<T>::Insert (const T &item)
{
	assertion(mSize+1<=mMaxSize, "mSize should be right range.\n");

	++mSize;

	mHeap[mSize] = item;

	ReorderUpwards(mHeap, mSize);
}
//----------------------------------------------------------------------------
template<class T>
T PriorityQ<T>::Pop()
{
	Swap(mHeap[1], mHeap[mSize]);

	ReorderDownwards(mHeap, 1, mSize-1);

	return mHeap[mSize--];
}
//----------------------------------------------------------------------------
template<class T>
const T& PriorityQ<T>::Peek () const
{
	return mHeap[1];
}
//----------------------------------------------------------------------------
template<class T>
void PriorityQ<T>::ReorderUpwards(std::vector<T>& heap, int index)
{
	//move up the heap swapping the elements until the heap is ordered
	while ( (index>1) && (heap[index/2] < heap[index]) )
	{
		Swap(heap[index/2], heap[index]);

		index /= 2;
	}
}
//----------------------------------------------------------------------------
template<class T>
void PriorityQ<T>::ReorderDownwards(std::vector<T>& heap, int nd, 
	int heapSize)
{
	// move down the heap from node nd swapping the elements until
	// the heap is reordered
	while (2*nd <= heapSize)
	{
		int child = 2 * nd;

		//set child to largest of nd's two children
		if ( (child < heapSize) && (heap[child] < heap[child+1]) )
		{
			++child;
		}

		//if this nd is smaller than its child, swap
		if (heap[nd] < heap[child])
		{
			Swap(heap[child], heap[nd]);

			// move the current node down the tree
			nd = child;
		}
		else
		{
			break;
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// IndexedPriorityQLow
//----------------------------------------------------------------------------
template<class KeyType>
IndexedPriorityQLow<KeyType>::IndexedPriorityQLow (std::vector<KeyType>& keys,
	int maxSize)
	:
mVecKeys(keys),
mMaxSize(maxSize),
mSize(0)
{
	mHeap.assign(maxSize+1, 0);
	mInvHeap.assign(maxSize+1, 0);
}
//----------------------------------------------------------------------------
template<class KeyType>
bool IndexedPriorityQLow<KeyType>::IsEmpty () const
{
	return (mSize==0);
}
//----------------------------------------------------------------------------
template<class KeyType>
void IndexedPriorityQLow<KeyType>::Insert (int index)
{
	assert((mSize+1)<=mMaxSize);

	++mSize;

	mHeap[mSize] = index;

	mInvHeap[index] = mSize;

	ReorderUpwards(mSize);
}
//----------------------------------------------------------------------------
template<class KeyType>
int IndexedPriorityQLow<KeyType>::Pop()
{
	Swap(1, mSize);

	ReorderDownwards(1, mSize-1);

	return mHeap[mSize--];
}
//----------------------------------------------------------------------------
template<class KeyType>
void IndexedPriorityQLow<KeyType>::ChangePriority(const int index)
{
	ReorderUpwards(mInvHeap[index]);
}
//----------------------------------------------------------------------------
template<class KeyType>
void IndexedPriorityQLow<KeyType>::Swap (int a, int b)
{
	int temp = mHeap[a]; 
	mHeap[a] = mHeap[b];
	mHeap[b] = temp;

	//change the handles too
	mInvHeap[mHeap[a]] = a; 
	mInvHeap[mHeap[b]] = b;
}
//----------------------------------------------------------------------------
template<class KeyType>
void IndexedPriorityQLow<KeyType>::ReorderUpwards(int nd)
{
	//move up the heap swapping the elements until the heap is ordered
	while ( (nd>1) && (mVecKeys[mHeap[nd/2]] > mVecKeys[mHeap[nd]]) )
	{      
		Swap(nd/2, nd);

		nd /= 2;
	}
}
//----------------------------------------------------------------------------
template<class KeyType>
void IndexedPriorityQLow<KeyType>::ReorderDownwards(int nd, int HeapSize)
{
	// move down the heap from node nd swapping the elements until
	// the heap is reordered
	while (2*nd <= HeapSize)
	{
		int child = 2 * nd;

		//set child to smaller of nd's two children
		if ((child < HeapSize) && (mVecKeys[mHeap[child]] > mVecKeys[mHeap[child+1]]))
		{
			++child;
		}

		//if this nd is larger than its child, swap
		if (mVecKeys[mHeap[nd]] > mVecKeys[mHeap[child]])
		{
			Swap(child, nd);

			//move the current node down the tree
			nd = child;
		}

		else
		{
			break;
		}
	}
}
//----------------------------------------------------------------------------