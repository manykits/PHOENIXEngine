// PX2NetException.hpp

#ifndef PX2NETEXCEPTION_HPP
#define PX2NETEXCEPTION_HPP

#include "PX2NetPre.hpp"
#include "PX2Exception.hpp"

namespace PX2
{

	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, NetException, IOException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, InvalidAddressException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, InvalidSocketException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, ServiceNotFoundException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, ConnectionAbortedException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, ConnectionResetException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, ConnectionRefusedException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, DNSException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, HostNotFoundException, DNSException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, NoAddressFoundException, DNSException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, InterfaceNotFoundException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, NoMessageException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, MessageException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, MultipartException, MessageException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, HTTPException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, NotAuthenticatedException, HTTPException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, UnsupportedRedirectException, HTTPException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, FTPException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, SMTPException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, POP3Exception, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, ICMPException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, NTPException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, HTMLFormException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, WebSocketException, NetException)
	PX2_DECLARE_EXCEPTION(PX2_ENGINE_ITEM, UnsupportedFamilyException, NetException)

}

#endif