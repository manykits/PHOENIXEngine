// PX2Application.inl

//----------------------------------------------------------------------------
inline const std::string &Application::GetHostName() const
{
	return mHostName;
}
//----------------------------------------------------------------------------
inline bool Application::IsInEditor() const
{
	return mIsInEditor;
}
//----------------------------------------------------------------------------
inline AppBoostInfo &Application::GetBoostInfo()
{
	return mBoostInfo;
}
//----------------------------------------------------------------------------
inline const Sizef &Application::GetBoostSize() const
{
	return mBoostInfo.BoostSize;
}
//----------------------------------------------------------------------------
inline const std::string &Application::GetBoostProjectName() const
{
	return mBoostInfo.ProjectName;
}
//----------------------------------------------------------------------------
inline const std::string &Application::GetProjectName() const
{
	return mProjectName;
}
//----------------------------------------------------------------------------
inline const std::string &Application::GetProjectFilePath() const
{
	return mProjectFilePath;
}
//----------------------------------------------------------------------------
inline const Sizef &Application::GetScreenSize() const
{
	return mScreenSize;
}
//----------------------------------------------------------------------------
inline AppBoostInfo::PlayLogicMode Application::GetPlayLogicMode() const
{
	return mBoostInfo.ThePlayLogicMode;
}
//----------------------------------------------------------------------------
inline Canvas *Application::GetEngineCanvas()
{
	return mEngineCanvas;
}
//----------------------------------------------------------------------------
inline Application::PlayType Application::GetPlayType() const
{
	return mPlayType;
}
//----------------------------------------------------------------------------