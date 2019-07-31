// PX2LidarSerialConnection.hpp

#ifndef PX2LIDARSERIALCONNECTION_HPP
#define PX2LIDARSERIALCONNECTION_HPP

#include "PX2CorePre.hpp"
#include "CDeviceConnection.h"
#include "PX2Serial.hpp"

namespace PX2
{

	class SerialConnection : public everest::hwdrivers::CDeviceConnection
	{
	public:
		SerialConnection();
		virtual ~SerialConnection();

		int SetParam(const std::string &port, int baudrate);

		virtual int read(const char *data, unsigned int size,
			unsigned int ms_wait = 0) ;
		virtual int write(const char *data, unsigned int size);

		virtual int getStatus(void);

		virtual bool openSimple(void);

		virtual bool close(void);

		virtual const char * getOpenMessage(int message_number);

	protected:
		Serial *mSerial;
		int mStatus;
		std::string mProt;
		int mBaudrate;
	};

}

#endif