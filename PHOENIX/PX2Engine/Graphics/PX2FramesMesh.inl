// PX2FramesMesh.inl

//----------------------------------------------------------------------------
inline void FramesMesh::SetNumDir(int numFrames)
{
	mNumDir = numFrames;
	mIsNeedRecal = true;
}
//----------------------------------------------------------------------------
inline int FramesMesh::GetNumDir() const
{
	return mNumDir;
}
//----------------------------------------------------------------------------