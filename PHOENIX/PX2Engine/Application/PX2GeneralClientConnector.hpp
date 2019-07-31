// PX2GeneralClientConnector.hpp

#ifndef PX2GENERALCLIENTCONNECT_HPP
#define PX2GENERALCLIENTCONNECT_HPP

#include "PX2CorePre.hpp"
#include "PX2NetClientConnector.hpp"

namespace PX2
{

	const int GeneralClientConnectorMsgID = 2;

	class PX2_ENGINE_ITEM GeneralClientConnector : public ClientConnector
	{
	public:
		GeneralClientConnector();
		virtual ~GeneralClientConnector();

		virtual int Update(float elapsedSeconds);

		bool IsConnected() const;
		void OnConnectedToServer();
		void OnDisConnectedToServer();

		void SendString(const std::string &str);
		void SendRawString(const std::string &str);

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
		int OnStringMsg(const void *pbuffer, int buflen);

		bool mIsServerConnected;

		std::vector<std::string> mOnConnectCallbacks;
		std::vector<std::string> mOnDisconnectCallbacks;
		std::vector<std::string> mOnRecvCallbacks;
		static bool sIsDownloadingFile;
	};
	typedef Pointer0<GeneralClientConnector> GeneralClientConnectorPtr;

}

#endif