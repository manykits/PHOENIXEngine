// PX2DoubleChainList.cpp

//----------------------------------------------------------------------------
template <typename T>
ChainList<T>::ChainList()
{
	Clear();
}
//----------------------------------------------------------------------------
template <typename T>
int ChainList<T>::GetNumNodes()
{
	return mNumNodes; 
}
//----------------------------------------------------------------------------
template <typename T>
bool ChainList<T>::IsEmpty()
{
	return mNumNodes==0; 
}
//----------------------------------------------------------------------------
template <typename T>
void ChainList<T>::Clear()
{ 
	mEndNode.NextNode = mEndNode.PrevNode = &mEndNode;
	mNumNodes = 0;
}
//----------------------------------------------------------------------------
template <typename T>
void ChainList<T>::PushFront (ChainListNode *node)
{
	InsertAfter(node, &mEndNode); 
}
//----------------------------------------------------------------------------
template <typename T>
void ChainList<T>::PushBack( ChainListNode *node )
{
	InsertBefore(node, &mEndNode); 
}
//----------------------------------------------------------------------------
template <typename T>
void ChainList<T>::PopFront()
{
	assertion(!IsEmpty(), "ChainList should not be empty.");

	Remove( mEndNode.NextNode );
}
//----------------------------------------------------------------------------
template <typename T>
void ChainList<T>::PopBack()
{
	assertion(!IsEmpty(), "ChainList should not be empty.");

	Remove( mEndNode.PrevNode );
}
//----------------------------------------------------------------------------
template <typename T>
void ChainList<T>::InsertBefore (ChainListNode *node, ChainListNode *ref)
{
	assertion(node&&ref, "node and ref must not be 0.");

	node->NextNode = ref;
	node->PrevNode = ref->PrevNode;

	ref->PrevNode->NextNode = node;
	ref->PrevNode = node;

	mNumNodes++;
}
//----------------------------------------------------------------------------
template <typename T>
void ChainList<T>::InsertAfter (ChainListNode *node, ChainListNode *ref)
{
	assertion(node&&ref, "node and ref must not be 0.");

	node->PrevNode = ref;
	node->NextNode = ref->NextNode;

	ref->NextNode->PrevNode = node;
	ref->NextNode = node;

	mNumNodes++;
}
//----------------------------------------------------------------------------
template <typename T>
T *ChainList<T>::Remove (ChainListNode *node)
{
	assertion(node&&node!=&mEndNode, 
		"node must not be 0, and must not be the last.");

	ChainListNode *pnext = node->NextNode;
	node->NextNode->PrevNode = node->PrevNode;
	node->PrevNode->NextNode = node->NextNode;

	node->NextNode = node->PrevNode = NULL;

	mNumNodes--;

	return pnext==&mEndNode ? NULL : static_cast<T *>(pnext);
}
//----------------------------------------------------------------------------
template <typename T>
bool ChainList<T>::IsNodeInList (ChainListNode *node)
{
	ChainListNode *pcur = mEndNode.NextNode;
	while (pcur != &mEndNode)
	{
		if( pcur == node ) return true;
		pcur = pcur->NextNode;
	}

	return false;
}
//----------------------------------------------------------------------------
template <typename T>
void ChainList<T>::DeleteAllNodes ()
{
	ChainListNode *cur = mEndNode.NextNode;
	while (cur != &mEndNode)
	{
		ChainListNode *next = cur->NextNode;

		if (static_cast<T *>(cur))
		{
			delete0(static_cast<T *>(cur));
			cur = 0;
		}

		cur = next;
	}
	Clear();
}
//----------------------------------------------------------------------------
template <typename T>
void ChainList<T>::ReleaseAllNodes()
{
	ChainListNode *cur = mEndNode.NextNode;
	while (cur != &mEndNode)
	{
		ChainListNode *next = cur->NextNode;

		if (static_cast<T *>(cur))
		{
			static_cast<T *>(cur)->Release();
		}

		cur = next;

	}
	Clear();
}
//----------------------------------------------------------------------------
template <typename T>
T *ChainList<T>::BeginIterate()
{ 
	return mEndNode.NextNode==&mEndNode 
		? NULL : static_cast<T *>(mEndNode.NextNode);
}
//----------------------------------------------------------------------------
template <typename T>
T *ChainList<T>::Iterate( ChainListNode *node )
{ 
	return node->NextNode==&mEndNode 
		? NULL : static_cast<T *>(node->NextNode);
}
//----------------------------------------------------------------------------
template <typename T>
T *ChainList<T>::Front()
{
	return mEndNode.NextNode==&mEndNode
		? NULL : static_cast<T *>(mEndNode.NextNode);
}
//----------------------------------------------------------------------------
template <typename T>
T *ChainList<T>::Back()
{
	return mEndNode.PrevNode==&mEndNode
		? NULL : static_cast<T *>(mEndNode.PrevNode);
}
//----------------------------------------------------------------------------