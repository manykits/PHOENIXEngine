// PX2RedoUndo.cpp

#include "PX2RedoUndo.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2Movable.hpp"
#include "PX2EditEventType.hpp"
#include "PX2Time.hpp"
#include "PX2Creater.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
URDo::URDo()
{
}
//----------------------------------------------------------------------------
URDo::~URDo()
{
}
//----------------------------------------------------------------------------
void URDo::OnUnDo()
{
}
//----------------------------------------------------------------------------
void URDo::OnReDo()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// MovablesTransURDo
//----------------------------------------------------------------------------
MovablesTransURDo::MovablesTransURDo(bool isAnchorLayout) :
mIsAnchorLayout(isAnchorLayout)
{
	int num = PX2_SELECTM_E->GetNumObjects();

	mObjects.resize(num);
	mPositions.resize(num);
	mRotations.resize(num);
	mRotationMats.resize(num);
	mScales.resize(num);
	mAnchorHors.resize(num);
	mAnchorParamHors.resize(num);
	mAnchorVers.resize(num);
	mAnchorParamVers.resize(num);

	mPositionsReDo.resize(num);
	mRotationsReDo.resize(num);
	mRotationMatsReDo.resize(num);
	mScalesReDo.resize(num);
	mAnchorHorsReDo.resize(num);
	mAnchorParamHorsReDo.resize(num);
	mAnchorVersReDo.resize(num);
	mAnchorParamVersReDo.resize(num);

	for (int i = 0; i < num; i++)
	{
		Object *obj = PX2_SELECTM_E->GetObjectAt(i);
		Movable *mov = DynamicCast<Movable>(obj);
		SizeNode *sz = DynamicCast<SizeNode>(obj);

		if (!mIsAnchorLayout)
		{
			if (mov)
			{
				mObjects[i] = mov;
				mPositions[i] = mov->LocalTransform.GetTranslate();
				mRotationMats[i] = mov->LocalTransform.GetRotate();
				mScales[i] = mov->LocalTransform.GetScale();
			}
		}
		else
		{
			if (sz)
			{
				mObjects[i] = mov;
				mAnchorHors[i] = sz->GetAnchorHor();
				mAnchorParamHors[i] = sz->GetAnchorParamHor();
				mAnchorVers[i] = sz->GetAnchorVer();
				mAnchorParamVers[i] = sz->GetAnchorParamVer();
			}
		}
	}
}
//----------------------------------------------------------------------------
MovablesTransURDo::~MovablesTransURDo()
{
}
//----------------------------------------------------------------------------
void MovablesTransURDo::OnUnDo()
{
	for (int i = 0; i < (int)mObjects.size(); i++)
	{
		Movable *mov = DynamicCast<Movable>(mObjects[i]);
		SizeNode *sz = DynamicCast<SizeNode>(mObjects[i]);

		if (!mIsAnchorLayout)
		{
			if (mov)
			{
				mPositionsReDo[i] = mov->LocalTransform.GetTranslate();
				mRotationMatsReDo[i] = mov->LocalTransform.GetRotate();
				mScalesReDo[i] = mov->LocalTransform.GetScale();

				mov->LocalTransform.SetTranslate(mPositions[i]);
				mov->LocalTransform.SetRotate(mRotationMats[i]);
				mov->LocalTransform.SetScale(mScales[i]);

				mov->Update(Time::GetTimeInSeconds(), 0.0f, false);
			}
		}
		else
		{
			if (sz)
			{
				mAnchorHorsReDo[i] = sz->GetAnchorHor();
				mAnchorParamHorsReDo[i] = sz->GetAnchorParamHor();
				mAnchorVersReDo[i] = sz->GetAnchorVer();
				mAnchorParamVersReDo[i] = sz->GetAnchorParamVer();

				sz->SetAnchorHor(mAnchorHors[i]);
				sz->SetAnchorParamHor(mAnchorParamHors[i]);
				sz->SetAnchorVer(mAnchorVers[i]);
				sz->SetAnchorParamVer(mAnchorParamVers[i]);

				sz->Update(Time::GetTimeInSeconds(), 0.0f, false);
			}
		}
	}
}
//----------------------------------------------------------------------------
void MovablesTransURDo::OnReDo()
{
	for (int i = 0; i < (int)mObjects.size(); i++)
	{
		Movable *mov = DynamicCast<Movable>(mObjects[i]);
		SizeNode *sz = DynamicCast<SizeNode>(mObjects[i]);
		if (!mIsAnchorLayout)
		{
			if (mov)
			{
				mov->LocalTransform.SetTranslate(mPositionsReDo[i]);
				mov->LocalTransform.SetRotate(mRotationMatsReDo[i]);
				mov->LocalTransform.SetScale(mScalesReDo[i]);

				mov->Update(Time::GetTimeInSeconds(), 0.0f, false);
			}
		}
		else
		{
			if (sz)
			{
				sz->SetAnchorHor(mAnchorHorsReDo[i]);
				sz->SetAnchorParamHor(mAnchorParamHorsReDo[i]);
				sz->SetAnchorVer(mAnchorVersReDo[i]);
				sz->SetAnchorParamVer(mAnchorParamVersReDo[i]);

				sz->Update(Time::GetTimeInSeconds(), 0.0f, false);
			}
		}
	}
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// MovablesTransURDo
//----------------------------------------------------------------------------
MovableTransURDo::MovableTransURDo(Movable *mov, bool isAnchorLayout) :
mIsAnchorLayout(isAnchorLayout)
{
	SizeNode *sz = DynamicCast<SizeNode>(mov);

	mObject = mov;

	if (!mIsAnchorLayout)
	{
		if (mov)
		{
			mPosition = mov->LocalTransform.GetTranslate();
			mRotationMat = mov->LocalTransform.GetRotate();
			mScale = mov->LocalTransform.GetScale();
		}
	}
	else
	{
		if (sz)
		{
			mAnchorHor = sz->GetAnchorHor();
			mAnchorParamHor = sz->GetAnchorParamHor();
			mAnchorVer = sz->GetAnchorVer();
			mAnchorParamVer = sz->GetAnchorParamVer();
		}
	}
}
//----------------------------------------------------------------------------
MovableTransURDo::~MovableTransURDo()
{
}
//----------------------------------------------------------------------------
void MovableTransURDo::OnUnDo()
{
	Movable *mov = DynamicCast<Movable>(mObject);
	SizeNode *sz = DynamicCast<SizeNode>(mObject);

	if (!mIsAnchorLayout)
	{
		if (mov)
		{
			mPositionReDo = mov->LocalTransform.GetTranslate();
			mRotationMatReDo = mov->LocalTransform.GetRotate();
			mScaleReDo = mov->LocalTransform.GetScale();

			mov->LocalTransform.SetTranslate(mPosition);
			mov->LocalTransform.SetRotate(mRotationMat);
			mov->LocalTransform.SetScale(mScale);

			mov->Update(Time::GetTimeInSeconds(), 0.0f, false);
		}
	}
	else
	{
		if (sz)
		{
			mAnchorHorReDo = sz->GetAnchorHor();
			mAnchorParamHorReDo = sz->GetAnchorParamHor();
			mAnchorVerReDo = sz->GetAnchorVer();
			mAnchorParamVerReDo = sz->GetAnchorParamVer();

			sz->SetAnchorHor(mAnchorHor);
			sz->SetAnchorParamHor(mAnchorParamHor);
			sz->SetAnchorVer(mAnchorVer);
			sz->SetAnchorParamVer(mAnchorParamVer);

			sz->Update(Time::GetTimeInSeconds(), 0.0f, false);
		}
	}
}
//----------------------------------------------------------------------------
void MovableTransURDo::OnReDo()
{
	Movable *mov = DynamicCast<Movable>(mObject);
	SizeNode *sz = DynamicCast<SizeNode>(mObject);
	if (!mIsAnchorLayout)
	{
		if (mov)
		{
			mov->LocalTransform.SetTranslate(mPositionReDo);
			mov->LocalTransform.SetRotate(mRotationMatReDo);
			mov->LocalTransform.SetScale(mScaleReDo);

			mov->Update(Time::GetTimeInSeconds(), 0.0f, false);
		}
	}
	else
	{
		if (sz)
		{
			sz->SetAnchorHor(mAnchorHorReDo);
			sz->SetAnchorParamHor(mAnchorParamHorReDo);
			sz->SetAnchorParamVer(mAnchorParamVerReDo);

			sz->Update(Time::GetTimeInSeconds(), 0.0f, false);
		}
	}
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// ObjectAddDeleteURDo
//----------------------------------------------------------------------------
ObjectAddDeleteURDo::ObjectAddDeleteURDo(bool isAdd,
	PX2::Object *object, Object *repalceParent)
{
	mIsAdd = isAdd;
	mObject = object;
	mReplaceParent = repalceParent;

	Movable *mov = DynamicCast<Movable>(mObject);
	if (mov)
	{
		mParent = mov->GetParent();
	}

	Controller *ctrl = DynamicCast<Controller>(mObject);
	if (ctrl)
	{
		mParent = ctrl->GetControlledable();
	}

	Controller *comp = DynamicCast<Controller>(mObject);
	if (comp)
	{
		mParent = comp->GetControlledable();
	}

	SizeNode *sz = DynamicCast<SizeNode>(object);
	if (sz)
	{
		mAnchorHor = sz->GetAnchorHor();
		mAnchorParamHor = sz->GetAnchorParamHor();
		mAnchorVer = sz->GetAnchorVer();
		mAnchorParamVer = sz->GetAnchorParamVer();
	}
}
//----------------------------------------------------------------------------
ObjectAddDeleteURDo::~ObjectAddDeleteURDo()
{
}
//----------------------------------------------------------------------------
void ObjectAddDeleteURDo::OnUnDo()
{
	if (mIsAdd)
	{
		PX2_CREATER.RemoveObject(mObject, false);

		if (mReplaceParent)
		{
			PX2_CREATER.AddObject(mReplaceParent, mObject, false);

			SizeNode *sz = DynamicCast<SizeNode>(mObject);
			if (sz)
			{
				sz->SetAnchorHor(mAnchorHor_InReplaceParent);
				sz->SetAnchorParamHor(mAnchorParamHor_InReplaceParent);
				sz->SetAnchorVer(mAnchorVer_InReplaceParent);
				sz->SetAnchorParamVer(mAnchorParamVer_InReplaceParent);
			}
		}
	}
	else
	{
		PX2_CREATER.AddObject(mParent, mObject, false);

		SizeNode *sz = DynamicCast<SizeNode>(mObject);
		if (sz)
		{
			sz->SetAnchorHor(mAnchorHor);
			sz->SetAnchorParamHor(mAnchorParamHor);
			sz->SetAnchorVer(mAnchorVer);
			sz->SetAnchorParamVer(mAnchorParamVer);
		}
	}
}
//----------------------------------------------------------------------------
void ObjectAddDeleteURDo::OnReDo()
{
	if (mIsAdd)
	{
		if (mReplaceParent)
		{
			PX2_CREATER.RemoveObject(mObject, false);
		}

		PX2_CREATER.AddObject(mParent, mObject, false);

		SizeNode *sz = DynamicCast<SizeNode>(mObject);
		if (sz)
		{
			sz->SetAnchorHor(mAnchorHor);
			sz->SetAnchorParamHor(mAnchorParamHor);
			sz->SetAnchorVer(mAnchorVer);
			sz->SetAnchorParamVer(mAnchorParamVer);
		}
	}
	else
	{
		PX2_CREATER.RemoveObject(mObject, false);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// URDoManager
//----------------------------------------------------------------------------
URDoManager::URDoManager()
{

}
//----------------------------------------------------------------------------
URDoManager::~URDoManager()
{

}
//----------------------------------------------------------------------------
void URDoManager::Clear()
{
	mUnDoCommands.clear();
	mReDoCommands.clear();

	PX2::Event *ent = EditES::CreateEventX(EditES::NoUnDo);
	PX2_EW.BroadcastingLocalEvent(ent);

	PX2::Event *ent1 = EditES::CreateEventX(EditES::NoReDo);
	PX2_EW.BroadcastingLocalEvent(ent1);
}
//----------------------------------------------------------------------------
bool URDoManager::IsHasUnDo() const
{
	return (int)mUnDoCommands.size() > 0;
}
//----------------------------------------------------------------------------
bool URDoManager::IsHasReDo() const
{
	return (int)mReDoCommands.size() > 0;
}
//----------------------------------------------------------------------------
void URDoManager::PushUnDo(URDo *command)
{
	mUnDoCommands.push_back(command);

	if ((int)mUnDoCommands.size() > 21)
		mUnDoCommands.pop_front();

	PX2::Event *ent = EditES::CreateEventX(EditES::PushUnDo);
	EventWorld::GetSingleton().BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void URDoManager::UnDo()
{
	if ((int)mUnDoCommands.size() > 0)
	{
		URDoPtr command = mUnDoCommands.back();
		if (command)
			command->OnUnDo();

		mReDoCommands.push_back(command);
		mUnDoCommands.pop_back();

		PX2::Event *ent = EditES::CreateEventX(EditES::UnDo);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);

		if ((int)mUnDoCommands.size() == 0)
		{
			PX2::Event *ent1 = EditES::CreateEventX(EditES::NoUnDo);
			EventWorld::GetSingleton().BroadcastingLocalEvent(ent1);
		}
	}
}
//----------------------------------------------------------------------------
void URDoManager::ReDo()
{
	if ((int)mReDoCommands.size() > 0)
	{
		URDoPtr command = mReDoCommands.back();
		if (command)
			command->OnReDo();

		mUnDoCommands.push_back(command);
		mReDoCommands.pop_back();

		PX2::Event *ent = EditES::CreateEventX(EditES::ReDo);
		PX2_EW.BroadcastingLocalEvent(ent);

		if ((int)mReDoCommands.size() == 0)
		{
			PX2::Event *ent1 = EditES::CreateEventX(EditES::NoReDo);
			PX2_EW.BroadcastingLocalEvent(ent1);
		}
	}
}
//----------------------------------------------------------------------------