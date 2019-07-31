// PX2EffectableController.cpp

#include "PX2EffectableController.hpp"
#include "PX2Effectable.hpp"
#include "PX2EffectObject.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, EffectableController);
PX2_IMPLEMENT_STREAM(EffectableController);
PX2_IMPLEMENT_FACTORY(EffectableController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, EffectableController);

//----------------------------------------------------------------------------
EffectableController::EffectableController ()
{
}
//----------------------------------------------------------------------------
EffectableController::~EffectableController ()
{
}
//----------------------------------------------------------------------------
void EffectableController::Reset ()
{
	Controller::Reset();

	Effectable *eba = DynamicCast<Effectable>(GetControlledable());
	if (eba)
	{
		eba->SetBufferEverGenerated(false);
	}
}
//----------------------------------------------------------------------------
void EffectableController::AddModule (EffectModule *moule)
{
	assertion(moule!=0, "moule must not be 0.");
	mModules.push_back(moule);
	moule->SetEffectableController(this);

	std::sort(mModules.begin(), mModules.end(), EffectModule::LessThan);

	AddObjectData data;
	data.ParentObj = this;
	data.Obj = moule;

	Event *ent = GraphicsES::CreateEventX(GraphicsES::AddObject,
		"GraphicsES::AddObject");
	ent->SetDataPointer0(moule);
	ent->SetDataPointer1(this);
	ent->SetData<AddObjectData>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void EffectableController::RemoveModule (EffectModule *moule)
{
	ModuleList::iterator it = mModules.begin();
	for (; it!=mModules.end(); it++)
	{
		if (*it == moule)
		{
			(*it)->OnRemove(*this);
			moule->SetEffectableController(0);

			mModules.erase(it);

			Event *ent = GraphicsES::CreateEventX(GraphicsES::RemoveObject,
				"GraphicsES::RemoveObject");
			ent->SetData<Object*>(moule);
			ent->SetDataPointer0((Object*)moule);
			PX2_EW.BroadcastingLocalEvent(ent);

			return;
		}
	}
}
//----------------------------------------------------------------------------
bool EffectableController::IsHasModule (
	const std::string &moduleRttiName) const
{
	ModuleList::const_iterator it = mModules.begin();
	for (; it!=mModules.end(); it++)
	{
		std::string rttiName = (*it)->GetRttiType().GetName();
		if (rttiName == moduleRttiName)
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool EffectableController::IsHasModuleByTypeName (const std::string &typeName) const
{
	ModuleList::const_iterator it = mModules.begin();
	for (; it!=mModules.end(); it++)
	{
		std::string tn = (*it)->GetTypeName();
		if (typeName == tn)
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
EffectModule *EffectableController::GetModuleByRttiName (
	const std::string &moduleRttiName) const
{
	ModuleList::const_iterator it = mModules.begin();
	for (; it!=mModules.end(); it++)
	{
		std::string rttiName = (*it)->GetRttiType().GetName();
		if (rttiName == moduleRttiName)
		{
			return (*it);
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
EffectModule *EffectableController::GetModuleByTypeName (
	const std::string &typeName) const
{
	ModuleList::const_iterator it = mModules.begin();
	for (; it!=mModules.end(); it++)
	{
		std::string tn = (*it)->GetTypeName();
		if (typeName == tn)
		{
			return (*it);
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
EffectModule *EffectableController::GetModule (int i)
{
	if (0<=i && i<(int)mModules.size())
		return mModules[i];

	return 0;
}
//----------------------------------------------------------------------------
void EffectableController::ModulesUpdateEffectable (float ctrlTime)
{
	Effectable *effectable = DynamicCast<Effectable>(GetControlledable());
	if (!effectable)
		return;

	for (int i=0; i<(int)mModules.size(); i++)
	{
		if (mModules[i] && mModules[i]->IsEnable())
		{
			mModules[i]->UpdateEffectable(effectable, ctrlTime);
		}
	}
}
//----------------------------------------------------------------------------
void EffectableController::ModulesUpdateEffectObject (EffectObject *obj)
{
	for (int i=0; i<(int)mModules.size(); i++)
	{
		if (mModules[i] && mModules[i]->IsEnable())
		{
			mModules[i]->UpdateEffectObject(obj);
		}
	}
}
//----------------------------------------------------------------------------
bool EffectableController::Update(double applicationTime, double elapsedTime)
{
	if (!Controller::Update(applicationTime, elapsedTime))
		return false;

	float playedTimeMinusDelay = GetPlayedTimeMinusDelay();
	if (playedTimeMinusDelay < 0.0f)
		return false;

	ModulesUpdateEffectable(playedTimeMinusDelay);

	return true;
}
//----------------------------------------------------------------------------
void EffectableController::OnNewAEffectObject (EffectObject *obj)
{
	Effectable *eb = (Effectable*)GetControlledable();

	obj->SizeX = eb->GetEmitSizeX();
	obj->SizeY = eb->GetEmitSizeY();
	obj->SizeZ = eb->GetEmitSizeZ();
	obj->SizeXInit = obj->SizeX;
	obj->SizeYInit = obj->SizeY;
	obj->SizeZInit = obj->SizeZ;

	obj->UV0Offset = eb->GetEmitUV0_Offset();
	obj->UV0OffsetInit = obj->UV0Offset;
	obj->UV0Speed = eb->GetEmitUV0_Speed();
	obj->UV0SpeedInit = obj->UV0Speed;
	obj->UV0Repeat = eb->GetEmitUV0_Repeat();
	obj->UV0RepeatInit = obj->UV0Repeat;
	obj->UV0Begin = obj->UV0Offset;
	obj->UV0End[0] = obj->UV0Begin[0] + 1.0f*obj->UV0Repeat[0];
	obj->UV0End[1] = obj->UV0Begin[1] + 1.0f*obj->UV0Repeat[1];

	obj->Color = eb->GetEmitColor();
	obj->ColorInit = obj->Color;

	obj->Alpha = eb->GetEmitAlpha();
	obj->AlphaInit = obj->Alpha;

	obj->Life = eb->GetEmitLife();	

	if (eb->IsAnimStartDoRandom())
		obj->StartRandomIndex = eb->GetAnimStartRandomIndex();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
EffectableController::EffectableController (LoadConstructor value)
	:
Controller(value)
{
}
//----------------------------------------------------------------------------
void EffectableController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	int numModules = 0;
	source.Read(numModules);
	if (numModules > 0)
	{
		mModules.resize(numModules);
		source.ReadPointerVV(numModules, &mModules[0]);
	}

	PX2_END_DEBUG_STREAM_LOAD(EffectableController, source);
}
//----------------------------------------------------------------------------
void EffectableController::Link (InStream& source)
{
	Controller::Link(source);

	const int numModules = (int)mModules.size();
	for (int i=0; i<numModules; ++i)
	{
		if (mModules[i])
		{
			source.ResolveLink(mModules[i]);
		}
	}
}
//----------------------------------------------------------------------------
void EffectableController::PostLink ()
{
	Controller::PostLink();

	for (int i=0; i<GetNumModules(); i++)
	{
		mModules[i]->SetEffectableController(this);
	}
}
//----------------------------------------------------------------------------
bool EffectableController::Register (OutStream& target) const
{
	if (Controller::Register(target))
	{
		const int numModules = (int)mModules.size();
		for (int i=0; i<numModules; ++i)
		{
			if (mModules[i])
			{
				target.Register(mModules[i]);
			}
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EffectableController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	const int numModules = (int)mModules.size();
	target.Write(numModules);
	for (int i=0; i<numModules; ++i)
	{
		if (mModules[i])
		{
			target.WritePointer(mModules[i]);
		}
		else
		{
			target.WritePointer((EffectModule*)0);
		}
	}

	PX2_END_DEBUG_STREAM_SAVE(EffectableController, target);
}
//----------------------------------------------------------------------------
int EffectableController::GetStreamingSize (Stream &stream) const
{
	int numModules = (int)mModules.size();

	int size = Controller::GetStreamingSize(stream)
		+ PX2_VERSION_SIZE(mVersion)
		+ sizeof(numModules) // numModules
		+ PX2_POINTERSIZE(mModules[i])*numModules; 

	return size;
}
//----------------------------------------------------------------------------
