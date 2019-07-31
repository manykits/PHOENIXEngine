// PX2Socket.hpp

#ifndef PX2SOCKET_HPP
#define PX2SOCKET_HPP

#include "PX2NetPre.hpp"
#include "PX2NetDefine.hpp"
#include "PX2Timespan.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2SocketImpl.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Socket
	{
	public:
		typedef std::vector<Socket> SocketList;

	public:
		Socket();
		Socket(const Socket& socket);
		Socket(px2_socket_t socket);
		Socket& operator = (const Socket& socket);
		virtual ~Socket();

		void Close();

		SocketAddress GetPeerAddress() const;
		SocketAddress GetAddress() const;

		bool IsValid() const;
		void SetNoDelay(bool flag);

		void SetSendTimeout(const Timespan& timeout);
		void SetReceiveTimeout(const Timespan& timeout);

		enum SelectMode
		{
			SELECT_READ  = 1,
			SELECT_WRITE = 2,
			SELECT_ERROR = 4
		};
		bool Poll(const Timespan& timeout, int mode) const;

		static int Select (SocketList& readList, SocketList& writeList, 
			SocketList &exceptList, const Timespan& timeout);

		SocketImpl* GetImpl() const;

		px2_socket_t GetSocket() const;

	protected:
		Socket (SocketImpl* impl);

		Pointer0<SocketImpl> mImpl;
	};

#include "PX2Socket.inl"

}

#endif