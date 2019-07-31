// PX2UIRound.cpp

#include "PX2UIRound.hpp"
#include "PX2Log.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIRound);
PX2_IMPLEMENT_STREAM(UIRound);
PX2_IMPLEMENT_FACTORY(UIRound);

//----------------------------------------------------------------------------
UIRound::UIRound() :
mPickTouchID(-1),
mRoundCurDragingDirSimple(0),
mDragingLeftMoveType(0),
mDragingLeftMoveSpeed(0.0f),
mDragingRightMoveType(0),
mDragingRightMoveSpeed(0.0f)
{
	SetWidget(true);

	mIsDragable = true;

	mIsPressed = false;
	mDraggingDir = Vector2f::ZERO;
	mDraggingPercent = 0.0f;
	mDraggedDir = Vector2f::ZERO;
	mDraggedPercent = 0.0f;

	mBack = new0 UIFPicBox();
	AttachChild(mBack);
	mBack->GetUIPicBox()->SetPicBoxType(UIPicBox::PBT_NORMAL);
	mBack->GetUIPicBox()->SetTexture("Data/engine/round_out.png");
	mBack->SetAnchorHor(0.0f, 1.0f);
	mBack->SetAnchorVer(0.0f, 1.0f);

	mBut = new0 UIButton(UIButtonBase::BT_PICBOXSWAP);
	AttachChild(mBut);
	mBut->SetName("ButDir");
	mBut->LocalTransform.SetTranslateY(-1.0f);
	mBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NORMAL);
	mBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetPicBoxType(UIPicBox::PBT_NORMAL);
	mBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetPicBoxType(UIPicBox::PBT_NORMAL);
	mBut->GetPicBoxAtState(UIButtonBase::BS_DISABLED)->SetPicBoxType(UIPicBox::PBT_NORMAL);
	mBut->SetStateColorDefaultWhite();
	mBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/round_center.png");
	mBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetBrightness(1.0f);
	mBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetTexture("Data/engine/round_center.png");
	mBut->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetBrightness(1.1f);
	mBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture("Data/engine/round_center.png");
	mBut->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetBrightness(1.2f);
	mBut->GetPicBoxAtState(UIButtonBase::BS_DISABLED)->SetTexture("Data/engine/round_center.png");
	mBut->GetPicBoxAtState(UIButtonBase::BS_DISABLED)->SetBrightness(0.6f);

	mBut->SetMemUICallback(this, (UIFrame::MemUICallback)(&UIRound::_UIButCallback));
	mBut->SetAnchorHor(0.5f, 0.5f);
	mBut->SetAnchorVer(0.5f, 0.5f);

	SetChildPickOnlyInSizeRange(false);

	RegistToScriptSystem();
}
//----------------------------------------------------------------------------
UIRound::~UIRound()
{
}
//----------------------------------------------------------------------------
void UIRound::SetDragable(bool dragable)
{
	mIsDragable = dragable;
}
//----------------------------------------------------------------------------
AVector UIRound::GetDraggingDirAvector() const
{
	return AVector(mDraggingDir.X(), mDraggingDir.Y(), mDraggingDir.Y());
}
//----------------------------------------------------------------------------
float UIRound::GetDraggingPercentPiece(int num) const
{
	if (num <= 0)
		return 0.0;

	float dragingPercent = GetDraggingPercent();
	float piece = 1.0f / (float)num;
	float fNum = (dragingPercent + 0.5f*piece) / piece;
	float numStart = Mathf::Floor(fNum);
	return (numStart / num);
}
//----------------------------------------------------------------------------
AVector UIRound::GetDraggedDirAvector() const
{
	return AVector(mDraggedDir.X(), mDraggedDir.Y(), mDraggedDir.Y());
}
//----------------------------------------------------------------------------
int UIRound::GetDragingDirSimple() const
{
	const Vector2f &dragingDir = GetDraggingDir();
	if (dragingDir == Vector2f::ZERO)
	{
		return 0;
	}
	else if (dragingDir.Y() >= 0.0f)
	{
		float absX = Mathf::FAbs(dragingDir.X());
		float absY = Mathf::FAbs(dragingDir.Y());
		if (absX <= absY)
		{
			return 1;
		}
		else if (dragingDir.X() < 0.0f)
		{
			return 3;
		}
		else if (dragingDir.X() > 0.0f)
		{
			return 4;
		}
	}
	else if (dragingDir.Y() < 0.0f)
	{
		float absX = Mathf::FAbs(dragingDir.X());
		float absY = Mathf::FAbs(dragingDir.Y());
		if (absX <= absY)
		{
			return 2;
		}
		else if (dragingDir.X() < 0.0f)
		{
			return 3;
		}
		else if (dragingDir.X() > 0.0f)
		{
			return 4;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
int UIRound::GetDragingLeftMoveType() const
{
	return mDragingLeftMoveType;
}
//----------------------------------------------------------------------------
float UIRound::GetDragingLeftMoveSpeed() const
{
	return mDragingLeftMoveSpeed;
}
//----------------------------------------------------------------------------
int UIRound::GetDragingRightMoveType() const
{
	return mDragingRightMoveType;
}
//----------------------------------------------------------------------------
float UIRound::GetDragingRightMoveSpeed() const
{
	return mDragingRightMoveSpeed;
}
//----------------------------------------------------------------------------
void UIRound::OnPressed()
{
	mIsPressed = true;

	mDraggingDir = Vector2f::ZERO;
	mDraggingPercent = 0.0f;

	_UICallbacksCalls(UICT_ROUND_PRESSED);
}
//----------------------------------------------------------------------------
void UIRound::OnReleased()
{
	mIsPressed = false;

	mDraggingDir = Vector2f::ZERO;
	mDraggingPercent = 0.0f;

	mBut->SetAnchorHor(0.5f, 0.5f);
	mBut->SetAnchorVer(0.5f, 0.5f);

	_UICallbacksCalls(UICT_ROUND_RELEASED);
}
//----------------------------------------------------------------------------
void UIRound::OnReleasedNotPick()
{
	mIsPressed = false;

	mDraggingDir = Vector2f::ZERO;
	mDraggingPercent = 0.0f;

	mBut->SetAnchorHor(0.5f, 0.5f);
	mBut->SetAnchorVer(0.5f, 0.5f);

	PX2_LOG_INFO("UIRound::OnReleasedNotPick");

	_UICallbacksCalls(UICT_ROUND_RELEASEDNOTPICK);
}
//----------------------------------------------------------------------------
void UIRound::OnDrag(const Vector2f &dir, float percent,
	const APoint &pickWorldPos)
{
	PX2_UNUSED(pickWorldPos);

	mDraggingDir = dir;
	mDraggingPercent = percent;
	mDraggedDir = dir;
	mDraggedPercent = percent;

	_UICallbacksCalls(UICT_ROUND_DRAG);
}
//----------------------------------------------------------------------------
void UIRound::SetRangeAcceptFrame(UIFrame *frame)
{
	UIFrame::SetRangeAcceptFrame(frame);

	if (mBut)
	{
		mBut->SetRangeAcceptFrame(frame);
	}
}
//----------------------------------------------------------------------------
void UIRound::PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas)
{
	if (!IsShow())
		return;

	if (!IsDoPick())
		return;

	if (!mIsDragable)
	{
		UIFrame::PreCanvasPick(inputData, canvas);
	}
	else
	{
		if (!mIsPressed)
		{
			UIFrame::PreCanvasPick(inputData, canvas);
		}
		else
		{
			OnSizeNodePicked(inputData);

			for (int i = 0; i < GetNumChildren(); i++)
			{
				UIFrame *childFrame = DynamicCast<UIFrame>(GetChild(i));
				if (childFrame)
				{
					childFrame->PreCanvasPick(inputData, canvas);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIRound::OnSizeNodePicked(const CanvasInputData &inputData)
{
	UIFrame::OnSizeNodePicked(inputData);

	if (mIsDragable && mPickTouchID == inputData.TouchID)
	{
		const APoint &screenPos = inputData.LogicPos;

		const Sizef &sz = GetSize();
		Transform inversTrans = WorldTransform.InverseTransform();
		APoint localPos = inversTrans * screenPos;

		if (mIsPressed)
		{
			float hor = (localPos.X() - LeftBottomCornerOffset.X()) / sz.Width;
			float ver = (localPos.Z() - LeftBottomCornerOffset.Z()) / sz.Height;

			Vector2f dir(hor - 0.5f, ver - 0.5f);
			float dirLength = dir.Normalize();

			if (dirLength > 0.5f)
				dirLength = 0.5f;

			Vector2f dirLen = dir * dirLength;
			float percent = dirLength / 0.5f;

			Vector2f horVer1 = Vector2f(0.5f, 0.5f) + dirLen;

			mBut->SetAnchorHor(horVer1.X(), horVer1.X());
			mBut->SetAnchorVer(horVer1.Y(), horVer1.Y());

			OnDrag(dir, percent, screenPos);
		}
	}
}
//----------------------------------------------------------------------------
void UIRound::_UIButCallback(UIFrame *frame, UICallType type)
{
	UIButton *but = DynamicCast<UIButton>(frame);
	if (but == mBut)
	{
		const CanvasInputData &lastPickData = but->GetLastPickData();
		if (UICallType::UICT_PRESSED == type)
		{
			mPickTouchID = lastPickData.TouchID;
			OnPressed();
		}
		else if (UICallType::UICT_RELEASED == type)
		{
			OnReleased();
		}
		else if (UICallType::UICT_RELEASED_NOTPICK == type)
		{
			OnReleasedNotPick();
		}
	}
}
//----------------------------------------------------------------------------
void UIRound::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	const Vector2f dragDir = GetDraggingDir();
	float dragingStrength = GetDraggingPercentPiece(4);
	float absX = Mathf::FAbs(dragDir.X());
	float signX = Mathf::Sign(dragDir.X());
	float absY = Mathf::FAbs(dragDir.Y());
	float signY = Mathf::Sign(dragDir.Y());
	float param = 1.0f;

	if (0.0f == absX && 0.0f == absY)
	{
		mDragingLeftMoveType = 0;
		mDragingLeftMoveSpeed = 0.0f;
		mDragingRightMoveType = 0;
		mDragingRightMoveSpeed = 0.0f;
	}
	else
	{
		if (absY > absX)
		{
			if (signY > 0.0f)
			{
				mDragingLeftMoveType = 1;
				mDragingRightMoveType = 1;
				if (signX < 0)
				{
					param = (45.0f - Mathf::ATan((float)absX / (float)absY) * Mathf::RAD_TO_DEG) / 45.0f;
					mDragingLeftMoveSpeed = 255.0f * Lerp<float, float>(0, 1, param);
					mDragingRightMoveSpeed = 255.0f;
				}
				else if (signX > 0)
				{
					param = (45.0f - Mathf::ATan((float)absX / (float)absY) * Mathf::RAD_TO_DEG) / 45.0f;
					mDragingLeftMoveSpeed = 255.0f;
					mDragingRightMoveSpeed = 255.0f  * Lerp<float, float>(0, 1, param);
				}
			}
			else if (signY < 0.0f)
			{
				mDragingLeftMoveType = 2;
				mDragingRightMoveType = 2;

				float param = 1.0f;
				if (signX < 0)
				{
					param = (45.0f - Mathf::ATan((float)absX / (float)absY) * Mathf::RAD_TO_DEG) / 45.0f;
					mDragingLeftMoveSpeed = 255.0f;
					mDragingRightMoveSpeed = 255.0f * Lerp<float, float>(0, 1, param);
				}
				else if (signX > 0)
				{
					param = (45.0f - Mathf::ATan((float)absX / (float)absY) * Mathf::RAD_TO_DEG) / 45.0f;
					mDragingLeftMoveSpeed = 255.0f  * Lerp<float, float>(0, 1, param);
					mDragingRightMoveSpeed = 255.0f;
				}
			}
		}
		else if (absY < absX)
		{
			if (signX > 0.0f)
			{
				mDragingLeftMoveType = 1;
				mDragingRightMoveType = 2;

				if (signY > 0.0f)
				{
					param = (45.0f - Mathf::ATan((float)absY / (float)absX) * Mathf::RAD_TO_DEG) / 45.0f;
					mDragingLeftMoveSpeed = 255.0f;
					mDragingRightMoveSpeed = 255.0f  * Lerp<float, float>(0, 1, param);
				}
				else if (signY < 0.0f)
				{
					param = (45.0f - Mathf::ATan((float)absY / (float)absX) * Mathf::RAD_TO_DEG) / 45.0f;
					mDragingLeftMoveSpeed = 255.0f * Lerp<float, float>(0, 1, param);
					mDragingRightMoveSpeed = 255.0f;
				}
			}
			else if (signX < 0.0f)
			{
				mDragingLeftMoveType = 2;
				mDragingRightMoveType = 1;

				if (signY > 0.0f)
				{
					param = (45.0f - Mathf::ATan((float)absY / (float)absX) * Mathf::RAD_TO_DEG) / 45.0f;
					mDragingLeftMoveSpeed = 255.0f * Lerp<float, float>(0, 1, param);
					mDragingRightMoveSpeed = 255.0f;
				}
				else if (signY < 0.0f)
				{
					param = (45.0f - Mathf::ATan((float)absY / (float)absX) * Mathf::RAD_TO_DEG) / 45.0f;
					mDragingLeftMoveSpeed = 255.0f;
					mDragingRightMoveSpeed = 255.0f * Lerp<float, float>(0, 1, param);
				}
			}
		}

		mDragingLeftMoveSpeed *= dragingStrength;
		mDragingRightMoveSpeed *= dragingStrength;
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIRound::UIRound(LoadConstructor value) :
UIFrame(value),
mRoundCurDragingDirSimple(0),
mDragingLeftMoveType(0),
mDragingLeftMoveSpeed(0.0f),
mDragingRightMoveType(0),
mDragingRightMoveSpeed(0.0f)
{
	mIsDragable = true;

	mIsPressed = false;
	mDraggingDir = Vector2f::ZERO;
	mDraggedDir = Vector2f::ZERO;
	mDraggingPercent = 0.0f;
	mDraggedPercent = 0.0f;
	mPickTouchID = -1;

	SetChildPickOnlyInSizeRange(false);
}
//----------------------------------------------------------------------------
void UIRound::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsDragable);

	source.ReadPointer(mBack);
	source.ReadPointer(mBut);

	PX2_END_DEBUG_STREAM_LOAD(UIRound, source);
}
//----------------------------------------------------------------------------
void UIRound::Link(InStream& source)
{
	UIFrame::Link(source);

	source.ResolveLink(mBack);
	source.ResolveLink(mBut);

	RegistToScriptSystem();
}
//----------------------------------------------------------------------------
void UIRound::PostLink()
{
	UIFrame::PostLink();

	if (mBack)
	{
		mBack->PostLink();
		mBut->SetMemUICallback(this,
			(UIFrame::MemUICallback)(&UIRound::_UIButCallback));
	}

	if (mBut)
	{
		mBut->PostLink();
	}
}
//----------------------------------------------------------------------------
bool UIRound::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		if (mBack)
			target.Register(mBack);

		if (mBut)
			target.Register(mBut);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIRound::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsDragable);

	target.WritePointer(mBack);
	target.WritePointer(mBut);

	PX2_END_DEBUG_STREAM_SAVE(UIRound, target);
}
//----------------------------------------------------------------------------
int UIRound::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsDragable);

	size += PX2_POINTERSIZE(mBack);
	size += PX2_POINTERSIZE(mBut);

	return size;
}
//----------------------------------------------------------------------------