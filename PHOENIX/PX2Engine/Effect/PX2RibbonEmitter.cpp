// PX2RibbonEmitter.cpp

#include "PX2RibbonEmitter.hpp"
#include "PX2RibbonEmitterController.hpp"
#include "PX2Log.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Effectable, RibbonEmitter);
PX2_IMPLEMENT_STREAM(RibbonEmitter);
PX2_IMPLEMENT_FACTORY(RibbonEmitter);
PX2_IMPLEMENT_DEFAULT_NAMES(Effectable, RibbonEmitter);

//----------------------------------------------------------------------------
std::vector<std::string> RibbonEmitter::msModuleNames_EA;
std::vector<std::string> RibbonEmitter::msModuleNames_EO;
//----------------------------------------------------------------------------
RibbonEmitter::RibbonEmitter () :
mLength(20.0f),
mSegLength(2.0f),
mNumMaxVertexs(0),
mNumMaxIndexs(0)
{
	SetName("RibbonEmitter");

	SetTex("Data/engine/default.png");
	
	UpdateMaxNumVertexs();
	
	SetLocal(false);
	SetFaceType(Effectable::FT_CAMERA);

	mEffectableCtrl = new0 RibbonEmitterController();
	mEffectableCtrl->SetName("RibbonEmitterController");
	AttachController(mEffectableCtrl);
}
//----------------------------------------------------------------------------
RibbonEmitter::~RibbonEmitter ()
{
}
//----------------------------------------------------------------------------
void RibbonEmitter::SetLength (float length)
{
	mLength = length;
	UpdateMaxNumVertexs();
}
//----------------------------------------------------------------------------
void RibbonEmitter::SetSegLength (float length)
{
	mSegLength = length;
	UpdateMaxNumVertexs();
}
//----------------------------------------------------------------------------
void RibbonEmitter::RegistProperties ()
{
	Effectable::RegistProperties();

	AddPropertyClass("RibbonEmitter");
	AddProperty("Length", PT_FLOAT, GetLength());
	AddProperty("SegLength", PT_FLOAT, GetSegLength());
}
//----------------------------------------------------------------------------
void RibbonEmitter::OnPropertyChanged (const PropertyObject &obj)
{
	Effectable::OnPropertyChanged(obj);

	if ("Length" == obj.Name)
	{
		SetLength(PX2_ANY_AS(obj.Data, float));
	}
	else if ("SegLength" == obj.Name)
	{
		SetSegLength(PX2_ANY_AS(obj.Data, float));
	}
}
//----------------------------------------------------------------------------
void RibbonEmitter::UpdateMaxNumVertexs ()
{
	int numSegments = (int)(mLength/mSegLength) + 2;
	int numTanglers = numSegments * 2;

	mNumMaxVertexs = (numSegments+1) * 2;
	mNumMaxIndexs = numTanglers * 6;

	VertexBufferPtr newVBuffer = new0 VertexBuffer(mNumMaxVertexs,
		GetVertexFormat()->GetStride(), Buffer::BU_DYNAMIC);
	SetVertexBuffer(newVBuffer);
	newVBuffer->SetNumElements(0);

	IndexBufferPtr newIBuffer = new0 IndexBuffer(mNumMaxIndexs, 2, 
		Buffer::BU_STATIC);
	unsigned short iFI, iFIp1, iFIp2, iFIp3;
	unsigned short* indices = (unsigned short*)newIBuffer->GetData();
	for (int i=0; i<numSegments; i++)
	{
		iFI = (unsigned short)(2*i);
		iFIp1 = iFI + 1;
		iFIp2 = iFI + 2;
		iFIp3 = iFI + 3;
		*indices++ = iFI;
		*indices++ = iFIp1;
		*indices++ = iFIp2;
		*indices++ = iFIp1;
		*indices++ = iFIp3;
		*indices++ = iFIp2;
	}
	SetIndexBuffer(newIBuffer);
	newIBuffer->SetNumElements(0);

	Reset();
}
//----------------------------------------------------------------------------
const std::vector<std::string> &RibbonEmitter::GetAllModuleNames_EA () const
{
	return msModuleNames_EA;
}
//----------------------------------------------------------------------------
const std::vector<std::string> &RibbonEmitter::GetAllModuleNames_EO () const
{
	return msModuleNames_EO;
}
//----------------------------------------------------------------------------
void RibbonEmitter::GenBuffers ()
{
	RibbonEmitterController *rbControl =
		(RibbonEmitterController*)GetEffectableController();

	VertexBufferAccessor vba(GetVertexFormat(), GetVertexBuffer());
	assertion(vba.HasPosition(), "Positions must exist\n");

	const std::list<RibbonObject> &ribbons = rbControl->GetRibbonObjects();

	float fixedLength = GetLength();
	if (0.0f == fixedLength)
	{
		fixedLength = 1.0f;
	}

	int vertexIndex = 0;
	int numIndex = 0;
	float lengthAll = 0.0f;
	int numRibbons = (int)ribbons.size();
	int indexRibbon = 0;
	std::list<RibbonObject>::const_iterator it = ribbons.begin();
	for (; it!=ribbons.end(); ++it)
	{
		const APoint &pos = it->Pos;
		const Float3 &color = it->Color;
		float alpha = it->Alpha;
		float size = it->SizeX;
		float halfSize = size/2.0f;

		AVector lastDir;
		if (indexRibbon==0 || indexRibbon==1 || indexRibbon==(numRibbons-1))
			lastDir = it->DVector;
		else
		{
			std::list<RibbonObject>::const_iterator beforeIt = it;
			int beforeIndex = indexRibbon-1;
			beforeIt--;
			std::list<RibbonObject>::const_iterator nextIt = it;
			nextIt++;
			int nextIndex = indexRibbon+1;

			if (beforeIndex>0 && nextIndex<numRibbons)
			{
				lastDir = (beforeIt->DVector + nextIt->DVector)/2.0f;
			}
		}

		AVector up = it->MinusDepthVector.Cross(lastDir);
		up.Normalize();

		Float4 colorAlpha = Float4(color[0], color[1], color[2], alpha);

		float uBegin = 0.0f;
		float uEnd =  1.0f;
		float vBegin =  0.0f;
		float vEnd =  1.0f;

		GetUV(rbControl->StartRandomIndex,
			rbControl->GetPlayedTimeMinusDelay(), uBegin, uEnd, vBegin, vEnd);

		float uRange = uEnd - uBegin;
		float uPercent = lengthAll/fixedLength;
		float u = uBegin + uPercent*uRange;

		vba.Position<Float3>(vertexIndex) = pos + up*halfSize;
		vba.Color<Float4>(0, vertexIndex) = colorAlpha;
		vba.TCoord<Float2>(0, vertexIndex) = Float2(u, vEnd);
		vertexIndex++;

		vba.Position<Float3>(vertexIndex) = pos - up*halfSize;
		vba.Color<Float4>(0, vertexIndex) = colorAlpha;
		vba.TCoord<Float2>(0, vertexIndex) = Float2(u, vBegin);
		vertexIndex++;

		lengthAll += it->Length;

		indexRibbon++;
	}

	// last point
	if (numRibbons > 2)
	{
		--it;

		AVector up = it->MinusDepthVector.Cross(it->DVector);
		up.Normalize();

		const APoint &pos = it->Pos;
		const Float3 &color = it->Color;
		float alpha = it->Alpha;
		const AVector &dir = it->DVector;
		float size = it->SizeX;
		float halfSize = size/2.0f;
		float lastPercent = it->Length/it->LengthOnCompleteOneSegment;

		Float4 colorAlpha = Float4(color[0], color[1], color[2], alpha);

		vba.Position<Float3>(vertexIndex) = pos + halfSize * up +(-lastPercent*dir);
		vba.Color<Float4>(0, vertexIndex) = colorAlpha;
		vba.TCoord<Float2>(0, vertexIndex) = Float2(1.0f, 0.0f);
		vertexIndex++;

		vba.Position<Float3>(vertexIndex) = pos - halfSize * up +(-lastPercent*dir);
		vba.Color<Float4>(0, vertexIndex) = colorAlpha;
		vba.TCoord<Float2>(0, vertexIndex) = Float2(1.0f, 1.0f);
		vertexIndex++;
	}

	if (vertexIndex > mNumMaxVertexs)
	{
		assertion(false, "too many vertexs.\n");
		PX2_LOG_ERROR("RibbonEmitter too many indexs.");
	}

	mVBuffer->SetNumElements(vertexIndex);

	numIndex = ((vertexIndex/2)-1)*6;
	if (numIndex < 0)
		numIndex = 0;
	mIBuffer->SetNumElements(numIndex);

	if (!mIsFixedBound)
	{
		UpdateModelBound();
	}

	Renderer::UpdateAll(mVBuffer);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
RibbonEmitter::RibbonEmitter (LoadConstructor value) :
Effectable(value),
mLength(20.0f),
mSegLength(2.0f),
mNumMaxVertexs(0),
mNumMaxIndexs(0)
{
}
//----------------------------------------------------------------------------
void RibbonEmitter::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Effectable::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mLength);
	source.Read(mSegLength);
	source.Read(mNumMaxVertexs);
	source.Read(mNumMaxIndexs);

	PX2_END_DEBUG_STREAM_LOAD(RibbonEmitter, source);
}
//----------------------------------------------------------------------------
void RibbonEmitter::Link (InStream& source)
{
	Effectable::Link(source);
}
//----------------------------------------------------------------------------
void RibbonEmitter::PostLink ()
{
	Effectable::PostLink();

	GetVertexBuffer()->SetNumElements(0);
	GetIndexBuffer()->SetNumElements(0);
}
//----------------------------------------------------------------------------
bool RibbonEmitter::Register (OutStream& target) const
{
	if (Effectable::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void RibbonEmitter::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Effectable::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mLength);
	target.Write(mSegLength);
	target.Write(mNumMaxVertexs);
	target.Write(mNumMaxIndexs);

	PX2_END_DEBUG_STREAM_SAVE(RibbonEmitter, target);
}
//----------------------------------------------------------------------------
int RibbonEmitter::GetStreamingSize (Stream &stream) const
{
	int size = Effectable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mLength);
	size += sizeof(mSegLength);
	size += sizeof(mNumMaxVertexs);
	size += sizeof(mNumMaxIndexs);

	return size;
}
//----------------------------------------------------------------------------