// PX2NetServer.hpp

#ifndef PX2SERVER_HPP
#define PX2SERVER_HPP

#include "PX2NetEventBuffer.hpp"
#include "PX2NetServerImp.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	enum ClientEventResult
	{
		CER_SUCCEED = 0,
		CER_MSGID_OUTRANGE = 1,		// msgid不在mMsgHandlers的范围内
		CER_HANDLER_NULL = 2,		// 对应的处理函数为null
		CER_PROTOBUF_PARSE = 3,		// protobuffer解析出错
		CER_NO_PLAYER = 4,			// 根据clientid找不到对应的数据， 可能是没有登录
		CER_WRONG_STATE = 5,		// 角色内部状态错误
		CER_WRONG_DATA = 6,			// 客户端上传数据出错
		CER_USER_ERROR = 7,			// 其它错误
	};

	class PX2_ENGINE_ITEM Server : public Object
	{
	public:
		typedef int (Server::*MsgHandleFunc)(unsigned int clientid,
			const void *pbuffer, int buflen);

	public:
		enum ServerType
		{
			ST_IOCP,
			ST_POLL,
			ST_MAX_TYPE
		};
		Server(ServerType serverType, int port, int numMaxConnects, 
			int numMaxMsgHandlers);
		virtual ~Server();

		static Server *New(ServerType serverType, int port, int numMaxConnects,
			int numMaxMsgHandlers);

		int GetPort() const;

		ServerType GetServerType() const;
		ServerSocket GetListenSocket();
		std::string GetAddress();

		bool Start();
		void Shutdown();
		virtual void Run(float elapsedTime);
		bool IsStarted() const;

		const std::vector<int> &GetThreadIDs() const;

		void LogStatus();

		ClientContext *GetClientContext(int clientID);
		int GetNumConnects();

		// register msg handle
		void RegisterHandler(int msgid, MsgHandleFunc msgfunc);

		static std::string GetIP(const std::string &address);

	protected:
		void HandleClientEvents();
		int HandleClientEvent(BufferEvent *ent);
		void DisconnectClient(unsigned int clientid);

		// Send
	public:
		template<class T>
		int SendMsgToClient(unsigned int clientid, int msgid, T&msg)
		{
			char buffer[MAX_OVERLAPBUF_ALLSZIE];
			int nbytes = MsgToRawBuffer(msgid, msg, buffer, sizeof(buffer));
			if (nbytes <= 0)
			{
				printf("SendMsgToClient, nbytes<=0\n");
				return -1;
			}

			if (!mServerImp->PostWrite(clientid, buffer, nbytes))
			{
				printf("SendMsgToClient, PostWrite false\n");
			}
			return 0;
		}

		template<class T>
		int BroadMsgToClients(int msgid, T&msg)
		{
			char buffer[MAX_OVERLAPBUF_ALLSZIE];
			int nbytes = MsgToRawBuffer(msgid, msg, buffer, sizeof(buffer));
			if (nbytes <= 0)
			{
				printf("BroadMsgToClient, nbytes<=0\n");
				return -1;
			}

			if (!mServerImp->PostBroadcast(buffer, nbytes))
			{
				printf("BroadMsgToClient, PostWrite false\n");
			}
			return 0;
		}

		int SendMsgToClientBuffer(int clientID, int msgid, const char *buf, int size);
		int SendMsgToClientRawBuffer(int clientID, const char *buf, int size);

	protected:
		Server();

		int OnReservedMsg (unsigned int clientid, const void *pbuffer, int buflen);
		virtual int OnConnect(unsigned int clientid);
		virtual int OnDisconnect(unsigned int clientid);

	protected:
		ServerType mServerType;
		int mPort;
		int mNumMaxConnects;
		int mNumMaxMsgHandlers;
		BufferEventQueue *mBufferEventQue;
		std::vector<MsgHandleFunc> mMsgHandlers;

		ServerImpPtr mServerImp;
	};

	typedef Pointer0<Server> ServerPtr;

}

#endif