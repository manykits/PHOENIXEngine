// PX2BillboardNode.inl

//----------------------------------------------------------------------------
inline void BillboardNode::AlignTo (Camera* camera)
{
	mCamera = camera;
}
//----------------------------------------------------------------------------
inline void BillboardNode::SetAlignType (BillboardNode::BillboardAlignType type)
{
	mAlignType = type;
}
//----------------------------------------------------------------------------
inline BillboardNode::BillboardAlignType BillboardNode::GetAlignType () const
{
	return mAlignType;
}
//----------------------------------------------------------------------------