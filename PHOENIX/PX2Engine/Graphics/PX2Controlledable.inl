// PX2Controlledable.inl

//----------------------------------------------------------------------------
inline bool Controlledable::IsSelfCtrled () const
{
	return mIsSelfCtrled;
}
//----------------------------------------------------------------------------
inline void Controlledable::SetAutoPlay(bool autoPlay)
{
	mIsAutoPlay = autoPlay;
}
//----------------------------------------------------------------------------
inline bool Controlledable::IsAutoPlay() const
{
	return mIsAutoPlay;
}
//----------------------------------------------------------------------------
inline int Controlledable::GetNumControllers () const
{
    return (int)mControllers.size();
}
//----------------------------------------------------------------------------
inline Controller* Controlledable::GetController (int i) const
{
    if (0 <= i && i < GetNumControllers())
    {
        return mControllers[i];
    }

    return 0;
}
//----------------------------------------------------------------------------
template <typename CLASSTYPE>
CLASSTYPE *Controlledable::GetController() const
{
	for (int i = 0; i < (int)mControllers.size(); i++)
	{
		CLASSTYPE *ctrl = DynamicCast<CLASSTYPE>(mControllers[i]);
		if (ctrl)
			return ctrl;
	}

	return 0;
}
//----------------------------------------------------------------------------