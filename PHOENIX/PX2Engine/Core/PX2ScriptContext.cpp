// PX2ScriptContext.cpp

#include "PX2ScriptContext.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ScriptContext::ScriptContext(ContextType type) :
mType(type)
{
}
//----------------------------------------------------------------------------
ScriptContext::~ScriptContext()
{

}
//----------------------------------------------------------------------------
ScriptContext::ContextType ScriptContext::GetType() const
{
	return mType;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallString(const std::string &str)
{
	PX2_UNUSED(str);

	return false;
}
//----------------------------------------------------------------------------
void ScriptContext::Update(double appSeconds, double elapsedSeconds)
{
	PX2_UNUSED(appSeconds);
	PX2_UNUSED(elapsedSeconds);
}
//----------------------------------------------------------------------------
bool ScriptContext::CallFile(const std::string &filename)
{
	PX2_UNUSED(filename);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallBuffer(const char *buffer, unsigned long size, 
	const std::string &name)
{
	PX2_UNUSED(buffer);
	PX2_UNUSED(size);
	PX2_UNUSED(name);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallFileFunction(const std::string &filename,
	const std::string &funName)
{
	PX2_UNUSED(filename);
	PX2_UNUSED(funName);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallFunction(const std::string &funName, Object *obj0)
{
	PX2_UNUSED(funName);
	PX2_UNUSED(obj0);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallFunction(const std::string &funName,
	Object *obj0, int paramData1)
{
	PX2_UNUSED(funName);
	PX2_UNUSED(obj0);
	PX2_UNUSED(paramData1);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallFunction(const std::string &funName,
	Object *obj0, const std::string &paramStr)
{
	PX2_UNUSED(funName);
	PX2_UNUSED(obj0);
	PX2_UNUSED(paramStr);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallFunction(const std::string &funName,
	Object *obj0, int iData, const std::string &paramStr)
{
	PX2_UNUSED(funName);
	PX2_UNUSED(obj0);
	PX2_UNUSED(iData);
	PX2_UNUSED(paramStr);

	return false;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallObjectFunction(const std::string &funName,
	Object *paramObj, const char *format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	bool ret = CallObjectFuntionValist(funName, paramObj, format, argptr);
	va_end(argptr);

	return ret;
}
//----------------------------------------------------------------------------
bool ScriptContext::CallObjectFuntionValist(const std::string &funName,
	Object *paramObj, const std::string &format, va_list valist)
{
	PX2_UNUSED(funName);
	PX2_UNUSED(paramObj);
	PX2_UNUSED(format);
	PX2_UNUSED(valist);

	return false;
}
//----------------------------------------------------------------------------
void ScriptContext::SetUserTypePointer(const std::string &luaName,
	const std::string &className, void *ptr)
{
	PX2_UNUSED(luaName);
	PX2_UNUSED(className);
	PX2_UNUSED(ptr);
}
//----------------------------------------------------------------------------
void ScriptContext::SetUserFunction(const std::string &funName,
	const std::string &returnClassTypeName, ScriptGlobalFun globalFun)
{
	PX2_UNUSED(returnClassTypeName);
	PX2_UNUSED(funName);
	PX2_UNUSED(globalFun);
}
//----------------------------------------------------------------------------
void ScriptContext::RegistOperators()
{
}
//----------------------------------------------------------------------------
void ScriptContext::ClearEventFunctions()
{
}
//----------------------------------------------------------------------------
void ScriptContext::ClearEventObjectFunctions()
{
}
//----------------------------------------------------------------------------
void ScriptContext::UnRegistAllEventFunctions(const char *entName)
{
	PX2_UNUSED(entName);
}
//----------------------------------------------------------------------------