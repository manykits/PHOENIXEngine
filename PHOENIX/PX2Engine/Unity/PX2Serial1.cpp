// PX2Serial1.cpp

#include "PX2Serial1.hpp"
#include "PX2Log.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Assert.hpp"
#include "PX2System.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;

#if defined(__LINUX__)
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int fd, res;
struct termios oldtio, newtio;
#endif

//---------------------------------------------------------------------------
Serial1::Serial1() :
flag_opened(0),
baudrate(9600),
mIsThreadRunning(false)
{
#ifdef WIN32
	hcom = 0;
	mask = 0;
#endif
}
//---------------------------------------------------------------------------
Serial1::~Serial1()
{
	EndThread();
}
//---------------------------------------------------------------------------
int Serial1::Open(const std::string &comport_in, int baudrate)
{
	if (comport_in.empty())
		return -1;

	bool flag = false;

#ifdef WIN32
	flag = true;

	DCB dcb1;

	sprintf(comport, comport_in.c_str());

	if (1 == flag_opened)
		this->Close();

	hcom = CreateFile(comport,
		GENERIC_READ | GENERIC_WRITE,	// 读写
		0,								// 独占
		NULL,
		OPEN_EXISTING,
		NULL,							// 同步
		NULL);

	// 获得当前串口配置信息
	GetCommState(hcom, &dcb1);

	dcb1.BaudRate = baudrate;

	dcb1.fParity = 1;//0
	dcb1.Parity = NOPARITY;//for AI_Motor
	//dcb1.Parity=EVENPARITY;
	dcb1.StopBits = ONESTOPBIT;//for AI_Motor
	dcb1.ByteSize = 8;//for AI Motor
	dcb1.fNull = FALSE;//
	//dcb1.EvtChar=STX;
	flag = (TRUE == SetCommState(hcom, &dcb1));

	if (flag == true)
	{
		GetCommMask(hcom, &mask);
		//mask=EV_RXCHAR|EV_RXFLAG;
		mask = EV_RXCHAR;
		flag = (TRUE == SetCommMask(hcom, mask));
	}

	if (flag == true)
	{
		GetCommTimeouts(hcom, &ctmo);
		ctmo.ReadIntervalTimeout = 1000;//0;//30;//SICKLMS偱偼6ms
		ctmo.ReadTotalTimeoutMultiplier = 5000;//=1;//0928
		//ctmo.ReadTotalTimeoutConstant=1000;//=6;//0928

		flag = (TRUE == SetCommTimeouts(hcom, &ctmo));
	}

	// 清空缓冲区，为读写串口做准备
	PurgeComm(hcom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR |
		PURGE_RXCLEAR);
	flag_opened = 1;

#endif


#if defined(__LINUX__)
	flag = true;

	fd = open(comport_in.c_str(), O_RDWR | O_NOCTTY);
	//fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);
	if (fd < 0)
	{
		perror(comport_in.c_str());
		PX2_LOG_ERROR("init() error. %s is not opend. fd=%d", comport_in.c_str(), fd);

		flag = false;
	}

	if (flag)
	{
		flag_opened = 1;
		PX2_LOG_INFO("init() com opened flag %d", flag_opened);

		tcgetattr(fd, &oldtio);

		bzero(&newtio, sizeof(newtio));

		tcflag_t baud;
		if (baudrate == 9600) baud = B9600;
		if (baudrate == 38400) baud = B38400;
		if (baudrate == 57600) baud = B57600;

		//newtio.c_cflag = baud | CRTSCTS | CS8 | CLOCAL | CREAD;
		newtio.c_cflag = baud | IGNPAR | CS8 | CLOCAL | CREAD;
		newtio.c_iflag = IGNPAR;
		newtio.c_oflag = 0;

		/* set input mode (non-canonical, no echo,...) */
		newtio.c_lflag = 0;

		tcsetattr(fd, TCSAFLUSH, &newtio);
	}

#endif

	if (flag)
		return 0;

	return -1;
}
//---------------------------------------------------------------------------
void Serial1::Clear()
{
#ifdef WIN32
	PurgeComm(hcom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR |
		PURGE_RXCLEAR);
#endif

#if defined(__LINUX__)
	tcflush(fd, TCIFLUSH);
#endif

}
//---------------------------------------------------------------------------
bool Serial1::IsOpened() const
{
	return flag_opened != 0;
}
//---------------------------------------------------------------------------
void Serial1::Close()
{
	if (flag_opened != 1)
		return;
	flag_opened = 0;

#ifdef WIN32
	CloseHandle(hcom);
#endif

#if defined(__LINUX__)
	::close(fd);
#endif
}
//---------------------------------------------------------------------------
void Serial1::Update(float elapsedSeconds)
{
	PX2_UNUSED(elapsedSeconds);

	ScopedCS cs(&mMutex);

	if (mRecingBuf.size() > 0)
	{
		mRecvStr += mRecingBuf;
		_ProcessRevBuf(mRecvStr);
		mRecingBuf.clear();
	}
}
//---------------------------------------------------------------------------
void Serial1::_ProcessRevBuf(std::string &recvBuf)
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
void Serial1::_OnCmd(const std::string &cmd)
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		if (mCallbacks[i])
			mCallbacks[i](cmd);
	}
}
//----------------------------------------------------------------------------
void Serial1::ClearRecvCallbacks()
{
	mCallbacks.clear();
}
//----------------------------------------------------------------------------
bool Serial1::IsHasReceiveCallback(SerialReceiveCallback callBack)
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		if (callBack == mCallbacks[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Serial1::AddReceiveCallback(SerialReceiveCallback callBack)
{
	if (IsHasReceiveCallback(callBack))
		return;

	mCallbacks.push_back(callBack);
}
//----------------------------------------------------------------------------
void Serial1::RemoveReceiveCallback(SerialReceiveCallback callback)
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
const std::string &Serial1::GetPort() const
{
	return mPort;
}
//---------------------------------------------------------------------------
void Serial1::StartThread()
{
	EndThread();

	mIsThreadRunning = true;

	mThread = new0 Thread("SerailThread1");
	mThread->Start(*this);
}
//---------------------------------------------------------------------------
void Serial1::EndThread()
{
	if (mThread)
	{
		mIsThreadRunning = false;

		mThread->Join();
		mThread = 0;
	}
}
//---------------------------------------------------------------------------
int Serial1::Read(char *buf_ptr, int size)
{
	unsigned long byte, event;
	byte = 0;
	bool flag = false;

#ifdef WIN32
	while (!flag)
	{
		WaitCommEvent(hcom, &event, NULL);
		if (event | EV_RXCHAR)
		{
			if (ReadFile(hcom, buf_ptr, size, &byte, NULL))
				flag = true;
			break;
		}
		byte = 0;
	}
#endif

#if defined(__LINUX__)
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);
	byte = read(fd, buf_ptr, size);
#endif

	return byte;
}
//---------------------------------------------------------------------------
int Serial1::Write(const char *buf_ptr, int size)
{
	unsigned long byte;
	if (flag_opened != 1)
	{
		PX2_LOG_ERROR("send() error. port Not opend");
		PX2_LOG_ERROR("flag_opened=%d", flag_opened);//debug
		return -1;
	}

#ifdef WIN32
	if (WriteFile(hcom, buf_ptr, size, &byte, NULL))
	{
		return byte;
	}
	else return -1;

#elif defined(__LINUX__)
	byte = write(fd, buf_ptr, size);
	if (byte == 0)
	{
		PX2_LOG_ERROR("write error byte 0");
		return -1;
	}

	PX2_LOG_INFO("write byte=%d", byte);

	return (byte);
#endif

	return 0;
}
//---------------------------------------------------------------------------
int Serial1::Write(const std::string &buffer)
{
	return Write(buffer.c_str(), (int)buffer.size());
}
//---------------------------------------------------------------------------
int Serial1::Read(std::string &buffer)
{
	return Read((char*)buffer.c_str(), (int)buffer.size());
}
//---------------------------------------------------------------------------
int Serial1::InputWait(int msec)
{
	if (!IsOpened())
		return -1;

#if defined(LINUX) || defined(MACOSX)
	fd_set rfds;
	struct timeval tv;
	tv.tv_sec = msec / 1000;
	tv.tv_usec = (msec % 1000) * 1000;
	FD_ZERO(&rfds);
	FD_SET(port_fd, &rfds);
	return select(port_fd + 1, &rfds, NULL, NULL, &tv);
#elif defined(WIN32) || defined(_WIN32)
	// http://msdn2.microsoft.com/en-us/library/aa363479(VS.85).aspx
	// http://msdn2.microsoft.com/en-us/library/aa363424(VS.85).aspx
	// http://source.winehq.org/WineAPI/WaitCommEvent.html

	COMSTAT st;
	DWORD errmask = 0, eventmask = EV_RXCHAR, ret;
	OVERLAPPED ov;
	int r;
	// first, request comm event when characters arrive
	if (!SetCommMask(hcom, EV_RXCHAR))
		return -1;

	// look if there are characters in the buffer already
	if (!ClearCommError(hcom, &errmask, &st))
		return -1;

	//printf("InputWait, %lu buffered, timeout = %d ms\n", st.cbInQue, msec);
	if (st.cbInQue > 0)
		return 1;

	ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (ov.hEvent == NULL) return -1;
	ov.Internal = ov.InternalHigh = 0;
	ov.Offset = ov.OffsetHigh = 0;
	if (WaitCommEvent(hcom, &eventmask, &ov))
	{
		//printf("InputWait, WaitCommEvent, immediate success\n");
		r = 1;
	}
	else
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			ret = WaitForSingleObject(ov.hEvent, msec);
			if (ret == WAIT_OBJECT_0)
			{
				//printf("InputWait, WaitCommEvent, delayed success\n");
				r = 1;
			}
			else if (ret == WAIT_TIMEOUT)
			{
				//printf("InputWait, WaitCommEvent, timeout\n");
				GetCommMask(hcom, &eventmask);
				r = 0;
			}
			else
			{  // WAIT_FAILED or WAIT_ABANDONED
				//printf("InputWait, WaitCommEvent, delayed error\n");
				r = -1;
			}
		}
		else
		{
			//printf("InputWait, WaitCommEvent, immediate error\n");
			r = -1;
		}
	}

	SetCommMask(hcom, 0);
	CloseHandle(ov.hEvent);
	return r;
#endif
}
//---------------------------------------------------------------------------
void Serial1::Run()
{
	while (mIsThreadRunning)
	{
		if (!mIsThreadRunning)
			break;

		//char buf[1024];

		//int r = InputWait(40);
		//if (r > 0)
		//{
		//	r = Read(buf, sizeof(buf));
		//	if (r < 0)
		//	{
		//		/*_*/
		//	}
		//	if (r > 0)
		//	{
		//		std::string str((char*)buf, r);
		//		ScopedCS cs(&mMutex);
		//		mRecingBuf.append(str);
		//	}
		//}

		// recv
		std::string buf;
		buf.resize(1);
		int recvedBytes = Read(buf);
		if (recvedBytes > 0)
		{
			ScopedCS cs(&mMutex);
			mRecingBuf.append(buf);
		}
	}
}
//---------------------------------------------------------------------------