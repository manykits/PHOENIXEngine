// PX2ErrorHandler.cpp

#include "PX2ErrorHandler.hpp"
#include "PX2Log.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ErrorHandler* ErrorHandler::Get()
{
	return mHandler;
}
//----------------------------------------------------------------------------
ErrorHandler* ErrorHandler::mHandler = 0;
Mutex ErrorHandler::mMutex;
//----------------------------------------------------------------------------
ErrorHandler::ErrorHandler()
{
}
//----------------------------------------------------------------------------
ErrorHandler::~ErrorHandler()
{
}
//----------------------------------------------------------------------------
void ErrorHandler::OnException(const Exception& exc)
{
	assertion(false, exc.what());
	PX2_LOG_ERROR(exc.what());
}
//----------------------------------------------------------------------------
void ErrorHandler::OnException(const std::exception& exc)
{
	assertion(false, exc.what());
	PX2_LOG_ERROR(exc.what());
}
//----------------------------------------------------------------------------
void ErrorHandler::OnException()
{
	assertion(false, "unknown exception");
	PX2_LOG_ERROR("unknown exception");
}
//----------------------------------------------------------------------------
void ErrorHandler::Handle(const Exception& exc)
{
	ScopedCS cs(&mMutex);

	try
	{
		if (mHandler)
			mHandler->OnException(exc);
	}
	catch (...)
	{
	}
}
//----------------------------------------------------------------------------
void ErrorHandler::Handle(const std::exception& exc)
{
	ScopedCS cs(&mMutex);

	try
	{
		if (mHandler)	
			mHandler->OnException(exc);
	}
	catch (...)
	{
	}
}
//----------------------------------------------------------------------------
void ErrorHandler::Handle()
{
	ScopedCS cs(&mMutex);

	try
	{
		if (mHandler)
			mHandler->OnException();
	}
	catch (...)
	{
	}
}
//----------------------------------------------------------------------------
ErrorHandler* ErrorHandler::Set(ErrorHandler* handler)
{
	assertion(0 != handler, "pointer must not be 0");

	ScopedCS cs(&mMutex);

	ErrorHandler* old = mHandler;
	mHandler = handler;

	return old;
}
//----------------------------------------------------------------------------