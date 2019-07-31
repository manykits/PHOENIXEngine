// PX2MaxClassDesc.hpp

#ifndef PX2MAXCLASSDESC_HPP
#define PX2MAXCLASSDESC_HPP

#include "PX2MaxToPX2PluginPre.hpp"

class PX2MaxClassDesc : public ClassDesc2
{
public:
	PX2MaxClassDesc ();

	virtual int IsPublic ();
	virtual void *Create (BOOL loading = FALSE);

	virtual const TCHAR* ClassName ();
	virtual SClass_ID SuperClassID ();
	virtual Class_ID ClassID ();
	virtual const TCHAR* Category ();
	virtual const TCHAR* InternalName ();
	virtual HINSTANCE HInstance();

	static HINSTANCE msInstance;

	static Class_ID ID;
	static PX2MaxClassDesc TheClassDesc;
};

#endif