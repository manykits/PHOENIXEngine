// PX2Movable.cpp

#include "PX2Movable.hpp"
#include "PX2Culler.hpp"
#include "PX2FunObject.hpp"
#include "PX2Node.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controlledable, Movable);
PX2_IMPLEMENT_STREAM(Movable);
PX2_IMPLEMENT_ABSTRACT_FACTORY(Movable);

//----------------------------------------------------------------------------
Movable::Movable() :
WorldTransformIsCurrent(false),
WorldBoundIsCurrent(false),
Culling(CULL_DYNAMIC),
mParent(0),
mIsAlphaSelfCtrled(false),
mAlpha(1.0f),
mIsColorSelfCtrled(false),
mColor(Float3::BLACK),
mIsBrightnessSelfCtrled(false),
mBrightness(1.0f),
mLastAlpha(1.0f),
mLastColor(Float3::WHITE),
mLastBrightness(1.0f),
mIsAlphaColorBrightnessChanged(true),
mIsEnableSelfCtrled(true),
mIsActivateSelfCtrled(true),
mUpdateTime(-1.0f),
mUpdateTiming(0.0f),
mIsOnlyShowUpdate(false),
mUpdatePriority(0),
mIsDoPick(true),
mIsPickIngoreCullingMode(false),
mIsNotPickedParentChildrenNotPicked(false),
mIsIngoreParent_Translate(false),
mIsIngoreParent_Rotate(false),
mIsIngoreParent_Scale(false),
IsSkinCtrlSetWroldTrans(false),
mIsSaveWriteIngore(false),
mIsCastShadow(true),
mIsReceiveShadow(true)
{
}
//----------------------------------------------------------------------------
Movable::~Movable ()
{
	// mParent不需要被释放
}
//----------------------------------------------------------------------------
void Movable::Show (bool show)
{
	if (show)
	{
		if (CULL_ALWAYS == Culling)
		{
			Culling = CULL_DYNAMIC;
		}
	}
	else
	{
		Culling = CULL_ALWAYS;
	}
}
//----------------------------------------------------------------------------
void Movable::SetDoPick (bool doPick)
{
	mIsDoPick = doPick;
}
//----------------------------------------------------------------------------
bool Movable::IsShow () const
{
	return Culling!=CULL_ALWAYS;
}
//----------------------------------------------------------------------------
void Movable::Update(double applicationTime, double elapsedTime, 
	bool initiator)
{
	if (IsOnlyShowUpdate())
	{
		if (!IsShow())
			return;
	}

	bool doUpdateTrans = true;
	if (mUpdateTime > 0.0f)
	{
		if (mUpdateTiming > 0.0f)
		{
			doUpdateTrans = true;
			mUpdateTiming -= (float)elapsedTime;
		}
		else
		{
			doUpdateTrans = false;
		}
	}

	if (doUpdateTrans)
	{
		UpdateWorldData(applicationTime, elapsedTime);
		UpdateWorldBound();
		if (initiator)
		{
			PropagateBoundToRoot();
		}
	}
}
//----------------------------------------------------------------------------
void Movable::OnBeAttached()
{
	ResetFixUpdateTiming();
}
//----------------------------------------------------------------------------
void Movable::OnBeDetach()
{
}
//----------------------------------------------------------------------------
void Movable::DetachFromParent()
{
	Node *parentNode = DynamicCast<Node>(GetParent());
	if (parentNode)
	{
		parentNode->DetachChild(this);
	}
}
//----------------------------------------------------------------------------
void Movable::SetParentTransformIngore (bool trans, bool rotate, bool scale)
{
	mIsIngoreParent_Translate = trans;
	mIsIngoreParent_Rotate = rotate;
	mIsIngoreParent_Scale = scale;
}
//----------------------------------------------------------------------------
void Movable::GetParentTransformIngore (bool &trans, bool &rotate, bool &scale)
{
	trans = mIsIngoreParent_Translate;
	rotate = mIsIngoreParent_Rotate;
	scale = mIsIngoreParent_Scale;
}
//----------------------------------------------------------------------------
Movable *Movable::GetTopestParent()
{
	Movable *topestParentTemp = mParent;
	Movable *topestParent = mParent;

	while (topestParentTemp)
	{
		topestParent = topestParentTemp;

		topestParentTemp = topestParentTemp->GetParent();
	}

	return topestParent;
}
//----------------------------------------------------------------------------
void Movable::OnPicked (int pickInfo)
{
	PX2_UNUSED(pickInfo);
}
//----------------------------------------------------------------------------
void Movable::OnNotPicked (int pickInfo)
{
	PX2_UNUSED(pickInfo);
}
//----------------------------------------------------------------------------
void Movable::MarkAlphaColorBrightnessChanged()
{
	mIsAlphaColorBrightnessChanged = true;
}
//----------------------------------------------------------------------------
void Movable::_AdjustAlphaColorBrightnes()
{
	Movable *parent = GetParent();
	if (parent)
	{
		if (!mIsAlphaSelfCtrled)
		{
			mLastAlpha = parent->GetLastAlpha() * GetAlpha();
		}
		else
		{
			mLastAlpha = GetAlpha();
		}

		if (!mIsColorSelfCtrled)
		{
			mLastColor[0] = parent->GetLastColor()[0] * GetColor()[0];
			mLastColor[1] = parent->GetLastColor()[1] * GetColor()[1];
			mLastColor[2] = parent->GetLastColor()[2] * GetColor()[2];
		}
		else
		{
			mLastColor = GetColor();
		}

		if (!mIsBrightnessSelfCtrled)
		{
			mLastBrightness = parent->GetLastBrightness() * mBrightness;
		}
		else
		{
			mLastBrightness = mBrightness;
		}
	}

	mIsAlphaColorBrightnessChanged = false;
}
//----------------------------------------------------------------------------
void Movable::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UpdateControllers(applicationTime, elapsedTime);

	if (mIsAlphaColorBrightnessChanged)
		_AdjustAlphaColorBrightnes();

	if (LocalTransform.IsRSMatrix())
	{
		const APoint &scale = LocalTransform.GetScale();
		if (scale.X()==scale.Y() && scale.Y()==scale.Z())
			LocalTransform.SetUniformScale(scale.X());
	}

	// 更新世界变换
	if (!WorldTransformIsCurrent)
	{
		if (mParent)
		{
			if (mIsIngoreParent_Translate || mIsIngoreParent_Rotate ||
				mIsIngoreParent_Scale)
			{
				Transform trans = mParent->WorldTransform;

				if (mIsIngoreParent_Translate)
				{
					trans.SetTranslate(APoint::ORIGIN);
				}
				if (mIsIngoreParent_Rotate)
				{
					trans.SetRotate(HMatrix::IDENTITY);
				}
				if (mIsIngoreParent_Scale)
				{
					trans.SetScale(APoint(1.0f, 1.0f, 1.0f));
				}
				WorldTransform = trans*LocalTransform;
			}
			else
			{
				WorldTransform = mParent->WorldTransform*LocalTransform;
			}
		}
		else
		{
			WorldTransform = LocalTransform;
		}
	}

	if (IsSkinCtrlSetWroldTrans)
	{
		if (mParent)
		{
			if (mIsIngoreParent_Translate || mIsIngoreParent_Rotate ||
				mIsIngoreParent_Scale)
			{
				Transform trans = mParent->WorldTransform;

				if (mIsIngoreParent_Translate)
				{
					trans.SetTranslate(APoint::ORIGIN);
				}
				if (mIsIngoreParent_Rotate)
				{
					trans.SetRotate(HMatrix::IDENTITY);
				}
				if (mIsIngoreParent_Scale)
				{
					trans.SetScale(APoint(1.0f, 1.0f, 1.0f));
				}
				BoundWorldTransform = trans*LocalTransform;
			}
			else
			{
				BoundWorldTransform = mParent->WorldTransform*LocalTransform;
			}
		}
		else
		{
			BoundWorldTransform = LocalTransform;
		}
	}
	else
	{
		BoundWorldTransform = WorldTransform;
	}
}
//----------------------------------------------------------------------------
void Movable::PropagateBoundToRoot ()
{
    if (mParent)
    {
        mParent->UpdateWorldBound();
        mParent->PropagateBoundToRoot();
    }
}
//----------------------------------------------------------------------------
void Movable::GetVisibleSet (Culler& culler, bool noCull)
{
	int flag_CastShadow = culler.GetFlag_CastShadow();
	if (0 == flag_CastShadow)
	{
	}
	else if (1 == flag_CastShadow)
	{
		if (!IsCastShadow()) return;
	}
	else if (2 == flag_CastShadow)
	{
		if (IsCastShadow()) return;
	}

    if (Culling == CULL_ALWAYS)
    {
		OnCulled(culler);
        return;
    }

    if (Culling == CULL_NEVER)
    {
        noCull = true;
    }

    unsigned int savePlaneState = culler.GetPlaneState();
    if (noCull || culler.IsVisible(WorldBound))
    {
        OnGetVisibleSet(culler, noCull);
    }
	else
	{
		OnCulled(culler);
	}

    culler.SetPlaneState(savePlaneState);
}
//----------------------------------------------------------------------------
void Movable::OnCulled(Culler& culler)
{
	PX2_UNUSED(culler);
}
//----------------------------------------------------------------------------
void Movable::SetParent(Movable* parent)
{
	mParent = parent;
}
//----------------------------------------------------------------------------
void Movable::SetAlpha (float alpha)
{
	if (mAlpha == alpha) return;

	mAlpha = alpha;

	MarkAlphaColorBrightnessChanged();
}
//----------------------------------------------------------------------------
void Movable::SetColor (const Float3 &color)
{
	if (mColor == color) return;

	mColor = color;

	MarkAlphaColorBrightnessChanged();
}
//----------------------------------------------------------------------------
void Movable::SetBrightness (float brightness)
{
	if (mBrightness == brightness) return;

	mBrightness = brightness;

	MarkAlphaColorBrightnessChanged();
}
//----------------------------------------------------------------------------
void Movable::SetCastShadow(bool castShadow)
{
	mIsCastShadow = castShadow;
}
//----------------------------------------------------------------------------
void Movable::SetReceiveShadow(bool reciveShadow)
{
	mIsReceiveShadow = reciveShadow;
}
//----------------------------------------------------------------------------
void Movable::SetUpdatePriority(int updatePriority)
{
	mUpdatePriority = updatePriority;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称
//----------------------------------------------------------------------------
Object* Movable::GetObjectByName (const std::string& name)
{
    // mParent不用查找，否则会无限循环
    return Controlledable::GetObjectByName(name);
}
//----------------------------------------------------------------------------
void Movable::GetAllObjectsByName (const std::string& name,
    std::vector<Object*>& objects)
{
    // mParent不用查找，否则会无限循环
    Controlledable::GetAllObjectsByName(name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Movable::RegistProperties ()
{
	Controlledable::RegistProperties();

	AddPropertyClass("Movable");

	PropertyObject objLocalTrans;
	objLocalTrans.Name = "LocalTransform";
	objLocalTrans.Type = Object::PT_TRANSFORM;
	objLocalTrans.Data = LocalTransform;
	objLocalTrans.Data1 = true;
	AddProperty(objLocalTrans);

	PropertyObject objWorldTrans;
	objWorldTrans.Name = "WorldTransform";
	objWorldTrans.Type = Object::PT_TRANSFORM;
	objWorldTrans.Data = WorldTransform;
	objWorldTrans.Data1 = false;
	AddProperty(objWorldTrans);
	AddProperty("WorldTransformIsCurrent", Object::PT_BOOL, Any(WorldTransformIsCurrent));

	AddProperty("WorldBoundCenter", Object::PT_APOINT3, Any(WorldBound.GetCenter()));
	AddProperty("WorldBoundRadius", Object::PT_FLOAT, Any(WorldBound.GetRadius()));
	AddProperty("WorldBoundIsCurrent", Object::PT_BOOL, Any(WorldBoundIsCurrent));

	std::vector<std::string> cullings;
	cullings.push_back("CULL_DYNAMIC");
	cullings.push_back("CULL_ALWAYS");
	cullings.push_back("CULL_NEVER");
	AddPropertyEnum("CullingMode", (int)Culling, cullings);
	AddProperty("IsShow", Object::PT_BOOL, IsShow());

	AddProperty("Alpha", Object::PT_FLOAT, Any(GetAlpha()));
	AddProperty("IsAlphaSelfCtrled", Object::PT_BOOL, Any(IsAlphaSelfCtrled()));
	AddProperty("Color", Object::PT_COLOR3FLOAT3, Any(GetColor()));
	AddProperty("IsColorSelfCtrled", Object::PT_BOOL, Any(IsColorSelfCtrled()));
	AddProperty("Brightness", Object::PT_FLOAT, GetBrightness());
	AddProperty("IsBrightnessSelfCtrled", Object::PT_BOOL, IsBrightnessSelfCtrled());

	AddProperty("IsCastShadow", Object::PT_BOOL, IsCastShadow());
	AddProperty("IsReceiveShadow", Object::PT_BOOL, IsReceiveShadow());

	AddProperty("UpdateTime", Object::PT_FLOAT, GetFixUpdateTime());
}
//----------------------------------------------------------------------------
void Movable::OnPropertyChanged (const PropertyObject &obj)
{
	Controlledable::OnPropertyChanged(obj);

	if ("LocalTransform" == obj.Name)
	{
		LocalTransform = *Any_Cast<Transform>(&obj.Data);
	}
	else if ("WorldTransform" == obj.Name)
	{
		WorldTransform = *Any_Cast<Transform>(&obj.Data);
	}
	else if ("WorldTransformIsCurrent" == obj.Name)
	{
		WorldTransformIsCurrent = *Any_Cast<bool>(&obj.Data);
	}
	else if ("WorldBoundIsCurrent" == obj.Name)
	{
		WorldBoundIsCurrent = PX2_ANY_AS(obj.Data, bool);
	}
	else if ("WorldBoundCenter" == obj.Name)
	{
		WorldBound.SetCenter(*Any_Cast<APoint>(&obj.Data));
	}
	else if ("WorldBoundRadius" == obj.Name)
	{
		WorldBound.SetRadius(*Any_Cast<float>(&obj.Data));
	}
	else if ("CullingMode" == obj.Name)
	{
		Culling = (CullingMode)(*Any_Cast<int>(&obj.Data));
	}
	else if ("IsShow" == obj.Name)
	{
		Show(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("Alpha" == obj.Name)
	{
		SetAlpha(*Any_Cast<float>(&obj.Data));
	}
	else if ("IsAlphaSelfCtrled" == obj.Name)
	{
		SetAlphaSelfCtrled(*Any_Cast<bool>(&obj.Data));
	}
	else if ("Color" == obj.Name)
	{
		SetColor(*Any_Cast<Float3>(&obj.Data));
	}
	else if ("IsColorSelfCtrled" == obj.Name)
	{
		SetColorSelfCtrled(*Any_Cast<bool>(&obj.Data));
	}
	else if ("Brightness" == obj.Name)
	{
		SetBrightness(PX2_ANY_AS(obj.Data, float));
	}
	else if ("IsBrightnessSelfCtrled" == obj.Name)
	{
		SetBrightnessSelfCtrled(PX2_ANY_AS(obj.Data, bool));
	}
	//else if ("IsCastShadow" == obj.Name)
	//{
	//	SetCastShadow(PX2_ANY_AS(obj.Data, bool));
	//}
	//else if ("IsReceiveShadow" == obj.Name)
	//{
	//	SetReceiveShadow(PX2_ANY_AS(obj.Data, bool));
	//}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化
//----------------------------------------------------------------------------
Movable::Movable(LoadConstructor value) :
Controlledable(value),
WorldTransformIsCurrent(false),
WorldBoundIsCurrent(false),
Culling(CULL_DYNAMIC),
mParent(0),
mIsAlphaSelfCtrled(false),
mAlpha(1.0f),
mIsColorSelfCtrled(false),
mColor(Float3::WHITE),
mIsBrightnessSelfCtrled(false),
mBrightness(1.0f),
mLastAlpha(1.0f),
mLastColor(Float3::WHITE),
mLastBrightness(1.0f),
mIsAlphaColorBrightnessChanged(true),
mIsEnableSelfCtrled(true),
mIsActivateSelfCtrled(true),
mUpdateTime(-1.0f),
mUpdateTiming(0.0f),
mIsOnlyShowUpdate(false),
mUpdatePriority(0),
mIsDoPick(true),
mIsPickIngoreCullingMode(false),
mIsNotPickedParentChildrenNotPicked(false),
mIsIngoreParent_Translate(false),
mIsIngoreParent_Rotate(false),
mIsIngoreParent_Scale(false),
IsSkinCtrlSetWroldTrans(false),
mIsSaveWriteIngore(false),
mIsCastShadow(true),
mIsReceiveShadow(true)
{
}
//----------------------------------------------------------------------------
void Movable::Load (InStream& source)
{
    PX2_BEGIN_DEBUG_STREAM_LOAD(source);

    Controlledable::Load(source);
	PX2_VERSION_LOAD(source);

    source.ReadAggregate(LocalTransform);
    source.ReadAggregate(WorldTransform);
    source.ReadBool(WorldTransformIsCurrent);
    source.ReadAggregate(WorldBound);
    source.ReadBool(WorldBoundIsCurrent);
    source.ReadEnum(Culling);

	int readedVersion = GetReadedVersion();
	PX2_UNUSED(readedVersion);

	source.Read(mUpdateTime);

	// mParent不被保存，它会在Node::Link中调用Node::SetChild被设置。

	source.ReadBool(mIsAlphaSelfCtrled);
	source.Read(mAlpha);

	source.ReadBool(mIsColorSelfCtrled);
	source.ReadAggregate(mColor);

	source.ReadBool(mIsBrightnessSelfCtrled);
	source.Read(mBrightness);

	source.ReadBool(mIsIngoreParent_Translate);
	source.ReadBool(mIsIngoreParent_Rotate);
	source.ReadBool(mIsIngoreParent_Scale);

	source.ReadBool(mIsCastShadow);
	source.ReadBool(mIsReceiveShadow);

	source.ReadBool(mIsDoPick);

    PX2_END_DEBUG_STREAM_LOAD(Movable, source);
}
//----------------------------------------------------------------------------
void Movable::Link (InStream& source)
{
	// mParent不被保存，它会在Node::Link中调用Node::SetChild被设置。

    Controlledable::Link(source);
}
//----------------------------------------------------------------------------
void Movable::PostLink ()
{
    Controlledable::PostLink();
}
//----------------------------------------------------------------------------
bool Movable::Register (OutStream& target) const
{
	// mParent不需要被注册

    return Controlledable::Register(target);
}
//----------------------------------------------------------------------------
void Movable::Save (OutStream& target) const
{
    PX2_BEGIN_DEBUG_STREAM_SAVE(target);

    Controlledable::Save(target);
	PX2_VERSION_SAVE(target);

    target.WriteAggregate(LocalTransform);
    target.WriteAggregate(WorldTransform);
    target.WriteBool(WorldTransformIsCurrent);
    target.WriteAggregate(WorldBound);
    target.WriteBool(WorldBoundIsCurrent);
    target.WriteEnum(Culling);
	target.Write(mUpdateTime);

	target.WriteBool(mIsAlphaSelfCtrled);
	target.Write(mAlpha);

	target.WriteBool(mIsColorSelfCtrled);
	target.WriteAggregate(mColor);

	target.WriteBool(mIsBrightnessSelfCtrled);
	target.Write(mBrightness);

	target.WriteBool(mIsIngoreParent_Translate);
	target.WriteBool(mIsIngoreParent_Rotate);
	target.WriteBool(mIsIngoreParent_Scale);

	target.WriteBool(mIsCastShadow);
	target.WriteBool(mIsReceiveShadow);

	target.WriteBool(mIsDoPick);

	// mParent不被保存，它会在Node::Link中调用Node::SetChild被设置。

    PX2_END_DEBUG_STREAM_SAVE(Movable, target);
}
//----------------------------------------------------------------------------
int Movable::GetStreamingSize(Stream &stream) const
{
	int size = Controlledable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += LocalTransform.GetStreamingSize();;
	size += WorldTransform.GetStreamingSize();
	size += PX2_BOOLSIZE(WorldTransformIsCurrent);
	size += WorldBound.GetStreamingSize();
	size += PX2_BOOLSIZE(WorldBoundIsCurrent);
	size += PX2_ENUMSIZE(Culling);

	size += sizeof(mUpdateTime);

	size += PX2_BOOLSIZE(mIsAlphaSelfCtrled);
	size += sizeof(mAlpha);

	size += PX2_BOOLSIZE(mIsColorSelfCtrled);
	size += sizeof(mColor);

	size += PX2_BOOLSIZE(mIsBrightnessSelfCtrled);
	size += sizeof(mBrightness);

	size += PX2_BOOLSIZE(mIsIngoreParent_Translate);
	size += PX2_BOOLSIZE(mIsIngoreParent_Rotate);
	size += PX2_BOOLSIZE(mIsIngoreParent_Scale);

	size += PX2_BOOLSIZE(mIsCastShadow);
	size += PX2_BOOLSIZE(mIsReceiveShadow);

	size += PX2_BOOLSIZE(mIsDoPick);

	// mParent不被持久化

	return size;
}
//----------------------------------------------------------------------------
