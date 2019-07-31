// PX2NetServerIocp.hpp

#ifndef PX2NETSERVERIOCP_HPP
#define PX2NETSERVERIOCP_HPP

#include "PX2NetServerImp.hpp"

#if defined (WIN32) || defined(_WIN32)

namespace PX2
{

	class PX2_ENGINE_ITEM ServerIocp : public ServerImp
	{
	public:
		ServerIocp(int port, int numMaxConnects, int numMaxMsgHandlers,
			BufferEventQueue *peventque);
		virtual ~ServerIocp();

		virtual bool Start();
		virtual void Shutdown();
		virtual void OnRun();

		virtual void DisconnectClient(unsigned int clientID);
		virtual void DisconnectAll();

		int GetNumUsedOLBuffer();

	protected:
		static DWORD WINAPI _IOWorkerThreadProc(LPVOID pparam);
		static DWORD WINAPI _ListenThreadProc(LPVOID pparam);

	protected:
		bool _SetupIOWorkers();
		bool _SetupListener();
		unsigned int _DoIOWork();
		unsigned int _DoListen();

		virtual bool _AddClientSocket(Socket &s);
		virtual void _CloseClientSocket(ClientContext *pcontext);
		virtual bool _PostRead(ClientContext *pcontext, ServerBuffer *pbuf);
		virtual bool _PostWrite(ClientContext *pcontext, char *psrc, int srclen);

		bool _OnRead(ClientContext *pcontext, ServerBuffer *pbuf, unsigned int nbytes);
		bool _OnWrite(ClientContext *pcontext, ServerBuffer *pbuf, unsigned int nbytes);

		px2_socket_t mSktListenSocket;
		WSAEVENT mhEvent;
		HANDLE mhCompletionPort;
	};

#include "PX2NetServerIocp.inl"

}

#endif

#endif