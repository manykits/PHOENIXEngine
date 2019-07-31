// PX2GPIO.cpp

#include "PX2GPIO.hpp"
#include "PX2Log.hpp"
#include "PX2StringHelp.hpp"
#include "PX2System.hpp"

#if defined (__LINUX__)
#include <sys/stat.h>
#endif

using namespace PX2;

//----------------------------------------------------------------------------
GPIO::GPIO() :
mPort(0),
mDir(DT_OUT)
{
}
//----------------------------------------------------------------------------
GPIO::GPIO(unsigned gpio, DirectionType dir)
{
	this->mPort = gpio;
	this->mDir = dir;

	this->_ExportPort();
	this->SetDirection(dir);
}
//----------------------------------------------------------------------------
GPIO::~GPIO()
{
	_UnexportPort();
}
//----------------------------------------------------------------------------
bool GPIO::IsExist(unsigned gpio)
{
	PX2_UNUSED(gpio);

#if defined (__LINUX__)
	struct stat buff;
	std::stringstream ss;
	ss << "/sys/class/gpio/gpio";
	ss << gpio;

	if (0 == lstat(ss.str().c_str(), &buff))
		return true;
#endif

	return false;
}
//----------------------------------------------------------------------------
void GPIO::SetValue(int val)
{
	SetValue(mPort, val);
}
//----------------------------------------------------------------------------
int GPIO::GetValue() const
{
	int val = 0;
	if (GetValue(mPort, val))
	{
		return val;
	}

	return -1;
}
//----------------------------------------------------------------------------
GPIO::DirectionType GPIO::GetDirection() const 
{
	std::stringstream ss;
	ss << "/sys/class/gpio/gpio";
	ss << this->mPort;
	ss << "/direction";

	std::ifstream in(ss.str().c_str());

	if (in.fail()) 
	{
		std::cout << "read gpio" << this->mPort
			<< " Direction fail!" << std::endl;
		return DirectionType::DT_IN;
	}

	std::string str;

	in >> str;
	in.close();

	if (str == "in")
		return DirectionType::DT_IN;
	else
		return DirectionType::DT_OUT;
}
//----------------------------------------------------------------------------
void GPIO::SetDirection(DirectionType dir)
{
	if (!IsExist(mPort))
		_ExportPort();

	std::stringstream ss;
	ss << "/sys/class/gpio/gpio";
	ss << this->mPort;
	ss << "/direction";

	std::ofstream out(ss.str().c_str());

	if (out.fail()) 
	{
		std::cout << "set gpio" << this->mPort
			<< " Direction fail!" << std::endl;
		return;
	}

	if (dir == DT_IN)
		out << "in";
	else
		out << "out";

	out.close();
}
//----------------------------------------------------------------------------
void GPIO::SetPort(unsigned port)
{
	if (port == this->mPort)
		return;

	if (mPort> 0 && IsExist(mPort))
		this->_UnexportPort();
	this->mPort = port;
	this->_ExportPort();
	this->SetDirection(this->mDir);
}
//----------------------------------------------------------------------------
void GPIO::Down(unsigned times) const
{
	while (times--) 
	{
		this->SetHigh();
		this->SetLow();
		_Sleep(1);
	}
}
//----------------------------------------------------------------------------
void GPIO::_Sleep(int val) const
{
	System::SleepSeconds(val * 0.000001f);
}
//----------------------------------------------------------------------------
void GPIO::Pulse(unsigned lowWidth, unsigned highWidth, unsigned times) const {

	while (times--) 
	{
		this->SetLow();
		_Sleep(lowWidth);
		this->SetHigh();
		_Sleep(highWidth);
	}
}
//----------------------------------------------------------------------------
void GPIO::Up(unsigned times) const 
{

	while (times--) {
		this->SetLow();
		this->SetHigh();
		_Sleep(1);
	}
}
//----------------------------------------------------------------------------
void GPIO::SetHigh() const 
{
	std::stringstream ss;
	ss << "/sys/class/gpio/gpio";
	ss << this->mPort;
	ss << "/value";

	std::ofstream out(ss.str().c_str());

	if (out.fail()) 
	{
		std::cout << "set gpio" << this->mPort
			<< " to SetHigh fail!" << std::endl;
		return;
	}

	out << 1;
	out.close();
}
//----------------------------------------------------------------------------
void GPIO::SetLow() const 
{
	std::stringstream ss;
	ss << "/sys/class/gpio/gpio";
	ss << this->mPort;
	ss << "/value";

	std::ofstream out(ss.str().c_str());

	if (out.fail()) {
		std::cout << "set gpio" << this->mPort
			<< " to low fail!" << std::endl;
		return;
	}

	out << 0;
	out.close();
}
//----------------------------------------------------------------------------
void GPIO::_UnexportPort() const
{
	std::string str("/sys/class/gpio/unexport");

	std::ofstream out(str.c_str());

	if (out.fail()) 
	{
		std::cout << "unexport GPIO" << this->mPort << " fail!" << std::endl;
		return;
	}

	out << this->mPort;
	out.close();

	std::cout << "unexport GPIO" << this->mPort << std::endl;
}
//----------------------------------------------------------------------------
void GPIO::_ExportPort() const
{
	if (this->IsExist(this->mPort)) 
	{
		std::cout << "Warning: GPIO" << this->mPort << " already setup!" << std::endl;
		return;
	}

	std::string str("/sys/class/gpio/export");

	std::ofstream out(str.c_str());

	if (out.fail())
	{
		std::cout << "export GPIO" << this->mPort << " fail!" << std::endl;
		return;
	}

	out << this->mPort;

	out.close();
}
//----------------------------------------------------------------------------
void GPIO::Set(unsigned port, DirectionType dir) 
{
	this->SetPort(port);
	this->SetDirection(dir);
}
//----------------------------------------------------------------------------
bool GPIO::Export(int gpio)
{
	if (IsExist(gpio))
	{
		return true;
	}

	std::string strGPIO = StringHelp::IntToString(gpio);

	std::string export_str = "/sys/class/gpio/export";
	std::ofstream exportgpio(export_str.c_str());
	if (!exportgpio.is_open())
	{
		PX2_LOG_ERROR("OPERATION FAILED : Unable to export GPIO %s", strGPIO.c_str());
		return false;
	}

	exportgpio << strGPIO;
	exportgpio.close();

	return true;
}
//----------------------------------------------------------------------------
bool GPIO::Unexport(int gpio)
{
	std::string strGPIO = StringHelp::IntToString(gpio);

	std::string unexport_str = "/sys/class/gpio/unexport";
	std::ofstream unexportgpio(unexport_str.c_str());
	if (!unexportgpio.is_open())
	{
		PX2_LOG_ERROR("OPERATION FAILED: Unable to unexport GPIO %s", strGPIO.c_str());
		return false;
	}

	unexportgpio << strGPIO;
	unexportgpio.close();

	return true;
}
//----------------------------------------------------------------------------
bool GPIO::SetDriection(int gpio, const std::string &dir)
{
	std::string strGPIO = StringHelp::IntToString(gpio);

	std::string setdir_str = "/sys/class/gpio/gpio" + strGPIO + "/direction";
	std::ofstream setdirgpio(setdir_str.c_str());
	if (!setdirgpio.is_open())
	{
		PX2_LOG_ERROR("OPERATION FAILED: Unable to set direction of GPIO %s", strGPIO.c_str());
		return false;
	}

	setdirgpio << dir;
	setdirgpio.close();

	return true;
}
//----------------------------------------------------------------------------
bool GPIO::SetValue(int gpio, int val)
{
	std::string strGPIO = StringHelp::IntToString(gpio);
	std::string strVal = StringHelp::IntToString(val);

	std::string setval_str = "/sys/class/gpio/gpio" + strGPIO + "/value";
	std::ofstream setvalgpio(setval_str.c_str());
	if (!setvalgpio.is_open())
	{
		PX2_LOG_ERROR("OPERATION FAILED: Unable to set the value of GPIO %s", strGPIO.c_str());
		return false;
	}

	setvalgpio << strVal;
	setvalgpio.close();

	return true;
}
//----------------------------------------------------------------------------
bool GPIO::GetValue(int gpio, int& val)
{
	std::string strGPIO = StringHelp::IntToString(gpio);

	std::string getval_str = "/sys/class/gpio/gpio" + strGPIO + "/value";
	std::ifstream getvalgpio(getval_str.c_str());
	if (!getvalgpio.is_open())
	{
		PX2_LOG_ERROR("OPERATION FAILED: Unable to get the value of GPIO %s", strGPIO.c_str());
		return false;
	}

	std::string strVal;
	getvalgpio >> strVal;

	if (strVal == "0")
		val = 0;
	else
		val = 1;

	getvalgpio.close();

	return true;
}
//----------------------------------------------------------------------------