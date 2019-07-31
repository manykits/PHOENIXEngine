// PX2UIFrame.cpp

#include "PX2UIFrame.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UIMenu.hpp"
#include "PX2UIMenuItem.hpp"
#include "PX2FunObject.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2Picker.hpp"
#include "PX2UI.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, SizeNode, UIFrame);
PX2_IMPLEMENT_STREAM(UIFrame);
PX2_IMPLEMENT_FACTORY(UIFrame);

//----------------------------------------------------------------------------
UIFrame::UIFrame() :
mRoundRadius(-1.0f),
mMemObject(0),
mMemUICallback(0),
mActivatedColor(Float3::WHITE),
mActivatedAlpha(1.0f),
mActivatedBrightness(1.0f),
mIsNeedAdjustChildrenMask(false),
mRangeAcceptFrame(0),
mIsUseSkin(false),
mMaskVal(0)
{
	SetColor(Float3::WHITE);

	SetName("UIFrame");
	SetActivate(false);
}
//----------------------------------------------------------------------------
UIFrame::~UIFrame()
{
}
//----------------------------------------------------------------------------
void UIFrame::UpdateWorldData(double applicationTime, double elapsedTime)
{
	SizeNode::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsNeedAdjustChildrenMask)
	{
		_AdjustChildrenMask();
	}
}
//----------------------------------------------------------------------------
void UIFrame::SetUseSkin(bool isUseSkin)
{
	mIsUseSkin = isUseSkin;
}
//----------------------------------------------------------------------------
bool UIFrame::IsUseSkin() const
{
	return mIsUseSkin;
}
//----------------------------------------------------------------------------
void UIFrame::ConfigSkinColor()
{
	for (int i = 0; i < GetNumChildren(); i++)
	{
		UIFrame *frame = DynamicCast<UIFrame>(GetChild(i));
		if (frame)
		{
			frame->ConfigSkinColor();
		}
	}
}
//----------------------------------------------------------------------------
void UIFrame::AutoMakeSizeFixable(float scale)
{
	PX2_UNUSED(scale);
}
//----------------------------------------------------------------------------
void UIFrame::OnChildAttached(Movable *child)
{
	SizeNode::OnChildAttached(child);

	if (mGridAlignCtrl)
		mGridAlignCtrl->MarkLayoutChanged();
}
//----------------------------------------------------------------------------
void UIFrame::OnSizeChanged()
{
	if (mBackgroundPicBox)
	{
		mBackgroundPicBox->SetSize(mSize);
	}

	if (mMaskPicBox)
	{
		mMaskPicBox->SetSize(mSize);
	}

	SizeNode::OnSizeChanged();
}
//----------------------------------------------------------------------------
UIPicBox *UIFrame::CreateAddBackgroundPicBox(bool setWhite,
	const Float3& color)
{
	if (mBackgroundPicBox)
	{
		DetachChild(mBackgroundPicBox);
		mBackgroundPicBox = 0;
	}

	mBackgroundPicBox = new0 UIPicBox();
	AttachChild(mBackgroundPicBox);
	mBackgroundPicBox->SetName("BackPicBox");
	mBackgroundPicBox->UseAlphaBlend(false);
	mBackgroundPicBox->SetFakeTransparent(true);

	mBackgroundPicBox->SetSize(mSize);
	mBackgroundPicBox->SetPivot(mPivot);
	mBackgroundPicBox->SetColor(color);

	if (setWhite)
	{
		mBackgroundPicBox->SetTexture("Data/engine/white.png");
	}

	return mBackgroundPicBox;
}
//----------------------------------------------------------------------------
void UIFrame::DestoryBackgroundPicBox()
{
	if (mBackgroundPicBox)
	{
		DetachChild(mBackgroundPicBox);
		mBackgroundPicBox = 0;
	}
}
//----------------------------------------------------------------------------
void UIFrame::OnPivotChanged()
{
	if (mBackgroundPicBox)
		mBackgroundPicBox->SetPivot(mPivot);

	if (mMaskPicBox)
		mMaskPicBox->SetPivot(mPivot);

	SizeNode::OnPivotChanged();
}
//----------------------------------------------------------------------------
void UIFrame::SetActivateColor(const Float3 &color)
{
	mActivatedColor = color;

	SetActivate(IsActivated());
}
//----------------------------------------------------------------------------
void UIFrame::SetActivateAlpha(float alpha)
{
	mActivatedAlpha = alpha;

	SetActivate(IsActivated());
}
//----------------------------------------------------------------------------
void UIFrame::SetActivateBrightness(float brightness)
{
	mActivatedBrightness = brightness;

	SetActivate(IsActivated());
}
//----------------------------------------------------------------------------
void UIFrame::SetRoundRadius(float radius)
{
	mRoundRadius = radius;
}
//----------------------------------------------------------------------------
void UIFrame::SetPickRangeMovable(Movable *mov)
{
	mPickRangeMovable = mov;
}
//----------------------------------------------------------------------------
Movable *UIFrame::GetPickRangeMovable()
{
	return mPickRangeMovable;
}
//----------------------------------------------------------------------------
void UIFrame::SetPickRangeMovables(std::vector<SizeNode*> nodes)
{
	mPickRangeSizeNodes = nodes;
}
//----------------------------------------------------------------------------
void UIFrame::SetRangeAcceptFrame(UIFrame *frame)
{
	mRangeAcceptFrame = frame;
}
//----------------------------------------------------------------------------
UIFrame *UIFrame::GetRangeAcceptFrame()
{
	return mRangeAcceptFrame;
}
//----------------------------------------------------------------------------
void UIFrame::AddUICallback(UICallback callback)
{
	mUICallbacks.push_back(callback);
}
//----------------------------------------------------------------------------
void UIFrame::_UICallbacksCalls(UICallType callType)
{
	ObjectPtr ptrTshi = this;

	for (int i = 0; i < (int)mUICallbacks.size(); i++)
	{
		UICallback uiCallback = mUICallbacks[i];
		uiCallback(this, callType);
	}

	if (mMemObject && mMemUICallback)
	{
		(mMemObject->*mMemUICallback)(this, callType);
	}

	std::vector<Visitor *>::iterator it = mVisitors.begin();
	for (; it != mVisitors.end(); it++)
	{
		(*it)->Visit(this, (int)callType);
	}

	if (!mUIScriptHandler.empty())
	{
		PX2_SC_LUA->CallFunction(mUIScriptHandler, this, (int)callType);
	}
}
//----------------------------------------------------------------------------
void UIFrame::CallString(const std::string &funStr, const char *format, ...)
{
	ScriptManager *sm = ScriptManager::GetSingletonPtr();
	if (!sm) return;

	va_list argptr;
	va_start(argptr, format);
	PX2_SC_LUA->CallObjectFuntionValist(funStr, this, format, argptr);
	va_end(argptr);
}
//----------------------------------------------------------------------------
void UIFrame::PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas)
{
	if (!IsShow())
		return;

	if (!IsDoPick())
		return;

	const Rectf &rect = GetWorldRect();
	bool isPosInSizeRange = _IsInRect(inputData.CameraLogicPos);

	if (isPosInSizeRange)
	{
		float roundRadius = GetRoundRadius();
		if (roundRadius > 0)
		{
			float worldRadius = roundRadius * WorldTransform.GetUniformScale();

			APoint center;
			center.X() = rect.Left + rect.Width()*0.5f;
			center.Z() = rect.Bottom + rect.Height()*0.5f;
			AVector diffDir = inputData.LogicPos - center;
			float diffLengthSquare = diffDir.X()*diffDir.X() + diffDir.Z()*diffDir.Z();
			if (diffLengthSquare > worldRadius*worldRadius)
			{
				isPosInSizeRange = false;
			}
		}
	}

	if (isPosInSizeRange)
	{
		if (mPickRangeMovable)
		{
			UI *ui = GetFirstParentDerivedFromType<UI>();
			if (ui && (int)ui->GetCameras().size() > 0)
			{
				std::vector<CameraPtr> &camVec = ui->GetCameras();
				CameraPtr cam = camVec[0];
				const Sizef &uiSize = ui->GetSize();

				APoint origin;
				AVector dir;
				cam->GetPickRay(inputData.CameraLogicPos.X(),
					inputData.CameraLogicPos.Z(), uiSize, origin, dir);

				Picker picker;
				picker.Execute(mPickRangeMovable, origin, dir, 0.0f, 10000.0f);

				if (!picker.GetClosestNonnegative().Intersected)
				{
					isPosInSizeRange = false;
				}
			}
		}

		if ((int)mPickRangeSizeNodes.size() > 0)
		{
			bool isInFrame = false;

			for (int i = 0; i < (int)mPickRangeSizeNodes.size(); i++)
			{
				SizeNode *frame = mPickRangeSizeNodes[i];
				const Rectf &rect = frame->GetWorldRect();
				if (rect.IsInsize(inputData.CameraLogicPos.X(), inputData.CameraLogicPos.Z()))
				{
					isInFrame = true;
				}
			}

			if (!isInFrame)
			{
				isPosInSizeRange = false;
			}
		}
	}

	if (isPosInSizeRange)
	{
		OnSizeNodePicked(inputData);

		if (mIsWidget)
			canvas->_AddInRangePickWidget(this, inputData);
	}
	else
	{
		OnSizeNodeNotPicked(inputData);
	}

	bool pickChild = true;
	if (mIsChildPickOnlyInSizeRange && !isPosInSizeRange)
		pickChild = false;

	if (pickChild)
	{
		for (int i = 0; i < GetNumChildren(); i++)
		{
			SizeNode *childFrame = DynamicCast<SizeNode>(GetChild(i));
			if (childFrame)
			{
				childFrame->PreCanvasPick(inputData, canvas);
			}
		}
	}
}
//----------------------------------------------------------------------------
void _TravelSetMask(Movable *mov, Any *data, bool &goOn)
{
	PX2_UNUSED(data);
	PX2_UNUSED(goOn);

	UIFrame *frame = DynamicCast<UIFrame>(mov);
	Renderable *renderable = DynamicCast<Renderable>(mov);
	if (frame)
	{
		Canvas *canvas = DynamicCast<Canvas>(frame->GetParent());
		UIFrame *parentFrame = DynamicCast<UIFrame>(frame->GetParent());

		if (canvas)
		{
			if (!frame->GetMask())
			{
				frame->SetMaskVal(0);
			}
			else
			{
				frame->SetMaskVal(1);
			}
		}
		else if (parentFrame)
		{
			if (!frame->GetMask())
			{
				frame->SetMaskVal(parentFrame->GetMaskVal());
			}
			else
			{
				frame->SetMaskVal(parentFrame->GetMaskVal() + 1);
			}
		}
	}
	else if (renderable)
	{
		UIFrame *parentFrame = DynamicCast<UIFrame>(renderable->GetParent());
		if (parentFrame)
		{
			int parentMask = parentFrame->GetMaskVal();
			Canvas *canvas = DynamicCast<Canvas>(parentFrame->GetParent());

			StencilProperty *sp = renderable->GetMaterialInstance()->GetMaterial()
				->GetStencilProperty(0, 0);

			if ("MaskPicBox" == renderable->GetName())
			{
				if (canvas)
				{
					sp->Enabled = true;
					sp->Reference = parentMask;
					sp->Compare = StencilProperty::CM_ALWAYS;
					sp->OnZPass = StencilProperty::OT_REPLACE;
				}
				else
				{
					sp->Enabled = true;
					sp->Reference = parentMask - 1;
					sp->Compare = StencilProperty::CM_EQUAL;
					sp->OnZPass = StencilProperty::OT_INCREMENT;
				}
			}
			else
			{
				sp->Enabled = true;
				sp->Reference = parentMask;
				sp->Compare = StencilProperty::CM_EQUAL;
			}
		}
	}
}
//----------------------------------------------------------------------------
void _UITravelSetMask(Movable *mov, Any *data, bool &goOn)
{
	PX2_UNUSED(data);
	PX2_UNUSED(goOn);

	Renderable *renderable = DynamicCast<Renderable>(mov);
	if (renderable)
	{
		const std::string &name = renderable->GetName();
		StencilProperty *sp = renderable->GetMaterialInstance()->GetMaterial()
			->GetStencilProperty(0, 0);

		if ("MaskPicBox" == name)
		{
			sp->Enabled = true;
			sp->Reference = 1;
			sp->Compare = StencilProperty::CM_ALWAYS;
			sp->OnZPass = StencilProperty::OT_REPLACE;
		}
		else
		{
			sp->Enabled = true;
			sp->Reference = 1;
			sp->Compare = StencilProperty::CM_EQUAL;
			sp->OnZPass = StencilProperty::OT_KEEP;
		}
	}
}
//----------------------------------------------------------------------------
UIPicBox *UIFrame::CreateAddMask()
{
	DestoryMask();

	mMaskPicBox = new0 UIPicBox();
	AttachChild(mMaskPicBox);
	mMaskPicBox->SetName("MaskPicBox");
	Material *mtl = mMaskPicBox->GetMaterialInstance()->GetMaterial();
	OffsetProperty *op = mtl->GetOffsetProperty(0, 0);
	op->AllowRed = false;
	op->AllowGreen = false;
	op->AllowBlue = false;
	op->AllowAlpha = false;
	StencilProperty *sp = mtl->GetStencilProperty(0, 0);
	sp->Enabled = true;

	mMaskPicBox->LocalTransform.SetTranslateY(0.2f);
	mMaskPicBox->SetSize(mSize);
	mMaskPicBox->SetPivot(mPivot);

	SetNeedAdjustChildrenMask(true);

	return mMaskPicBox;
}
//----------------------------------------------------------------------------
void UIFrame::SetMaskVal(int maskVal)
{
	mMaskVal = maskVal;
}
//----------------------------------------------------------------------------
void UIFrame::DestoryMask()
{
	if (!mMaskPicBox) return;

	DetachChild(mMaskPicBox);
	mMaskPicBox = 0;
}
//----------------------------------------------------------------------------
void UIFrame::SetNeedAdjustChildrenMask(bool adjustMask)
{
	mIsNeedAdjustChildrenMask = adjustMask;
}
//----------------------------------------------------------------------------
bool UIFrame::IsNeedAdjustChildrenMask() const
{
	return mIsNeedAdjustChildrenMask;
}
//----------------------------------------------------------------------------
void UIFrame::_AdjustChildrenMask()
{
	mIsNeedAdjustChildrenMask = false;

	Node::TravelExecute(this, _UITravelSetMask);
}
//----------------------------------------------------------------------------
UIFrameGridAlignControl *UIFrame::CreateAddGridAlignCtrl(bool doResetPlay)
{
	DestoryGridAlignCtrl();

	mGridAlignCtrl = new0 UIFrameGridAlignControl();
	AttachController(mGridAlignCtrl);
	mGridAlignCtrl->SetName("GridAlignControl");

	if (doResetPlay) mGridAlignCtrl->ResetPlay();

	return mGridAlignCtrl;
}
//----------------------------------------------------------------------------
void UIFrame::DestoryGridAlignCtrl()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Properties
//----------------------------------------------------------------------------
void UIFrame::RegistProperties()
{
	SizeNode::RegistProperties();

	AddPropertyClass("UIFrame");

	AddProperty("ScriptHandler", PT_STRING, GetScriptHandler());

	std::vector<std::string> relativesStr;
	relativesStr.push_back("PT_NONE");
	relativesStr.push_back("LT_ZERO");
	relativesStr.push_back("LT_HALF");
	relativesStr.push_back("LT_ONE");
}
//----------------------------------------------------------------------------
void UIFrame::OnPropertyChanged(const PropertyObject &obj)
{
	SizeNode::OnPropertyChanged(obj);

	if ("ScriptHandler" == obj.Name)
	{
		SetScriptHandler(PX2_ANY_AS(obj.Data, std::string));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIFrame::UIFrame(LoadConstructor value) :
SizeNode(value),
mRoundRadius(-1.0f),
mMemObject(0),
mMemUICallback(0),
mActivatedColor(Float3::WHITE),
mActivatedAlpha(1.0f),
mActivatedBrightness(1.0f),
mIsNeedAdjustChildrenMask(false),
mRangeAcceptFrame(0),
mIsUseSkin(false),
mMaskVal(0)
{
}
//----------------------------------------------------------------------------
void UIFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	SizeNode::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mBackgroundPicBox);

	source.ReadAggregate(mActivatedColor);
	source.Read(mActivatedAlpha);
	source.Read(mActivatedBrightness);

	source.Read(mRoundRadius);
	source.ReadPointer(mPickRangeMovable);

	source.ReadString(mUIScriptHandler);

	source.ReadPointer(mGridAlignCtrl);

	source.ReadString(mScriptHandlerWidgetPicked);
	source.ReadString(mScriptHandlerNodePicked);
	source.ReadString(mScriptHandlerNodeNotPicked);

	PX2_END_DEBUG_STREAM_LOAD(UIFrame, source);
}
//----------------------------------------------------------------------------
void UIFrame::Link(InStream& source)
{
	SizeNode::Link(source);

	if (mBackgroundPicBox)
		source.ResolveLink(mBackgroundPicBox);

	if (mGridAlignCtrl)
		source.ResolveLink(mGridAlignCtrl);

	if (mPickRangeMovable)
		source.ResolveLink(mPickRangeMovable);
}
//----------------------------------------------------------------------------
void UIFrame::PostLink()
{
	SizeNode::PostLink();

	SetActivate(false);
}
//----------------------------------------------------------------------------
bool UIFrame::Register(OutStream& target) const
{
	if (SizeNode::Register(target))
	{
		if (mBackgroundPicBox)
			target.Register(mBackgroundPicBox);

		if (mGridAlignCtrl)
			target.Register(mGridAlignCtrl);

		if (mPickRangeMovable)
			target.Register(mPickRangeMovable);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	SizeNode::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mBackgroundPicBox);

	target.WriteAggregate(mActivatedColor);
	target.Write(mActivatedAlpha);
	target.Write(mActivatedBrightness);

	target.Write(mRoundRadius);
	target.WritePointer(mPickRangeMovable);

	target.WriteString(mUIScriptHandler);

	target.WritePointer(mGridAlignCtrl);

	target.WriteString(mScriptHandlerWidgetPicked);
	target.WriteString(mScriptHandlerNodePicked);
	target.WriteString(mScriptHandlerNodeNotPicked);

	PX2_END_DEBUG_STREAM_SAVE(UIFrame, target);
}
//----------------------------------------------------------------------------
int UIFrame::GetStreamingSize(Stream &stream) const
{
	int size = SizeNode::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mBackgroundPicBox);

	size += sizeof(mActivatedColor);
	size += sizeof(mActivatedAlpha);
	size += sizeof(mActivatedBrightness);

	size += sizeof(mRoundRadius);
	size += PX2_POINTERSIZE(mPickRangeMovable);

	size += PX2_STRINGSIZE(mUIScriptHandler);
	
	size += PX2_POINTERSIZE(mGridAlignCtrl);

	size += PX2_STRINGSIZE(mScriptHandlerWidgetPicked);
	size += PX2_STRINGSIZE(mScriptHandlerNodePicked);
	size += PX2_STRINGSIZE(mScriptHandlerNodeNotPicked);

	return size;
}
//----------------------------------------------------------------------------