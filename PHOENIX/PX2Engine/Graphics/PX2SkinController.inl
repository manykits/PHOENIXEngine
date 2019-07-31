// PX2SkinController.inl

//----------------------------------------------------------------------------
inline void SkinController::SetUseCPU (bool cpu)
{
	mIsUseCPU = cpu;
}
//----------------------------------------------------------------------------
inline bool SkinController::IsUseCPU () const
{
	return mIsUseCPU;
}
//----------------------------------------------------------------------------
inline int SkinController::GetNumVertices () const
{
	return mNumVertices;
}
//----------------------------------------------------------------------------
inline int SkinController::GetNumBones () const
{
	return mNumBones;
}
//----------------------------------------------------------------------------
inline Node** SkinController::GetBones () const
{
	return mBones;
}
//----------------------------------------------------------------------------
inline float** SkinController::GetWeights () const
{
	return mWeights;
}
//----------------------------------------------------------------------------
inline APoint** SkinController::GetOffsets () const
{
	return mOffsets;
}
//----------------------------------------------------------------------------
inline HMatrix* SkinController::GetTMMatrixs () const
{
	return mTMMatrixs;
}
//----------------------------------------------------------------------------
inline HMatrix* SkinController::GetGPUMatrix () const
{
	return mGPUMatrixs;
}
//----------------------------------------------------------------------------