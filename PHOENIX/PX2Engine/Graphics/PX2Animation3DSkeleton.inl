// PX2Animation3DSkeleton.inl

//----------------------------------------------------------------------------
inline Node *Animation3DSkeleton::GetAnimNode()
{
	return mAnimNode;
}
//----------------------------------------------------------------------------
inline std::map<FString, KeyframeControllerPtr> &Animation3DSkeleton
::GetKeyframeCtrlMap()
{
	return mKeyframeCtrlMap;
}
//----------------------------------------------------------------------------
inline KeyframeController *Animation3DSkeleton::GetRootKeyframeCtrl()
{
	return mKeyframeCtrlRoot;
}
//----------------------------------------------------------------------------