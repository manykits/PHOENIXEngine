// PX2LuaScriptController.cpp

#include "PX2LuaScriptController.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2Controlledable.hpp"
#include "PX2LuaPlusContext.hpp"
#include "PX2Log.hpp"
using namespace PX2;
using namespace LuaPlus;

PX2_IMPLEMENT_RTTI(PX2, ScriptController, LuaScriptController);
PX2_IMPLEMENT_STREAM(LuaScriptController);
PX2_IMPLEMENT_DEFAULT_NAMES(ScriptController, LuaScriptController);
PX2_IMPLEMENT_FACTORY(LuaScriptController);

//---------------------------------------------------------------------------
const char* LuaSCRIPCONTROLLER_NAME = "LuaScriptController";
const char* gST_NAME = "gScriptTable";
//---------------------------------------------------------------------------
LuaScriptController *LuaScriptController::sCurOperatorNewController = 0;
//---------------------------------------------------------------------------
LuaScriptController::LuaScriptController() :
mLuaState(0),
mSelfP(0)
{
	SetName("LuaScriptController");
	_Init();
}
//---------------------------------------------------------------------------
LuaScriptController::~LuaScriptController()
{
	if (0 != mSelfP)
	{
		LuaObject nilLO;
		nilLO.AssignNil(mLuaState);

		LuaPlus::LuaObject gst = mLuaState->GetGlobals().Lookup(gST_NAME);
		assertion(!gst.IsNil(), "");
		gst.SetObject(mSelfPObject, nilLO);
	}

	UnRegistToScriptSystem();
}
//---------------------------------------------------------------------------
LuaPlus::LuaObject LuaScriptController::New(LuaPlus::LuaObject self,
	LuaPlus::LuaObject constructionData,
	LuaPlus::LuaObject originalSubClass)
{
	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;
	LuaPlus::LuaState *luaState = luaContext->GetLuaPlusState();

	LuaScriptController *ctrl = 0;
	if (sCurOperatorNewController)
	{
		ctrl = sCurOperatorNewController;
	}
	else
		ctrl = new0 LuaScriptController;

	ctrl->mLO_Self.AssignNewTable(*luaState);
	if (ctrl->BuildCppDataFromScript(originalSubClass, constructionData))
	{
		LuaPlus::LuaObject metaTableObj = luaState->GetGlobals().Lookup(LuaSCRIPCONTROLLER_NAME);
		assertion(!metaTableObj.IsNil(), "");

		ctrl->mLO_Self.SetLightUserdata("__object", ctrl);
		ctrl->mLO_Self.SetMetatable(metaTableObj);

		LuaPlus::LuaObject gst = luaState->GetGlobals().Lookup(gST_NAME);
		assertion(!gst.IsNil(), "");
		ctrl->mSelfPObject = luaState->BoxPointer(ctrl);
		gst.SetObject(ctrl->mSelfPObject, ctrl->mLO_Self);
		ctrl->mSelfP = ctrl->mSelfPObject.m_stackIndex;
	}
	else
	{
		ctrl->mLO_Self.AssignNil(luaState);
		delete0(ctrl);
	}

	return ctrl->mLO_Self;
}
//---------------------------------------------------------------------------
void LuaScriptController::SetFileClass(const std::string &filename,
	const std::string &className)
{
	ScriptController::SetFileClass(filename, className);

	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;
	luaContext->CallFile(filename);

	_CallOnClass(className);
}
//---------------------------------------------------------------------------
void LuaScriptController::SetStringClass(const std::string &str,
	const std::string &className)
{
	ScriptController::SetStringClass(str, className);

	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;
	luaContext->CallString(str);

	_CallOnClass(className);
}
//---------------------------------------------------------------------------
void LuaScriptController::_CallOnClass(const std::string &className)
{
	LuaPlus::LuaObject subClass = mLuaState->GetGlobal(className.c_str());
	LuaPlus::LuaObject funNew = subClass.GetByName("New");
	if (!funNew.IsNil())
	{
		sCurOperatorNewController = this;
		LuaCall call = funNew;
		call << subClass << LuaRun();
		sCurOperatorNewController = 0;
	}

	//LuaPlus::LuaObject constructionData;
	//constructionData.AssignNewTable(mLuaState);

	//mLO_Self.AssignNewTable(mLuaState);
	//if (BuildCppDataFromScript(subClass, constructionData))
	//{
	//	LuaPlus::LuaObject metaTableObj = mLuaState->GetGlobals().Lookup(
	//		LuaSCRIPCONTROLLER_NAME);
	//	assertion(!metaTableObj.IsNil(), "");

	//	mLO_Self.SetLightUserdata("__object", this);
	//	mLO_Self.SetMetatable(metaTableObj);

	//	LuaPlus::LuaObject gst = mLuaState->GetGlobals().Lookup(gST_NAME);
	//	assertion(!gst.IsNil(), "");
	//	mSelfPObject = mLuaState->BoxPointer(this);
	//	gst.SetObject(mSelfPObject, mLO_Self);
	//	mSelfP = mSelfPObject.m_stackIndex;
	//}
	//else
	//{
	//	mLO_Self.AssignNil(mLuaState);
	//}
}
//---------------------------------------------------------------------------
bool LuaScriptController::BuildCppDataFromScript(
	LuaPlus::LuaObject &scriptClass,
	LuaPlus::LuaObject &constructionData)
{
	if (scriptClass.IsTable())
	{
		// OnInit()
		LuaPlus::LuaObject temp;
		temp = scriptClass.GetByName("OnAttached");
		if (temp.IsFunction())
			mLO_OnAttached = temp;

		temp = scriptClass.GetByName("OnDetach");
		if (temp.IsFunction())
			mLO_OnDetach = temp;

		temp = scriptClass.GetByName("OnInitUpdate");
		if (temp.IsFunction())
			mLO_OnInitUpdate = temp;

		temp = scriptClass.GetByName("OnUpdate");
		if (temp.IsFunction())
			mLO_OnUpdate = temp;

		temp = scriptClass.GetByName("OnFixUpdate");
		if (temp.IsFunction())
			mLO_OnFixUpdate = temp;

		temp = scriptClass.GetByName("OnPReset");
		if (temp.IsFunction())
			mLO_OnReset = temp;

		temp = scriptClass.GetByName("OnPPlay");
		if (temp.IsFunction())
			mLO_OnPlay = temp;

		temp = scriptClass.GetByName("OnPStop");
		if (temp.IsFunction())
			mLO_OnStop = temp;

		temp = scriptClass.GetByName("OnPUpdate");
		if (temp.IsFunction())
			mLO_OnPlayUpdate = temp;
	}
	else
	{
		PX2_LOG_ERROR(
			"scriptClass is not a table in LuaScriptController::BuildCppDataFromScript()");
		return false;
	}

	if (constructionData.IsTable())
	{
		for (LuaPlus::LuaTableIterator constructionDataIt(constructionData); 
			constructionDataIt; constructionDataIt.Next())
		{
			const char* key = constructionDataIt.GetKey().GetString();
			LuaPlus::LuaObject val = constructionDataIt.GetValue();

			mLO_Self.SetObject(key, val);
		}
	}

	return true;
}
//---------------------------------------------------------------------------
bool LuaScriptController::_Init()
{
	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;
	mLuaState = luaContext->GetLuaPlusState();

	mLO_OnAttached.AssignNil(mLuaState);
	mLO_OnDetach.AssignNil(mLuaState);
	mLO_OnUpdate.AssignNil(mLuaState);
	mLO_OnFixUpdate.AssignNil(mLuaState);

	mLO_OnReset.AssignNil(mLuaState);
	mLO_OnPlay.AssignNil(mLuaState);
	mLO_OnStop.AssignNil(mLuaState);
	mLO_OnPlayUpdate.AssignNil(mLuaState);

	mLO_Self.AssignNil(mLuaState);

	mLuaState->GetGlobals().RegisterDirect("SelfP", (*this), &LuaScriptController::SelfP);

	return true;
}
//---------------------------------------------------------------------------
int LuaScriptController::SelfP() const
{
	return mSelfP;
}
//---------------------------------------------------------------------------
void LuaScriptController::SetVal(const std::string &key, int val)
{
	mLO_Self.SetInteger(key.c_str(), val);
}
//---------------------------------------------------------------------------
void LuaScriptController::SetVal(const std::string &key, bool val)
{
	mLO_Self.SetBoolean(key.c_str(), val);
}
//---------------------------------------------------------------------------
void LuaScriptController::SetVal(const std::string &key, float val)
{
	mLO_Self.SetNumber(key.c_str(), (double)val);
}
//---------------------------------------------------------------------------
void LuaScriptController::SetVal(const std::string &key, const std::string &val)
{
	mLO_Self.SetString(key.c_str(), val.c_str(), (int)val.length());
}
//---------------------------------------------------------------------------
void LuaScriptController::RegisterScriptClass()
{
	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;
	LuaPlus::LuaState *luaState = luaContext->GetLuaPlusState();
	PX2_UNUSED(luaState);

	LuaPlus::LuaObject metaTableObj = luaContext->GetLuaPlusState()
			->GetGlobals().CreateTable(LuaSCRIPCONTROLLER_NAME);

	metaTableObj.SetObject("__index", metaTableObj);
	metaTableObj.SetObject("base", metaTableObj);  // base refers to the parent class; ie the metatable
	metaTableObj.SetBoolean("cpp", true);

	// new
	metaTableObj.RegisterDirect("New", &LuaScriptController::New);

	// 
	LuaPlus::LuaObject gst = metaTableObj = luaContext->GetLuaPlusState()
		->GetGlobals().CreateTable(gST_NAME);
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnAttached()
{
	if (!mLO_OnAttached.IsNil()) 
	{
		try
		{
			LuaCall call = mLO_OnAttached;
			call << mLO_Self << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnDetach()
{
	if (!mLO_OnDetach.IsNil())
	{
		try
		{
			LuaCall call = mLO_OnDetach;
			call << mLO_Self << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnInitUpdate()
{
	if (!mLO_OnInitUpdate.IsNil())
	{
		try
		{
			LuaCall call = mLO_OnInitUpdate;
			call << mLO_Self << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnUpdate()
{
	if (!mLO_OnUpdate.IsNil())
	{
		try
		{
			LuaCall call = mLO_OnUpdate;
			call << mLO_Self << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnFixUpdate()
{
	if (!mLO_OnFixUpdate.IsNil())
	{
		try
		{
			LuaCall call = mLO_OnFixUpdate;
			call << mLO_Self << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnReset()
{
	if (!mLO_OnReset.IsNil())
	{
		try
		{
			LuaCall call = mLO_OnReset;
			call << mLO_Self << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnPlay()
{
	if (!mLO_OnPlay.IsNil())
	{
		try
		{
			LuaCall call = mLO_OnPlay;
			call << mLO_Self << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnStop()
{
	if (!mLO_OnStop.IsNil())
	{
		try
		{
			LuaCall call = mLO_OnStop;
			call << mLO_Self << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}
	}
}
//---------------------------------------------------------------------------
void LuaScriptController::ScriptOnPlayUpdate()
{
	if (!mLO_OnPlayUpdate.IsNil())
	{
		try
		{
			LuaCall call = mLO_OnPlayUpdate;
			call << mLO_Self << LuaRun();
		}
		catch (LuaException &e)
		{
			const char* msg = e.GetErrorMessage();
			PX2_LOG_ERROR("%s", msg);
			assertion(false, "%s", msg);
		}
	}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ScriptController
//---------------------------------------------------------------------------
void LuaScriptController::RegistProperties()
{
	ScriptController::RegistProperties();

	AddPropertyClass("LuaScriptController");
}
//---------------------------------------------------------------------------
void LuaScriptController::OnPropertyChanged(const PropertyObject &obj)
{
	ScriptController::OnPropertyChanged(obj);
}
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
LuaScriptController::LuaScriptController(LoadConstructor value) :
ScriptController(value),
mLuaState(0),
mSelfP(0)
{
}
//----------------------------------------------------------------------------
void LuaScriptController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	ScriptController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LuaScriptController, source);
}
//----------------------------------------------------------------------------
void LuaScriptController::Link(InStream& source)
{
	ScriptController::Link(source);

	_Init();
	SetFileClass(mFilename, mClassName);
}
//----------------------------------------------------------------------------
void LuaScriptController::PostLink()
{
	ScriptController::PostLink();

	RegistToScriptSystem();
}
//----------------------------------------------------------------------------
bool LuaScriptController::Register(OutStream& target) const
{
	if (ScriptController::Register(target))
	{
		target.Register(mObject);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LuaScriptController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	ScriptController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LuaScriptController, target);
}
//----------------------------------------------------------------------------
int LuaScriptController::GetStreamingSize(Stream &stream) const
{
	int size = ScriptController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------