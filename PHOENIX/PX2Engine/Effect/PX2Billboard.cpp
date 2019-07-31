// PX2Billboard.cpp

#include "PX2Billboard.hpp"
#include "PX2BillboardController.hpp"
#include "PX2Effectable.hpp"
#include "PX2Renderer.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Effectable, Billboard);
PX2_IMPLEMENT_STREAM(Billboard);
PX2_IMPLEMENT_FACTORY(Billboard);
PX2_IMPLEMENT_DEFAULT_NAMES(Effectable, Billboard);

//----------------------------------------------------------------------------
std::vector<std::string> Billboard::msModuleNames_EA;
std::vector<std::string> Billboard::msModuleNames_EO;
//----------------------------------------------------------------------------
Billboard::Billboard () :
mIsDynamic(true),
mIsUseTexAsSize(false),
mIsDoAlphaDisAfterStop(true),
mDoAlphaDisAfterStopSpeed(0.5f),
mIsUseTrim(false)
{
	SetName("Billboard");

	SetTex("Data/engine/default.png");

	mPivotPoint = Float2(0.5f, 0.5f);

	SetLocal(true);
	SetDynamic(mIsDynamic);

	IndexBuffer *iBuffer = new0 IndexBuffer(6, 2, Buffer::BU_STATIC);
	unsigned short *indices = (unsigned short*)iBuffer->GetData();
	unsigned short v0 = 0;
	unsigned short v1 = 1;
	unsigned short v2 = 2;
	unsigned short v3 = 3;
	*indices++ = v0;
	*indices++ = v1;
	*indices++ = v2;
	*indices++ = v1;
	*indices++ = v3;
	*indices++ = v2;
	SetIndexBuffer(iBuffer);

	mEffectableCtrl = new0 BillboardController();
	mEffectableCtrl->SetName("BillboardController");
	AttachController(mEffectableCtrl);
}
//----------------------------------------------------------------------------
Billboard::~Billboard ()
{
}
//----------------------------------------------------------------------------
void Billboard::SetDynamic (bool isDynamic)
{
	mIsDynamic = isDynamic;

	Buffer::Usage usage = mIsDynamic ? Buffer::BU_DYNAMIC : Buffer::BU_STATIC;

	VertexBuffer *vBuffer = new0 VertexBuffer(4, 
		GetVertexFormat()->GetStride(), usage);
	SetVertexBuffer(vBuffer);
}
//----------------------------------------------------------------------------
void Billboard::SetPivot (float anchX, float anchZ)
{
	mPivotPoint[0] = anchX;
	mPivotPoint[1] = anchZ;
}
//----------------------------------------------------------------------------
void Billboard::SetPivot (const Float2 &anchor)
{
	mPivotPoint = anchor;
}
//----------------------------------------------------------------------------
void Billboard::SetUseTexAsSize (bool use)
{
	mIsUseTexAsSize = use;
}
//----------------------------------------------------------------------------
void Billboard::SetUseTrim (bool useTrim)
{
	mIsUseTrim = useTrim;
}
//----------------------------------------------------------------------------
void Billboard::SetSizeImmediate (float sizeX, float sizeY, float sizeZ)
{
	EffectObject *eo = ((BillboardController*)GetEffectableController())
		->GetBillboardObject();

	if (eo)
	{
		eo->SizeX = sizeX;
		eo->SizeY = sizeY;
		eo->SizeZ = sizeZ;
	}

	SetEmitSizeX(sizeX);
	SetEmitSizeY(sizeY);
	SetEmitSizeZ(sizeZ);
}
//----------------------------------------------------------------------------
void Billboard::GenBuffers ()
{
	Effectable::GenBuffers();

	BillboardController *billCtrl = DynamicCast<BillboardController>(
		mEffectableCtrl);
	const EffectObject *billboardObject = billCtrl->GetBillboardObject();
	if (!billboardObject)
		return;

	float age = billboardObject->Age;

	float uBegin = billboardObject->UV0Begin[0];
	float uEnd =  billboardObject->UV0End[0];
	float vBegin =  billboardObject->UV0Begin[1];
	float vEnd =  billboardObject->UV0End[1];
	int uvIndex = GetUV(billboardObject->StartRandomIndex, 
		age, uBegin, uEnd, vBegin, vEnd);

	float xPlusPer = 0.0f;
	float zPlusPer = 0.0f;
	float widthPer = 1.0f;
	float heightPer = 1.0f;

	if (IsUseTrim() && Effectable::TM_TEXPACK_ANIM==GetTexMode() && mTexPackAnim_Frames.size()>0)
	{
		if (0 != mTexPackAnim_Frames[uvIndex].OW)
		{
			xPlusPer = (float)mTexPackAnim_Frames[uvIndex].OX
				/ (float)mTexPackAnim_Frames[uvIndex].OW;
			widthPer = (float)mTexPackAnim_Frames[uvIndex].W
				/(float)mTexPackAnim_Frames[uvIndex].OW;
		}
		if (0 != mTexPackAnim_Frames[uvIndex].OH)
		{
			zPlusPer = 1.0f -
				(float)(mTexPackAnim_Frames[uvIndex].OY+mTexPackAnim_Frames[uvIndex].H)
				/ (float)mTexPackAnim_Frames[uvIndex].OH;
			heightPer = (float)mTexPackAnim_Frames[uvIndex].H
				/(float)mTexPackAnim_Frames[uvIndex].OH;
		}
	}

	float width = billboardObject->SizeX;
	float height = billboardObject->SizeY;

	float xPos = 0.0f - width * mPivotPoint[0];
	float zPos = 0.0f - height * mPivotPoint[1];
	xPos += xPlusPer * width;
	zPos += zPlusPer * height;	

	width *= widthPer;
	height *= heightPer;

	Float3 color = billboardObject->Color;
	float alpha = billboardObject->Alpha;

	Float3 p0;
	Float3 p1;
	Float3 p2;
	Float3 p3;
	FaceType faceType = GetFaceType();
	if (FT_X == faceType)
	{
		p0 = Float3(0.0f,		xPos,		zPos);
		p1 = Float3(0.0f,		xPos+width,	zPos);
		p2 = Float3(0.0f,		xPos,		zPos+height);
		p3 = Float3(0.0f,		xPos+width, zPos+height);
	}
	else if (FT_NX == faceType)
	{
		p0 = Float3(0.0f,		-xPos,			zPos);
		p1 = Float3(0.0f,		-(xPos+width),	zPos);
		p2 = Float3(0.0f,		-xPos,			zPos+height);
		p3 = Float3(0.0f,		-(xPos+width),	zPos+height);
	}
	else if (FT_Y == faceType)
	{
		p0 = Float3(-xPos,			0.0f,		zPos);
		p1 = Float3(-(xPos+width),	0.0f,		zPos);
		p2 = Float3(-xPos,			0.0f,		zPos+height);
		p3 = Float3(-(xPos+width),	0.0f,		zPos+height);
	}
	else if (FT_NY == faceType)
	{
		p0 = Float3(xPos,		0.0f,		zPos);
		p1 = Float3(xPos+width,	0.0f,		zPos);
		p2 = Float3(xPos,		0.0f,		zPos+height);
		p3 = Float3(xPos+width, 0.0f,		zPos+height);
	}
	else if (FT_Z == faceType)
	{
		p0 = Float3(xPos,		zPos,			0.0f);
		p1 = Float3(xPos+width,	zPos,			0.0f);
		p2 = Float3(xPos,		zPos+height,	0.0f);
		p3 = Float3(xPos+width,	zPos+height,	0.0f);
	}
	else if (FT_NZ == faceType)
	{
		p0 = Float3(-xPos,			zPos,			0.0f);
		p1 = Float3(-(xPos+width),	zPos,			0.0f);
		p2 = Float3(-xPos,			zPos+height,	0.0f);
		p3 = Float3(-(xPos+width),	zPos+height,	0.0f);
	}
	else if (FT_CAMERA == faceType)
	{
		Camera *camera = PX2_GR.GetCurUpdateCamera();
		if (!camera)
			return;

		const AVector &camUp = camera->GetUVector();
		const AVector &camRight = camera->GetRVector();

		float anchorWidth = mPivotPoint[0]*width;
		float anchorHeight = mPivotPoint[1]*height;

		APoint position;
		p0 = position 
			- camRight * anchorWidth
			- camUp * anchorHeight;

		p1 = position 
			+ camRight * (width-anchorWidth)
			- camUp * anchorHeight;

		p2 = position 
			- camRight * anchorWidth
			+ camUp * (height-anchorHeight);

		p3 = position 
			+ camRight * (width-anchorWidth)
			+ camUp * (height-anchorHeight);
	}
	else if (FT_SPEEDDIR==faceType || FT_FREE==faceType)
	{
		p0 = Float3(xPos,		0.0f,		zPos);
		p1 = Float3(xPos+width,	0.0f,		zPos);
		p2 = Float3(xPos,		0.0f,		zPos+height);
		p3 = Float3(xPos+width, 0.0f,		zPos+height);
	}

	//SetUseShareBuffers(true);
	if (!IsUseShareBuffers())
	{
		VertexBufferAccessor vba(this);
		vba.Position<Float3>(0) = p0;
		vba.Position<Float3>(1) = p1;
		vba.Position<Float3>(2) = p2;
		vba.Position<Float3>(3) = p3;

		vba.Color<Float4>(0, 0) = Float4(color[0], color[1], color[2], alpha);
		vba.Color<Float4>(0, 1) = Float4(color[0], color[1], color[2], alpha);
		vba.Color<Float4>(0, 2) = Float4(color[0], color[1], color[2], alpha);
		vba.Color<Float4>(0, 3) = Float4(color[0], color[1], color[2], alpha);
		vba.TCoord<Float2>(0, 0) = Float2(uBegin, vBegin);
		vba.TCoord<Float2>(0, 1) = Float2(uEnd, vBegin);
		vba.TCoord<Float2>(0, 2) = Float2(uBegin, vEnd);
		vba.TCoord<Float2>(0, 3) = Float2(uEnd, vEnd);

		if (!IsFixedBound())
			UpdateModelSpace(GU_MODEL_BOUND_ONLY);

		Renderer::UpdateAll(GetVertexBuffer());
	}
	else
	{
		mDBObject_V = PX2_DBM.AllocVertexBuffer(4);
		mDBObject_I = PX2_DBM.AllocIndexBuffer(6);

		SetShareDBObject_V(mDBObject_V);
		SetShareDBObject_I(mDBObject_I);

		VertexBufferAccessor vba;
		vba.ApplyTo(GetVertexFormat(), mDBObject_V->Buf);

		vba.Position<Float3>(mDBObject_V->Offset+0) = p0;
		vba.Position<Float3>(mDBObject_V->Offset+1) = p1;
		vba.Position<Float3>(mDBObject_V->Offset+2) = p2;
		vba.Position<Float3>(mDBObject_V->Offset+3) = p3;

		vba.Color<Float4>(0, mDBObject_V->Offset+0) = Float4(color[0], color[1], color[2], alpha);
		vba.Color<Float4>(0, mDBObject_V->Offset+1) = Float4(color[0], color[1], color[2], alpha);
		vba.Color<Float4>(0, mDBObject_V->Offset+2) = Float4(color[0], color[1], color[2], alpha);
		vba.Color<Float4>(0, mDBObject_V->Offset+3) = Float4(color[0], color[1], color[2], alpha);
		vba.TCoord<Float2>(0, mDBObject_V->Offset+0) = Float2(uBegin, vBegin);
		vba.TCoord<Float2>(0, mDBObject_V->Offset+1) = Float2(uEnd, vBegin);
		vba.TCoord<Float2>(0, mDBObject_V->Offset+2) = Float2(uBegin, vEnd);
		vba.TCoord<Float2>(0, mDBObject_V->Offset+3) = Float2(uEnd, vEnd);

		unsigned short *indices = (unsigned short*)mDBObject_I->Buf->GetData();
		indices += mDBObject_I->Offset;

		unsigned short v0 = (unsigned short)(mDBObject_V->Offset+0);
		unsigned short v1 = (unsigned short)(mDBObject_V->Offset+1);
		unsigned short v2 = (unsigned short)(mDBObject_V->Offset+2);
		unsigned short v3 = (unsigned short)(mDBObject_V->Offset+3);
		*indices++ = v0;
		*indices++ = v1;
		*indices++ = v2;
		*indices++ = v1;
		*indices++ = v3;
		*indices++ = v2;
	}
}
//----------------------------------------------------------------------------
void Billboard::UpdateWorldData(double applicationTime, double elapsedTime1)
{
	Movable::UpdateWorldData(applicationTime, elapsedTime1);

	Renderer *defaultRenderer = Renderer::GetDefaultRenderer();
	Camera *camera = defaultRenderer->GetCamera();
	FaceType faceType = GetFaceType();

	if (FT_CAMERA==faceType || FT_SPEEDDIR==faceType || FT_FREE==faceType)
	{
		//APoint camModelPos = WorldTransform.Inverse()*camera->GetPosition();
		//float angle0 = Mathf::ATan2(camModelPos[0], -camModelPos[1]);
		//HMatrix orient0(AVector::UNIT_Z, angle0);
		//float angle1 = Mathf::ATan(camModelPos[2]/camModelPos[1]);
		//HMatrix orient1(AVector::UNIT_X, angle1);
		//WorldTransform.SetRotate(WorldTransform.GetRotate()*orient1*orient0);
	}
	else if (FT_CAMERA_ZUP == faceType)
	{
		APoint camModelPos = WorldTransform.Inverse()*camera->GetPosition();
		float angle = Mathf::ATan2(camModelPos[0], -camModelPos[1]);
		HMatrix orient(AVector::UNIT_Z, angle);
		WorldTransform.SetRotate(WorldTransform.GetRotate()*orient);
	}
}
//----------------------------------------------------------------------------
void Billboard::RegistProperties ()
{
	Effectable::RegistProperties();

	AddPropertyClass("Billboard");
	AddProperty("IsDynamic", PT_BOOL, IsDynamic());
	AddProperty("AnchorPoint", PT_FLOAT2, GetPivot());
	AddProperty("IsUseTexAsSize", PT_BOOL, IsUseTexAsSize());
	AddProperty("IsDoAlphaDisAfterStop", PT_BOOL, IsDoAlphaDisAfterStop());
	AddProperty("AlphaDisAfterStopSpeed", PT_FLOAT, GetDoAlphaDisAfterStopSpeed());
}
//----------------------------------------------------------------------------
void Billboard::OnPropertyChanged (const PropertyObject &obj)
{
	Effectable::OnPropertyChanged(obj);

	if ("IsDynamic" == obj.Name)
	{
		SetDynamic(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("AnchorPoint" == obj.Name)
	{
		SetPivot(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("IsUseTexAsSize" == obj.Name)
	{
		SetUseTexAsSize(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsDoAlphaDisAfterStop" == obj.Name)
	{
		SetDoAlphaDisAfterStop(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("AlphaDisAfterStopSpeed" == obj.Name)
	{
		SetDoAlphaDisAfterStopSpeed(PX2_ANY_AS(obj.Data, float));
	}
}
//----------------------------------------------------------------------------
const std::vector<std::string> &Billboard::GetAllModuleNames_EA () const
{
	return msModuleNames_EA;
}
//----------------------------------------------------------------------------
const std::vector<std::string> &Billboard::GetAllModuleNames_EO () const
{
	return msModuleNames_EO;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
Billboard::Billboard (LoadConstructor value) :
Effectable(value),
mIsDynamic(true),
mIsUseTexAsSize(false),
mIsDoAlphaDisAfterStop(true),
mDoAlphaDisAfterStopSpeed(0.5f),
mIsUseTrim(false)
{
}
//----------------------------------------------------------------------------
void Billboard::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Effectable::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsDynamic);
	source.ReadAggregate(mPivotPoint);
	source.ReadBool(mIsUseTexAsSize);
	source.ReadBool(mIsDoAlphaDisAfterStop);
	source.Read(mDoAlphaDisAfterStopSpeed);

	source.ReadBool(mIsUseTrim);

	PX2_END_DEBUG_STREAM_LOAD(Billboard, source);
}
//----------------------------------------------------------------------------
void Billboard::Link (InStream& source)
{
	Effectable::Link(source);
}
//----------------------------------------------------------------------------
void Billboard::PostLink ()
{
	Effectable::PostLink();
}
//----------------------------------------------------------------------------
bool Billboard::Register (OutStream& target) const
{
	if (Effectable::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Billboard::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Effectable::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsDynamic);
	target.WriteAggregate(mPivotPoint);
	target.WriteBool(mIsUseTexAsSize);
	target.WriteBool(mIsDoAlphaDisAfterStop);
	target.Write(mDoAlphaDisAfterStopSpeed);
	target.WriteBool(mIsUseTrim);

	PX2_END_DEBUG_STREAM_SAVE(Billboard, target);
}
//----------------------------------------------------------------------------
int Billboard::GetStreamingSize (Stream &stream) const
{
	int size = Effectable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsDynamic);
	size += sizeof(mPivotPoint);
	size += PX2_BOOLSIZE(mIsUseTexAsSize);
	size += PX2_BOOLSIZE(mIsDoAlphaDisAfterStop);
	size += sizeof(mDoAlphaDisAfterStopSpeed);

	size += PX2_BOOLSIZE(mIsUseTrim);

	return size;
}
//----------------------------------------------------------------------------