// PX2HTTPServerParams.hpp

#ifndef PX2HTTPSERVERPARAMS_HPP
#define PX2HTTPSERVERPARAMS_HPP

#include "PX2NetPre.hpp"
#include "PX2TCPServerParams.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM HTTPServerParams : public TCPServerParams
	{
	public:
		HTTPServerParams();
		virtual ~HTTPServerParams();

		void SetServerName(const std::string& serverName);
		const std::string& GetServerName() const;

		void SetSoftwareVersion(const std::string& softwareVersion);
		const std::string& GetSoftwareVersion() const;

		void SetTimeout(const Timespan& timeout);
		const Timespan& GetTimeout() const;

		void SetKeepAlive(bool keepAlive);
		bool GetKeepAlive() const;

		void SetKeepAliveTimeout(const Timespan& timeout);
		const Timespan& GetKeepAliveTimeout() const;

		void SetMaxKeepAliveRequests(int maxKeepAliveRequests);
		int GetMaxKeepAliveRequests() const;

	private:
		std::string mServerName;
		std::string mSoftwareVersion;
		Timespan mTimeout;
		bool mIsKeepAlive;
		int mMaxKeepAliveRequests;
		Timespan mKeepAliveTimeout;
	};

	typedef Pointer0<HTTPServerParams> HTTPServerParamsPtr;

}

#endif