// PX2SizeNode.cpp

#include "PX2SizeNode.hpp"
#include "PX2Canvas.hpp"
#include "PX2FunObject.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, SizeNode);
PX2_IMPLEMENT_STREAM(SizeNode);
PX2_IMPLEMENT_FACTORY(SizeNode);
PX2_IMPLEMENT_DEFAULT_NAMES(Node, SizeNode);

//----------------------------------------------------------------------------
SizeNode::SizeNode() :
mIsLayoutChanged(true),
mIsAnchorLayoutEnable(true),
mIsScreenRectChanged(true),
mIsEnableScreenRectLayout(true),
mSize(Sizef(100.0f, 100.0f)),
mPivot(Float2(0.5f, 0.5f)),
mSizeChangeTellToObject(0),
mSizeChangeCallback(0),
mIsWidget(false),
mIsChildPickOnlyInSizeRange(true),
mIsNotPickRecursion(true),
mMemObject(0),
mMemPickCallback(0)
{
	mAnchorHor = Float2(0.5f, 0.5f);
	mAnchorVer = Float2(0.5f, 0.5f);
}
//----------------------------------------------------------------------------
SizeNode::~SizeNode()
{
}
//----------------------------------------------------------------------------
void SizeNode::UpdateLayout()
{
	if (mIsLayoutChanged)
	{
		if (mIsAnchorLayoutEnable)
		{
			UpdateLayout(GetParent());
			UpdateLeftBottomCornerOffset(GetParent());
		}
		else
		{
			UpdateLeftBottomCornerOffset(GetParent());
		}

		_MarkRelatvieLayoutChange();

		mIsLayoutChanged = false;
	}
}
//----------------------------------------------------------------------------
void SizeNode::UpdateScreenRect()
{
	if (mIsScreenRectChanged)
	{
		if (mIsEnableScreenRectLayout)
		{
			UpdateScreenRect(GetParent());
		}

		_MarkRelatvieScreenChange();

		mIsScreenRectChanged = false;
	}
}
//----------------------------------------------------------------------------
void SizeNode::SetWidget(bool isWidget)
{
	mIsWidget = isWidget;
}
//----------------------------------------------------------------------------
void SizeNode::SetSelfPickSizeExtend(const Float2 &extend)
{
	mSelfPickSizeExtend = extend;
}
//----------------------------------------------------------------------------
const Float2 &SizeNode::GetSelfPickSizeExtend() const
{
	return mSelfPickSizeExtend;
}
//----------------------------------------------------------------------------
void SizeNode::SetChildPickOnlyInSizeRange(bool onlyInRange)
{
	mIsChildPickOnlyInSizeRange = onlyInRange;
}
//----------------------------------------------------------------------------
bool SizeNode::IsChildPickOnlyInSizeRange() const
{
	return mIsChildPickOnlyInSizeRange;
}
//----------------------------------------------------------------------------
void SizeNode::SetChildNotPickRecursion(bool notPickRecursion)
{
	mIsNotPickRecursion = notPickRecursion;
}
//----------------------------------------------------------------------------
bool SizeNode::IsChildNotPickRecursion() const
{
	return mIsNotPickRecursion;
}
//----------------------------------------------------------------------------
void SizeNode::PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas)
{
	if (!IsShow())
		return;

	if (!IsDoPick())
		return;

	bool isPosInSizeRange = _IsInRect(inputData.CameraLogicPos);

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
void SizeNode::OnWidgetPicked(const CanvasInputData &inputData)
{
	mLastPickData = inputData;

	if (mMemObject && mMemPickCallback)
	{
		(mMemObject->*mMemPickCallback)(this, SNPT_WIDGET_PICKED, inputData);
	}

	if (!mScriptHandlerWidgetPicked.empty())
	{
		PX2_SC_LUA->CallFunction(mScriptHandlerWidgetPicked, this);
	}
}
//----------------------------------------------------------------------------
void SizeNode::OnSizeNodePicked(const CanvasInputData &inputData)
{
	mLastPickData = inputData;

	if (mMemObject && mMemPickCallback)
	{
		(mMemObject->*mMemPickCallback)(this, SNPT_SIZENODE_PICKED, inputData);
	}

	if (!mScriptHandlerNodePicked.empty())
	{
		PX2_SC_LUA->CallFunction(mScriptHandlerNodePicked, this);
	}
}
//----------------------------------------------------------------------------
void SizeNode::OnSizeNodeNotPicked(const CanvasInputData &inputData)
{
	mLastPickData = inputData;

	if (mMemObject && mMemPickCallback)
	{
		(mMemObject->*mMemPickCallback)(this, SNPT_SIZENODE_NOTPICKED, inputData);
	}

	if (!mScriptHandlerNodeNotPicked.empty())
	{
		PX2_SC_LUA->CallFunction(mScriptHandlerNodeNotPicked, this);
	}

	if (mIsChildPickOnlyInSizeRange && mIsNotPickRecursion)
	{
		for (int i = 0; i < GetNumChildren(); i++)
		{
			SizeNode *childFrame = DynamicCast<SizeNode>(GetChild(i));
			if (childFrame)
			{
				childFrame->OnSizeNodeNotPicked(inputData);
			}
		}
	}
}
//----------------------------------------------------------------------------
void SizeNode::SetMemPickCallback(Object *object, MemPickCallback callback)
{
	mMemObject = object;
	mMemPickCallback = callback;
}
//----------------------------------------------------------------------------
SizeNode::MemPickCallback SizeNode::GetMemPickCallback() const
{
	return mMemPickCallback;
}
//----------------------------------------------------------------------------
void SizeNode::SetScriptHandlerWidgetPicked(const std::string &scriptHandler)
{
	mScriptHandlerWidgetPicked = scriptHandler;
}
//----------------------------------------------------------------------------
void SizeNode::SetScriptHandlerNodePicked(const std::string &scriptHandler)
{
	mScriptHandlerNodePicked = scriptHandler;
}
//----------------------------------------------------------------------------
void SizeNode::SetScriptHandlerNodeNotPicked(const std::string &scriptHandler)
{
	mScriptHandlerNodeNotPicked = scriptHandler;
}
//----------------------------------------------------------------------------
std::string SizeNode::GetScriptHandlerWidgetPicked() const
{
	return mScriptHandlerWidgetPicked;
}
//----------------------------------------------------------------------------
std::string SizeNode::GetScriptHandlerNodePicked() const
{
	return mScriptHandlerNodePicked;
}
//----------------------------------------------------------------------------
std::string SizeNode::GetScriptHandlerNodeNotPicked() const
{
	return mScriptHandlerNodeNotPicked;
}
//----------------------------------------------------------------------------
bool SizeNode::_IsInRect(const APoint &logicPos)
{
	Rectf worldRect = GetWorldRect();
	Rectf rect = Rectf(worldRect.Left - mSelfPickSizeExtend[0],
		worldRect.Bottom - mSelfPickSizeExtend[1],
		worldRect.Right + mSelfPickSizeExtend[0],
		worldRect.Top + mSelfPickSizeExtend[1]);

	bool isPosInSizeRange = rect.IsInsize(logicPos.X(), logicPos.Z());

	return isPosInSizeRange;
}
//----------------------------------------------------------------------------
const CanvasInputData &SizeNode::GetLastPickData() const
{
	return mLastPickData;
}
//----------------------------------------------------------------------------
void SizeNode::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UpdateLayout();
	
	Movable::UpdateWorldData(applicationTime, elapsedTime);

	UpdateScreenRect();

	_UpdateWorldDataChild(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void SizeNode::OnBeAttached()
{
	Node::OnBeAttached();

	mIsLayoutChanged = true;
	mIsScreenRectChanged = true;
}
//----------------------------------------------------------------------------
void SizeNode::OnBeDetach()
{
	mIsLayoutChanged = true;
	mIsScreenRectChanged = true;
}
//----------------------------------------------------------------------------
void SizeNode::SetSize(float width, float height)
{
	SetSize(Sizef(width, height));
}
//----------------------------------------------------------------------------
void SizeNode::SetSize(const Sizef &size)
{
	mSize = size;
	OnSizeChanged();
}
//----------------------------------------------------------------------------
void SizeNode::OnSizeChanged()
{
	mIsLayoutChanged = true;
	mIsScreenRectChanged = true;

	if (mSizeChangeTellToObject && mSizeChangeCallback)
	{
		(mSizeChangeTellToObject->*mSizeChangeCallback)(this);
	}

	if (!mScriptHandlerSizeChanged.empty())
	{
		PX2_SC_LUA->CallFunction(mScriptHandlerSizeChanged, this);
	}
}
//----------------------------------------------------------------------------
void SizeNode::SetWidth(float width)
{
	SetSize(Sizef(width, mSize.Height));
}
//----------------------------------------------------------------------------
void SizeNode::SetHeight(float height)
{
	SetSize(Sizef(mSize.Width, height));
}
//----------------------------------------------------------------------------
void SizeNode::SetScriptHandlerSizeChanged(const std::string &scriptHandler)
{
	mScriptHandlerSizeChanged = scriptHandler;
}
//----------------------------------------------------------------------------
const std::string &SizeNode::GetScriptSizeChanged() const
{
	return mScriptHandlerSizeChanged;
}
//----------------------------------------------------------------------------
Rectf SizeNode::GetLocalRect() const
{
	Rectf rect;
	rect.Left = LeftBottomCornerOffset.X();
	rect.Bottom = LeftBottomCornerOffset.Z();
	rect.Right = rect.Left + GetWidth();
	rect.Top = rect.Bottom + GetHeight();

	return rect;
}
//----------------------------------------------------------------------------
Rectf SizeNode::GetWorldRect() const
{
	Rectf rect = GetLocalRect();

	const APoint worldPos = WorldTransform.GetTranslate();
	rect.Left += worldPos.X();
	rect.Right += worldPos.X();
	rect.Bottom += worldPos.Z();
	rect.Top += worldPos.Z();

	return rect;
}
//----------------------------------------------------------------------------
APoint SizeNode::WorldPosToViewPortPos(const APoint &screenPos)
{
	Rectf worldRect = GetWorldRect();

	APoint viewPortPos;
	viewPortPos.X() = screenPos.X() - worldRect.Left;
	viewPortPos.Z() = screenPos.Z() - worldRect.Bottom;

	return viewPortPos;
}
//----------------------------------------------------------------------------
bool SizeNode::IsInSizeRange(const SizeNode *node) const
{
	Rectf worldRect = GetWorldRect();
	Rectf nodeWorldRect = node->GetWorldRect();

	return  worldRect.IsInSide(nodeWorldRect);
}
//----------------------------------------------------------------------------
bool SizeNode::IsIntersectSizeRange(const SizeNode *node) const
{
	Rectf worldRect = GetWorldRect();
	Rectf nodeWorldRect = node->GetWorldRect();

	return worldRect.IsIntersect(nodeWorldRect);
}
//----------------------------------------------------------------------------
void SizeNode::SetSizeChangeCallback(SizeNode *object, SizeChangeCallback callback)
{
	mSizeChangeTellToObject = object;
	mSizeChangeCallback = callback;
}
//----------------------------------------------------------------------------
void SizeNode::SetPivot(float x, float y)
{
	SetPivot(Float2(x, y));
}
//----------------------------------------------------------------------------
void SizeNode::SetPivot(const Float2 &pvoit)
{
	mPivot = pvoit;

	OnPivotChanged();
}
//----------------------------------------------------------------------------
void SizeNode::OnPivotChanged()
{
	mIsLayoutChanged = true;
	mIsScreenRectChanged = true;
}
//----------------------------------------------------------------------------
void SizeNode::EnableAnchorLayout(bool enable)
{
	mIsAnchorLayoutEnable = enable;
	mIsLayoutChanged = true;
	mIsScreenRectChanged = true;
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorHor(float anchorX, float anchorY)
{
	SetAnchorHor(Float2(anchorX, anchorY));
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorHor(const Float2 &anchor)
{
	Float2 beforeAnchor = mAnchorHor;
	mAnchorHor = anchor;

	if (beforeAnchor != mAnchorHor)
	{
		mIsLayoutChanged = true;
		mIsScreenRectChanged = true;
	}
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorVer(float anchorX, float anchorY)
{
	SetAnchorVer(Float2(anchorX, anchorY));
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorVer(const Float2 &anchor)
{
	Float2 beforeAnchor = mAnchorVer;
	mAnchorVer = anchor;

	if (beforeAnchor != mAnchorVer)
	{
		mIsLayoutChanged = true;
		mIsScreenRectChanged = true;
	}
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorParamHor(float param0, float param1)
{
	SetAnchorParamHor(Float2(param0, param1));
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorParamHor(const Float2 &param)
{
	Float2 beforeParam = mAnchorParamHor;
	mAnchorParamHor = param;

	if (beforeParam != mAnchorParamHor)
	{
		mIsLayoutChanged = true;
		mIsScreenRectChanged = true;
	}
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorParamVer(float param0, float param1)
{
	SetAnchorParamVer(Float2(param0, param1));
}
//----------------------------------------------------------------------------
void SizeNode::SetAnchorParamVer(const Float2 &param)
{
	Float2 beforeParam = mAnchorParamVer;
	mAnchorParamVer = param;

	if (beforeParam != mAnchorParamVer)
	{
		mIsLayoutChanged = true;
		mIsScreenRectChanged = true;
	}
}
//----------------------------------------------------------------------------
void SizeNode::_MarkRelatvieLayoutChange()
{
	mIsLayoutChanged = true;

	for (int i = 0; i < GetNumChildren(); i++)
	{
		SizeNode *childSizeNode = DynamicCast<SizeNode>(GetChild(i));
		if (childSizeNode)
		{
			childSizeNode->_MarkRelatvieLayoutChange();
		}
	}
}
//----------------------------------------------------------------------------
void SizeNode::_MarkRelatvieScreenChange()
{
	mIsScreenRectChanged = true;

	for (int i = 0; i < GetNumChildren(); i++)
	{
		SizeNode *childSizeNode = DynamicCast<SizeNode>(GetChild(i));
		if (childSizeNode)
		{
			childSizeNode->_MarkRelatvieScreenChange();
		}
	}
}
//----------------------------------------------------------------------------
void SizeNode::UpdateLayout(Movable *parent)
{
	Movable *par = parent;
	if (!par) return;

	SizeNode *parNode = DynamicCast<SizeNode>(par);

	// parent pvoit决定了从那一点作为原点，计算当前Frame原点位置
	// 例如
	// (0.0f, 0.0f)是左下角
	// (0.5f, 0.5f)是中心点	
	Sizef parentSize;
	AVector parentLeftBottomOffset;
	if (parNode)
	{
		parentSize = parNode->GetSize();
		parentLeftBottomOffset = parNode->LeftBottomCornerOffset;
	}

	APoint localPos = LocalTransform.GetTranslate();
	Sizef localSize = GetSize();

	APoint newPos;
	Sizef newSize = localSize;
	if (parNode)
	{
		float width = 0.0f;
		float height = 0.0f;

		bool isHEqual = (mAnchorHor[0] == mAnchorHor[1]);
		bool isVEqual = (mAnchorVer[0] == mAnchorVer[1]);

		if (isHEqual)
		{
			width = localSize.Width;
			newPos.X() = parentLeftBottomOffset.X() +
				parentSize.Width * mAnchorHor[0] + mAnchorParamHor[0];
		}
		else
		{
			// 如果是范围，直接取中心点，作为原点
			width = parentSize.Width * (mAnchorHor[1] - mAnchorHor[0]) - mAnchorParamHor[0] + mAnchorParamHor[1];
			newPos.X() = parentLeftBottomOffset.X() + 
				parentSize.Width*mAnchorHor[0] + mAnchorParamHor[0] + width * mPivot[0];
		}

		if (isVEqual)
		{
			height = localSize.Height;
			newPos.Z() = parentLeftBottomOffset.Z() +
				parentSize.Height * mAnchorVer[0] + mAnchorParamVer[0];
		}
		else
		{
			// 如果是范围，直接取中心点，作为原点
			height = parentSize.Height * (mAnchorVer[1] - mAnchorVer[0]) - mAnchorParamVer[0] + mAnchorParamVer[1];
			newPos.Z() = parentLeftBottomOffset.Z() + 
				parentSize.Height*mAnchorVer[0] + mAnchorParamVer[0] + height * mPivot[1];
		}

		newSize = Sizef(width, height);
	}

	newPos.Y() = localPos.Y();
	LocalTransform.SetTranslate(newPos);

	if (newSize != localSize)
	{
		SetSize(newSize);
	}
}
//----------------------------------------------------------------------------
void SizeNode::UpdateLeftBottomCornerOffset(Movable *parent)
{
	if (parent)
	{
		LeftBottomCornerOffset.X() = -mSize.Width * mPivot[0];
		LeftBottomCornerOffset.Z() = -mSize.Height * mPivot[1];
	}
	else
	{
		LeftBottomCornerOffset.X() = 0.0f;
		LeftBottomCornerOffset.Z() = 0.0f;
	}
}
//----------------------------------------------------------------------------
void SizeNode::EnableScreenRectLayout(bool enable)
{
	mIsEnableScreenRectLayout = enable;
}
//----------------------------------------------------------------------------
void SizeNode::UpdateScreenRect(Movable *parent)
{
	Movable *par = parent;
	if (!par) return;

	SizeNode *parNode = DynamicCast<SizeNode>(par);
	if (!parNode)
		return;

	const Rectf &parScreenRect = parNode->GetScreenRect();

	Rectf parWorldRect = parNode->GetWorldRect();
	Rectf selfWorldRect = GetWorldRect();

	float leftPercent = (selfWorldRect.Left - parWorldRect.Left) / parWorldRect.Width();
	float downPercent = (selfWorldRect.Bottom - parWorldRect.Bottom) / parWorldRect.Height();

	float width = (selfWorldRect.Width() / parWorldRect.Width()) * parScreenRect.Width();
	float height = (selfWorldRect.Height() / parWorldRect.Height()) * parScreenRect.Height();

	Rectf selfScreenRect;
	selfScreenRect.Left = parScreenRect.Left + parScreenRect.Width() * leftPercent;
	selfScreenRect.Bottom = parScreenRect.Bottom + parScreenRect.Height() * downPercent;
	selfScreenRect.Right = selfScreenRect.Left + width;
	selfScreenRect.Top = selfScreenRect.Bottom + height;

	if (selfScreenRect != GetScreenRect())
	{
		SetScreenRect(selfScreenRect);
	}
}
//----------------------------------------------------------------------------
void SizeNode::SetScreenRect(const Rectf &rect)
{
	mScreenRect = rect;
	mIsScreenRectChanged = true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void SizeNode::RegistProperties()
{
	Node::RegistProperties();

	AddPropertyClass("SizeNode");

	AddProperty("Size", Object::PT_SIZE, GetSize());
	AddProperty("Pivot", Object::PT_FLOAT2, GetPivot());
	
	AddProperty("IsAnchorLayoutEnable", Object::PT_BOOL, IsAnchorLayoutEnable());

	AddProperty("AnchorHor", Object::PT_FLOAT2, GetAnchorHor());
	AddProperty("AnchorParamHor", Object::PT_FLOAT2, GetAnchorParamHor());

	AddProperty("AnchorVer", Object::PT_FLOAT2, GetAnchorVer());
	AddProperty("AnchorParamVer", Object::PT_FLOAT2, GetAnchorParamVer());
}
//----------------------------------------------------------------------------
void SizeNode::OnPropertyChanged(const PropertyObject &obj)
{
	Node::OnPropertyChanged(obj);

	if ("Size" == obj.Name)
	{
		SetSize(PX2_ANY_AS(obj.Data, Sizef));
	}
	else if ("Pivot" == obj.Name)
	{
		SetPivot(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("IsAnchorLayoutEnable" == obj.Name)
	{
		EnableAnchorLayout(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("AnchorHor" == obj.Name)
	{
		SetAnchorHor(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("AnchorParamHor" == obj.Name)
	{
		SetAnchorParamHor(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("AnchorVer" == obj.Name)
	{
		SetAnchorVer(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("AnchorParamVer" == obj.Name)
	{
		SetAnchorParamVer(PX2_ANY_AS(obj.Data, Float2));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
SizeNode::SizeNode(LoadConstructor value):
Node(value),
mIsLayoutChanged(true),
mIsAnchorLayoutEnable(true),
mIsScreenRectChanged(true),
mIsEnableScreenRectLayout(true),
mSize(Sizef(100.0f, 100.0f)),
mPivot(Float2(0.5f, 0.5f)),
mSizeChangeTellToObject(0),
mSizeChangeCallback(0),
mIsWidget(false),
mIsChildPickOnlyInSizeRange(true),
mIsNotPickRecursion(true),
mMemObject(0),
mMemPickCallback(0)
{
}
//----------------------------------------------------------------------------
void SizeNode::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadAggregate(mSize);

	source.ReadAggregate(mPivot);

	source.ReadBool(mIsAnchorLayoutEnable);
	source.ReadAggregate(mAnchorHor);
	source.ReadAggregate(mAnchorVer);
	source.ReadAggregate(mAnchorParamHor);
	source.ReadAggregate(mAnchorParamVer);

	source.ReadBool(mIsEnableScreenRectLayout);

	source.ReadBool(mIsWidget);
	source.ReadAggregate(mSelfPickSizeExtend);
	source.ReadBool(mIsChildPickOnlyInSizeRange);
	source.ReadBool(mIsNotPickRecursion);

	PX2_END_DEBUG_STREAM_LOAD(SizeNode, source);
}
//----------------------------------------------------------------------------
void SizeNode::Link(InStream& source)
{
	Node::Link(source);
}
//----------------------------------------------------------------------------
void SizeNode::PostLink()
{
	Node::PostLink();
}
//----------------------------------------------------------------------------
bool SizeNode::Register(OutStream& target) const
{
	return Node::Register(target);
}
//----------------------------------------------------------------------------
void SizeNode::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteAggregate(mSize);

	target.WriteAggregate(mPivot);

	target.WriteBool(mIsAnchorLayoutEnable);
	target.WriteAggregate(mAnchorHor);
	target.WriteAggregate(mAnchorVer);
	target.WriteAggregate(mAnchorParamHor);
	target.WriteAggregate(mAnchorParamVer);

	target.WriteBool(mIsEnableScreenRectLayout);

	target.WriteBool(mIsWidget);
	target.WriteAggregate(mSelfPickSizeExtend);
	target.WriteBool(mIsChildPickOnlyInSizeRange);
	target.WriteBool(mIsNotPickRecursion);

	PX2_END_DEBUG_STREAM_SAVE(SizeNode, target);
}
//----------------------------------------------------------------------------
int SizeNode::GetStreamingSize(Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mSize);

	size += sizeof(mPivot);

	size += PX2_BOOLSIZE(mIsAnchorLayoutEnable);
	size += sizeof(mAnchorHor);
	size += sizeof(mAnchorVer);
	size += sizeof(mAnchorParamHor);
	size += sizeof(mAnchorParamVer);

	size += PX2_BOOLSIZE(mIsEnableScreenRectLayout);

	size += PX2_BOOLSIZE(mIsWidget);
	size += sizeof(mSelfPickSizeExtend);
	size += PX2_BOOLSIZE(mIsChildPickOnlyInSizeRange);
	size += PX2_BOOLSIZE(mIsNotPickRecursion);

	return size;
}
//----------------------------------------------------------------------------