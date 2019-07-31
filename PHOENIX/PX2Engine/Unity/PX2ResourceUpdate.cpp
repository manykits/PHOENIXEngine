// PX2ResourceUpdate.cpp

#include "PX2ResourceUpdate.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ResourceVersion::ResourceVersion ()
	:
mMain(0),
mLib(0),
mRes(0)
{
}
//----------------------------------------------------------------------------
ResourceVersion::ResourceVersion (const std::string &version)
{
	std::string temp;

	temp = version.substr(0, version.find_first_of('.'));
	mMain = atoi(temp.c_str());

	temp = version.substr(version.find_first_of('.') + 1,
		version.find_last_of('.') - version.find_first_of('.') - 1);
	mLib = atoi(temp.c_str());

	temp = version.substr(version.find_last_of('.') + 1);
	mRes = atoi(temp.c_str());
}
//----------------------------------------------------------------------------
ResourceVersion::ResourceVersion (const ResourceVersion &sv)
{
	mMain = sv.mMain;
	mLib = sv.mRes;
	mRes = sv.mRes;
}
//----------------------------------------------------------------------------
ResourceVersion::~ResourceVersion ()
{
}
//----------------------------------------------------------------------------
std::string ResourceVersion::GetAsString () const
{
	char str[128];
	sprintf(str, "%d.%d.%d", mMain, mLib, mRes);

	return std::string(str);
}
//----------------------------------------------------------------------------
bool ResourceVersion::operator== (const ResourceVersion &b) const
{
	if (mMain == b.mMain &&
		mLib == b.mLib &&
		mRes == b.mRes)
		return true;

	return false;
}
//----------------------------------------------------------------------------
bool ResourceVersion::operator != (const ResourceVersion &b) const
{
	if (mMain != b.mMain || mLib != b.mLib ||
		mRes != b.mRes)
		return true;

	return false;
}
//----------------------------------------------------------------------------
bool operator > (const ResourceVersion& a, const ResourceVersion& b)
{
	if (a.mMain > b.mMain)
	{
		return true;
	}
	else if (a.mMain == b.mMain && a.mLib > b.mLib)
	{
		return true;
	}
	else
	{
		if (a.mMain == b.mMain
			&& a.mLib == b.mLib 
			&& a.mRes > b.mRes)
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool operator < (const ResourceVersion& a, const ResourceVersion& b)
{
	if(a.mMain > b.mMain)
	{
		return false;
	}
	else if(a.mMain == b.mMain 
		&& a.mLib > b.mLib)
	{
		return false;
	}
	else
	{
		if(a.mMain == b.mMain &&
			a.mLib == b.mLib && 
			a.mRes > b.mRes)
		{
			return false;
		}
	}

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ResourceVersionItem
//----------------------------------------------------------------------------
ResourceVersionItem::ResourceVersionItem ():
IsCurrent(false)
{
}
//----------------------------------------------------------------------------
ResourceVersionItem::~ResourceVersionItem()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
ResourceFileInfoItem::ResourceFileInfoItem ()
{
}
//----------------------------------------------------------------------------
ResourceFileInfoItem::~ResourceFileInfoItem ()
{
}
//----------------------------------------------------------------------------