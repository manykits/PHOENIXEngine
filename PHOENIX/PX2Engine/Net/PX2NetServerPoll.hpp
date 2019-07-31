// PX2NetServerPoll.hpp

#ifndef PX2NETSERVERPOLL_HPP
#define PX2NETSERVERPOLL_HPP

#include "PX2NetServerImp.hpp"
#include "PX2ServerSocket.hpp"
#include "PX2Runnable.hpp"
#include "PX2Thread.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ServerPoll : public ServerImp, public Runnable
	{
	public:
		ServerPoll(int port, int numMaxConnects, int numMaxMsgHandlers,
			BufferEventQueue *peventque);
		virtual ~ServerPoll();

		virtual bool Start();
		virtual void Shutdown();
		virtual void Run();
		virtual void OnRun();

		virtual void DisconnectClient(unsigned int clientID);
		virtual void DisconnectAll();

	protected:
		virtual bool _AddClientSocket(Socket &s);
		virtual void _CloseClientSocket(ClientContext *pcontext);
		virtual bool _PostRead(ClientContext *pcontext, ServerBuffer *pbuf);
		virtual bool _PostWrite(ClientContext *pcontext, char *psrc, int srclen);

		void _AcceptRun();
		void _ReadWriteRun();
		virtual int _OnRead(ClientContext *pcontext);
		virtual int _OnWrite(ClientContext *clientContext);

		Thread *mThreadListen;
	};

}

#endif