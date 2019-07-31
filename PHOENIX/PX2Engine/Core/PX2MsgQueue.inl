// PX2MsgQueue.inl

//----------------------------------------------------------------------------
template <class MsgPacket>
MsgQueue<MsgPacket>::MsgQueue ()
	:
mCurIndex(0)
{
}
//----------------------------------------------------------------------------
template <class MsgPacket>
MsgQueue<MsgPacket>::~MsgQueue ()
{
	Clear();
}
//----------------------------------------------------------------------------
template <class MsgPacket>
void MsgQueue<MsgPacket>::Clear()
{
	ScopedCS cs(&mMutex);

	std::queue<MsgPacket*> &proQueue = mQueues[mCurIndex % 2];
	while (!proQueue.empty())
	{
		delete proQueue.front();
		proQueue.pop();
	}

	std::queue<MsgPacket*> & conQueue = mQueues[(mCurIndex + 1) % 2];
	while (!conQueue.empty())
	{
		delete conQueue.front();
		conQueue.pop();
	}
}
//----------------------------------------------------------------------------
template <class MsgPacket>
size_t MsgQueue<MsgPacket>::GetAllSize()
{
	ScopedCS cs(&mMutex);

	return mQueues[1].size() + mQueues[0].size();
}
//----------------------------------------------------------------------------
template <class MsgPacket>
void MsgQueue<MsgPacket>::AddMsg (MsgPacket *msg)
{
	ScopedCS cs(&mMutex);

	std::queue<MsgPacket*> &proQueue = mQueues[mCurIndex % 2];

	proQueue.push(msg);
}
//----------------------------------------------------------------------------
template <class MsgPacket>
MsgPacket *MsgQueue<MsgPacket>::PopMsg()
{
	std::queue<MsgPacket*> &que = GetConsumeQueue();
	if (!que.empty())
	{
		MsgPacket* packet = que.front();
		que.pop();
		return packet;
	}
	return 0;
}
//----------------------------------------------------------------------------
template <class MsgPacket>
MsgPacket *MsgQueue<MsgPacket>::Front()
{
	std::queue<MsgPacket*> &conQueue = GetConsumeQueue();

	if(!conQueue.empty())
	{
		MsgPacket* packet = conQueue.front();
		return packet;
	}

	return 0;
}
//----------------------------------------------------------------------------
template <class MsgPacket>
std::queue<MsgPacket*> &MsgQueue<MsgPacket>::GetConsumeQueue()
{
	std::queue<MsgPacket*> &conQueue = mQueues[(mCurIndex + 1) % 2];

	if (conQueue.empty())
	{
		ScopedCS cs(&mMutex);

		mCurIndex++;
		std::queue<MsgPacket*> &cQueue  = mQueues[(mCurIndex + 1) % 2];

		return cQueue;
	}

	return conQueue;
}
//----------------------------------------------------------------------------