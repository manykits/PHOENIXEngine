// PX2GraphicsRoot.inl

//----------------------------------------------------------------------------
inline bool GraphicsRoot::IsInEditor () const
{
	return mIsInEditor;
}
//----------------------------------------------------------------------------
inline bool GraphicsRoot::IsUIShareDraw() const
{
	return mIsUIShareDraw;
}
//----------------------------------------------------------------------------
inline void GraphicsRoot::SetCurEnvirParamController(EnvirParamController *param)
{
	mCurEnvirParamController = param;
}
//----------------------------------------------------------------------------
inline EnvirParamController *GraphicsRoot::GetCurEnvirParamController()
{
	return mCurEnvirParamController;
}
//----------------------------------------------------------------------------
inline void GraphicsRoot::SetCurUpdateCamera(Camera *camera)
{
	mCurUpdateCamera = camera;
}
//----------------------------------------------------------------------------
inline Camera *GraphicsRoot::GetCurUpdateCamera()
{
	return mCurUpdateCamera;
}
//----------------------------------------------------------------------------
inline RenderWindow *GraphicsRoot::GetMainWindow()
{
	return mMainRenderWindow;
}
//----------------------------------------------------------------------------
inline void GraphicsRoot::SetPlayType(PlayType type)
{
	mPlayType = type;
}
//----------------------------------------------------------------------------
inline GraphicsRoot::PlayType GraphicsRoot::GetPlayType() const
{
	return mPlayType;
}
//----------------------------------------------------------------------------
inline TriMesh *GraphicsRoot::GetXYPlane()
{
	return mTriMeshXY;
}
//----------------------------------------------------------------------------
inline TriMesh *GraphicsRoot::GetXZPlane()
{
	return mTriMeshXZ;
}
//----------------------------------------------------------------------------
inline TriMesh *GraphicsRoot::GetYZPlane()
{
	return mTriMeshYZ;
}
//----------------------------------------------------------------------------
