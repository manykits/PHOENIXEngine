// PX2LuaStackBackup.hpp

#ifndef PX2LUASTACKBACKUP_HPP
#define PX2LUASTACKBACKUP_HPP

#include "PX2UnityPre.hpp"
#include "PX2ScriptContext.hpp"

struct lua_State;

namespace PX2
{

	class PX2_ENGINE_ITEM LuaStackBackup
	{
	public:
		LuaStackBackup(lua_State *luaState);
		~LuaStackBackup();

	private:
		lua_State *mState;
		int	mTop;
	};

}

#endif