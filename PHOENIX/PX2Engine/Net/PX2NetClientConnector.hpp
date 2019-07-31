// PX2ClientConnect.hpp

#ifndef PX2NETCLIENTCONNECT_HPP
#define PX2NETCLIENTCONNECT_HPP

#include "PX2NetPre.hpp"
#include "PX2NetEventBuffer.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2SocketAddress.hpp"
#include "PX2StreamSocket.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	enum
	{
		CONNSTATE_INIT = 0,
		CONNSTATE_TRYCONNECT,
		CONNSTATE_CONNECTED,
		CONNSTATE_CONN_ERROR, //连接出错状态， 等待重连
		CONNSTATE_WAITSERVER, //向服务器发送完消息， 等待回复消息
	};

	class ClientConnector;
	typedef void(*ClientConnectorRecvCallback) (ClientConnector *connector, const std::string &recvStr);

	class PX2_ENGINE_ITEM ClientConnector : public Object
	{
	public:
		typedef int (ClientConnector::*ServerMsgHandleFunc)(const void *pbuffer, int buflen);

	public:
		ClientConnector(int num_msghandlers);
		virtual ~ClientConnector();

		bool IsHasRecvCallback(ClientConnectorRecvCallback callback);
		bool AddRecvCallback(ClientConnectorRecvCallback callback);

		void RegisterHandler(int msgid, ServerMsgHandleFunc msgfunc, bool need_answer=true, int other_answer_msg=-1);

		StreamSocket &GetSocket ();
		int ConnectB(const std::string &ip, int port);
		int ConnectB(const SocketAddress &addr);
		int ConnectNB(const std::string &ip, int port);
		int ConnectNB(const SocketAddress &addr);
		void Disconnect();
		int GetConnectState ();
		void SetConnectState(int state);

		//出现网络错误时， 调用此函数试图重连，并且把之前没确认发到的消息重新再发
		//传入需要在消息重发前需要发送的复位消息， 如果为NULL， 就不会发复位消息
		int Reconnect(BufferEvent *ent = 0);

		template<class T>
		int SendMsgToServer(int msgid, const T &msg);

		int SendMsgToServerBuffer(int msgid, const char *buf, int size);

		int SendRawBuffer(const char *buf, int size);

		virtual int Update(float elapsedSeconds);

	protected:
		struct ServerMsgDesc
		{
			ServerMsgHandleFunc handler;
			bool need_answer;
			int other_answermsg;
		};
		std::vector<ServerMsgDesc>mMsgHandlers;
		std::vector<ClientConnectorRecvCallback> mCallbacks;

		BufferEventQueue *mSendQue;
		BufferEventQueue *mRecvQue;

		int mConnectState;
		float mConnStateTime; //处在此状态的时间

		char *mRecvBuf;
		int mRecvLen;
		char *mSendBuf;
		int mSendLen;
		std::string mRawSendStr;

		SocketAddress mConnectToAddr;
		StreamSocket mSocket;

	private:
		int _InternalConnectB();
		int _InternalConnectNB();
		void _InternalDisconnect();
		int _HandleServerMsg ();
		int _HandleServerBufferEvent(BufferEvent *pevent);
		int _OnReservedMsg (const void *pbuffer, int buflen);
		int _ClientOnRead();
		int _ClientOnWrite();
		void _OnRecvCallbacks(const std::string &recvStr);
	};

#include "PX2NetClientConnector.inl"
	typedef Pointer0<ClientConnector> ClientConnectorPtr;

}

#endif