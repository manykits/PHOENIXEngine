// PX2Serial2.hpp

#ifndef PX2SERIAL2_HPP
#define PX2SERIAL2_HPP

#include "PX2UnityPre.hpp"
#include "PX2Runnable.hpp"
#include "PX2Thread.hpp"
#include "PX2EventSystem.hpp"
#include "PX2Object.hpp"

namespace serial
{
	class Serial;
}

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM SerialES)
	PX2_EVENT(Open)
	PX2_EVENT(Close)
	PX2_EVENT(OpenFailed)
	PX2_DECLARE_EVENT_END(SerialES)

	class Serial;

	typedef void(*SerialCallback) (Serial*, std::string recvVal, int length);
	typedef void(*SerialCMDCallback) (Serial*, std::string recvVal);

	class PX2_ENGINE_ITEM Serial : public Object
	{
	public:
		static Serial *New();
		static void Delete(Serial *serial);

		Serial();
		virtual ~Serial();

		static void SetDefaultSerial(Serial *serial);
		static Serial *GetDefaultSerial();

		int Open(const std::string &port, int baudrate);
		void Close();
		bool IsOpened() const;

		std::string GetPort() const;
		void Update();

		int Write(const char *buf_ptr, int size);
		int Read(char *buf_ptr, int size);

		int Write(const std::string &buffer);
		int Read(std::string &buffer);

		void UpdatePortList();
		std::vector<std::string> GetPortList();
		std::vector<std::string> GetPortDescList();
		std::vector<std::string> GetPortHardIDs();

		int GetNumPorts() const;
		std::string GetPort(int i) const;
		std::string GetPortDesc(int i) const;
		std::string GetPortHardID(int i) const;

		void ClearCMDCallbacks();
		bool IsHasCMDCallback(SerialCMDCallback callBack);
		void AddCMDCallback(SerialCMDCallback callBack);
		void RemoveCMDCallback(SerialCMDCallback callback);

		void ClearCallbacks();
		bool IsHasCallback(SerialCallback callBack);
		void AddCallback(SerialCallback callBack);
		void RemoveCallback(SerialCallback callback);

		void ClearScirptHandlers();
		bool IsHasScriptHandler(const std::string &scriptHandler);
		void AddScriptHandler(const std::string &scriptHandler);
		void RemoveScriptHandler(const std::string &scriptHandler);

		void ClearScriptHandlersHex();
		bool IsHasScriptHandlerHex(const std::string &scriptHandler);
		void AddScriptHandlerHex(const std::string &scriptHandler);
		void RemoveScriptHandlerHex(const std::string &scriptHandler);

		void OnAndroidUSBReceive(const char *buf, int size);

		void SetDoProcessRecv(bool recv);
		bool IsDoProcessRecv() const;

	protected:
		std::string _ProcessRevBuf(const std::string &recvBuf);
		void _OnCmd(const std::string &cmd);

		std::vector<SerialCMDCallback> mCMDCallbacks;
		std::vector<SerialCallback> mCallbacks;
		std::vector<std::string> mScriptHandlers;
		std::vector<std::string> mScriptHandlersHex;

		std::string mSerialPortName;

		std::string mRecvStr;
		int mAllRecvSize;

#if defined (_WIN32) || defined(WIN32) || defined(__LINUX__)
		serial::Serial *mSerial;
#endif
		std::vector<std::string> mPortList;
		std::vector<std::string> mPortDesc;
		std::vector<std::string> mHardIDs;

		bool mIsDoProcessRecv;
		static Serial *msDefaultSerial;
	};

}

#endif