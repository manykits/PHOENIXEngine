// PX2UIPicBoxListController.cpp

#include "PX2UIPicBoxListController.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, UIPicBoxListController);
PX2_IMPLEMENT_STREAM(UIPicBoxListController);
PX2_IMPLEMENT_FACTORY(UIPicBoxListController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, UIPicBoxListController);

//----------------------------------------------------------------------------
UIPicBoxListController::UIPicBoxListController() :
mInterval(1.0f),
mLastShowIndex(-1)
{
}
//----------------------------------------------------------------------------
UIPicBoxListController::~UIPicBoxListController()
{
}
//----------------------------------------------------------------------------
void UIPicBoxListController::SetInterval(float seconds)
{
	mInterval = seconds;
}
//----------------------------------------------------------------------------
float UIPicBoxListController::GetInterval() const
{
	return mInterval;
}
//----------------------------------------------------------------------------
UIFPicBox *UIPicBoxListController::AddPicBox(const std::string &filename)
{
	UIFPicBox *picBox = new0 UIFPicBox();
	picBox->GetUIPicBox()->SetTexture(filename);
	mPicBoxList.push_back(picBox);
	picBox->SetAnchorHor(0.0f, 1.0);
	picBox->SetAnchorVer(0.0f, 1.0);
	picBox->Show(false);

	Node *node = DynamicCast<Node>(mObject);
	if (node)
	{
		node->AttachChild(picBox);
	}

	mLastShowIndex = -1;

	return picBox;
}
//----------------------------------------------------------------------------
void UIPicBoxListController::SetControlledable(Controlledable* object)
{
	Node *node = DynamicCast<Node>(mObject);
	if (node)
	{
		for (int i = 0; i < (int)mPicBoxList.size(); i++)
		{
			node->DetachChild(mPicBoxList[i]);
		}
	}

	Controller::SetControlledable(object);

	Node *node1 = DynamicCast<Node>(mObject);
	if (node1)
	{
		for (int i = 0; i < (int)mPicBoxList.size(); i++)
		{
			node1->AttachChild(mPicBoxList[i]);
		}
	}
}
//----------------------------------------------------------------------------
bool UIPicBoxListController::Update(double applicationTime,
	double elapsedTime)
{
	if (!Controller::Update(applicationTime, elapsedTime))
		return false;

	float playedTimeMinusDelay = GetPlayedTimeMinusDelay();
	if (playedTimeMinusDelay < 0.0f)
		return false;

	float interval = GetInterval();
	if (interval <= 0.0f)
		interval = 1.0f;

	int numFrames = (int)mPicBoxList.size();
	if (numFrames <= 0)
		return false;

	int index = (int)(playedTimeMinusDelay / mInterval);
	index = index%numFrames;

	if (mLastShowIndex >= 0)
		mPicBoxList[mLastShowIndex]->Show(false);

	mPicBoxList[index]->Show(true);
	mLastShowIndex = index;

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
UIPicBoxListController::UIPicBoxListController(LoadConstructor value) :
Controller(value),
mInterval(1.0f),
mLastShowIndex(-1)
{
}
//----------------------------------------------------------------------------
void UIPicBoxListController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIPicBoxListController, source);
}
//----------------------------------------------------------------------------
void UIPicBoxListController::Link(InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void UIPicBoxListController::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool UIPicBoxListController::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIPicBoxListController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIPicBoxListController, target);
}
//----------------------------------------------------------------------------
int UIPicBoxListController::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream)
		+ PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
