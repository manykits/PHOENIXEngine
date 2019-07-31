// PX2Renderable.inl

//----------------------------------------------------------------------------
inline Renderable::PrimitiveType Renderable::GetPrimitiveType () const
{
    return mType;
}
//----------------------------------------------------------------------------
inline void Renderable::SetVertexFormat (VertexFormat* vformat)
{
    mVFormat = vformat;
}
//----------------------------------------------------------------------------
inline const VertexFormat* Renderable::GetVertexFormat () const
{
    return mVFormat;
}
//----------------------------------------------------------------------------
inline VertexFormat* Renderable::GetVertexFormat ()
{
    return mVFormat;
}
//----------------------------------------------------------------------------
inline void Renderable::SetVertexBuffer (VertexBuffer* vbuffer)
{
    mVBuffer = vbuffer;
}
//----------------------------------------------------------------------------
inline const VertexBuffer* Renderable::GetVertexBuffer () const
{
    return mVBuffer;
}
//----------------------------------------------------------------------------
inline VertexBuffer* Renderable::GetVertexBuffer ()
{
    return mVBuffer;
}
//----------------------------------------------------------------------------
inline void Renderable::SetIndexBuffer (IndexBuffer* ibuffer)
{
    mIBuffer = ibuffer;
}
//----------------------------------------------------------------------------
inline const IndexBuffer* Renderable::GetIndexBuffer () const
{
    return mIBuffer;
}
//----------------------------------------------------------------------------
inline IndexBuffer* Renderable::GetIndexBuffer ()
{
    return mIBuffer;
}
//----------------------------------------------------------------------------
inline void Renderable::SetShareDBObject_V (DBObject_V *obj)
{
	mDBObject_V = obj;
}
//----------------------------------------------------------------------------
inline const DBObject_V *Renderable::GetShareDBObject_V () const
{
	return mDBObject_V;
}
//----------------------------------------------------------------------------
inline DBObject_V *Renderable::GetShareDBObject_V ()
{
	return mDBObject_V;
}
//----------------------------------------------------------------------------
inline void Renderable::SetShareDBObject_I (DBObject_I *obj)
{
	mDBObject_I = obj;
}
//----------------------------------------------------------------------------
inline const DBObject_I *Renderable::GetShareDBObject_I () const
{
	return mDBObject_I;
}
//----------------------------------------------------------------------------
inline DBObject_I *Renderable::GetShareDBObject_I ()
{
	return mDBObject_I;
}
//----------------------------------------------------------------------------
inline void Renderable::SetIBOffset (int offset)
{
	mIBOffset = offset;
}
//----------------------------------------------------------------------------
inline int Renderable::GetIBOffset () const
{
	return mIBOffset;
}
//----------------------------------------------------------------------------
inline void Renderable::SetIBNumElements (int num)
{
	mIBNumElements = num;
}
//----------------------------------------------------------------------------
inline int Renderable::GetIBNumElements () const
{
	return mIBNumElements;
}
//----------------------------------------------------------------------------
inline void Renderable::SetUseShareBuffers (bool use)
{
	mIsUseShareBuffers = use;
}
//----------------------------------------------------------------------------
inline bool Renderable::IsUseShareBuffers () const
{
	return mIsUseShareBuffers;
}
//----------------------------------------------------------------------------
inline const Bound& Renderable::GetModelBound () const
{
    return mModelBound;
}
//----------------------------------------------------------------------------
inline Bound& Renderable::GetModelBound ()
{
    return mModelBound;
}
//----------------------------------------------------------------------------
inline void Renderable::SetUseBoundPick (bool isUseBoundPick)
{
	mIsUseBoundPick = isUseBoundPick;
}
//----------------------------------------------------------------------------
inline bool Renderable::IsUseBoundPick () const
{
	return mIsUseBoundPick;
}
//----------------------------------------------------------------------------
inline void Renderable::_SetTransparent (bool transparent)
{
	mSortIndex = (mSortIndex&0xffff0000)|(transparent?1:0);
}
//----------------------------------------------------------------------------
inline bool Renderable::IsFakeTransparent() const
{
	return mIsFakeTransparent;
}
//----------------------------------------------------------------------------
inline bool Renderable::IsTransparent () const
{
	return (mSortIndex&1); 
}
//----------------------------------------------------------------------------
inline Renderable::RenderLayer Renderable::GetRenderLayer () const
{
	return mLayer;
}
//----------------------------------------------------------------------------
inline int Renderable::GetSubLayer () const
{
	return mSubLayer;
}
//----------------------------------------------------------------------------
inline unsigned int Renderable::GetSortIndex () const
{
	return mSortIndex;
}
//----------------------------------------------------------------------------
inline void Renderable::SetMaterialInstance (MaterialInstance* material)
{
    mMaterialInstance = material;
}
//----------------------------------------------------------------------------
inline MaterialInstance* Renderable::GetMaterialInstance () const
{
    return mMaterialInstance;
}
//----------------------------------------------------------------------------
inline void Renderable::SetShine (Shine *shine)
{
	mDefaultShine = shine;
}
//----------------------------------------------------------------------------
inline Shine *Renderable::GetShine ()
{
	return mDefaultShine;
}
//----------------------------------------------------------------------------
inline const Shine *Renderable::GetShine () const
{
	return mDefaultShine;
}
//----------------------------------------------------------------------------
inline Shine *Renderable::GetBakeShine ()
{
	return mBakeShine;
}
//----------------------------------------------------------------------------
inline const Shine *Renderable::GetBakeShine () const
{
	return mBakeShine;
}
//----------------------------------------------------------------------------
inline void Renderable::SetBakeObject (bool bakeObject)
{
	mIsBackObject = bakeObject;
}
//----------------------------------------------------------------------------
inline bool Renderable::IsBakeObject () const
{
	return mIsBackObject;
}
//----------------------------------------------------------------------------
inline void Renderable::SetBakeTarget (bool bakeTarget)
{
	mIsBackTarget = bakeTarget;
}
//----------------------------------------------------------------------------
inline bool Renderable::IsBakeTarget () const
{
	return mIsBackTarget;
}
//----------------------------------------------------------------------------
inline void Renderable::SetBakeSizeType (BakeSizeType type)
{
	mBakeSizeType = type;
}
//----------------------------------------------------------------------------
inline Renderable::BakeSizeType Renderable::GetBakeSizeType () const
{
	return mBakeSizeType;
}
//----------------------------------------------------------------------------
inline const std::string &Renderable::GetLightTexture () const
{
	return mLightTexPath;
}
//----------------------------------------------------------------------------
inline const std::string &Renderable::GetNormalTexture () const
{
	return mNormalTexPath;
}
//----------------------------------------------------------------------------
inline float Renderable::GetFogInfulenceParam_Height () const
{
	return mFogIP_Height;
}
//----------------------------------------------------------------------------
inline float Renderable::GetFogInfulenceParam_Distance () const
{
	return mFogIP_Distance;
}
//----------------------------------------------------------------------------
inline void Renderable::SetPhysicsType (PhysicsType type)
{
	mPhysicsType = type;
}
//----------------------------------------------------------------------------
inline Renderable::PhysicsType Renderable::GetPhysicsType () const
{
	return mPhysicsType;
}
//----------------------------------------------------------------------------
inline void Renderable::SetPhysicsParam (const Float3 &param)
{
	mPhysicsParam = param;
}
//----------------------------------------------------------------------------
inline const Float3 &Renderable::GetPhysicsParam () const
{
	return mPhysicsParam;
}
//----------------------------------------------------------------------------
inline void Renderable::EnableShareDraw(bool enableShareDraw)
{
	mIsEnableShareDraw = enableShareDraw;
}
//----------------------------------------------------------------------------
inline bool Renderable::IsEnableShareDraw() const
{
	return mIsEnableShareDraw;
}
//----------------------------------------------------------------------------