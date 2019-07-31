// PX2UISplitterFrame.cpp

#include "PX2UISplitterFrame.hpp"
#include "PX2InputManager.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIAuiManager.hpp"
#include "PX2RenderWindow.hpp"
#include "PX2Time.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UISplitterFrame);
PX2_IMPLEMENT_STREAM(UISplitterFrame);
PX2_IMPLEMENT_FACTORY(UISplitterFrame);

//----------------------------------------------------------------------------
UISplitterFrame::UISplitterFrame(bool hor) :
mPosType(PT_NONE),
mIsHor(hor),
mDragType(DT_VALUE),
mIsDragable(true),
mIsDraging(false),
mIsOverMe(false)
{
	float size = PX2_UISM.Size_Splitter;

	SetSize(size, size);

	SetAnchorHor(Float2(0.0f, 0.0f));
	SetAnchorVer(Float2(0.0f, 0.0f));

	if (mIsHor)
	{
		SetAnchorParamHor(Float2(20.0f, -20.0f));
	}
	else
	{
		SetAnchorParamVer(Float2(20.0f, -20.0f));
	}

	ComeInEventWorld();

	UIPicBox *picBoxBottom = CreateAddBackgroundPicBox();
	picBoxBottom->SetColor(Float3::WHITE);
	SetColor(PX2_UISM.Color_Splitter);

	SetWidget(true);

	SetColorSelfCtrled(true);

	SetUpdatePriority(5);
}
//----------------------------------------------------------------------------
UISplitterFrame::~UISplitterFrame()
{
}
//----------------------------------------------------------------------------
void UISplitterFrame::SetHor(bool isHor)
{
	mIsHor = isHor;

	if (mIsHor)
	{
		SetAnchorParamHor(Float2(20.0f, -20.0f));
	}
	else
	{
		SetAnchorParamVer(Float2(20.0f, -20.0f));
	}
}
//----------------------------------------------------------------------------
void UISplitterFrame::SetDragType(DragType dt)
{
	mDragType = dt;
}
//----------------------------------------------------------------------------
void UISplitterFrame::SetDragRange(const Float2 &range)
{
	mDragRange = range;
}
//----------------------------------------------------------------------------
void UISplitterFrame::SetPosType(PosType pt)
{
	mPosType = pt;
}
//----------------------------------------------------------------------------
void UISplitterFrame::OnWidgetPicked(const CanvasInputData &inputData)
{
	UIFrame::OnWidgetPicked(inputData);

	if (!IsEnable()) return;

	if (UIPT_PRESSED == inputData.PickType)
	{
		if (mIsDragable)
			SetDraging(true);
	}
	else if (UIPT_RELEASED == inputData.PickType)
	{
		if (mIsDragable)
			SetDraging(false);
	}
	else if (UIPT_MOVED == inputData.PickType)
	{
		if (!IsDraging())
		{
			if (!mIsOverMe && mIsDragable)
				_SetOverMe(true);
		}
		else
		{
			_UpdateDraging(inputData);
		}
	}
}
//----------------------------------------------------------------------------
void UISplitterFrame::_UpdateDraging(const CanvasInputData &inputData)
{
	if (mIsDragable && mIsDraging)
	{
		float sizeSP = PX2_UISM.Size_Splitter;

		UIFrame *parent = DynamicCast<UIFrame>(GetParent());
		Sizef parSize = parent->GetSize();
		float param = 0.0f;

		Transform inverTrans = parent->WorldTransform.InverseTransform();
		APoint localPos = inverTrans * inputData.LogicPos;

		if (mIsHor)
		{
			if (DT_VALUE == mDragType)
			{
				float framePosOrigin = parent->LeftBottomCornerOffset.Z() + parSize.Height * GetAnchorVer()[0];
				param = localPos.Z() - framePosOrigin;
			}
			else if (DT_PERCENT == mDragType)
			{
				param = (localPos.Z() - parent->LeftBottomCornerOffset.Z()) / parSize.Height;
			}

			bool doDraging = IsCanDraging(param);
			if (doDraging)
			{
				if (DT_VALUE == mDragType)
				{
					SetAnchorParamVer(param, 0.0f);
				}
				else
				{
					SetAnchorVer(param, param);
				}

				UpdateWorldData(Time::GetTimeInSeconds(), 0.0);

				_MarkBlockFramesChange();

				OnDraging();
			}
		}
		else
		{
			if (DT_VALUE == mDragType)
			{
				float framePosOrigin = parent->LeftBottomCornerOffset.X() + parSize.Width * GetAnchorHor()[0];
				param = localPos.X() - framePosOrigin;
			}
			else if (DT_PERCENT == mDragType)
			{
				param = (localPos.X() - parent->LeftBottomCornerOffset.X()) / parSize.Width;
			}

			bool doDraging = IsCanDraging(param);

			if (doDraging)
			{
				if (DT_VALUE == mDragType)
				{
					SetAnchorParamHor(Float2(param, 0.0f));
				}
				else
				{
					SetAnchorHor(param, param);
				}

				UpdateWorldData(Time::GetTimeInSeconds(), 0.0);

				_MarkBlockFramesChange();

				OnDraging();
			}
		}
	}
}
//----------------------------------------------------------------------------
void UISplitterFrame::OnSizeNodeNotPicked(const CanvasInputData &inputData)
{
	if (UIPT_RELEASED == inputData.PickType)
	{
		if (mIsDragable)
			SetDraging(false);
	}
	else if (UIPT_MOVED == inputData.PickType)
	{
		if (!IsDraging())
		{
			if (mIsOverMe && mIsDragable)
				_SetOverMe(false);
		}
		else
		{
			_UpdateDraging(inputData);
		}
	}

	UIFrame::OnSizeNodeNotPicked(inputData);
}
//----------------------------------------------------------------------------
void UISplitterFrame::_SetOverMe(bool over)
{
	mIsOverMe = over;

	if (mIsOverMe)
	{
		SetColor(PX2_UISM.Color_Splitter_Over);

		if (IsHor())
		{
			RenderWindow::SetCursorType(RenderWindow::CT_CURSOR_SIZENS);
		}
		else
		{
			RenderWindow::SetCursorType(RenderWindow::CT_CURSOR_SIZEWE);
		}
	}
	else
	{
		RenderWindow::SetCursorType(RenderWindow::CT_CURSOR_ARROW);

		SetColor(PX2_UISM.Color_Splitter);
	}
}
//----------------------------------------------------------------------------
void UISplitterFrame::SetDragable(bool isDragable)
{
	mIsDragable = isDragable;
}
//----------------------------------------------------------------------------
void UISplitterFrame::SetDraging(bool isDraged)
{
	mIsDraging = isDraged;

	if (mIsDraging)
	{
		SetColor(PX2_UISM.Color_Splitter_Draging);
	}
	else
	{
		SetColor(PX2_UISM.Color_Splitter);
	}
}
//----------------------------------------------------------------------------
void UISplitterFrame::OnDraging()
{
	_UICallbacksCalls(UICT_SPLITTER_DRAGING);
}
//----------------------------------------------------------------------------
bool UISplitterFrame::IsCanDraging(float toParam)
{
	bool isCanDrag = true;

	float widthMin = 20.0f;
	float heightMin = 55.0f;
	float delta = 0.0f;

	float toWorldPosX = 0.0f;
	float toWorldPosZ = 0.0f;

	if (Float2::ZERO != mDragRange)
	{
		if (toParam <= mDragRange[0] || toParam >= mDragRange[1])
			isCanDrag = false;
	}

	for (int i = 0; i < (int)mAuiBlockFrames0.size(); i++)
	{
		if (mIsHor)
		{
			if (DT_VALUE == mDragType)
			{
				delta = toParam - GetAnchorParamVer()[0];

				if (delta < 0.0f)
				{
					float bottomWorldPos = mAuiBlockFrames0[i]->GetSideFrameHor0()->WorldTransform.GetTranslate().Z();
					float toPos = WorldTransform.GetTranslate().Z() + delta;

					if (toPos < (bottomWorldPos + heightMin))
						isCanDrag = false;
				}
			}
			else
			{
				delta = toParam - GetAnchorVer()[0];

				if (delta < 0.0f)
				{
					if (toParam < 0.1f)
						isCanDrag = false;
				}		
			}
		}
		else
		{
			if (DT_VALUE == mDragType)
			{
				delta = toParam - GetAnchorParamHor()[0];

				if (delta < 0.0f)
				{
					float leftWorldPos = mAuiBlockFrames0[i]->GetSideFrameVer0()->WorldTransform.GetTranslate().X();
					float toPos = WorldTransform.GetTranslate().X() + delta;

					if (toPos < (leftWorldPos + widthMin))
						isCanDrag = false;
				}
			}
			else
			{
				delta = toParam - GetAnchorHor()[0];

				if (delta < 0.0f)
				{
					if (toParam < 0.1f)
						isCanDrag = false;
				}
			}
		}
	}

	for (int i = 0; i < (int)mAuiBlockFrames1.size(); i++)
	{
		if (mIsHor)
		{
			if (DT_VALUE == mDragType)
			{
				delta = toParam - GetAnchorParamVer()[0];

				if (delta > 0.0f)
				{
					float topWorldPos = mAuiBlockFrames1[i]->GetSideFrameHor1()->WorldTransform.GetTranslate().Z();
					float toPos = WorldTransform.GetTranslate().Z() + delta;

					if (toPos > (topWorldPos - heightMin))
						isCanDrag = false;
				}
			}
			else
			{
				delta = toParam - GetAnchorVer()[0];

				if (delta > 0.0f)
				{
					if (toParam > 0.9f)
						isCanDrag = false;
				}
			}
		}
		else
		{
			if (DT_VALUE == mDragType)
			{
				delta = toParam - GetAnchorParamHor()[0];

				if (delta > 0.0f)
				{
					float rightWorldPos = mAuiBlockFrames1[i]->GetSideFrameVer1()->WorldTransform.GetTranslate().X();
					float toPos = WorldTransform.GetTranslate().X() + delta;

					if (toPos > (rightWorldPos - widthMin))
						isCanDrag = false;
				}
			}
			else
			{
				delta = toParam - GetAnchorHor()[0];

				if (delta > 0.0f)
				{
					if (toParam > 0.9f)
						isCanDrag = false;
				}
			}
		}
	}

	return isCanDrag;
}
//----------------------------------------------------------------------------
void UISplitterFrame::_AddAuiBlockFrame0(UIAuiBlockFrame *blockframe)
{
	if (!blockframe)
		return;

	for (int i = 0; i < (int)mAuiBlockFrames0.size(); i++)
	{
		if (blockframe == mAuiBlockFrames0[i])
			return;
	}

	mAuiBlockFrames0.push_back(blockframe);
}
//----------------------------------------------------------------------------
void UISplitterFrame::_RemoveAuiBlockFrame0(UIAuiBlockFrame *blockframe)
{
	if (!blockframe)
		return;

	std::vector<UIAuiBlockFrame*>::iterator it = mAuiBlockFrames0.begin();
	for (; it != mAuiBlockFrames0.end(); it++)
	{
		if (blockframe == *it)
		{
			mAuiBlockFrames0.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
void UISplitterFrame::_AddAuiBlockFrame1(UIAuiBlockFrame *blockframe)
{
	if (!blockframe)
		return;

	for (int i = 0; i < (int)mAuiBlockFrames1.size(); i++)
	{
		if (blockframe == mAuiBlockFrames1[i])
			return;
	}

	mAuiBlockFrames1.push_back(blockframe);
}
//----------------------------------------------------------------------------
void UISplitterFrame::_RemoveAuiBlockFrame1(UIAuiBlockFrame *blockframe)
{
	if (!blockframe)
		return;

	std::vector<UIAuiBlockFrame*>::iterator it = mAuiBlockFrames1.begin();
	for (; it != mAuiBlockFrames1.end(); it++)
	{
		if (blockframe == *it)
		{
			mAuiBlockFrames1.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
void UISplitterFrame::_MarkBlockFramesChange()
{
	for (int i = 0; i < (int)mAuiBlockFrames0.size(); i++)
	{
		mAuiBlockFrames0[i]->_MarkRelatvieLayoutChange();
	}

	for (int i = 0; i < (int)mAuiBlockFrames1.size(); i++)
	{
		mAuiBlockFrames1[i]->_MarkRelatvieLayoutChange();
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UISplitterFrame::UISplitterFrame(LoadConstructor value) :
UIFrame(value),
mIsHor(true),
mDragType(DT_VALUE),
mIsDragable(true),
mIsDraging(false),
mIsOverMe(false)
{
}
//----------------------------------------------------------------------------
void UISplitterFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UISplitterFrame, source);
}
//----------------------------------------------------------------------------
void UISplitterFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UISplitterFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UISplitterFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UISplitterFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UISplitterFrame, target);
}
//----------------------------------------------------------------------------
int UISplitterFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------