// PX2SwitchNode.inl

//----------------------------------------------------------------------------
inline SwitchNode *SwitchNode::Create()
{
	return new0 SwitchNode();
}
//----------------------------------------------------------------------------
inline void SwitchNode::SetActiveChild (int activeChild)
{
    assertion(
        activeChild == SN_INVALID_CHILD || activeChild < (int)mChild.size(),
        "Invalid active child specified\n");

    mActiveChild = activeChild;
}
//----------------------------------------------------------------------------
inline void SwitchNode::SetActiveChild (Movable *activeChild)
{
	for (int i=0; i<GetNumChildren(); i++)
	{
		if (GetChild(i) == activeChild)
		{
			mActiveChild = i;
		}
	}
}
//----------------------------------------------------------------------------
inline int SwitchNode::GetActiveChild () const
{
    return mActiveChild;
}
//----------------------------------------------------------------------------
inline Movable *SwitchNode::GetActiveChildPointer ()
{
	return GetChild((mActiveChild));
}
//----------------------------------------------------------------------------
inline void SwitchNode::DisableAllChildren ()
{
    mActiveChild = SN_INVALID_CHILD;
}
//----------------------------------------------------------------------------
