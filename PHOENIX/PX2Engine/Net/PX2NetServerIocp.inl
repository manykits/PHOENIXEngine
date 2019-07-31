// PX2NetServerIocp.inl

#if defined (WIN32) || defined(_WIN32)

//-----------------------------------------------------------------------------
inline int ServerIocp::GetNumUsedOLBuffer()
{
	return mServerBufMgr.GetUsedBuffers();
}
//-----------------------------------------------------------------------------

#endif