// PX2Property.hpp

#ifndef PX2PROPERTY_HPP
#define PX2PROPERTY_HPP

#include "PX2CorePre.hpp"

//----------------------------------------------------------------------------
#define PX2_DECLARE_PROPERTY \
public: \
	virtual void RegistProperties (); \
	virtual void OnPropertyChanged (const PropertyObject &obj)
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_DEFAULT_PROPERTY(baseclassname, classname) \
void classname::RegistProperties () \
{ \
	baseclassname::RegistProperties(); \
} \
\
void classname::OnPropertyChanged (const PropertyObject &obj) \
{ \
	baseclassname::OnPropertyChanged(obj); \
}
//----------------------------------------------------------------------------

#endif