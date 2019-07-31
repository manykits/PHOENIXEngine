// PX2LuaContext.hpp

#ifndef PX2LUACONTEXT_HPP
#define PX2LUACONTEXT_HPP

#include "PX2UnityPre.hpp"
#include "PX2ScriptContext.hpp"

struct lua_State;

namespace PX2
{

	class PX2_ENGINE_ITEM LuaContext : public ScriptContext
	{
	public:
		LuaContext();
		virtual ~LuaContext();

		void SetLuaState (lua_State *state);
		lua_State *GetLuaState ();

		// general
		virtual bool CallString(const std::string &str);
		virtual bool CallFile(const std::string &filename);
		virtual bool CallBuffer(const char *buffer, unsigned long size, 
			const std::string &name);

		// file function
		// 直接调用文件
		virtual bool CallFileFunction(const std::string &filename,
			const std::string &funName);

		// type pointer function
		virtual void SetUserTypePointer(const std::string &luaName,
			const std::string &className, void *ptr);
		virtual void SetUserFunction(const std::string &funName,
			const std::string &returnClassTypeName,
			ScriptGlobalFun globalFun);

		// object functions
		virtual bool CallObjectFuntionValist(const std::string &funName,
			Object *paramObj, const std::string &format, va_list valist);

	protected:
		lua_State *mState;
	};

#include "PX2LuaContext.inl"

}

#endif