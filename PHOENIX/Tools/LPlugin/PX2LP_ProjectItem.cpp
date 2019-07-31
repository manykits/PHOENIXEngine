// PX2LP_ProjectItem.cpp

#include "PX2LP_ProjectItem.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
LP_ProjectItem::LP_ProjectItem()
{
	ID = 0;

	IsLocalExist = false;
	IsCloud = false;

	TheItem = 0;
	TheCheckButton = 0;
	TheCloudButton = 0;
}
//----------------------------------------------------------------------------
LP_ProjectItem::~LP_ProjectItem()
{
}
//----------------------------------------------------------------------------