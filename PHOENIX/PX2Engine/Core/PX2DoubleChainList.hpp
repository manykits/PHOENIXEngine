// PX2DoubleChainList.hpp

#ifndef PX2DOUBLECHAINLIST_HPP
#define PX2DOUBLECHAINLIST_HPP

#include "PX2Memory.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ChainListNode
	{
	public:
		ChainListNode *NextNode;
		ChainListNode *PrevNode;
	};

	template <class T>
	class ChainList
	{
	public:
		ChainList ();

		//得到当前加入的node数目
		int GetNumNodes ();
		bool IsEmpty ();
		void Clear ();

		void PushFront (ChainListNode *node);
		void PushBack (ChainListNode *node);
		void PopFront ();
		void PopBack ();

		void InsertBefore (ChainListNode *node, ChainListNode *ref);
		void InsertAfter (ChainListNode *node, ChainListNode *ref);

		T *Remove (ChainListNode *node);

		bool IsNodeInList(ChainListNode *node);

		void DeleteAllNodes ();
		void ReleaseAllNodes ();
		T *BeginIterate ();
		T *Iterate (ChainListNode *pnode);
		T *Front();
		T *Back();

	private:
		ChainListNode mEndNode;
		int mNumNodes;
	};

#include "PX2DoubleChainList.inl"

}

#endif