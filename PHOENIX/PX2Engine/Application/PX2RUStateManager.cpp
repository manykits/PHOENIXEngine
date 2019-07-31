// PX2RUStateManager.cpp

#include "PX2RUStateManager.hpp"
#include "PX2EditEventType.hpp"
#include "PX2SizeNode.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
URState::URState()
{
}
//----------------------------------------------------------------------------
URState::~URState()
{
}
//----------------------------------------------------------------------------
void URState::Leave()
{
}
//----------------------------------------------------------------------------
void URState::Enter()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
TransStateObj::TransStateObj()
{

}
//----------------------------------------------------------------------------
TransStateObj::~TransStateObj()
{

}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
TransState::TransState(Object *fromParent, const TransStateObj &fromTrans,
	Object *obj, bool isAnchorLayout):
mIsAnchorLayout(isAnchorLayout)
{
	mParentFrom = fromParent;
	mTransStateFrom = fromTrans;

	mObject = obj;

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

	SizeNode *sz = DynamicCast<SizeNode>(obj);
	if (sz && mIsAnchorLayout)
	{
		if (mParent)
		{
			mTransState.mAnchorHor = sz->GetAnchorHor();
			mTransState.mAnchorParamHor = sz->GetAnchorParamHor();
			mTransState.mAnchorVer = sz->GetAnchorVer();
			mTransState.mAnchorParamVer = sz->GetAnchorParamVer();
		}
	}
}
//----------------------------------------------------------------------------
TransState::~TransState()
{
}
//----------------------------------------------------------------------------
void TransState::Leave()
{
	Object *curParent = 0;
	Movable *mov = DynamicCast<Movable>(mObject);
	if (mov)
	{
		curParent = mov->GetParent();
	}

	if (curParent && curParent != mParentFrom)
	{
		Node *curParentNode = DynamicCast<Node>(curParent);
		if (curParentNode)
		{
			curParentNode->DetachChild(mov);
		}
	}

	if (mParentFrom)
	{
		Node *parentFromNodeNode = DynamicCast<Node>(mParentFrom);
		if (parentFromNodeNode)
		{
			SizeNode *sz = DynamicCast<SizeNode>(mObject);
			if (sz && mIsAnchorLayout)
			{
				sz->SetAnchorHor(mTransStateFrom.mAnchorHor);
				sz->SetAnchorParamHor(mTransStateFrom.mAnchorParamHor);
				sz->SetAnchorVer(mTransStateFrom.mAnchorVer);
				sz->SetAnchorParamVer(mTransStateFrom.mAnchorParamVer);
			}

			if (curParent != mParentFrom)
				parentFromNodeNode->AttachChild(mov);
		}
	}
}
//----------------------------------------------------------------------------
void TransState::Enter()
{
	Object *curParent = 0;
	Movable *mov = DynamicCast<Movable>(mObject);
	if (mov)
	{
		curParent = mov->GetParent();
	}

	if (curParent != mParent)
	{
		Node *curParentNode = DynamicCast<Node>(curParent);
		if (curParentNode)
		{
			curParentNode->DetachChild(mov);
		}
		Node *parentNode = DynamicCast<Node>(mParent);
		if (parentNode)
		{
			parentNode->AttachChild(mov);
		}
	}

	SizeNode *sz = DynamicCast<SizeNode>(mObject);
	if (sz && mIsAnchorLayout)
	{
		sz->SetAnchorHor(mTransState.mAnchorHor);
		sz->SetAnchorParamHor(mTransState.mAnchorParamHor);
		sz->SetAnchorVer(mTransState.mAnchorVer);
		sz->SetAnchorParamVer(mTransState.mAnchorParamVer);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// URStateManager
//----------------------------------------------------------------------------
URStateManager::URStateManager()
{
}
//----------------------------------------------------------------------------
URStateManager::~URStateManager()
{
}
//----------------------------------------------------------------------------
void URStateManager::Clear()
{
	mUnDoStates.clear();
	mReDoStates.clear();

	PX2::Event *ent = EditES::CreateEventX(EditES::NoUnDo);
	PX2_EW.BroadcastingLocalEvent(ent);

	PX2::Event *ent1 = EditES::CreateEventX(EditES::NoReDo);
	PX2_EW.BroadcastingLocalEvent(ent1);
}
//----------------------------------------------------------------------------
void URStateManager::ClearReDo()
{
	mReDoStates.clear();

	PX2::Event *ent1 = EditES::CreateEventX(EditES::NoReDo);
	PX2_EW.BroadcastingLocalEvent(ent1);
}
//----------------------------------------------------------------------------
bool URStateManager::IsHasUnDo() const
{
	return (int)mUnDoStates.size() > 0;
}
//----------------------------------------------------------------------------
bool URStateManager::IsHasReDo() const
{
	return (int)mReDoStates.size() > 0;
}
//----------------------------------------------------------------------------
void URStateManager::PushUnDo(URState *command)
{
	mUnDoStates.push_back(command);

	if ((int)mUnDoStates.size() > 21)
		mUnDoStates.pop_front();

	PX2::Event *ent = EditES::CreateEventX(EditES::PushUnDo);
	EventWorld::GetSingleton().BroadcastingLocalEvent(ent);

	ClearReDo();
}
//----------------------------------------------------------------------------
void URStateManager::UnDo()
{
	if ((int)mUnDoStates.size() > 0)
	{
		URStatePtr command = mUnDoStates.back();
		command->Leave();
		mReDoStates.push_back(command);
		mUnDoStates.pop_back();

		PX2::Event *ent = EditES::CreateEventX(EditES::UnDo);
		EventWorld::GetSingleton().BroadcastingLocalEvent(ent);

		if ((int)mUnDoStates.size() == 0)
		{
			PX2::Event *ent1 = EditES::CreateEventX(EditES::NoUnDo);
			EventWorld::GetSingleton().BroadcastingLocalEvent(ent1);
		}
	}
}
//----------------------------------------------------------------------------
void URStateManager::ReDo()
{
	if ((int)mReDoStates.size() > 0)
	{
		URStatePtr state = mReDoStates.back();
		mUnDoStates.push_back(state);
		mReDoStates.pop_back();
		state->Enter();

		PX2::Event *ent = EditES::CreateEventX(EditES::ReDo);
		PX2_EW.BroadcastingLocalEvent(ent);

		if ((int)mReDoStates.size() == 0)
		{
			PX2::Event *ent1 = EditES::CreateEventX(EditES::NoReDo);
			PX2_EW.BroadcastingLocalEvent(ent1);
		}
	}
}
//----------------------------------------------------------------------------