// PX2EventBuffer.hpp

#ifndef PX2NETEVENTBUFFER_HPP
#define PX2NETEVENTBUFFER_HPP

#include "PX2NetPre.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM BufferEvent
	{
	public:
		void Init(int bufsize);
		void Reset();
		void PushData(const char *pdata, int datalen);
		char *PrepareDataSpace(int datalen);

		int GetMessageID();

	public:
		enum
		{
			HEAD_SIZE = 16,
			MSGID_RESERVED = 0,
		};

	public:
		unsigned int ClientID;
		int mBufferSize;
		int mDataLength;
		char mBuffer[1];
	};


	class PX2_ENGINE_ITEM BufferEventPool
	{
	public:
		BufferEventPool(int bufsize, int num_reserve, int max_event);
		~BufferEventPool();

		BufferEvent *AllocBufferEvent();
		void FreeBufferEvent(BufferEvent *pevent);

		int GetBufferSize(){ return mBufferSize; }
		int GetNumEventMalloc(){ return mNumEventMalloc; }

	private:
		int mBufferSize;
		int mNumEventMalloc;
		int mMaxEventMalloc;
		std::vector<BufferEvent *>mPool;
	};


	class PX2_ENGINE_ITEM BufferEventQueue
	{
	public:
		// minbufsize, maxbufsize must be pow of 2
		BufferEventQueue(int minbufsize, int maxbufsize, int *max_events);
		~BufferEventQueue();

		BufferEvent *AllocBufferEvent(int nbytes);
		void FreeBufferEvent(BufferEvent *pevent);

		void InsertEventFront(BufferEvent *pevent);
		void PostBufferEvent(BufferEvent *pevent);
		BufferEvent *PopBufferEvent();

		bool PostConnectEvent(unsigned int clientid);
		bool PostDisconnectEvent(unsigned int clientid);

		int GetNumAllocEvent(){ return mnAllocEvent; }
		int GetNumEventInPool(int *num_per_pool);
		int GetEventQueLen();

	private:
		std::vector<BufferEventPool *>mPools;
		int mMinBufSizeIndex;
		int mMaxBufSizeIndex;

		int mCurUsedMemory;
		int mMemoryLimit;
		int mnAllocEvent;

		std::deque<BufferEvent *>mEventQue;

		Mutex mPoolMutex;
		Mutex mQueMutex;
	};

	const int MSGID_BYTES = 2;			//msgid用多少个字节表示
	const int MSGLEN_BYTES = 2;			//msg的长度用多少字节表示

	inline int ReadMessageID(const void *pbuffer)
	{
		return *(unsigned char *)pbuffer;
	}

	inline void WriteMessageID(void *pbuffer, int msgid)
	{
		if (1 == MSGID_BYTES)
		{
			*(unsigned char *)pbuffer = (unsigned char)msgid;
		}
		else if (2 == MSGID_BYTES)
		{
			*(unsigned short *)pbuffer = (unsigned short)msgid;
		}
	}

	inline int ReadMessageLen(const void *pbuffer)
	{
		const unsigned char *ptmp = (const unsigned char *)pbuffer;
		return ptmp[0] + (ptmp[1] << 8);
	}

	inline void WriteMessageLen(void *pbuffer, int len)
	{
		unsigned char *ptmp = (unsigned char *)pbuffer;
		ptmp[0] = len & 0xff;
		ptmp[1] = (len >> 8) & 0xff;
	}

	template<class T>
	BufferEvent *MsgToBufferEvent(BufferEventQueue *peventque, int msgid, T &msg)
	{
		int nbytes = msg.ByteSize();

		BufferEvent *pevent = peventque->AllocBufferEvent(nbytes + MSGID_BYTES);
		if (pevent == 0) return 0;

		WriteMessageID(pevent->mBuffer, msgid);
		if (!msg.SerializeToArray(pevent->mBuffer + MSGID_BYTES, nbytes))
		{
			peventque->FreeBufferEvent(pevent);
			assert(false);
			return 0;
		}

		pevent->mDataLength = nbytes + MSGID_BYTES;
		return pevent;
	}

	template<class T>
	int MsgToRawBuffer(int msgid, T &msg, char *buffer, int buffersize)
	{
		int nbytes = msg.ByteSize();
		assert(nbytes < buffersize - MSGLEN_BYTES - MSGID_BYTES);

		WriteMessageLen(buffer, (MSGID_BYTES + nbytes));
		WriteMessageID(buffer + MSGLEN_BYTES, msgid);

		if (!msg.SerializeToArray(buffer + MSGLEN_BYTES + MSGID_BYTES, nbytes))
		{
			assert(false);
			return -1;
		}

		return nbytes + MSGLEN_BYTES + MSGID_BYTES;
	}

	inline BufferEvent *MsgToBufferEventBuffer(
		BufferEventQueue *peventque,
		int msgid, const char *buf, int nbytes)
	{
		BufferEvent *pevent = peventque->AllocBufferEvent(nbytes + MSGID_BYTES);
		if (pevent == 0) return 0;

		WriteMessageID(pevent->mBuffer, msgid);
		memcpy(pevent->mBuffer + MSGID_BYTES, buf, nbytes);

		pevent->mDataLength = nbytes + MSGID_BYTES;
		return pevent;
	}

#include "PX2NetEventBuffer.inl"

}

#endif