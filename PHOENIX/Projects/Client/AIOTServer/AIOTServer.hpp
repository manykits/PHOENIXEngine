// AIOTServer.hpp

#ifndef AIOTSERVER_HPP
#define AIOTSERVER_HPP

#include "AIOTPre.hpp"
#include "PX2NetServer.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class AIOTServer : public Server, public Singleton < AIOTServer >
	{
	public:
		AIOTServer(PX2::Server::ServerType serverType, int port,
			int numMaxConnects, int numMaxMsgHandlers);
		virtual ~AIOTServer();

		virtual void Run(float elapsedTime);

		int OnProcessString(int clientid, const std::string &cmdStr);

	protected:
		virtual int OnConnect(unsigned int clientid);
		virtual int OnDisconnect(unsigned int clientid);

	protected:
		unsigned int GetNextUID();
		int OnString(unsigned int clientid, const void *pbuffer, int buflen);

		void SendToClient(unsigned int clientid, const std::string &str);
		void SendToAll(const std::string &str);

		unsigned int mNextUID;
		int mNextGroupID;
		float mUpdateSeconds;
	};

#define  V_SERVER AIOTServer::GetSingleton()
	typedef Pointer0<AIOTServer> AIOTServerPtr;

}

#endif