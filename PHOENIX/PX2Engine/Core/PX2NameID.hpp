// PX2NameID.hpp

#ifndef PX2NAMEID_HPP
#define PX2NAMEID_HPP

#include "PX2CorePre.hpp"

//----------------------------------------------------------------------------
#define PX2_DECLARE_NAMES \
public: \
    virtual Object* GetObjectByName (const std::string& name); \
    virtual void GetAllObjectsByName (const std::string& name, \
        std::vector<Object*>& objects)
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_DEFAULT_NAMES(baseclassname, classname) \
Object* classname::GetObjectByName (const std::string& name) \
{ \
    return baseclassname::GetObjectByName(name); \
} \
\
void classname::GetAllObjectsByName (const std::string& name, \
    std::vector<Object*>& objects) \
{ \
    baseclassname::GetAllObjectsByName(name, objects); \
}
//----------------------------------------------------------------------------
#define PX2_GET_OBJECT_BY_NAME(source, name, found) \
if (source) \
{ \
    found = source->GetObjectByName(name); \
    if (found) \
    { \
        return found; \
    } \
}
//----------------------------------------------------------------------------
#define PX2_GET_ALL_OBJECTS_BY_NAME(source, name, objects) \
if (source) \
{ \
    source->GetAllObjectsByName(name, objects); \
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
#define PX2_DECLARE_IDS \
public: \
	virtual Object* GetObjectByID(int id); \
	virtual void GetAllObjectsByID(int id, \
	std::vector<Object*>& objects)
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_DEFAULT_IDS(baseclassname, classname) \
Object* classname::GetObjectByID(int id) \
{ \
	return baseclassname::GetObjectByID(id); \
} \
	\
void classname::GetAllObjectsByID(int id, \
	std::vector<Object*>& objects) \
{ \
	baseclassname::GetAllObjectsByID(id, objects); \
}
//----------------------------------------------------------------------------
#define PX2_GET_OBJECT_BY_ID(source, id, found) \
if (source) \
{ \
	found = source->GetObjectByID(id); \
	if (found) \
	{ \
		return found; \
	} \
}
//----------------------------------------------------------------------------
#define PX2_GET_ALL_OBJECTS_BY_ID(source, id, objects) \
if (source) \
{ \
	source->GetAllObjectsByID(id, objects); \
}
//----------------------------------------------------------------------------


#endif