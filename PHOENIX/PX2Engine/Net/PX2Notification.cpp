// PX2Notification.cpp

#include "PX2Notification.hpp"
#include <typeinfo>
using namespace PX2;

//----------------------------------------------------------------------------
Notification::Notification()
{
}
//----------------------------------------------------------------------------
Notification::~Notification()
{
}
//----------------------------------------------------------------------------
std::string Notification::name() const
{
	return typeid(*this).name();
}
//----------------------------------------------------------------------------