// PX2LuaPlusContext.hpp

#ifndef PX2LUAPLUSCONTEXT_HPP
#define PX2LUAPLUSCONTEXT_HPP

#include "PX2UnityPre.hpp"
#include "PX2ScriptContext.hpp"

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}

namespace PX2
{

	class LuaPlusCallObject;

	class PX2_ENGINE_ITEM LuaPlusContext : public ScriptContext
	{
	public:
		LuaPlusContext();
		virtual ~LuaPlusContext();

		LuaPlus::LuaState *GetLuaPlusState();
		void *GetLuaState();

		virtual void Update(double appSeconds, double elapsedSeconds);

		bool CallString1(const char *str);
		bool CallFile1(const char *filename);

		virtual bool CallString(const std::string &str);
		virtual bool CallFile(const std::string &filename);
		virtual bool CallBuffer(const char *buffer, unsigned long size, 
			const std::string &name);

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

		virtual bool CallObjectFuntionValist(const std::string &funName,
			Object *paramObj, const std::string &format, va_list valist);

		void ClearEventFunctions();
		bool RegistEventFunction(const char *entName,
			LuaPlus::LuaObject callFunObject);
		void ClearEventObjectFunctions();
		bool RegistEventObjectFunction(const char *entName, LuaPlus::LuaObject objReset,
			LuaPlus::LuaObject callFunObject);
		virtual void UnRegistAllEventFunctions(const char *entName);
		virtual void OnEvent(Event *ent);

	protected:
		void OnError(int errorNum);
		void ClearStack(void);

		static LuaPlusContext *mSingletonLuaPlusContext;

		LuaPlus::LuaState* mLuaPlusState;
		std::string mLastError;
		std::map<std::string, std::vector<LuaPlusCallObject*> > mEventFunObjects;
	};

}

#endif