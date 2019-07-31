// PX2DataBase.inl

//----------------------------------------------------------------------------
inline DatabaseDriver::DatabaseType DatabaseDriver::GetDatabaseType() const
{
	return mDatabaseType;
}
//----------------------------------------------------------------------------
inline bool DatabaseDriver::IsConnected() const
{
	return mIsConnected;
}
//----------------------------------------------------------------------------
inline bool DatabaseDriver::IsSupportMultiLine() const
{
	return mIsSupportMultiLine;
}
//----------------------------------------------------------------------------