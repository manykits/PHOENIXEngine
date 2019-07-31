// PX2EventBuffer.cpp

#include "PX2NetEventBuffer.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void BufferEvent::PushData(const char *pdata, int datalen)
{
	if(mDataLength+datalen > mBufferSize)
	{
		PX2_LOG_ERROR(
		"PushData get wrong params, datalen=%d, mDataLength=%d, mBufferSize=%d",
			datalen, mDataLength, mBufferSize);
		return;
	}

	memcpy(mBuffer+mDataLength, pdata, datalen);
	mDataLength += datalen;
}
//----------------------------------------------------------------------------
char *BufferEvent::PrepareDataSpace(int datalen)
{
	if(mDataLength+datalen > mBufferSize)
	{
		PX2_LOG_ERROR(
		"PrepareDataSpace get wrong params, datalen=%d, mDataLength=%d, mBufferSize=%d", 
			datalen, mDataLength, mBufferSize);
		return 0;
	}

	char *pret = mBuffer + mDataLength;
	mDataLength += datalen;

	return pret;
}
//----------------------------------------------------------------------------
BufferEventPool::BufferEventPool(int bufsize, int num_reserve, int max_event): 
mBufferSize(bufsize),
mNumEventMalloc(0), 
mMaxEventMalloc(max_event)
{
	for(int i=0; i<num_reserve; i++)
	{
		BufferEvent *pevent =
			(BufferEvent *)malloc(mBufferSize+BufferEvent::HEAD_SIZE);
		pevent->Init(mBufferSize);
		mPool.push_back(pevent);

		mNumEventMalloc++;
	}
}
//----------------------------------------------------------------------------
BufferEventPool::~BufferEventPool()
{
	for(size_t i=0; i<mPool.size(); i++)
	{
		free(mPool[i]);
	}
}
//----------------------------------------------------------------------------
BufferEvent *BufferEventPool::AllocBufferEvent()
{
	if(mPool.empty())
	{
		if(mNumEventMalloc >= mMaxEventMalloc) return 0;

		int buflen = mBufferSize+BufferEvent::HEAD_SIZE;
		BufferEvent *pevent = (BufferEvent *)malloc(buflen);
		if(pevent == 0) return 0;

		pevent->Init(mBufferSize);
		mNumEventMalloc++;
		return pevent;
	}
	else
	{
		BufferEvent *pevent = mPool.back();
		pevent->Reset();
		mPool.pop_back();
		return pevent;
	}
}
//----------------------------------------------------------------------------
void BufferEventPool::FreeBufferEvent(BufferEvent *pevent)
{
	if(pevent->mBufferSize != mBufferSize)
	{
		PX2_LOG_ERROR("wrong bufferevent, input_size=%d, size=%d", 
			pevent->mBufferSize, mBufferSize);
		return;
	}
	mPool.push_back(pevent);
}
//----------------------------------------------------------------------------
inline bool IsPowerOfTwo( int n )
{
	return (((n & (n-1)) == 0) && (n != 0));
}
//----------------------------------------------------------------------------
inline int PrevIntLog2( unsigned x )
{
	float fx;
	unsigned int rx;

	if( x<=1 ) return x - 1;

	fx = (float)(int)(x);
	rx = *(unsigned int*)(&fx);

	return (rx >> 23) - 127;
}
//----------------------------------------------------------------------------
inline int NextIntLog2( unsigned x )
{
	float fx;
	unsigned int rx;

	if (x <= 1) return x - 1;

	fx = (float)(int)(x - 1);
	rx = *(unsigned int*)(&fx);

	return (rx >> 23) - 126;
}
//----------------------------------------------------------------------------
BufferEventQueue::BufferEventQueue(int minbufsize, int maxbufsize, 
	int *max_events) : 
