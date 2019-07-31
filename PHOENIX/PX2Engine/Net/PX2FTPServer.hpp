// PX2FTPServer.hpp

#ifndef PX2FTPSERVER_HPP
#define PX2FTPSERVER_HPP

#include "PX2NetPre.hpp"
#include "PX2Socket.hpp"
#include "PX2StreamSocket.hpp"
#include "PX2ServerSocket.hpp"

namespace PX2
{

#define HOSTNAME_LENGTH 30
#define FILENAME_LENGTH 30
#define BUFFER_SIZE 512
#define LISTEN_PORT 1234


#define LISTEN_ON_PORT 1234
#define MAX_CLIENTS 10
	
// Status messages
#define CLIENT_DISC 0
#define CLIENT_CONN 1
#define CLIENT_X 2

	typedef struct 
	{
		int header;
		char buffer[BUFFER_SIZE];
		int footer;
	} FTPPacket;

	typedef struct
	{
		int dir;
		char filename[FILENAME_LENGTH];
	} FTPFrame;


	struct FTPClient
	{
		FTPClient();
		~FTPClient();

		bool IsConnected;	//Set true if a client is connected
		SocketAddress addr;	//Client info like ip address
		StreamSocket TheSocket;	//Client socket
		fd_set set;			//used to check if there is data in the socket
		int Index;				//any piece of additional info
		FTPFrame frame;
		bool IsRunning;
	};

	class PX2_ENGINE_ITEM FTPServer
	{
	public:
		FTPServer();
		~FTPServer();

		void SetListenPort(int port);
		void Start();


		void Accept_New_Clients();
		bool Accept_Client(FTPClient *client);
		void GetFrame(FTPClient *client);
		bool HandleFrame(FTPClient *client);

	protected:
		bool _DoUpload(FTPClient* client);
		bool _DoDownload(FTPClient* client);
		void _UpdateStatus(int msg);
		bool _IsFileExist(const std::string &filename);
		bool _ReadFileAndSend(const std::string &filename, StreamSocket sock);
		bool _RecvFileAndWrite(char * filename, StreamSocket sock);

		int mListenPort;
		ServerSocket mListenSocket;
		int mNumConnections;
		FTPClient mClients[MAX_CLIENTS];
	};

}

#endif