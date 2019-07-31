// PX2Controlledable.cpp

#include "PX2Controlledable.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, Controlledable);
PX2_IMPLEMENT_STREAM(Controlledable);
PX2_IMPLEMENT_ABSTRACT_FACTORY(Controlledable);

//----------------------------------------------------------------------------
Controlledable::Controlledable () :
mIsSelfCtrled(false),
mIsAutoPlay(false)
{
	mControlUpdateTimeLast = -1.0;
	mControlUpdateTimeMin = 0.02;
}
//----------------------------------------------------------------------------
Controlledable::~Controlledable ()
{
	// we do not use it
	for (int i = 0; i < (int)mControllers.size(); i++)
	{
		if (mControllers[i])
		{
			mControllers[i]->SetControlledable(0);
			mControllers[i] = 0;
		}
	}
}
//----------------------------------------------------------------------------
void Controlledable::ResetPlay ()
{
	Reset();
	Play();
}
//----------------------------------------------------------------------------
void Controlledable::Play ()
{
	for (int i=0; i<GetNumControllers(); i++)
	{
		if (mControllers[i]->IsEnable() && !mControllers[i]->IsPlaySelfCtrl())
			mControllers[i]->Play();
	}
}
//----------------------------------------------------------------------------
bool Controlledable::IsPlaying () const
{
	for (int i=0; i<GetNumControllers(); i++)
	{
		if (mControllers[i]->IsEnable() && !mControllers[i]->IsPlaySelfCtrl() &&
			mControllers[i]->IsPlaying())
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Controlledable::Pause()
{
	for (int i=0; i<GetNumControllers(); i++)
	{
		if (mControllers[i]->IsEnable() && !mControllers[i]->IsPlaySelfCtrl())
			mControllers[i]->Pause();
	}
}
//----------------------------------------------------------------------------
void Controlledable::Reset ()
{
	for (int i=0; i<GetNumControllers(); i++)
	{
		if (!mControllers[i]->IsPlaySelfCtrl())
			mControllers[i]->Reset();
	}
}
//----------------------------------------------------------------------------
void Controlledable::SetSelfCtrled (bool selfCtrled)
{
	mIsSelfCtrled = selfCtrled;
}
//----------------------------------------------------------------------------
Controller *Controlledable::GetControllerByName(const std::string &name) const
{
	for (int i=0; i<GetNumControllers(); i++)
	{
		if (mControllers[i]->GetName() == name)
			return mControllers[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
bool Controlledable::HasController (Controller* controller)
{
	for (int i=0; i<GetNumControllers(); i++)
	{
		if (mControllers[i] == controller)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Controlledable::AttachController (Controller* controller)
{
	// 控制器必须存在
	if (!controller)
	{
		assertion(false, "Cannot attach a null controller\n");
		return;
	}

	if (HasController(controller))
		return;

	mControllers.push_back(controller);
	controller->SetControlledable(this);

	AddObjectData data;
	data.ParentObj = this;
	data.Obj = controller;

	Event *ent = GraphicsES::CreateEventX(GraphicsES::AddObject,
		"GraphicsES::AddObject");
	ent->SetDataPointer0(controller);
	ent->SetDataPointer1(this);
	ent->SetData<AddObjectData>(data);
	PX2_EW.BroadcastingLocalEvent(ent);

	SortControls();
}
//----------------------------------------------------------------------------
void Controlledable::DetachController (Controller* controller)
{
	std::vector<ControllerPtr>::iterator it = mControllers.begin();

	for (; it!=mControllers.end(); it++)
	{
		if (controller == *it)
		{
			controller->SetControlledable(0);
			mControllers.erase(it);

			Event *ent = GraphicsES::CreateEventX(GraphicsES::RemoveObject,
				"GraphicsES::RemoveObject");
			ent->SetData<Object*>(controller);
			ent->SetDataPointer0(controller);
			PX2_EW.BroadcastingLocalEvent(ent);

			return;
		}
	}
}
//----------------------------------------------------------------------------
void Controlledable::DetachAllControllers ()
{
	for (int i=0; i<GetNumControllers(); ++i)
	{
		mControllers[i]->SetControlledable(0);

		Event *ent = GraphicsES::CreateEventX(GraphicsES::RemoveObject,
			"GraphicsES::RemoveObject");
		ent->SetData<Object*>((Object*)mControllers[i]);
		ent->SetDataPointer0((Object*)mControllers[i]);
		PX2_EW.BroadcastingLocalEvent(ent);
	}

	mControllers.clear();
}
//----------------------------------------------------------------------------
void Controlledable::SortControls ()
{
	std::sort(mControllers.begin(), mControllers.end(), Controller::LessThan);
}
//----------------------------------------------------------------------------
bool Controlledable::UpdateControllers(double applicationTime, 
	double elapsedTime)
{
	if (mIsAutoPlay)
	{
		if (!IsPlaying())
		{
			ResetPlay();
		}
	}

	bool someoneUpdated = false;

	for (int i = 0; i < GetNumControllers(); ++i)
	{
		if (mControllers[i]->IsEnable() && 
			mControllers[i]->Update(applicationTime, elapsedTime))
		{
			someoneUpdated = true;
		}
	}

	return someoneUpdated;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Controlledable::RegistProperties ()
{
	Object::RegistProperties();

	AddPropertyClass("Controlledable");

	AddProperty("IsPlaying", PT_BOOL, IsPlaying(), false);

	AddProperty("IsSelfCtrled", PT_BOOL, mIsSelfCtrled);
	AddProperty("IsAutoPlay", PT_BOOL, mIsAutoPlay);
}
//----------------------------------------------------------------------------
void Controlledable::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("IsSelfCtrled" == obj.Name)
	{
		SetSelfCtrled(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsAutoPlay" == obj.Name)
	{
		SetAutoPlay(PX2_ANY_AS(obj.Data, bool));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* Controlledable::GetObjectByName(const std::string& name)
{
	Object* found = Object::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	for (int i = 0; i < GetNumControllers(); ++i)
	{
		PX2_GET_OBJECT_BY_NAME(mControllers[i], name, found);
	}

	return 0;
}
//----------------------------------------------------------------------------
void Controlledable::GetAllObjectsByName (const std::string& name,
											std::vector<Object*>& objects)
{
	Object::GetAllObjectsByName(name, objects);

	for (int i = 0; i < GetNumControllers(); ++i)
	{
		PX2_GET_ALL_OBJECTS_BY_NAME(mControllers[i], name, objects);
	}
}
//----------------------------------------------------------------------------
Object* Controlledable::GetObjectByID(int id)
{
	Object* found = Object::GetObjectByID(id);
	if (found)
	{
		return found;
	}

	for (int i = 0; i < GetNumControllers(); ++i)
	{
		PX2_GET_OBJECT_BY_ID(mControllers[i], id, found);
	}

	return 0;
}
//----------------------------------------------------------------------------
void Controlledable::GetAllObjectsByID(int id,
	std::vector<Object*>& objects)
{
	Object::GetAllObjectsByID(id, objects);

	for (int i = 0; i < GetNumControllers(); ++i)
	{
		PX2_GET_ALL_OBJECTS_BY_ID(mControllers[i], id, objects);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Controlledable::Controlledable (LoadConstructor value)
:
Object(value),
mIsSelfCtrled(false),
mControllers(0),
mIsAutoPlay(false)
{
	mControlUpdateTimeLast = -1.0;
	mControlUpdateTimeMin = 0.02;
}
//----------------------------------------------------------------------------
void Controlledable::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsSelfCtrled);
	source.ReadBool(mIsAutoPlay);

	int numCtrls;
	source.Read(numCtrls);
	mControllers.resize(numCtrls);
	for (int i=0; i<numCtrls; i++)
	{
		source.ReadPointer(mControllers[i]);
	}

	PX2_END_DEBUG_STREAM_LOAD(Controlledable, source);
}
//----------------------------------------------------------------------------
void Controlledable::Link (InStream& source)
{
	Object::Link(source);

	int numCtrls = (int)mControllers.size();
	for (int i=0; i<numCtrls; i++)
	{
		source.ResolveLink(mControllers[i]);
	}
}
//----------------------------------------------------------------------------
void Controlledable::PostLink ()
{
	Object::PostLink();

	for (int i = 0; i < (int)mControllers.size(); i++)
	{
		if (mControllers[i])
		{
			mControllers[i]->SetControlledable(this);
		}
	}
}
//----------------------------------------------------------------------------
bool Controlledable::Register (OutStream& target) const
{
	if (Object::Register(target))
	{
		int numCtrls = (int)mControllers.size();
		for (int i=0; i<numCtrls; i++)
		{
			target.Register(mControllers[i]);
		}
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void Controlledable::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsSelfCtrled);
	target.WriteBool(mIsAutoPlay);

	int numCtrls = (int)mControllers.size();
	target.Write(numCtrls);
	for (int i=0; i<numCtrls; i++)
	{
		target.WritePointer(mControllers[i]);
	}

	PX2_END_DEBUG_STREAM_SAVE(Controlledable, target);
}
//----------------------------------------------------------------------------
int Controlledable::GetStreamingSize (Stream &stream) const
{
	int numCtrls = (int)mControllers.size();

	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsSelfCtrled);
	size += PX2_BOOLSIZE(mIsAutoPlay);

	size += sizeof(numCtrls);
	size += numCtrls*PX2_POINTERSIZE(mControllers[0]);
	return size;
}
//----------------------------------------------------------------------------