// PX2ErrorHandler.hpp

#ifndef PX2ERRORHANDLER_HPP
#define PX2ERRORHANDLER_HPP

#include "PX2CorePre.hpp"
#include "PX2Mutex.hpp"
#include "PX2Exception.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ErrorHandler : public Singleton<ErrorHandler>
	{
	public:
		ErrorHandler();
		virtual ~ErrorHandler();

		virtual void OnException(const Exception& exc);
		virtual void OnException(const std::exception& exc);
		virtual void OnException();

		static void Handle(const Exception& exc);
		static void Handle(const std::exception& exc);
		static void Handle();

		static ErrorHandler* Set(ErrorHandler* handler);
		static ErrorHandler* Get();

	private:
		static ErrorHandler* mHandler;
		static Mutex mMutex;
	};

#define  PX2_ERRORHANDLER ErrorHandler::GetSingleton()

}

#endif