mnAllocEvent(0)
{
	if(!IsPowerOfTwo(minbufsize) || !IsPowerOfTwo(maxbufsize))
	{
		PX2_LOG_ERROR("wrong minbufsize=%d, maxbufsize=%d", minbufsize,
			maxbufsize);
	}

	mMinBufSizeIndex = PrevIntLog2(minbufsize);
	mMaxBufSizeIndex = PrevIntLog2(maxbufsize);

	int nreserve = 0;
	int npool = mMaxBufSizeIndex-mMinBufSizeIndex+1;
	mPools.resize(npool);
	for(int i=0; i<npool; i++)
	{
		int bufsize = 1<<(i+mMinBufSizeIndex);

		mPools[i] = new BufferEventPool(bufsize, nreserve, max_events[i]);
	}
}
//----------------------------------------------------------------------------
BufferEventQueue::~BufferEventQueue()
{
	size_t i;
	for(i=0; i<mPools.size(); i++)
	{
		delete mPools[i];
	}
}
//----------------------------------------------------------------------------
BufferEvent *BufferEventQueue::AllocBufferEvent(int nbytes)
{
	ScopedCS lock(&mPoolMutex);

	int index = NextIntLog2(nbytes);
	if(index > mMaxBufSizeIndex)
	{
		PX2_LOG_ERROR("alloc too large buffer");
		return 0;
	}

	if(index < mMinBufSizeIndex) index = mMinBufSizeIndex;
	BufferEvent *pevent = mPools[index-mMinBufSizeIndex]->AllocBufferEvent();
	if (pevent != 0)
	{
		mnAllocEvent++;
	}
	else
	{
		PX2_LOG_ERROR("alloc buffer return 0");
	}

	return pevent;
}
//----------------------------------------------------------------------------
void BufferEventQueue::FreeBufferEvent(BufferEvent *pevent)
{
	ScopedCS lock(&mPoolMutex);

	if(pevent == 0)
	{
		PX2_LOG_ERROR("freebufferevent 0");
		return;
	}

	int index = NextIntLog2(pevent->mBufferSize);
	if(index<mMinBufSizeIndex && index>mMaxBufSizeIndex)
	{
		PX2_LOG_ERROR(
			"freebufferevent received wrong buffer, buffersize=%d",
			pevent->mBufferSize);
		return;
	}

	mnAllocEvent--;
	mPools[index-mMinBufSizeIndex]->FreeBufferEvent(pevent);
}
//----------------------------------------------------------------------------
void BufferEventQueue::InsertEventFront(BufferEvent *pevent)
{
	ScopedCS lock(&mQueMutex);

	if(!mEventQue.empty() && pevent->GetMessageID()
		==mEventQue.front()->GetMessageID()) //多次插入reconnect消息导致问题
	{
		return;
	}

	mEventQue.push_front(pevent);
}
//----------------------------------------------------------------------------
void BufferEventQueue::PostBufferEvent(BufferEvent *pevent)
{
	ScopedCS lock(&mQueMutex);

	mEventQue.push_back(pevent);
}
//----------------------------------------------------------------------------
BufferEvent *BufferEventQueue::PopBufferEvent()
{
	ScopedCS lock(&mQueMutex);

	if(mEventQue.empty())
	{
		return 0;
	}
	else
	{
		BufferEvent *pevent = mEventQue.front();
		mEventQue.pop_front();
		return pevent;
	}
}
//----------------------------------------------------------------------------
bool BufferEventQueue::PostConnectEvent(unsigned int clientid)
{
	BufferEvent *pevent = AllocBufferEvent(MSGID_BYTES+1);
	if(pevent == 0) return false;

	pevent->ClientID = clientid;
	WriteMessageID(pevent->mBuffer, BufferEvent::MSGID_RESERVED);
	pevent->mBuffer[MSGID_BYTES] = 0;
	pevent->mDataLength = MSGID_BYTES+1;

	PostBufferEvent(pevent);

	return true;
}
//----------------------------------------------------------------------------
bool BufferEventQueue::PostDisconnectEvent(unsigned int clientid)
{
	BufferEvent *pevent = AllocBufferEvent(MSGID_BYTES+1);
	if(pevent == 0) return false;

	pevent->ClientID = clientid;
	WriteMessageID(pevent->mBuffer, BufferEvent::MSGID_RESERVED);
	pevent->mBuffer[MSGID_BYTES] = 1;
	pevent->mDataLength = MSGID_BYTES+1;

	PostBufferEvent(pevent);

	return true;
}
//----------------------------------------------------------------------------
int BufferEventQueue::GetNumEventInPool(int *num_per_pool)
{
	ScopedCS lock(&mPoolMutex);

	int count = 0;
	for(size_t i=0; i<mPools.size(); i++)
	{
		num_per_pool[count++] = mPools[i]->GetNumEventMalloc();
	}

	return count;
}
//----------------------------------------------------------------------------
int BufferEventQueue::GetEventQueLen()
{
	ScopedCS lock(&mQueMutex);

	return int(mEventQue.size());
}
//----------------------------------------------------------------------------