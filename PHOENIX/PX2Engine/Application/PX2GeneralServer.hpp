// PX2GeneralServer.hpp

#ifndef PX2SIMPLESERVER_HPP
#define PX2SIMPLESERVER_HPP

#include "PX2NetPre.hpp"
#include "PX2NetServer.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	const int GeneralServerMsgID = 2;

	enum GeneralServerCallType
	{
		GSCT_ON_CONNECT,
		GSCT_ON_DISCONNECT,
		GSCT_ON_RECVEDMSG,
		GSCT_MAX_TYPE
	};

	struct PX2_ENGINE_ITEM GeneralServerCallData
	{
		GeneralServerCallData();

		GeneralServerCallType TheCallType;
		int ClientID;
		std::string strData;
	};
	typedef void (*GeneralServerCallback) (GeneralServerCallData data);

	class PX2_ENGINE_ITEM GeneralServer : public Server, public Singleton < GeneralServer >
	{
	public:
		GeneralServer(PX2::Server::ServerType serverType, int port,
			int numMaxConnects, int numMaxMsgHandlers);
		virtual ~GeneralServer();

		void AddCallback(GeneralServerCallback callBack);

		void SendString(int clientid, const std::string &str);
		void SendRawString(int clientid, const std::string &str);

		bool IsHasOnConnectCallback(const std::string &callback) const;
		void AddOnConnectCallback(const std::string &callback);
		void RemoveOnConnectCallback(const std::string &callback);

		bool IsHasOnDisConnectCallback(const std::string &callback) const;
		void AddOnDisconnectCallback(const std::string &callback);
		void RemoveOnDisconnectCallback(const std::string &callback);

		bool IsHasOnRecvCallback(const std::string &callback) const;
		void AddOnRecvCallback(const std::string &callback);
		void RemoveOnRecvCallback(const std::string &callback);

	protected:
		virtual int OnConnect(unsigned int clientid);
		virtual int OnDisconnect(unsigned int clientid);
		int OnString(unsigned int clientid, const void *pbuffer, int buflen);

		std::vector<GeneralServerCallback> mCallbacks;

		std::vector<std::string> mOnConnectCallbacks;
		std::vector<std::string> mOnDisconnectCallbacks;
		std::vector<std::string> mOnRecvCallbacks;
	};

#define  PX2_SIMPLE_SERVER GeneralServer::GetSingleton()
	typedef Pointer0<GeneralServer> GeneralServerPtr;

}

#endif