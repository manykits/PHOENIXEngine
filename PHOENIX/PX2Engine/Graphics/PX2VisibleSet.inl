// PX2VisibleSet.inl

//----------------------------------------------------------------------------
inline void VisibleSet::SetSortFun (SortFun fun)
{
	mSortFun = fun;
}
//----------------------------------------------------------------------------
inline SortFun VisibleSet::GetSortFun () const
{
	return mSortFun;
}
//----------------------------------------------------------------------------
inline int VisibleSet::GetNumVisible () const
{
    return mNumVisible;
}
//----------------------------------------------------------------------------
inline const std::vector<Renderable*>& VisibleSet::GetAllVisible () const
{
    return mVisible;
}
//----------------------------------------------------------------------------
inline Renderable* VisibleSet::GetVisible (int i) const
{
    assertion(0 <= i && i < mNumVisible, "Invalid index to GetVisible\n");
    return mVisible[i];
}
//----------------------------------------------------------------------------
inline void VisibleSet::Clear ()
{
	mVisible.clear();
	mNumVisible = 0;
}
//----------------------------------------------------------------------------