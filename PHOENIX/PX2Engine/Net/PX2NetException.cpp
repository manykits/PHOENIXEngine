// PX2NetException.cpp

#include "PX2NetException.hpp"
using namespace PX2;


PX2_IMPLEMENT_EXCEPTION(NetException, IOException, "Net Exception")
PX2_IMPLEMENT_EXCEPTION(InvalidAddressException, NetException, "Invalid address")
PX2_IMPLEMENT_EXCEPTION(InvalidSocketException, NetException, "Invalid socket")
PX2_IMPLEMENT_EXCEPTION(ServiceNotFoundException, NetException, "Service not found")
PX2_IMPLEMENT_EXCEPTION(ConnectionAbortedException, NetException, "Software caused connection abort")
PX2_IMPLEMENT_EXCEPTION(ConnectionResetException, NetException, "Connection reset by peer")
PX2_IMPLEMENT_EXCEPTION(ConnectionRefusedException, NetException, "Connection refused")
PX2_IMPLEMENT_EXCEPTION(DNSException, NetException, "DNS error")
PX2_IMPLEMENT_EXCEPTION(HostNotFoundException, DNSException, "Host not found")
PX2_IMPLEMENT_EXCEPTION(NoAddressFoundException, DNSException, "No address found")
PX2_IMPLEMENT_EXCEPTION(InterfaceNotFoundException, NetException, "Interface not found")
PX2_IMPLEMENT_EXCEPTION(NoMessageException, NetException, "No message received")
PX2_IMPLEMENT_EXCEPTION(MessageException, NetException, "Malformed message")
PX2_IMPLEMENT_EXCEPTION(MultipartException, MessageException, "Malformed multipart message")
PX2_IMPLEMENT_EXCEPTION(HTTPException, NetException, "HTTP Exception")
PX2_IMPLEMENT_EXCEPTION(NotAuthenticatedException, HTTPException, "No authentication information found")
PX2_IMPLEMENT_EXCEPTION(UnsupportedRedirectException, HTTPException, "Unsupported HTTP redirect (protocol change)")
PX2_IMPLEMENT_EXCEPTION(FTPException, NetException, "FTP Exception")
PX2_IMPLEMENT_EXCEPTION(SMTPException, NetException, "SMTP Exception")
PX2_IMPLEMENT_EXCEPTION(POP3Exception, NetException, "POP3 Exception")
PX2_IMPLEMENT_EXCEPTION(ICMPException, NetException, "ICMP Exception")
PX2_IMPLEMENT_EXCEPTION(NTPException, NetException, "NTP Exception")
PX2_IMPLEMENT_EXCEPTION(HTMLFormException, NetException, "HTML Form Exception")
PX2_IMPLEMENT_EXCEPTION(WebSocketException, NetException, "WebSocket Exception")
PX2_IMPLEMENT_EXCEPTION(UnsupportedFamilyException, NetException, "Unknown or unsupported socket family.")