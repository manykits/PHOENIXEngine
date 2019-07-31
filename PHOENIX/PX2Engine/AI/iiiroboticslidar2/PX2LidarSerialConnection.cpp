// PX2LidarSerialConnection.cpp

#include "PX2LidarSerialConnection.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SerialConnection::SerialConnection()
{
	mSerial = new0 Serial();
	mStatus = STATUS_NEVER_OPENED;
}
//----------------------------------------------------------------------------
SerialConnection::~SerialConnection()
{
	if (mSerial)
	{
		mSerial->Close();
		delete0(mSerial);
	}
}
//----------------------------------------------------------------------------
int SerialConnection::SetParam(const std::string &port, int baudrate)
{
	mProt = port;
	mBaudrate = baudrate;

	return -1;
}
//----------------------------------------------------------------------------
int SerialConnection::read(const char *data, unsigned int size,
	unsigned int ms_wait)
{
	return mSerial->Read((char*)data, size);
}
//----------------------------------------------------------------------------
int SerialConnection::write(const char *data, unsigned int size)
{
	return mSerial->Write(data, size);
}
//----------------------------------------------------------------------------
int SerialConnection::getStatus(void)
{
	return mStatus;
}
//----------------------------------------------------------------------------
bool SerialConnection::openSimple(void)
{
	if (0 == mSerial->Open(mProt, mBaudrate))
	{
		mStatus = STATUS_OPEN;
		return true;
	}
	else
	{
		mStatus = STATUS_OPEN_FAILED;
		return false;
	}
}
//----------------------------------------------------------------------------
bool SerialConnection::close(void)
{
	if (mSerial)
	{
		mStatus = STATUS_CLOSED_NORMALLY;
		mSerial->Close();

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
const char * SerialConnection::getOpenMessage(int message_number)
{
	return m_str_map[message_number].c_str();
}
//----------------------------------------------------------------------------