// PX2ResourceManager.inl

//----------------------------------------------------------------------------
inline CurlObj *ResourceManager::GetCurlObj()
{
	return mCurl;
}
//----------------------------------------------------------------------------
inline void ResourceManager::DDSKeepCompressed (bool keep)
{
	mDDSKeepCompressed = keep;
}
//----------------------------------------------------------------------------
inline bool ResourceManager::IsDDSKeepCompressed() const
{
	return mDDSKeepCompressed;
}
//----------------------------------------------------------------------------
inline bool ResourceManager::IsUseGarbageCollect () const
{
	return mIsUseGarbageCollect;
}
//----------------------------------------------------------------------------
inline void ResourceManager::SetGarbageCollectTime (float seconds)
{
	mGarbageCollectTime = seconds;
}
//----------------------------------------------------------------------------
inline float ResourceManager::GetGarbageCollectTime () const
{
	return mGarbageCollectTime;
}
//----------------------------------------------------------------------------
inline void ResourceManager::SetVersion(const ResourceVersion &version)
{
	mVersion = version;
}
//----------------------------------------------------------------------------
inline const ResourceVersion &ResourceManager::GetVersion() const
{
	return mVersion;
}
//----------------------------------------------------------------------------
inline const std::string &ResourceManager::GetDataUpdateFromPath() const
{
	return mDataUpdateFromPath;
}
//----------------------------------------------------------------------------
inline const std::string &ResourceManager::GetDataUpdateWritePath() const
{
	return mDataUpdateWritePath;
}
//----------------------------------------------------------------------------
inline ResourceManager::ResourceFileTable &ResourceManager::
	GetDataFiletable ()
{
	return mLocalDataFileTable;
}
//----------------------------------------------------------------------------
inline ResourceManager::ResourceFileTable &ResourceManager::
	GetDataUpdateFileTable ()
{
	return mDataUpdateFileTable;
}
//----------------------------------------------------------------------------