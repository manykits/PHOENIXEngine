// PX2Rtti.cpp

#include "PX2Rtti.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Rtti::Rtti (const char* name, unsigned short version, const Rtti* baseType)
{
    mName = name;
	mVersion = version;
    mBaseType = baseType;
}
//----------------------------------------------------------------------------
Rtti::~Rtti ()
{
}
//----------------------------------------------------------------------------
bool Rtti::IsDerived (const Rtti& type) const
{
    const Rtti* search = this;
    while (search)
    {
        if (search == &type)
        {
            return true;
        }
        search = search->mBaseType;
    }
    return false;
}
//----------------------------------------------------------------------------
