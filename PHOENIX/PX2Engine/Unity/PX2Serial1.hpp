// PX2Serial1.hpp

#ifndef PX2SERIAL1_HPP
#define PX2SERIAL1_HPP

#include "PX2UnityPre.hpp"
#include "PX2Runnable.hpp"
#include "PX2Thread.hpp"
#include "PX2Eventt.hpp"
#include "PX2Semaphore.hpp"
#ifdef WIN32
#include <windows.h>
#endif

namespace PX2
{

	typedef void(*SerialReceiveCallback) (std::string recvVal);

	class PX2_ENGINE_ITEM Serial1 : public Runnable
	{
#ifdef WIN32
	private:
		HANDLE hcom;
		DWORD mask;
		COMMTIMEOUTS ctmo;
		OVERLAPPED o;
		COMMPROP cmp;
#endif

	public:
		Serial1();
		virtual ~Serial1();

		char flag_opened;
		char comport[16];
		int baudrate;

		int Open(const std::string &comport_in, int baudrate);
		void Close();
		bool IsOpened() const;

		const std::string &GetPort() const;
		void Update(float elapsedSeconds);

		void Clear();

		int Write(const char *buf_ptr, int size);
		int Read(char *buf_ptr, int size);

		int Write(const std::string &buffer);
		int Read(std::string &buffer);

		void StartThread();
		void EndThread();
		virtual void Run();

		void ClearRecvCallbacks();
		bool IsHasReceiveCallback(SerialReceiveCallback callBack);
		void AddReceiveCallback(SerialReceiveCallback callBack);
		void RemoveReceiveCallback(SerialReceiveCallback callback);

	protected:
		void _ProcessRevBuf(std::string &recvBuf);
		void _OnCmd(const std::string &cmd);
		int InputWait(int msec);

		std::string mPort;

		ThreadPtr mThread;
		bool mIsThreadRunning;
		Mutex mMutex;
		std::string mRecingBuf;

		std::string mRecvStr;
		std::string mCmdStr;

		std::vector<SerialReceiveCallback> mCallbacks;
	};

}

#endif