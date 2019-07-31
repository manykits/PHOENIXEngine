// PX2ClientContext.hpp

#ifndef PX2CLIENTCONTEXT_HPP
#define PX2CLIENTCONTEXT_HPP

#include "PX2NetPre.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2StreamSocket.hpp"
#include "PX2NetEventBuffer.hpp"
#include "PX2NetServerBuffer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ClientContext
	{
	public:
		ClientContext();
		~ClientContext();

	public:
		void Init(Socket &s, unsigned int clientID);

	public:
		Socket TheSocket;
		unsigned int ClientID;
		long NumPendingIO;

		BufferEvent *TheBufferEvent;
		int PackageTotalLength;

		// used in pool
	public:
		void InitOnPool();

		char *RecvBuf;
		int RecvLen;

		BufferEventQueue *SendQue;
		char *SendBuf;
		int SendLen;
	};

}

#endif