// PX2GlobalMaterial.hpp

#include "PX2GlobalMaterial.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, GlobalMaterial);
PX2_IMPLEMENT_STREAM(GlobalMaterial);
PX2_IMPLEMENT_ABSTRACT_FACTORY(GlobalMaterial);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, GlobalMaterial);
PX2_IMPLEMENT_DEFAULT_STREAM(Object, GlobalMaterial);

//----------------------------------------------------------------------------
GlobalMaterial::GlobalMaterial ()
{
}
//----------------------------------------------------------------------------
GlobalMaterial::~GlobalMaterial ()
{
}
//----------------------------------------------------------------------------