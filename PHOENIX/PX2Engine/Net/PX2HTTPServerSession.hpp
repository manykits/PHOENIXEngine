// PX2HTTPServerSession.hpp

#ifndef PX2HTTPSERVERSESSION_HPP
#define PX2HTTPSERVERSESSION_HPP

#include "PX2HTTPSession.hpp"
#include "PX2HTTPServerParams.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM HTTPServerSession : public HTTPSession
	{
	public:
		HTTPServerSession(const StreamSocket& socket, HTTPServerParams *params);
		virtual ~HTTPServerSession();

		bool HasMoreRequests();
		bool CanKeepAlive() const;
		SocketAddress GetClientAddress();
		SocketAddress GerverAddress();

	private:
		bool mFirstRequest;
		Timespan mKeepAliveTimeout;
		int mMaxKeepAliveRequests;
	};



}

#endif
