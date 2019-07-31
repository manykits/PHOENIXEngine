// PX2UIFrameGridAlignControl.cpp

#include "PX2UIFrameGridAlignControl.hpp"
#include "PX2UIFrame.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, UIFrameGridAlignControl);
PX2_IMPLEMENT_STREAM(UIFrameGridAlignControl);
PX2_IMPLEMENT_FACTORY(UIFrameGridAlignControl);

//----------------------------------------------------------------------------
UIFrameGridAlignControl::UIFrameGridAlignControl() :
mIsLayoutChanged(true)
{
	mCellSize = Sizef(100.0f, 50.0f);
	mSpacing = Float2(10.0f, 10.0f);
	mStartAxisType = SAT_HORIZONTAL;
	mChildAlignmentType = CAT_UPPER_CENTER;
	mConstraintType = CT_FLEXIBLE;
	mConstraintValue = 2;
	mIsConstraintExtend = false;
	mIsAutoAdjustContentSize = false;
}
//----------------------------------------------------------------------------
UIFrameGridAlignControl::~UIFrameGridAlignControl()
{
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetCellSize(const Sizef &cellSize)
{
	mCellSize = cellSize;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetSpacing(const Float2 &spacing)
{
	mSpacing = spacing;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetBorder(float left, float right,
	float bottom, float top)
{
	mBorder[0] = left;
	mBorder[1] = right;
	mBorder[2] = bottom;
	mBorder[3] = top;

	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::GetBorder(float &left, float &right, 
	float &bottom, float &top)
{
	left = mBorder[0];
	right = mBorder[1];
	bottom = mBorder[2];
	top = mBorder[3];
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetStartAxis(
	UIFrameGridAlignControl::StartAxisType sa)
{
	mStartAxisType = sa;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetChildAlignment(
	UIFrameGridAlignControl::ChildAlignmentType ca)
{
	mChildAlignmentType = ca;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetConstraintType(
	UIFrameGridAlignControl::ConstraintType type)
{
	mConstraintType = type;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetConstraintValue(int value)
{
	mConstraintValue = value;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::SetConstraintExtend(bool extend)
{
	mIsConstraintExtend = extend;
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::MarkLayoutChanged()
{
	mIsLayoutChanged = true;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::_Update(double applicationTime,
	double elapsedTime)
{
	Controller::_Update(applicationTime, elapsedTime);
	
	UIFrame *parent = DynamicCast<UIFrame>(GetControlledable());
	if (!parent) return;

	if (mIsLayoutChanged)
	{
		_UpdateAlignItems();

		mIsLayoutChanged = false;
	}
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::_UpdateAlignItems()
{
	UIFrame *parent = DynamicCast<UIFrame>(GetControlledable());
	if (!parent) return;

	std::vector<UIFrame*> frames;
	for (int i=0; i<parent->GetNumChildren(); i++)
	{
		UIFrame *uiFrame = DynamicCast<UIFrame>(parent->GetChild(i));
		if (uiFrame)
		{
			frames.push_back(uiFrame);
			uiFrame->SetPivot(0.5f, 0.5f);
		}
	}

	_UpdateItems(parent, frames, mStartAxisType, mChildAlignmentType);
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::_UpdateItems(UIFrame *parent, 
										   const std::vector<UIFrame*> &frames,
										   StartAxisType startAxisType,
										   ChildAlignmentType childAlignmentTypeType)
{
	int numFrames = (int)frames.size();
	if (0 == numFrames)
	{
		mContentSize.Width = 0.0f;
		mContentSize.Height = 0.0f;
		return;
	}

	const Sizef &size = parent->GetSize();
	const AVector &leftBottomCornerOffset = parent->LeftBottomCornerOffset;

	// cell size
	Sizef cellSize = mCellSize;
	if (CT_FLEXIBLE == mConstraintType)
	{
	}
	if (CT_FIXED_COLUMN_COUNT == mConstraintType && mIsConstraintExtend)
	{
		if (mConstraintValue >= 1)
		{
			cellSize.Width = (size.Width - mBorder[0] - mBorder[1] -
				mSpacing[0] * (mConstraintValue - 1)) / mConstraintValue;
		}
	}
	else if (CT_FIXED_ROW_COUNT == mConstraintType && mIsConstraintExtend)
	{
		if (mConstraintValue >= 1)
		{
			cellSize.Height = (size.Height - mBorder[2] - mBorder[3] -
				mSpacing[1] * (mConstraintValue - 1)) / mConstraintValue;
		}
	}
	int numWidth = 0;
	int numHeight = 0;
	float allCotWidth = 0.0f;
	float allCotHeight = 0.0f;
	if (SAT_HORIZONTAL == startAxisType)
	{
		float contentWidth = size.Width - mBorder[0] - mBorder[1];

		if (CT_FLEXIBLE == mConstraintType)
		{
			numWidth = (int)((contentWidth + mSpacing[0]) / (cellSize.Width + mSpacing[0]));
			numHeight = (int)Mathf::Ceil((float)numFrames / (float)numWidth);
		}
		else if (CT_FIXED_COLUMN_COUNT == mConstraintType)
		{
			numWidth = mConstraintValue;
			numHeight = (int)Mathf::Ceil((float)numFrames / (float)numWidth);
		}
		else if (CT_FIXED_ROW_COUNT == mConstraintType)
		{
			numHeight = mConstraintValue;
			numWidth = (int)Mathf::Ceil((float)numFrames / (float)numHeight);
		}

		if (0 == numWidth)
			numWidth = 1;

		if (0 == numHeight)
			numHeight = 1;
	}
	else if (SAT_VERTICAL == startAxisType)
	{
		numHeight = (int)((size.Height + mSpacing[1]) / cellSize.Height);

		if (0 == numHeight)
			numHeight = 1;

		numWidth = 0;
	}

	if (numWidth > 0)
	{
		allCotWidth = (cellSize.Width + mSpacing[0]) * numWidth - mSpacing[0];
		allCotHeight = (cellSize.Height + mSpacing[1]) * numHeight - mSpacing[1];
	}

	if (numHeight > 0)
	{
		allCotHeight = (cellSize.Height + mSpacing[1]) * numHeight - mSpacing[1];
	}

	for (int i = 0; i < (int)frames.size(); i++)
	{
		Sizef itemSize;

		UIFrame *frame = frames[i];

		if (CT_FLEXIBLE != mConstraintType && mIsConstraintExtend)
		{
			itemSize = cellSize;
		}
		else
		{
			itemSize = frame->GetSize();
		}
		frame->SetSize(itemSize);
		frame->SetPivot(0.5f, 0.5f);

		Float2 halfItemSize = Float2(itemSize.Width / 2.0f, itemSize.Height / 2.0f);
		Float2 halfCellSize = Float2(cellSize.Width / 2.0f, cellSize.Height / 2.0f);

		int indexX = 0;
		int indexZ = 0;
		if (SAT_HORIZONTAL == startAxisType)
		{
			indexX = (i + numWidth) % numWidth;
			indexZ = i / numWidth;
		}
		else
		{
		}

		Float2 anchorHor;
		Float2 anchorParamHor;
		Float2 anchorVer;
		Float2 anchorParamVer;

		if (CAT_UPPER_LEFT == mChildAlignmentType)
		{
			anchorHor = Float2(0.0f, 0.0f);
			anchorVer = Float2(1.0f, 1.0f);

			anchorParamHor[0] = halfCellSize[0] + (mSpacing[0] + cellSize.Width)*indexX + mBorder[0];
			anchorParamVer[0] = -(halfCellSize[1] + (mSpacing[1] + cellSize.Height)*indexZ) - mBorder[3];
		}
		else if (CAT_UPPER_CENTER == mChildAlignmentType)
		{
			anchorHor = Float2(0.5f, 0.5f);
			anchorVer = Float2(1.0f, 1.0f);

			anchorParamHor[0] = -allCotWidth / 2.0f + halfCellSize[0] + (mSpacing[0] + cellSize.Width)*indexX;
			anchorParamVer[0] = -(halfCellSize[1] + (mSpacing[1] + cellSize.Height)*indexZ) - mBorder[3];
		}
		else if (CAT_UPPER_RIGHT == mChildAlignmentType)
		{
			anchorHor = Float2(1.0f, 1.0f);
			anchorVer = Float2(1.0f, 1.0f);

			anchorParamHor[0] = -(halfCellSize[0] + (mSpacing[0] + cellSize.Width)*indexX) + mBorder[0];
			anchorParamVer[0] = -(halfCellSize[1] + (mSpacing[1] + cellSize.Height)*indexZ) - mBorder[3];
		}
		else if (CAT_MIDDLE_CENTER == mChildAlignmentType)
		{
			anchorHor = Float2(0.5f, 0.5f);
			anchorVer = Float2(0.5f, 0.5f);

			anchorParamHor[0] = -allCotWidth / 2.0f + halfCellSize[0] + (mSpacing[0] + cellSize.Width)*indexX;
			anchorParamVer[0] = 0.0f;
		}

		frame->SetAnchorHor(anchorHor);
		frame->SetAnchorParamHor(anchorParamHor);
		frame->SetAnchorVer(anchorVer);
		frame->SetAnchorParamVer(anchorParamVer);
	}

	if (SAT_HORIZONTAL == startAxisType)
	{
		mContentSize.Width = 0.0f;
		mContentSize.Height = mBorder[2] + mBorder[3] + allCotHeight;
	}
	else if (SAT_VERTICAL == startAxisType)
	{
		mContentSize.Width = 0.0f;
		mContentSize.Height = 0.0f;
	}

	SizeNode *sn = DynamicCast<SizeNode>(GetControlledable());
	if (sn && mIsAutoAdjustContentSize)
	{
		sn->SetSize(mContentSize);
		sn->UpdateLayout(sn->GetParent());
		sn->UpdateLeftBottomCornerOffset(sn->GetParent());
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIFrameGridAlignControl::UIFrameGridAlignControl(LoadConstructor value) :
Controller(value),
mIsLayoutChanged(true)
{
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(Controller, source);
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::Link(InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool UIFrameGridAlignControl::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIFrameGridAlignControl::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(Controller, target);
}
//----------------------------------------------------------------------------
int UIFrameGridAlignControl::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------