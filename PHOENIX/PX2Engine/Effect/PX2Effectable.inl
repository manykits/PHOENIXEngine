// PX2Effectable.inl

//----------------------------------------------------------------------------
inline EffectableController *Effectable::GetEffectableController () const
{
	return mEffectableCtrl;
}
//----------------------------------------------------------------------------
inline bool Effectable::IsFixedBound ()
{
	return mIsFixedBound;
}
//----------------------------------------------------------------------------
inline APoint Effectable::GetFixedBoundCenter ()
{
	return mFixedCenter;
}
//----------------------------------------------------------------------------
inline float Effectable::GetFixedBoundRadius ()
{
	return mFixedBoundRadius;
}
//----------------------------------------------------------------------------
inline bool Effectable::IsLocal () const
{
	return mIsLocal;
}
//----------------------------------------------------------------------------
inline void Effectable::SetEmitSizeX (float val)
{
	mEmitSizeX = val;
}
//----------------------------------------------------------------------------
inline float Effectable::GetEmitSizeX () const
{
	return mEmitSizeX;
}
//----------------------------------------------------------------------------
inline void Effectable::SetEmitSizeY (float val)
{
	mEmitSizeY = val;
}
//----------------------------------------------------------------------------
inline float Effectable::GetEmitSizeY () const
{
	return mEmitSizeY;
}
//----------------------------------------------------------------------------
inline void Effectable::SetEmitSizeZ (float val)
{
	mEmitSizeZ = val;
}
//----------------------------------------------------------------------------
inline float Effectable::GetEmitSizeZ () const
{
	return mEmitSizeZ;
}
//----------------------------------------------------------------------------
inline void Effectable::SetEmitSizeXYZ (float val)
{
	mEmitSizeX = val;
	mEmitSizeY = val;
	mEmitSizeZ = val;
}
//----------------------------------------------------------------------------
inline void Effectable::SetEmitColor (const Float3 &color)
{
	mEmitColor = color;
}
//----------------------------------------------------------------------------
inline const Float3 &Effectable::GetEmitColor () const
{
	return mEmitColor;
}
//----------------------------------------------------------------------------
inline void Effectable::SetEmitAlpha (float alpha)
{
	mEmitAlpha = alpha;
}
//----------------------------------------------------------------------------
inline float Effectable::GetEmitAlpha () const
{
	return mEmitAlpha;
}
//----------------------------------------------------------------------------
inline void Effectable::SetEmitLife (float life)
{
	mEmitLife = life;
}
//----------------------------------------------------------------------------
inline float Effectable::GetEmitLife () const
{
	return mEmitLife;
}
//----------------------------------------------------------------------------
inline Effectable::FaceType Effectable::GetFaceType () const
{
	return mFaceType;
}
//----------------------------------------------------------------------------
inline Effectable::MaterialType Effectable::GetMaterialType () const
{
	return mMtlType;
}
//----------------------------------------------------------------------------
inline bool Effectable::IsBackCull () const
{
	return mIsBackCull;
}
//----------------------------------------------------------------------------
inline Effectable::BlendMode Effectable::GetBlendMode () const
{
	return mBlendMode;
}
//----------------------------------------------------------------------------
inline Effectable::TexMode Effectable::GetTexMode () const
{
	return mTexMode;
}
//----------------------------------------------------------------------------
inline const std::string &Effectable::GetTex () const
{
	return mTexFilename;
}
//----------------------------------------------------------------------------
inline void Effectable::GetTexPack_Ele (std::string &packName, std::string &eleName) const
{
	packName = mTexFilename;
	eleName = mTexPackEleName;
}
//----------------------------------------------------------------------------
inline const TexPackElement &Effectable::GetTexPackElement () const
{
	return mTexPackEle;
}
//----------------------------------------------------------------------------
inline const std::vector<TexPackElement> &Effectable::
	GetTexPackAnim_Frames () const
{
	return mTexPackAnim_Frames;
}
//----------------------------------------------------------------------------
inline const std::string &Effectable::GetTexAnim () const
{
	return mTexFilename;
}
//----------------------------------------------------------------------------
inline const Float2 &Effectable::GetTexAnim_Repeat () const
{
	return mTexAnim_Repeat;
}
//----------------------------------------------------------------------------
inline const std::string &Effectable::GetTexPackAnim () const
{
	return mTexFilename;
}
//----------------------------------------------------------------------------
inline void Effectable::SetAnimInterval (float seconds)
{
	mAnimInterval = seconds;
}
//----------------------------------------------------------------------------
inline float Effectable::GetAnimInterval () const
{
	return mAnimInterval;
}
//----------------------------------------------------------------------------
inline void Effectable::SetAnimStartDoRandom (bool doRandom)
{
	mIsAnimStartDoRandom = doRandom;
}
//----------------------------------------------------------------------------
inline bool Effectable::IsAnimStartDoRandom () const
{
	return mIsAnimStartDoRandom;
}
//----------------------------------------------------------------------------
inline void Effectable::SetUserNumAnimFrames (int userNumAnimFrames)
{
	mUserNumAnimFrames = userNumAnimFrames;
}
//----------------------------------------------------------------------------
inline int Effectable::GetUserNumAnimFrames () const
{
	return mUserNumAnimFrames;
}
//----------------------------------------------------------------------------
inline int Effectable::GetNumTexAnimTexPackAnimFrames () const
{
	return (int)mTexPackAnim_Frames.size();
}
//----------------------------------------------------------------------------
inline void Effectable::SetAnimFramesPlayOnce (bool playOnce)
{
	mIsAnimFramesPlayOnce = playOnce;
}
//----------------------------------------------------------------------------
inline bool Effectable::IsAnimFramesPlayOnce () const
{
	return mIsAnimFramesPlayOnce;
}
//----------------------------------------------------------------------------
inline const std::string &Effectable::GetMaskTex () const
{
	return mTexMaskFilename;
}
//----------------------------------------------------------------------------
inline const Float2 &Effectable::GetEmitUV0_Offset () const
{
	return mEmitUV0_Offset;
}
//----------------------------------------------------------------------------
inline const Float2 &Effectable::GetEmitUV0_Repeat () const
{
	return mEmitUV0_Repeat;
}
//----------------------------------------------------------------------------
inline const Float2 &Effectable::GetEmitUV0_Speed () const
{
	return mEmitUV0_Speed;
}
//----------------------------------------------------------------------------
inline const Float2 &Effectable::GetUV1_Offset () const
{
	return mUV1_Offset;
}
//----------------------------------------------------------------------------
inline const Float2 &Effectable::GetUV1_Repeat () const
{
	return mUV1_Repeat;
}
//----------------------------------------------------------------------------
inline const Float2 &Effectable::GetUV1_Speed () const
{
	return mUV1_Speed;
}
//----------------------------------------------------------------------------
inline void Effectable::SetBufferEverGenerated (bool gen)
{
	mIsBufferEverGenerated = gen;
}
//----------------------------------------------------------------------------
inline bool Effectable::IsBufferEverGenerated () const
{
	return mIsBufferEverGenerated;
}
//----------------------------------------------------------------------------
inline Effectable::CoordinateType Effectable::GetCoordinateType0 () const
{
	return mCoordinateType0;
}
//----------------------------------------------------------------------------
 inline Effectable::CoordinateType Effectable::GetCoordinateType1 () const
 {
	 return mCoordinateType1;
 }
 //----------------------------------------------------------------------------