// PX2NetIocpOverlapBuffer.cpp

#include "PX2NetServerBuffer.hpp"
#include "PX2Assert.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;

//-----------------------------------------------------------------------------
// ServerBuffer
//-----------------------------------------------------------------------------
void ServerBuffer::Init(int op)
{
	assertion(MAX_OVERLAPBUF_ALLSZIE == (int)sizeof(ServerBuffer), "");

	mOperation = op;
	mDataPos = 0;
	mDataLength = 0;
}
//-----------------------------------------------------------------------------
void ServerBuffer::PrepareRecv()
{
#if defined(_WIN32) || defined(WIN32)
	mWSABuf.buf = mBuffer + GetFreePos();
	mWSABuf.len = GetFirstFreeBufLen();
#endif
}
//-----------------------------------------------------------------------------
void ServerBuffer::PrepareSend()
{
#if defined(_WIN32) || defined(WIN32)
	mWSABuf.buf = mBuffer + mDataPos;
	mWSABuf.len = GetFirstDataBufLen();
#endif
}
//-----------------------------------------------------------------------------
char *ServerBuffer::GetFreeBuf()
{
	return mBuffer + GetFreePos();
}
//-----------------------------------------------------------------------------
int ServerBuffer::GetFreePos()
{
	int pos = mDataPos + mDataLength;

	if (pos >= MAX_OVERLAPBUF_SIZE) 
		pos -= MAX_OVERLAPBUF_SIZE;

	return pos;
}
//-----------------------------------------------------------------------------
int ServerBuffer::GetFirstFreeBufLen()
{
	int free_pos = GetFreePos();
	int free_len = MAX_OVERLAPBUF_SIZE - mDataLength;

	if (free_pos + free_len > MAX_OVERLAPBUF_SIZE) 
		return MAX_OVERLAPBUF_SIZE - free_pos;
	else 
		return free_len;
}
//-----------------------------------------------------------------------------
int ServerBuffer::GetFirstDataBufLen()
{
	if (mDataPos + mDataLength > MAX_OVERLAPBUF_SIZE)
		return MAX_OVERLAPBUF_SIZE - mDataPos;
	else
		return mDataLength;
}
//-----------------------------------------------------------------------------
int ServerBuffer::PushData(const char *psrc, int srclen)
{
	int free_pos = GetFreePos();
	int free_len1 = GetFirstFreeBufLen();
	int free_len2 = MAX_OVERLAPBUF_SIZE - mDataLength - free_len1;

	int len1 = srclen <= free_len1 ? srclen : free_len1;
	memcpy(mBuffer + free_pos, psrc, len1);

	int len2 = 0;
	if (len1 < srclen && free_len2>0)
	{
		len2 = srclen - len1 < free_len2 ? srclen - len1 : free_len2;
		memcpy(mBuffer, psrc + len1, len2);
	}

	mDataLength += len1 + len2;

	return len1 + len2;
}
//-----------------------------------------------------------------------------
void ServerBuffer::PopDataTo(char *pdest, int len)
{
	assertion(len <= mDataLength, "");

	int datalen1 = GetFirstDataBufLen();

	int len1 = len <= datalen1 ? len : datalen1;
	memcpy(pdest, mBuffer + mDataPos, len1);

	if (len1 < len)
	{
		memcpy(pdest + len1, mBuffer, len - len1);
	}

	mDataPos += len;

	if (mDataPos >= MAX_OVERLAPBUF_SIZE) 
		mDataPos -= MAX_OVERLAPBUF_SIZE;

	mDataLength -= len;
}
//-----------------------------------------------------------------------------
unsigned short ServerBuffer::PopUShort()
{
	unsigned short v;
	PopDataTo((char *)&v, 2);
	return v;
}
//-----------------------------------------------------------------------------
unsigned int ServerBuffer::PopUInt()
{
	unsigned int v;
	PopDataTo((char *)&v, 4);
	return v;
}
//-----------------------------------------------------------------------------
void ServerBuffer::DataAppended(int len)
{
	mDataLength += len;
	assert(mDataLength <= MAX_OVERLAPBUF_SIZE);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// ServerBufferMgr
//-----------------------------------------------------------------------------
ServerBufferMgr::ServerBufferMgr(int numMaxBuffer) :
mMaxBuffers(numMaxBuffer)
{
	mAllBuffers = new ServerBuffer[numMaxBuffer];

	mFreeBuffers.reserve(numMaxBuffer);
	for (int i = 0; i < numMaxBuffer; i++)
	{
		mFreeBuffers.push_back(mAllBuffers + i);
	}
}
//-----------------------------------------------------------------------------
ServerBufferMgr::~ServerBufferMgr()
{
	delete[] mAllBuffers;
}
//-----------------------------------------------------------------------------
ServerBuffer *ServerBufferMgr::AllocBuffer(int op)
{
	ScopedCS cs(&mMutex);

	size_t i = mFreeBuffers.size();
	if (i == 0)
	{
		return 0;
	}
	else
	{
		ServerBuffer *pbuf = mFreeBuffers[i - 1];
		pbuf->Init(op);
		mFreeBuffers.pop_back();

		return pbuf;
	}
}
//-----------------------------------------------------------------------------
void ServerBufferMgr::FreeBuffer(ServerBuffer *p)
{
	ScopedCS cs(&mMutex);

	mFreeBuffers.push_back(p);
}
//-----------------------------------------------------------------------------
int ServerBufferMgr::GetUsedBuffers()
{
	ScopedCS cs(&mMutex);

	return mMaxBuffers - int(mFreeBuffers.size());
}
//-----------------------------------------------------------------------------