// PX2Visitor.cpp

#include "PX2Visitor.hpp"
#include "PX2Any.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Visitor::Visitor ()
{
}
//----------------------------------------------------------------------------
Visitor::~Visitor ()
{
}
//----------------------------------------------------------------------------
void Visitor::Visit (Object *obj, int info)
{
	PX2_UNUSED(obj);
	PX2_UNUSED(info);
}
//----------------------------------------------------------------------------
void Visitor::Visit (Object *obj, const std::string &info)
{
	PX2_UNUSED(obj);
	PX2_UNUSED(info);
}
//----------------------------------------------------------------------------
void Visitor::Visit (Object *obj, const Any &info)
{
	PX2_UNUSED(obj);
	PX2_UNUSED(info);
}
//----------------------------------------------------------------------------