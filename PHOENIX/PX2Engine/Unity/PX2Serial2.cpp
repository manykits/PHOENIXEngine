// PX2Serial2.cpp

#include "PX2Serial2.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;

#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
#include "serial/serial.h"
#endif

//---------------------------------------------------------------------------
Serial2::Serial2() 
{
#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
	mSerial = 0;
#endif
}
//---------------------------------------------------------------------------
Serial2::~Serial2()
{
	Close();
}
//---------------------------------------------------------------------------
int Serial2::Open(const std::string &port, int baudrate)
{
	Close();

#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
	mSerial = new serial::Serial(port, baudrate);
	if (mSerial->isOpen())
		return 0;
#endif

	return -1;
}
//---------------------------------------------------------------------------
bool Serial2::IsOpened() const
{
#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
	if (mSerial)
		return mSerial->isOpen();
#endif

	return false;
}
//---------------------------------------------------------------------------
void Serial2::Close()
{
#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
	if (mSerial)
	{
		mSerial->close();

		delete(mSerial);
		mSerial = 0;
	}
#endif
}
//---------------------------------------------------------------------------
void Serial2::Update(float elapsedSeconds)
{
	PX2_UNUSED(elapsedSeconds);

#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
	int numAvailable = mSerial->available();
	if (numAvailable > 0)
	{
		std::string readVal = mSerial->read(numAvailable);
		mRecingBuf.append(readVal);
	}
#endif

	if (mRecingBuf.size() > 0)
	{
		mRecvStr += mRecingBuf;
		_ProcessRevBuf(mRecvStr);
		mRecingBuf.clear();
	}
}
//---------------------------------------------------------------------------
void Serial2::_ProcessRevBuf(std::string &recvBuf)
{
	std::string cmdStr;
	int index = 0;
	for (index = 0; index < (int)recvBuf.size(); index++)
	{
		char chara = recvBuf[index];
		if ('\r' == chara)
		{
			continue;
		}
		else if ('\n' == chara)
		{
			if (!mCmdStr.empty())
			{
				_OnCmd(mCmdStr);
				mCmdStr.clear();
			}

			std::string subStr = recvBuf.substr(index + 1, recvBuf.length() - (index + 1));
			recvBuf = subStr;
			return;
		}
		else
		{
			mCmdStr += chara;
		}
	}
}
//---------------------------------------------------------------------------
void Serial2::_OnCmd(const std::string &cmd)
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		if (mCallbacks[i])
			mCallbacks[i](cmd);
	}
}
//----------------------------------------------------------------------------
void Serial2::ClearRecvCallbacks()
{
	mCallbacks.clear();
}
//----------------------------------------------------------------------------
bool Serial2::IsHasReceiveCallback(SerialReceiveCallback callBack)
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		if (callBack == mCallbacks[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Serial2::AddReceiveCallback(SerialReceiveCallback callBack)
{
	if (IsHasReceiveCallback(callBack))
		return;

	mCallbacks.push_back(callBack);
}
//----------------------------------------------------------------------------
void Serial2::RemoveReceiveCallback(SerialReceiveCallback callback)
{
	auto it = mCallbacks.begin();
	for (; it != mCallbacks.end();)
	{
		if (*it == callback)
		{
			it = mCallbacks.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//---------------------------------------------------------------------------
std::string Serial2::GetPort() const
{
#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
	if (mSerial)
		return mSerial->getPort();
#endif

	return "";
}
//---------------------------------------------------------------------------
void Serial2::StartThread()
{
	//EndThread();

	//mIsThreadRunning = true;

	//mThread = new0 Thread("SerailThread2");
	//mThread->Start(*this);
}
//---------------------------------------------------------------------------
void Serial2::EndThread()
{
	//if (mThread)
	//{
	//	mIsThreadRunning = false;

	//	mThread->Join();
	//	mThread = 0;
	//}
}
//---------------------------------------------------------------------------
int Serial2::Read(char *buf_ptr, int size)
{
#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
	if (mSerial)
	{
		return mSerial->read((uint8_t*)buf_ptr, size);
	}
#endif

	return 0;
}
//---------------------------------------------------------------------------
int Serial2::Write(const char *buf_ptr, int size)
{
#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
	if (mSerial)
	{
		return mSerial->write((const uint8_t *)buf_ptr, size);
	}
#endif

	return 0;
}
//---------------------------------------------------------------------------
int Serial2::Write(const std::string &buffer)
{
	return Write(buffer.c_str(), (int)buffer.size());
}
//---------------------------------------------------------------------------
int Serial2::Read(std::string &buffer)
{
	return Read((char*)buffer.c_str(), (int)buffer.size());
}
//---------------------------------------------------------------------------
void Serial2::Run()
{
	//if (!mSerial)
	//	return;

	//while (mIsThreadRunning)
	//{
		//int numAvailable = mSerial->available();
		//if (numAvailable > 0)
		//{
		//	std::string readVal = mSerial->read(numAvailable);

		//	ScopedCS cs(&mMutex);
		//	mRecingBuf.append(readVal);
		//}
	//}
}
//---------------------------------------------------------------------------