// PX2TCPServerConnection.cpp

#include "PX2TCPServerConnection.hpp"
#include "PX2ErrorHandler.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
TCPServerConnection::TCPServerConnection(const StreamSocket& socket) :
mSocket(socket)
{
}
//----------------------------------------------------------------------------
TCPServerConnection::~TCPServerConnection()
{
}
//----------------------------------------------------------------------------
void TCPServerConnection::Start()
{
	try
	{
		Run();
	}
	catch (std::exception& exc)
	{
		ErrorHandler::Handle(exc);
	}
	catch (...)
	{
		ErrorHandler::Handle();
	}
}
//----------------------------------------------------------------------------
StreamSocket& TCPServerConnection::GetSocket()
{
	return mSocket;
}
//----------------------------------------------------------------------------