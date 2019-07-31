// PX2NetServerBuffer.hpp

#ifndef PX2NETSERVERBUFFER_HPP
#define PX2NETSERVERBUFFER_HPP

#include "PX2NetPre.hpp"
#include "PX2Mutex.hpp"

namespace PX2
{

	enum
	{
		IOSERVER_READ = 1,
		IOSERVER_WRITE = 2,
	};

	const int MAX_OVERLAPBUF_ALLSZIE = 4096*2; // 4096
#if defined(_WIN32) || defined(WIN32)
	const int MAX_OVERLAPBUF_SIZE = MAX_OVERLAPBUF_ALLSZIE - sizeof(OVERLAPPED) - sizeof(WSABUF) - 3 * 4;
#else
	const int MAX_OVERLAPBUF_SIZE = MAX_OVERLAPBUF_ALLSZIE - 3 * 4;
#endif

	class PX2_ENGINE_ITEM ServerBuffer
	{
	public:
		void Init(int op);
		int GetOperation() { return mOperation; }

		void PrepareRecv();
		void PrepareSend();

		char *GetFreeBuf();
		int GetFreePos();
		int GetFirstFreeBufLen();
		int GetDataLen(){ return mDataLength; }
		int GetFirstDataBufLen();

		int PushData(const char *psrc, int srclen);
		void PopDataTo(char *pdest, int len);
		unsigned short PopUShort();
		unsigned int PopUInt();

		void DataAppended(int len);

	public:
#if defined(_WIN32) || defined(WIN32)
		OVERLAPPED mSysData;
		WSABUF mWSABuf;
#endif
		int mOperation;
		int mDataPos;
		int mDataLength;

		char mBuffer[MAX_OVERLAPBUF_SIZE];
	};

	class ServerBufferMgr
	{
	public:
		ServerBufferMgr(int numMaxBuffer);
		~ServerBufferMgr();

		ServerBuffer *AllocBuffer(int op);
		void FreeBuffer(ServerBuffer *p);

		int GetUsedBuffers();

	private:
		int mMaxBuffers;
		Mutex mMutex;
		ServerBuffer *mAllBuffers;
		std::vector<ServerBuffer *> mFreeBuffers;
	};
	
}

#endif