// PX2PriorityQueue.hpp

#ifndef PX2PRIORITYQUEUE_HPP
#define PX2PRIORITYQUEUE_HPP

#include "PX2CorePre.hpp"
#include "PX2Assert.hpp"
#include "PX2Helps.hpp"

namespace PX2
{

	template<class T>
	class PriorityQ
	{
	public:
		PriorityQ (int maxSize);
		~PriorityQ ();

		bool IsEmpty () const;
		void Insert (const T &item);
		T Pop ();
		const T& Peek () const;

	private:
		void ReorderUpwards (std::vector<T> &heap, int index);
		void ReorderDownwards (std::vector<T> &heap, int nd, int heapSize);

		std::vector<T> mHeap;
		int mSize;
		int mMaxSize;
	};

	template <class KeyType>
	class IndexedPriorityQLow
	{
	public:
		IndexedPriorityQLow (std::vector<KeyType>& keys, int maxSize);

		bool IsEmpty () const;
		void Insert (int idx);
		int Pop ();
		void ChangePriority (const int index);

	private:
		void Swap (int a, int b);
		void ReorderUpwards (int nd);
		void ReorderDownwards (int nd, int HeapSize);

		std::vector<KeyType> &mVecKeys;
		std::vector<int> mHeap;
		std::vector<int> mInvHeap; // 记录index在mHeap是第几个
		int mSize;
		int mMaxSize;
	};

#include "PX2PriorityQueue.inl"

}

#endif