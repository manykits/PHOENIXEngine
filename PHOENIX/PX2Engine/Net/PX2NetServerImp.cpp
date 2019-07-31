// PX2NetServerImp.cpp

#include "PX2NetServerImp.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Log.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

//-----------------------------------------------------------------------------
// ServerImp
//-----------------------------------------------------------------------------
ServerImp::ServerImp(int port, int numMaxConnects, int numMaxMsgHandlers,
	BufferEventQueue *peventque) :
mIsStarted(false),
mIsShutdown(true),
mListenPort(port),
mNumMaxConnects(numMaxConnects),
mNumMaxWorkers(numMaxMsgHandlers),
mBufferEventQue(peventque),
mCurClientID(0),
mNumListenThread(0),
mNumIOWorkerThread(0),
mServerBufMgr(50000)
{
	mAllContext = new ClientContext[numMaxConnects];
	mFreeContext.reserve(numMaxConnects);

	for (int i = 0; i < numMaxConnects; i++)
	{
		mFreeContext.push_back(&mAllContext[i]);
	}
}
//-----------------------------------------------------------------------------
ServerImp::~ServerImp()
{
	delete [] mAllContext;
	mAllContext = 0;
}
//----------------------------------------------------------------------------
bool ServerImp::IsStarted() const
{
	return mIsStarted;
}
//----------------------------------------------------------------------------
ServerSocket ServerImp::GetListenSocket()
{
	return mListenSocket;
}
//----------------------------------------------------------------------------
bool ServerImp::PostWrite(unsigned int clientid, char *psrc, int srclen)
{
	ScopedCS cs(&mContextMapMutex);

	ClientContext *pcontext = _GetClientContext(clientid);
	if (pcontext)
	{
		return _PostWrite(pcontext, psrc, srclen);
	}

	assertion(false, "");
	return false;
}
//-----------------------------------------------------------------------------
bool ServerImp::PostBroadcast(char *psrc, int srclen)
{
	bool err = false;
	ScopedCS locker(&mContextMapMutex);

	std::map<unsigned int, ClientContext *>::iterator iter = mClientMap.begin();
	for (; iter != mClientMap.end(); iter++)
	{
		ClientContext *pcontext = iter->second;

		if (!_PostWrite(pcontext, psrc, srclen))
			err = true;
	}

	return !err;
}
//-----------------------------------------------------------------------------
const std::vector<int> &ServerImp::GetThreadIDs() const
{
	return mThreadIDs;
}
//-----------------------------------------------------------------------------
int ServerImp::GetNumUsedContext()
{
	ScopedCS cs(&mContextMapMutex);

	return mNumMaxConnects - int(mFreeContext.size());
}
//-----------------------------------------------------------------------------
int ServerImp::GetClientMapSize()
{
	ScopedCS cs(&mContextMapMutex);

	return int(mClientMap.size());
}
//-----------------------------------------------------------------------------
ClientContext *ServerImp::_AllocContext(Socket &socket)
{
	ScopedCS cs(&mContextMapMutex);

	size_t i = mFreeContext.size();
	if (0 == i)
	{
		PX2_LOG_ERROR("alloc context failed");
		return 0;
	}
	else
	{
		ClientContext *pcontext = mFreeContext[i - 1];
		mFreeContext.pop_back();

		mCurClientID++;
		
		if (mCurClientID == 0) 
			mCurClientID++;

		pcontext->Init(socket, mCurClientID);

		assertion(mClientMap.find(mCurClientID) == mClientMap.end(), 
			"%d must not in mClientMap", mCurClientID);

		mClientMap[mCurClientID] = pcontext;

		return pcontext;
	}
}
//-----------------------------------------------------------------------------
void ServerImp::_FreeContext(ClientContext *pcontext)
{
	ScopedCS cs(&mContextMapMutex);

	//!!!
	assertion(!pcontext->TheSocket.IsValid(), "");

	if (pcontext->TheBufferEvent)
	{
		mBufferEventQue->FreeBufferEvent(pcontext->TheBufferEvent);
		pcontext->TheBufferEvent = NULL;
	}

	mClientMap.erase(pcontext->ClientID);
	mFreeContext.push_back(pcontext);
}
//-----------------------------------------------------------------------------
ClientContext *ServerImp::_GetClientContext(unsigned int clientid)
{
	ScopedCS cs(&mContextMapMutex);

	std::map<unsigned int, ClientContext *>::iterator iter = 
		mClientMap.find(clientid);
	if (iter == mClientMap.end())
	{
		return 0;
	}
	else
	{
		return iter->second;
	}
}
//-----------------------------------------------------------------------------
void ServerImp::_EnterPendingIO(ClientContext *pcontext)
{
	ScopedCS cs(&mContextLock);

	pcontext->NumPendingIO++;
}
//----------------------------------------------------------------------------
void ServerImp::_LeavePendingIO(ClientContext *pcontext)
{
	bool needfree = false;
	{
		ScopedCS tmplock(&mContextLock);

		pcontext->NumPendingIO--;

		if (pcontext->NumPendingIO == 0 && !pcontext->TheSocket.IsValid())
			needfree = true;
	}

	if (needfree)
	{
		mBufferEventQue->PostDisconnectEvent(pcontext->ClientID);

		_FreeContext(pcontext);
	}
}
//----------------------------------------------------------------------------