// PX2ScriptManager.hpp

#ifndef PX2SCRIPTMANAGER_HPP
#define PX2SCRIPTMANAGER_HPP

#include "PX2CorePre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2ScriptContext.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ScriptManager : public Singleton <ScriptManager>
	{
	public:
		ScriptManager();
		virtual ~ScriptManager();

		void AddContext(ScriptContext *ctex);
		ScriptContext *GetContext(ScriptContext::ContextType type);
		
		void Ternimate(ScriptContext::ContextType type);
		void TernimateAll();

	protected:
		ScriptContextPtr mContextLua;

	public:
		// 自动化的注册系统，用来自动生成唯一的ID
		bool AddGlobalName(const std::string &objectName, void *object);
		bool RemoveGlobalName(const std::string &objectName);
		bool IsGlobalNameExist(const std::string &objectName);
		int GetNextRegistObjectID();

		void SetUserTypePointer(const std::string &luaName,
			const std::string &className, void *ptr);

	protected:
		std::map<std::string, void*> mGlobals;
		int mNextRegistObjectID;
	};

#define PX2_SM ScriptManager::GetSingleton()
#define PX2_SC_LUA ScriptManager::GetSingleton().GetContext(ScriptContext::CT_LUA)

}

#endif