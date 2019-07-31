// PX2Serial2.hpp

#ifndef PX2SERIAL2_HPP
#define PX2SERIAL2_HPP

#include "PX2UnityPre.hpp"
#include "PX2Runnable.hpp"
#include "PX2Thread.hpp"

namespace serial
{
	class Serial;
}

namespace PX2
{

	typedef void(*SerialReceiveCallback) (std::string recvVal);

	class PX2_ENGINE_ITEM Serial2 : public Runnable
	{
	public:
		Serial2();
		virtual ~Serial2();

		int Open(const std::string &port, int baudrate);
		void Close();
		bool IsOpened() const;

		std::string GetPort() const;
		void Update(float elapsedSeconds);

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

		std::vector<SerialReceiveCallback> mCallbacks;

		ThreadPtr mThread;
		bool mIsThreadRunning;

		std::string mRecingBuf;

		std::string mRecvStr;
		std::string mCmdStr;

#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
		serial::Serial *mSerial;
#endif
	};

}

#endif