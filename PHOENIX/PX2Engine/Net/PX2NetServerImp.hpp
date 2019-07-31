// PX2NetServerImp.hpp

#ifndef PX2NETSERVERIMP_HPP
#define PX2NETSERVERIMP_HPP

#include "PX2NetPre.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2StreamSocket.hpp"
#include "PX2ServerSocket.hpp"
#include "PX2NetEventBuffer.hpp"
#include "PX2NetServerBuffer.hpp"
#include "PX2ClientContext.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ServerImp
	{
	public:
		ServerImp(int port, int numMaxConnects, int numMaxMsgHandlers,
			BufferEventQueue *peventque);
		virtual ~ServerImp();

		virtual bool Start() = 0;
		virtual void Shutdown() = 0;
		virtual void OnRun() = 0;
		bool IsStarted() const;
		ServerSocket GetListenSocket();

		const std::vector<int> &GetThreadIDs() const;

		virtual void DisconnectClient(unsigned int clientID) = 0;
		virtual void DisconnectAll() = 0;
		virtual bool PostWrite(unsigned int clientid, char *psrc, int srclen);
		virtual bool PostBroadcast(char *psrc, int srclen);

		int GetNumUsedContext();
		int GetClientMapSize();

		ClientContext *_GetClientContext(unsigned int clientid);

	protected:
		virtual bool _AddClientSocket(Socket &s) = 0;
		virtual void _CloseClientSocket(ClientContext *pcontext) = 0;
		virtual bool _PostRead(ClientContext *pcontext, ServerBuffer *pbuf) = 0;
		virtual bool _PostWrite(ClientContext *pcontext, char *psrc, int srclen) = 0;

		ClientContext *_AllocContext(Socket &socket);
		void _FreeContext(ClientContext *pcontext);

		void _EnterPendingIO(ClientContext *pcontext);
		void _LeavePendingIO(ClientContext *pcontext);

		bool mIsStarted;
		bool mIsShutdown;
		int mListenPort;
		int mNumMaxConnects;
		int mNumMaxWorkers;
		volatile long mNumListenThread;
		volatile long mNumIOWorkerThread;

		std::vector<int> mThreadIDs;

		ServerSocket mListenSocket;

		ClientContext *mAllContext;
		std::vector<ClientContext *> mFreeContext;
		std::map<unsigned int, ClientContext *> mClientMap;
		unsigned int mCurClientID;

		Mutex mContextMapMutex;
		Mutex mContextLock;

		BufferEventQueue *mBufferEventQue;

		ServerBufferMgr mServerBufMgr;
	};
	typedef Pointer0<ServerImp> ServerImpPtr;

}

#endif