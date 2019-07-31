// PX2ScriptContext.hpp

#ifndef PX2SCRIPTCONTEXT_HPP
#define PX2SCRIPTCONTEXT_HPP

#include "PX2CorePre.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	typedef void *(*ScriptGlobalFun) ();

	class PX2_ENGINE_ITEM ScriptContext : public Object
	{
	public:
		enum ContextType
		{
			CT_NONE,
			CT_LUA,
			CT_MAX_TYPE
		};

		ScriptContext(ContextType type = CT_NONE);
		virtual ~ScriptContext();

		ContextType GetType() const;

		virtual void Update(double appSeconds, double elapsedSeconds);

		// general
		virtual bool CallString(const std::string &str);
		virtual bool CallFile(const std::string &filename);
		virtual bool CallBuffer(const char *buffer, unsigned long size,
			const std::string &name);

		// file function
		virtual bool CallFileFunction(const std::string &filename, 
			const std::string &funName);
		virtual bool CallFunction(const std::string &funName, Object *obj0);
		virtual bool CallFunction(const std::string &funName,
			Object *obj0, int paramData1);
		virtual bool CallFunction(const std::string &funName,
			Object *obj0, const std::string &paramStr);
		virtual bool CallFunction(const std::string &funName,
			Object *obj0, int iData, const std::string &paramStr);

		// type pointer function
		virtual void SetUserTypePointer(const std::string &luaName,
			const std::string &className, void *ptr);
		virtual void SetUserFunction(const std::string &funName,
			const std::string &returnClassTypeName,
			ScriptGlobalFun globalFun);

		// object functions
		bool CallObjectFunction(const std::string &funName,
			Object *paramObj, const char *format, ...);
		virtual bool CallObjectFuntionValist(const std::string &funName,
			Object *paramObj, const std::string &format, va_list valist);

		virtual void RegistOperators();

		virtual void ClearEventFunctions();
		virtual void ClearEventObjectFunctions();
		virtual void UnRegistAllEventFunctions(const char *entName);

	protected:
		ContextType mType;
	};
	typedef Pointer0<ScriptContext> ScriptContextPtr;

}

#endif