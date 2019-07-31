// PX2FTPServer.cpp

#include "PX2FTPServer.hpp"
#include "PX2Log.hpp"
#include <fstream>
#include <iostream>
using namespace PX2;

//----------------------------------------------------------------------------
FTPClient::FTPClient()
{
	IsConnected = false;
	Index = 0;
	IsRunning = false;
}
//----------------------------------------------------------------------------
FTPClient::~FTPClient()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
FTPServer::FTPServer() :
mListenPort(8387),
mNumConnections(0)
{

}
//----------------------------------------------------------------------------
FTPServer::~FTPServer()
{
}
//----------------------------------------------------------------------------
void FTPServer::SetListenPort(int port)
{
	mListenPort = port;
}
//----------------------------------------------------------------------------
void FTPServer::Start()
{
	if (0 != mListenSocket.Bind(SocketAddress((uint16_t)mListenPort)))
		return;

	mListenSocket.Listen();
}
//----------------------------------------------------------------------------
void FTPServer::Accept_New_Clients()
{
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (!(mClients[i].IsConnected)) 
		{
			if (Accept_Client(&mClients[i])) 
			{
				mClients[i].IsRunning = true;
				_UpdateStatus(CLIENT_CONN);

				while (mClients[i].IsRunning)
				{
					GetFrame(&mClients[i]);

					mClients[i].IsRunning = HandleFrame(&mClients[i]);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
bool FTPServer::Accept_Client(FTPClient *client)
{
	client->Index = sizeof(sockaddr); // save address

	StreamSocket streamSocket = mListenSocket.AcceptConnection();
	client->TheSocket = streamSocket;
		
	if (client->TheSocket.IsValid())
	{
		client->IsConnected = true;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void FTPServer::GetFrame(FTPClient *client)
{
	FTPFrame f;
	client->TheSocket.ReceiveBytes(&f, sizeof(f));

	client->frame.dir = f.dir;
	strcpy(client->frame.filename, f.filename);
}
//----------------------------------------------------------------------------
bool FTPServer::HandleFrame(FTPClient *client)
{
	if (client->frame.dir == 0)
		return _DoUpload(client);
	else if (client->frame.dir == 1)
		return _DoDownload(client);

	return false;
}
//----------------------------------------------------------------------------
bool FTPServer::_DoUpload(FTPClient* client)
{
	PX2_LOG_INFO("Sending client ready state for upload\n");

	char rdyState[2] = "u";

	int sendedBytes = client->TheSocket.SendBytes(rdyState, sizeof(rdyState), 0);
	_RecvFileAndWrite(client->frame.filename, client->TheSocket);

	client->TheSocket.ReceiveBytes(rdyState, sizeof(rdyState));

	if (rdyState[0] == 'y')
		return 1;

	_UpdateStatus(CLIENT_DISC);

	return 0;
}
//----------------------------------------------------------------------------
bool FTPServer::_DoDownload(FTPClient* client)
{
	PX2_LOG_INFO("Sending client ready state for download\n");

	char rdyState[2] = "d";

	int sendedBytes = client->TheSocket.SendBytes(rdyState, sizeof(rdyState), 0);
	client->TheSocket.ReceiveBytes(rdyState, sizeof(rdyState));

	_ReadFileAndSend(client->frame.filename, client->TheSocket);

	client->TheSocket.ReceiveBytes(rdyState, sizeof(rdyState));

	if (rdyState[0] == 'y')
		return 1;

	_UpdateStatus(CLIENT_DISC);

	return 0;
}
//----------------------------------------------------------------------------
void FTPServer::_UpdateStatus(int msg)
{
	if (msg == CLIENT_CONN) 
	{
		mNumConnections++;
		//cout << "\nWow, a client has connected. There are now " << mNumConnections << " clients connected.";
	}
	else if (msg == CLIENT_DISC) 
	{
		mNumConnections--;
		//cout << "Wee, a client has disconnected. There are now " << mNumConnections << " clients connected.";
	}
	else 
	{
		//never leave out anything
		//cout << "\n>>>>>>We got an unknown message :" << msg;
	}
}
//----------------------------------------------------------------------------
bool FTPServer::_IsFileExist(const std::string &filename)
{
	std::ifstream ifile(filename);
	return ifile.is_open();
}
//----------------------------------------------------------------------------
bool FTPServer::_ReadFileAndSend(const std::string &filename, StreamSocket sock)
{
	FTPPacket p;
	p.header = 1;
	p.footer = 0;
	int test;

	// Open
	PX2_LOG_INFO("Opening the file for Reading");
	std::ifstream is(filename, std::ios::in | std::ios::binary);

	// Check if error
	if (!is)  
	{
		PX2_LOG_INFO("Error opening the file to read :");
		return 0;
	}

	// Read and Write entire contents
	while (is) 
	{
		// Read
		is.read(p.buffer, BUFFER_SIZE);
		//cout << "\nInput file pointer at: " << is.tellg() << "\nAmount read: " << is.gcount();

		// eof + last packet
		if (is.gcount() < BUFFER_SIZE)
			p.footer = is.gcount();

		sock.SendBytes(&p, sizeof(FTPPacket), 0);
	}

	PX2_LOG_INFO("closing read file");
	is.close();

	return 1;
}
//----------------------------------------------------------------------------
bool FTPServer::_RecvFileAndWrite(char * filename, StreamSocket sock)
{
	FTPPacket p;
	p.header = 1;
	p.footer = 0;
	int test;

	char fname[FILENAME_LENGTH + 1] = "2";
	strcat(fname, filename);

	// Open 
	PX2_LOG_INFO("Opening the file for writing...");
	std::ofstream os(fname, std::ios::out | std::ios::binary);

	// Check if error
	if (!os)  
	{
		PX2_LOG_INFO("Error opening the file to write :/\n");
		return 0;
	}

	int i = 0;
	while (true) 
	{
		sock.ReceiveBytes(&p, sizeof(FTPPacket));

		if (p.footer == 0)
			os.write(p.buffer, BUFFER_SIZE);
		else
		{
			os.write(p.buffer, p.footer);
			break;
		}
		i++;
	}
	
	PX2_LOG_INFO("closing written file.\n");

	os.close();

	return 1;
}
//----------------------------------------------------------------------------