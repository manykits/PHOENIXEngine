// PX2UISpBlockFrame.cpp

#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2UIAuiFrame.hpp"
#include "PX2UIAuiManager.hpp"
#include "PX2Math.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIAuiBlockFrame);
PX2_IMPLEMENT_STREAM(UIAuiBlockFrame);
PX2_IMPLEMENT_FACTORY(UIAuiBlockFrame);

//----------------------------------------------------------------------------
UIAuiBlockFrame::UIAuiBlockFrame(UILayoutPosType posType) :
mIsCaptured(false),
mBrotherFrame(0),
mPosType(posType),
mIsAutoExpand(true),
mSideFrameHor0(0),
mSideFrameHor1(0),
mSideFrameVer0(0),
mSideFrameVer1(0),
mSpliterFrame(0),
mParentAuiBlockFrame(0)
{
	SetActivateSelfCtrled(false);
}
//----------------------------------------------------------------------------
UIAuiBlockFrame::~UIAuiBlockFrame()
{
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::Caputure(bool capture)
{
	mIsCaptured = capture;

	if (capture)
	{
		PX2_UIAUIM.SetCaptureBlockFrame(this);
	}
	else
	{
		UIAuiBlockFrame *blockFrame = PX2_UIAUIM.GetCaptureBlockFrame();
		if (blockFrame == this)
		{
			PX2_UIAUIM.SetCaptureBlockFrame(0);
		}
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetBrotherFrame(UIAuiBlockFrame *frame)
{
	mBrotherFrame = frame;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetAutoExpand(bool expand)
{
	mIsAutoExpand = expand;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::OnChildAttached(Movable *child)
{
	UIFrame::OnChildAttached(child);

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(child);
	if (tabFrame)
	{
		mUITabFrame = tabFrame;
		mUITabFrame->SetAuiBlockFrame(this);
	}

	UISplitterFrame *splitFrame = DynamicCast<UISplitterFrame>(child);
	if (splitFrame)
	{
		splitFrame->SetUpdatePriority(10);
	}

	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::OnChildDetach(Movable *child)
{
	UIFrame::OnChildDetach(child);

	UITabFrame *tabFrame = DynamicCast<UITabFrame>(child);
	if (tabFrame)
	{
		tabFrame->SetAuiBlockFrame(0);
		mUITabFrame = 0;
	}

	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	float splitterSize = PX2_UISM.Size_Splitter;
	const Sizef &size = GetSize();

	if (mSpliterFrame)
	{
		UISplitterFrame::PosType pt = mSpliterFrame->GetPosType();

		if (UISplitterFrame::PT_LEFT == pt)
		{
			float spVal = Mathf::FAbs(mSpliterFrame->GetAnchorParamHor()[0]);
			float adjWidth = size.Width - splitterSize / 2.0f;

			if (adjWidth < spVal)
			{
				mSpliterFrame->SetAnchorParamHor(size.Width - splitterSize / 2.0f, 0.0f);
			}
		}
		else if (UISplitterFrame::PT_RIGHT == pt)
		{
			float spVal = Mathf::FAbs(mSpliterFrame->GetAnchorParamHor()[0]);
			float adjWidth = size.Width - splitterSize / 2.0f;

			if (adjWidth < spVal)
			{
				mSpliterFrame->SetAnchorParamHor(-size.Width + splitterSize / 2.0f, 0.0f);
			}
		}
		else if (UISplitterFrame::PT_BOTTOM == pt)
		{
			float spVal = Mathf::FAbs(mSpliterFrame->GetAnchorParamVer()[0]);
			float adjWidth = size.Height - splitterSize / 2.0f;

			if (adjWidth < spVal)
			{
				mSpliterFrame->SetAnchorParamVer(size.Height - splitterSize / 2.0f, 0.0f);
			}
		}
		else if (UISplitterFrame::PT_TOP == pt)
		{
			float spVal = Mathf::FAbs(mSpliterFrame->GetAnchorParamVer()[0]);
			float adjWidth = size.Height - splitterSize / 2.0f;

			if (adjWidth < spVal)
			{
				mSpliterFrame->SetAnchorParamVer(-size.Height + splitterSize / 2.0f, 0.0f);
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetMinSize(const Sizef &minSize)
{
	mMinSize = minSize;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetSideFrameHor0(UISplitterFrame *frame)
{
	if (mSideFrameHor0)
	{
		mSideFrameHor0->_RemoveAuiBlockFrame1(this);
	}

	mSideFrameHor0 = frame;

	if (frame)
	{
		frame->_AddAuiBlockFrame1(this);
	}

	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it =
		mSideFrames.find(UILPT_BOTTOM);
	if (it != mSideFrames.end())
	{
		it->second->SetSideFrameHor0(frame);
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetSideFrameHor1(UISplitterFrame *frame)
{
	if (mSideFrameHor1)
	{
		mSideFrameHor1->_RemoveAuiBlockFrame0(this);
	}

	mSideFrameHor1 = frame;

	if (frame)
	{
		frame->_AddAuiBlockFrame0(this);
	}

	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it =
		mSideFrames.find(UILPT_TOP);
	if (it != mSideFrames.end())
	{
		it->second->SetSideFrameHor1(frame);
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetSideFrameVer0(UISplitterFrame *frame)
{
	if (mSideFrameVer0)
	{
		mSideFrameVer0->_RemoveAuiBlockFrame1(this);
	}

	mSideFrameVer0 = frame;

	if (frame)
	{
		frame->_AddAuiBlockFrame1(this);
	}

	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it =
		mSideFrames.find(UILPT_LEFT);
	if (it != mSideFrames.end())
	{
		it->second->SetSideFrameVer0(frame);
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetSideFrameVer1(UISplitterFrame *frame)
{
	if (mSideFrameVer1)
	{
		mSideFrameVer1->_RemoveAuiBlockFrame0(this);
	}

	mSideFrameVer1 = frame;

	if (frame)
	{
		frame->_AddAuiBlockFrame0(this);
	}

	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it =
		mSideFrames.find(UILPT_RIGHT);
	if (it != mSideFrames.end())
	{
		it->second->SetSideFrameVer1(frame);
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetSpliterFrame(UISplitterFrame *frame)
{
	mSpliterFrame = frame;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::_UpdateLayout(UIAuiBlockFrame *auiBlockFrame,
	bool setToAuiBlockFrame,
	Sizef *outSize, APoint *outPos)
{
	UIFrame *parentFrame = DynamicCast<UIFrame>(auiBlockFrame->GetParent());
	if (!parentFrame) return;
	const AVector &plbOffset = parentFrame->LeftBottomCornerOffset;
	Transform parentWorldTransform = parentFrame->WorldTransform;
	const HMatrix& invMat = parentWorldTransform.Inverse();

	float spSize = PX2_UISM.Size_Splitter;

	UILayoutPosType posType = auiBlockFrame->GetLayOutPosType();
	UISplitterFrame *leftFrame = auiBlockFrame->GetSideFrameVer0();
	UISplitterFrame *rightFrame = auiBlockFrame->GetSideFrameVer1();
	UISplitterFrame *bottomFrame = auiBlockFrame->GetSideFrameHor0();
	UISplitterFrame *topFrame = auiBlockFrame->GetSideFrameHor1();

	const APoint &left = leftFrame->WorldTransform.GetTranslate();
	const APoint &right = rightFrame->WorldTransform.GetTranslate();
	const APoint &bottom = bottomFrame->WorldTransform.GetTranslate();
	const APoint &top = topFrame->WorldTransform.GetTranslate();

	float leftPos = left.X();
	float rightPos = rightFrame->WorldTransform.GetTranslate().X();
	float botPos = bottomFrame->WorldTransform.GetTranslate().Z();
	float topPos = topFrame->WorldTransform.GetTranslate().Z();

	float sizeWidth = (rightPos - spSize * auiBlockFrame->GetSideFrameVer1()->GetPivot()[0]) -
		(leftPos + spSize * (1.0f - auiBlockFrame->GetSideFrameVer0()->GetPivot()[0]));
	float sizeHeight = (topPos - spSize *  auiBlockFrame->GetSideFrameHor1()->GetPivot()[1]) -
		(botPos + spSize * (1.0f - auiBlockFrame->GetSideFrameHor0()->GetPivot()[1]));

	UISplitterFrame *spHor0 = auiBlockFrame->GetSideFrameHor0();
	UISplitterFrame *spHor1 = auiBlockFrame->GetSideFrameHor1();
	UISplitterFrame *spVer0 = auiBlockFrame->GetSideFrameVer0();
	UISplitterFrame *spVer1 = auiBlockFrame->GetSideFrameVer1();

	APoint localLeft = invMat * left;
	APoint localBot = invMat * bottom;

	float posX = localLeft.X() + spSize * (1.0f - leftFrame->GetPivot()[0]) + sizeWidth / 2.0f;
	float posZ = localBot.Z() + spSize * (1.0f - bottomFrame->GetPivot()[1]) + sizeHeight / 2.0f;

	if (setToAuiBlockFrame)
	{
		auiBlockFrame->SetSize(sizeWidth, sizeHeight);
		auiBlockFrame->LocalTransform.SetTranslate(posX, -10.0f, posZ);
		auiBlockFrame->LeftBottomCornerOffset.X() = -sizeWidth / 2.0f;
		auiBlockFrame->LeftBottomCornerOffset.Z() = -sizeHeight / 2.0f;
	}

	if (outSize)
	{
		*outSize = Sizef(sizeWidth, sizeHeight);
	}

	if (outPos)
	{
		*outPos = APoint(posX, -10.0f, posZ);
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::UpdateLayout(Movable *parent)
{
	_UpdateLayout(this);

	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it = 
		mSideFrames.begin();
	for (; it != mSideFrames.end(); it++)
	{
		UILayoutPosType posType = it->first;
		UIAuiBlockFrame *auiBlockFrame = it->second;
		if (auiBlockFrame)
			_UpdateLayout(auiBlockFrame);
	}

	mIsLayoutChanged = false;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *UIAuiBlockFrame::_GetSideFrame(UILayoutPosType pos)
{
	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it =
		mSideFrames.find(pos);

	if (it != mSideFrames.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetNewSideFrameHor0()
{
	UIAuiBlockFrame *bot = _GetSideFrame(UILPT_BOTTOM);
	if (bot)
	{
		return bot->GetSideFrameHor1();
	}

	return GetSideFrameHor0();
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetNewSideFrameHor1()
{
	UIAuiBlockFrame *top = _GetSideFrame(UILPT_TOP);
	if (top)
	{
		return top->GetSideFrameHor0();
	}

	return GetSideFrameHor1();
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetNewSideFrameVer0()
{
	UIAuiBlockFrame *left = _GetSideFrame(UILPT_LEFT);
	if (left)
	{
		return left->GetSideFrameVer1();
	}

	return GetSideFrameVer0();
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetNewSideFrameVer1()
{
	UIAuiBlockFrame *right = _GetSideFrame(UILPT_RIGHT);
	if (right)
	{
		return right->GetSideFrameVer0();
	}

	return GetSideFrameVer1();
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetLinkFrame0(UILayoutPosType pos)
{
	if (UILPT_LEFT == pos)
	{
		UIAuiBlockFrame *bottomFrame = _GetSideFrame(UILPT_BOTTOM);
		if (bottomFrame)
		{
			return bottomFrame->GetSideFrameHor1();
		}

		return GetSideFrameHor0();
	}
	else if (UILPT_RIGHT == pos)
	{
		UIAuiBlockFrame *botFrame = _GetSideFrame(UILPT_BOTTOM);
		if (botFrame)
		{
			return botFrame->GetSideFrameHor1();
		}

		return GetSideFrameHor0();
	}
	else if (UILPT_BOTTOM == pos)
	{
		UIAuiBlockFrame *leftFrame = _GetSideFrame(UILPT_LEFT);
		if (leftFrame)
		{
			return leftFrame->GetSideFrameVer1();
		}

		return GetSideFrameVer0();
	}
	else if (UILPT_TOP == pos)
	{
		UIAuiBlockFrame *leftFrame = _GetSideFrame(UILPT_LEFT);
		if (leftFrame)
		{
			return leftFrame->GetSideFrameVer1();
		}

		return GetSideFrameVer0();
	}

	return 0;
}
//----------------------------------------------------------------------------
UISplitterFrame *UIAuiBlockFrame::_CalGetLinkFrame1(UILayoutPosType pos)
{
	if (UILPT_LEFT == pos)
	{
		UIAuiBlockFrame *topFrame = _GetSideFrame(UILPT_TOP);
		if (topFrame)
		{
			return topFrame->GetSideFrameHor0();
		}

		return GetSideFrameHor1();
	}
	else if (UILPT_RIGHT == pos)
	{
		UIAuiBlockFrame *topFrame = _GetSideFrame(UILPT_TOP);
		if (topFrame)
		{
			return topFrame->GetSideFrameHor0();
		}

		return GetSideFrameHor1();
	}
	else if (UILPT_BOTTOM == pos)
	{
		UIAuiBlockFrame *rightFrame = _GetSideFrame(UILPT_RIGHT);
		if (rightFrame)
		{
			return rightFrame->GetSideFrameVer0();
		}

		return GetSideFrameVer1();
	}
	else if (UILPT_TOP == pos)
	{
		UIAuiBlockFrame *rightFrame = _GetSideFrame(UILPT_RIGHT);
		if (rightFrame)
		{
			return rightFrame->GetSideFrameVer0();
		}

		return GetSideFrameVer1();
	}

	return 0;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *UIAuiBlockFrame::CreateAddPosFrame(UILayoutPosType pos,
	const Sizef &size)
{
	std::map<UILayoutPosType, UIAuiBlockFramePtr>::iterator it =
		mSideFrames.find(pos);
	if (it != mSideFrames.end())
	{
		assertion(false, "frame already exist!");
		return 0;
	}
	
	UIAuiBlockFrame *auiBlockFrame = new0 UIAuiBlockFrame(pos);
	auiBlockFrame->SetSize(size);

	AttachChild(auiBlockFrame);
	auiBlockFrame->SetMinSize(Sizef(2.0f, 55.0f));
	auiBlockFrame->SetName("AuiBlockFrame");

	float splitterSize = PX2_UISM.Size_Splitter;

	float splitterY = -2.0f;
	if (UILPT_LEFT == pos)
	{
		UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(false);
		AttachChild(newSplitterFrame);
		newSplitterFrame->SetPosType(UISplitterFrame::PT_LEFT);
		newSplitterFrame->SetName("SpFrame_Left");
		newSplitterFrame->LocalTransform.SetTranslateY(splitterY);

		newSplitterFrame->SetAnchorHor(0.0f, 0.0f);
		newSplitterFrame->SetAnchorParamHor(size.Width + splitterSize / 2.0f, 0.0f);
		newSplitterFrame->SetAnchorVer(0.0f, 1.0f);
		newSplitterFrame->SetAnchorParamVer(0.0f, 0.0f);

		auiBlockFrame->SetSideFrameVer0(_CalGetNewSideFrameVer0());
		auiBlockFrame->SetSideFrameVer1(newSplitterFrame);
		auiBlockFrame->SetSideFrameHor0(_CalGetNewSideFrameHor0());
		auiBlockFrame->SetSideFrameHor1(_CalGetNewSideFrameHor1());

		SetSpliterFrame(newSplitterFrame);
	}
	else if (UILPT_RIGHT == pos)
	{
		UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(false);
		AttachChild(newSplitterFrame);
		newSplitterFrame->SetPosType(UISplitterFrame::PT_RIGHT);
		newSplitterFrame->SetName("SpFrame_Right");
		newSplitterFrame->LocalTransform.SetTranslateY(splitterY);

		newSplitterFrame->SetAnchorHor(1.0f, 1.0f);
		newSplitterFrame->SetAnchorParamHor(-size.Width - splitterSize / 2.0f, 0.0f);
		newSplitterFrame->SetAnchorVer(0.0f, 1.0f);
		newSplitterFrame->SetAnchorParamVer(0.0f, 0.0f);

		auiBlockFrame->SetSideFrameVer0(newSplitterFrame);
		auiBlockFrame->SetSideFrameVer1(_CalGetNewSideFrameVer1());
		auiBlockFrame->SetSideFrameHor0(_CalGetNewSideFrameHor0());
		auiBlockFrame->SetSideFrameHor1(_CalGetNewSideFrameHor1());

		SetSpliterFrame(newSplitterFrame);
	}
	else if (UILPT_BOTTOM == pos)
	{
		UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(true);
		AttachChild(newSplitterFrame);
		newSplitterFrame->SetPosType(UISplitterFrame::PT_BOTTOM);
		newSplitterFrame->SetName("SpFrame_Bottom");
		newSplitterFrame->LocalTransform.SetTranslateY(splitterY);

		newSplitterFrame->SetAnchorHor(0.0f, 1.0f);
		newSplitterFrame->SetAnchorParamHor(0.0f, 0.0f);
		newSplitterFrame->SetAnchorVer(0.0f, 0.0f);
		newSplitterFrame->SetAnchorParamVer(size.Height + splitterSize / 2.0f, 0.0f);

		auiBlockFrame->SetSideFrameVer0(_CalGetNewSideFrameVer0());
		auiBlockFrame->SetSideFrameVer1(_CalGetNewSideFrameVer1());
		auiBlockFrame->SetSideFrameHor0(_CalGetNewSideFrameHor0());
		auiBlockFrame->SetSideFrameHor1(newSplitterFrame);

		SetSpliterFrame(newSplitterFrame);
	}
	else if (UILPT_TOP == pos)
	{
		UISplitterFrame *newSplitterFrame = new0 UISplitterFrame(true);
		AttachChild(newSplitterFrame);
		newSplitterFrame->SetPosType(UISplitterFrame::PT_TOP);
		newSplitterFrame->SetName("SpFrame_Top");
		newSplitterFrame->LocalTransform.SetTranslateY(splitterY);

		newSplitterFrame->SetAnchorHor(0.0f, 1.0f);
		newSplitterFrame->SetAnchorParamHor(0.0f, 0.0f);
		newSplitterFrame->SetAnchorVer(1.0f, 1.0f);
		newSplitterFrame->SetAnchorParamVer(-size.Height - splitterSize / 2.0f, 0.0f);

		auiBlockFrame->SetSideFrameVer0(_CalGetNewSideFrameVer0());
		auiBlockFrame->SetSideFrameVer1(_CalGetNewSideFrameVer1());
		auiBlockFrame->SetSideFrameHor0(newSplitterFrame);
		auiBlockFrame->SetSideFrameHor1(_CalGetNewSideFrameHor1());

		SetSpliterFrame(newSplitterFrame);
	}
	else if (UILPT_CENTER == pos)
	{
		auiBlockFrame->SetSideFrameVer0(_CalGetNewSideFrameVer0());
		auiBlockFrame->SetSideFrameVer1(_CalGetNewSideFrameVer1());
		auiBlockFrame->SetSideFrameHor0(_CalGetNewSideFrameHor0());
		auiBlockFrame->SetSideFrameHor1(_CalGetNewSideFrameHor1());
	}

	mSideFrames[pos] = auiBlockFrame;
	auiBlockFrame->SetParentAuiBlockFrame(this);

	return auiBlockFrame;
}
//----------------------------------------------------------------------------
struct _SPReplaceData
{
	_SPReplaceData()
	{
		SPFrom = 0;
		SPTo = 0;
	}

	UISplitterFrame *SPFrom;
	UISplitterFrame *SPTo;
};
void _NodeTravelExecuteSetSideFrame(Movable *mov, Any *data, bool &goOn)
{
	PX2_UNUSED(goOn);

	_SPReplaceData dt = PX2_ANY_AS(*data, _SPReplaceData);
	UISplitterFrame *from = dt.SPFrom;
	UISplitterFrame *to = dt.SPTo;

	UIAuiBlockFrame *blockFrame = DynamicCast<UIAuiBlockFrame>(mov);
	UISplitterFrame *spFrame = DynamicCast<UISplitterFrame>(mov);
	if (blockFrame)
	{
		if (from == blockFrame->GetSideFrameHor0())
		{
			blockFrame->SetSideFrameHor0(to);
		}
		if (from == blockFrame->GetSideFrameHor1())
		{
			blockFrame->SetSideFrameHor1(to);
		}
		if (from == blockFrame->GetSideFrameVer0())
		{
			blockFrame->SetSideFrameVer0(to);
		}
		if (from == blockFrame->GetSideFrameVer1())
		{
			blockFrame->SetSideFrameVer1(to);
		}
	}
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::RemovePosFrame(UILayoutPosType pos)
{
	std::map<UILayoutPosType, UIAuiBlockFramePtr>::iterator it =
		mSideFrames.find(pos);
	if (it == mSideFrames.end())
		return;

	UIAuiBlockFramePtr blockFrame = it->second;
	UIAuiBlockFramePtr brotherFrame = blockFrame->GetBrotherFrame();
	UILayoutPosType brotherPos = brotherFrame->GetLayOutPosType();
	UISplitterFramePtr spFrame = GetSpliterFrame();
	bool isHor = spFrame->IsHor();

	mSideFrames.clear();
	DestoryBackgroundPicBox();
	DetachAllChildren();
	std::vector<MovablePtr> children = brotherFrame->GetChildren();
	brotherFrame->DetachAllChildren();

	UISplitterFrame *spFrameTo = 0;
	if (isHor)
	{
		if (UILPT_BOTTOM == pos)
		{
			spFrameTo = GetSideFrameHor0();
		}
		else if (UILPT_TOP == pos)
		{
			spFrameTo = GetSideFrameHor1();
		}
		else if (UILPT_CENTER == pos)
		{
			if (UILPT_BOTTOM == brotherPos)
			{
				spFrameTo = GetSideFrameHor1();
			}
			else if (UILPT_TOP == brotherPos)
			{
				spFrameTo = GetSideFrameHor0();
			}
		}
	}
	else
	{
		if (UILPT_LEFT == pos)
		{
			spFrameTo = GetSideFrameVer0();
		}
		else if (UILPT_RIGHT == pos)
		{
			spFrameTo = GetSideFrameVer1();
		}
		else if (UILPT_CENTER == pos)
		{
			if (UILPT_LEFT == brotherPos)
			{
				spFrameTo = GetSideFrameVer1();
			}
			else if (UILPT_RIGHT == brotherPos)
			{
				spFrameTo = GetSideFrameVer0();
			}
		}
	}
	
	for (int i = 0; i < (int)children.size(); i++)
	{
		Movable *mov = children[i];

		UIAuiBlockFrame *childBlockFrame = DynamicCast<UIAuiBlockFrame>(mov);
		UISplitterFrame *childSPFrame = DynamicCast<UISplitterFrame>(mov);
		UITabFrame *tabFrame = DynamicCast<UITabFrame>(mov);
		if (childBlockFrame)
		{
			AttachChild(childBlockFrame);
	
			UILayoutPosType pos = childBlockFrame->GetLayOutPosType();
			mSideFrames[pos] = childBlockFrame;
			childBlockFrame->SetParentAuiBlockFrame(this);
		}
		if (childSPFrame)
		{
			SetSpliterFrame(childSPFrame);
			AttachChild(childSPFrame);
		}
		if (tabFrame)
		{
			AttachChild(tabFrame);
		}

		if (childBlockFrame || childSPFrame)
		{
			_SPReplaceData data;
			data.SPFrom = spFrame;
			data.SPTo = spFrameTo;
			Any anyData = data;
			Node::TravelExecute(mov, _NodeTravelExecuteSetSideFrame, &anyData);
		}
	}

	UIPicBox *backPicBox = CreateAddBackgroundPicBox();
	backPicBox->SetColor(PX2_UISM.Color_ViewBackground);
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *UIAuiBlockFrame::_GetSameSizeBlockFrame(UIAuiBlockFrame *frame)
{
	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it =
		mSideFrames.begin();
	for (; it != mSideFrames.end(); it++)
	{
		UIAuiBlockFrame *compFrame = it->second;
		if (compFrame != frame)
		{
			const Sizef &frameSize = frame->GetSize();
			const Sizef &compSize = compFrame->GetSize();

			float widthDiff = Mathf::FAbs(frameSize.Width - compSize.Width);
			float heightDiff = Mathf::FAbs(frameSize.Height - compSize.Height);
			if ((widthDiff < 5.0f || heightDiff < 5.0f) && compFrame->IsAutoExpand())
			{
				return compFrame;
			}
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
UIAuiBlockFrame *UIAuiBlockFrame::GetPosFrame(UILayoutPosType pos)
{
	std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> >::iterator it =
		mSideFrames.find(pos);

	if (it != mSideFrames.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
int UIAuiBlockFrame::GetNumChildPosFrames()
{
	return (int)mSideFrames.size();
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::SetParentAuiBlockFrame(UIAuiBlockFrame *frame)
{
	mParentAuiBlockFrame = frame;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIAuiBlockFrame::UIAuiBlockFrame(LoadConstructor value) :
UIFrame(value),
mIsCaptured(false),
mBrotherFrame(0),
mIsAutoExpand(true),
mSideFrameHor0(0),
mSideFrameHor1(0),
mSideFrameVer0(0),
mSideFrameVer1(0),
mSpliterFrame(0),
mParentAuiBlockFrame(0)
{
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIAuiBlockFrame, source);
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIAuiBlockFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIAuiBlockFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIAuiBlockFrame, target);
}
//----------------------------------------------------------------------------
int UIAuiBlockFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------