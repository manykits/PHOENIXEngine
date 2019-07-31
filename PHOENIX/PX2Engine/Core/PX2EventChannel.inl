// PX2EventChannel.inl

template <int N>
EventChannel &EventChannel::AddUserChannel ()
{
	assertion(N>=0, "");
	assertion(N<=UserChannelLength, "");

	AddChannel(UserChannel<N>::Value);
	
	return (*this);
}
//----------------------------------------------------------------------------
template <int N>
EventChannel &EventChannel::RemoveUserChannel ()
{
	assertion(N>=0, "");
	assertion(N<=UserChannelLength, "");

	RemoveChannel(UserChannel<N>::Value);

	return (*this);
}
//----------------------------------------------------------------------------
template <int N>
EventChannel &EventChannel::AddSystemChannel ()
{
	assertion(N>=0, "");
	assertion(N<=SystemChannelLength, "");

	AddChannel(SystemChannel<N>::Value);

	return (*this);
}
//----------------------------------------------------------------------------
template <int N>
EventChannel &EventChannel::RemoveSystemChannel ()
{
	assertion(N>=0, "");
	assertion(N<=SystemChannelLength, "");

	RemoveChannel(SystemChannel<N>::Value);

	return (*this);
}
//----------------------------------------------------------------------------