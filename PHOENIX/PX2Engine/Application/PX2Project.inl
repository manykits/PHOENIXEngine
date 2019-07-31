// PX2Project.inl

//----------------------------------------------------------------------------
inline Project::ScreenOrientation Project::GetScreenOrientation() const
{
	return mScreenOrientation;
}
//----------------------------------------------------------------------------
inline const Sizef & Project::GetSize() const
{
	return mSize;
}
//----------------------------------------------------------------------------
inline float Project::GetWidth() const
{
	return mSize.Width;
}
//----------------------------------------------------------------------------
inline float Project::GetHeight() const
{
	return mSize.Height;
}
//----------------------------------------------------------------------------
inline Scene *Project::GetScene()
{
	return mScene;
}
//----------------------------------------------------------------------------
inline const std::string &Project::GetSceneFilename() const
{
	return mSceneFilename;
}
//----------------------------------------------------------------------------
inline const std::string &Project::GetUIFilename() const
{
	return mUIFilename;
}
//----------------------------------------------------------------------------
inline Logic *Project::GetLogic()
{
	return mLogic;
}
//----------------------------------------------------------------------------
inline const std::string &Project::GetLogicFilename() const
{
	return mLogicFilename;
}
//----------------------------------------------------------------------------
inline const std::vector<std::string> &Project::GetPlugins() const
{
	return mPlugins;
}
//----------------------------------------------------------------------------
inline void Project::SetEdit_UICameraPercent(float percent)
{
	mEdit_UICameraPercent = percent;
}
//----------------------------------------------------------------------------
inline float Project::GetEdit_UICameraPercent() const
{
	return mEdit_UICameraPercent;
}
//----------------------------------------------------------------------------