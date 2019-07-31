LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Net

LOCAL_CPP_EXTENSION := .cc  .cpp 

LOCAL_SRC_FILES := \
PX2IPAddress.cpp \
PX2IPAddressImpl.cpp \
PX2IPv4AddressImpl.cpp \
PX2IPv6AddressImpl.cpp \
PX2IPv4SocketAddressImpl.cpp \
PX2IPv6SocketAddressImpl.cpp \
PX2SocketAddress.cpp \
PX2SocketAddressImpl.cpp \
PX2DNS.cpp \
PX2HostEntry.cpp \
PX2NetBitStream.cpp \
PX2NetInitTerm.cpp \
PX2NetError.cpp \
PX2NetException.cpp \
PX2NetClientConnector.cpp \
PX2NetEventBuffer.cpp \
PX2NetServerPoll.cpp\
PX2ClientContext.cpp \
PX2NetServerImp.cpp \
PX2ServerUser.cpp \
PX2ServerUserManager.cpp \
PX2NetServer.cpp \
PX2ServerDBManager.cpp \
PX2HTTPBufferAllocator.cpp \
PX2HTTPChunkedStream.cpp \
PX2HTTPFixedLengthStream.cpp \
PX2HTTPHeaderStream.cpp \
PX2HTTPStream.cpp \
PX2HTTPCookie.cpp \
PX2HTTPMessage.cpp \
PX2HTTPRequest.cpp \
PX2HTTPResponse.cpp \
PX2HTTPSession.cpp \
PX2MessageHeader.cpp \
PX2HTTPRequestHandler.cpp \
PX2HTTPRequestHandlerFactory.cpp \
PX2HTTPServer.cpp \
PX2HTTPServerConnection.cpp \
PX2HTTPServerConnectionFactory.cpp \
PX2HTTPServerParams.cpp \
PX2HTTPServerRequest.cpp \
PX2HTTPServerRequestImpl.cpp \
PX2HTTPServerResponse.cpp \
PX2HTTPServerResponseImpl.cpp \
PX2HTTPServerSession.cpp \
PX2CountingStream.cpp \
PX2NameValueCollection.cpp \
PX2URI.cpp \
PX2Socket.cpp \
PX2SocketImpl.cpp \
PX2StreamSocket.cpp \
PX2StreamSocketImpl.cpp \
PX2ServerInfo.cpp\
PX2ServerInfoManager.cpp\
PX2ServerSocket.cpp \
PX2ServerSocketImpl.cpp \
PX2DatagramSocket.cpp \
PX2DatagramSocketImpl.cpp \
PX2WebSocket.cpp \
PX2WebSocketImpl.cpp \
PX2NetServerBuffer.cpp \
PX2Notification.cpp \
PX2NotificationQueue.cpp \
PX2TCPServer.cpp \
PX2TCPServerConnection.cpp \
PX2TCPServerConnectionFactory.cpp \
PX2TCPServerDispatcher.cpp \
PX2TCPServerParams.cpp \
PX2UDPServer.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
$(PX2_ROOT)/PX2Engine/Unity/ \
$(PX2_ROOT)/PX2SDK/Include/

LOCAL_CPPFLAGS := -std=c++11 -fexceptions
LOCAL_LDLIBS := -llog -pthread

include $(BUILD_STATIC_LIBRARY)
