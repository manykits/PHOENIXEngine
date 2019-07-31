// PX2LuaScriptController.hpp

#ifndef PX2LUASCRIPTCONTROLLER_HPP
#define PX2LUASCRIPTCONTROLLER_HPP

#include "PX2UnityPre.hpp"
#include "PX2ScriptController.hpp"
#include "LuaPlus.h"

namespace PX2
{

	class PX2_ENGINE_ITEM LuaScriptController : public ScriptController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(LuaScriptController);

	public:
		LuaScriptController();
		virtual ~LuaScriptController();

		void SetFileClass(const std::string &filename,
			const std::string &className);
		void SetStringClass(const std::string &str,
			const std::string &className);
		virtual int SelfP() const;

		virtual void ScriptOnAttached();
		virtual void ScriptOnDetach();
		virtual void ScriptOnInitUpdate();
		virtual void ScriptOnUpdate();
		virtual void ScriptOnFixUpdate();

		virtual void ScriptOnReset();
		virtual void ScriptOnPlay();
		virtual void ScriptOnStop();
		virtual void ScriptOnPlayUpdate();

		void SetVal(const std::string &key, int val);
		void SetVal(const std::string &key, bool val);
		void SetVal(const std::string &key, float val);
		void SetVal(const std::string &key, const std::string &val);

	public_internal:
		static void RegisterScriptClass();

	private:
		bool _Init();
		void _CallOnClass(const std::string &className);

		static LuaPlus::LuaObject New(LuaPlus::LuaObject self,
			LuaPlus::LuaObject constructionData,
			LuaPlus::LuaObject originalSubClass);

		static LuaScriptController *sCurOperatorNewController;

	public:
		bool BuildCppDataFromScript(LuaPlus::LuaObject &scriptClass, 
			LuaPlus::LuaObject &constructionData);

		LuaPlus::LuaObject mLO_OnAttached;
		LuaPlus::LuaObject mLO_OnDetach;
		LuaPlus::LuaObject mLO_OnInitUpdate;
		LuaPlus::LuaObject mLO_OnUpdate;
		LuaPlus::LuaObject mLO_OnFixUpdate;

		LuaPlus::LuaObject mLO_OnReset;
		LuaPlus::LuaObject mLO_OnPlay;
		LuaPlus::LuaObject mLO_OnStop;
		LuaPlus::LuaObject mLO_OnPlayUpdate;

	public:
		LuaPlus::LuaObject mLO_Self;
		LuaPlus::LuaState *mLuaState;

		int mSelfP;
		LuaPlus::LuaStackObject mSelfPObject;
	};

	PX2_REGISTER_STREAM(LuaScriptController);
	typedef Pointer0<LuaScriptController> LuaScriptControllerPtr;

}

#endif