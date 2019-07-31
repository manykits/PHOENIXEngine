// PX2HTTPMessage.hpp

#ifndef PX2HTTPMESSAGE_HPP
#define PX2HTTPMESSAGE_HPP

#include "PX2MessageHeader.hpp"

namespace PX2
{
	/// HTTPRequest和HTTPResponse的基类。
	///
	/// 定义所有HTTP消息的公共属性。
	/// 这些是版本，内容长度，内容类型
	/// 传输编码。
	class PX2_ENGINE_ITEM HTTPMessage : public MessageHeader
	{
	public:
		void SetVersion(const std::string& version);
		const std::string& GetVersion() const;

		void SetContentLength(std::streamsize length);
		std::streamsize GetContentLength() const;
		bool HasContentLength() const;

		void SetTransferEncoding(const std::string& transferEncoding);
		const std::string& GetTransferEncoding() const;

		void SetChunkedTransferEncoding(bool flag);
		bool GetChunkedTransferEncoding() const;

		/// 设置此消息的内容类型。 
		/// 指定NO CONTENT_TYPE以删除/// Content-Type头。
		void SetContentType(const std::string& mediaType);
		const std::string& GetContentType() const;

		void SetKeepAlive(bool keepAlive);
		bool GetKeepAlive() const;

		static const std::string HTTP_1_0;
		static const std::string HTTP_1_1;

		static const std::string IDENTITY_TRANSFER_ENCODING;
		static const std::string CHUNKED_TRANSFER_ENCODING;

		static const int         UNKNOWN_CONTENT_LENGTH;
		static const std::string UNKNOWN_CONTENT_TYPE;

		static const std::string CONTENT_LENGTH;
		static const std::string CONTENT_TYPE;
		static const std::string TRANSFER_ENCODING;
		static const std::string CONNECTION;

		static const std::string CONNECTION_KEEP_ALIVE;
		static const std::string CONNECTION_CLOSE;

		static const std::string EMPTY;

	protected:
		HTTPMessage();
		HTTPMessage(const std::string& version);
		virtual ~HTTPMessage();

	private:
		HTTPMessage(const HTTPMessage&);
		HTTPMessage& operator = (const HTTPMessage&);

		std::string _version;
	};

}

#endif