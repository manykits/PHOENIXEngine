// PX2MaxClassDesc.cpp

#include "resource.h"
#include "PX2MaxClassDesc.hpp"
#include "PX2SceneExport.hpp"

Class_ID PX2MaxClassDesc::ID(0x554dbc9c,0x8f17519);
PX2MaxClassDesc PX2MaxClassDesc::TheClassDesc;

HINSTANCE PX2MaxClassDesc::msInstance = 0;
//----------------------------------------------------------------------------
PX2MaxClassDesc::PX2MaxClassDesc ()
{
}
//----------------------------------------------------------------------------
int PX2MaxClassDesc::IsPublic () 
{ 
	return 1; 
}
//----------------------------------------------------------------------------
void* PX2MaxClassDesc::Create (BOOL) 
{ 
	return new PX2SceneExport; 
}
//----------------------------------------------------------------------------
const TCHAR* PX2MaxClassDesc::ClassName () 
{ 
	return _T("PX2SceneExport"); 
}
//----------------------------------------------------------------------------
SClass_ID PX2MaxClassDesc::SuperClassID () 
{ 
	return SCENE_EXPORT_CLASS_ID; 
}
//----------------------------------------------------------------------------
Class_ID PX2MaxClassDesc:: ClassID () 
{ 
	return ID;
}
//----------------------------------------------------------------------------
const TCHAR* PX2MaxClassDesc::Category ()
{ 
	return _T("Exporter"); 
}
//----------------------------------------------------------------------------
const TCHAR* PX2MaxClassDesc::InternalName () 
{ 
	return _T("Phoenix2");
}
//----------------------------------------------------------------------------
HINSTANCE PX2MaxClassDesc::HInstance () 
{ 
	return msInstance; 
}
//----------------------------------------------------------------------------