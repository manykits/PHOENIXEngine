// PX2Jungler.cpp

#include "PX2Jungler.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

int Jungler::msJunVertxNum[Jungler::JT_MAX_TYPE] = 
{4, 8};
int Jungler::msJunIndexNum[Jungler::JT_MAX_TYPE] = 
{6, 12};

PX2_IMPLEMENT_RTTI(PX2, TriMesh, Jungler);
PX2_IMPLEMENT_STREAM(Jungler);
PX2_IMPLEMENT_FACTORY(Jungler);
PX2_IMPLEMENT_DEFAULT_NAMES(TriMesh, Jungler);
//----------------------------------------------------------------------------
Jungler::Jungler (Texture2D *tex, int maxNum, JunglerType type)
	:
mTexture(tex),
mMaxNum(maxNum),
mJunglerType(type)
{
	VertexFormat *vf = VertexFormat::Create(3,
		VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
		VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
		VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
	SetVertexFormat(vf);

	int vNum = mMaxNum * msJunVertxNum[mJunglerType];
	int iNum = mMaxNum * msJunIndexNum[mJunglerType];

	VertexBuffer *vBuffer = new0 VertexBuffer(vNum, mVFormat->GetStride());
	IndexBuffer *iBuffer = new0 IndexBuffer(iNum, 2);
	vBuffer->SetNumElements((int)mPoses.size()*4);
	iBuffer->SetNumElements((int)mPoses.size()*6);

	SetVertexBuffer(vBuffer);
	SetIndexBuffer(iBuffer);
}
//----------------------------------------------------------------------------
Jungler::~Jungler ()
{
}
//----------------------------------------------------------------------------
void Jungler::Add (std::vector<JObj> objs)
{
	int hasNum = (int)mPoses.size();
	int toAddNum = (int)objs.size();
	if ((hasNum+toAddNum) > mMaxNum)
	{
		toAddNum = mMaxNum - hasNum;
	}

	for (int i=0; i<toAddNum; i++)
	{
		mPoses.push_back(objs[i].Pos);
		mNormals.push_back(objs[i].Normal);
		mWidths.push_back(objs[i].Width);
		mHeights.push_back(objs[i].Height);
	}

	ReGenerate();
}
//----------------------------------------------------------------------------
void Jungler::Remove (std::vector<int> indexs)
{
	int ind = 0;
	std::vector<APoint>::iterator it = mPoses.begin();
	std::vector<AVector>::iterator it1 = mNormals.begin();
	std::vector<float>::iterator it2 = mWidths.begin();
	std::vector<float>::iterator it3 = mHeights.begin();
	for (; it!=mPoses.end();)
	{
		if (std::find(indexs.begin(), indexs.end(), ind) != indexs.end())
		{
			it = mPoses.erase(it);
			it1 = mNormals.erase(it1);
			it2 = mWidths.erase(it2);
			it3 = mHeights.erase(it3);
		}
		else
		{
			it++;
			it1++;
			it2++;
			it3++;
		}

		ind++;
	}

	ReGenerate();
}
//----------------------------------------------------------------------------
void Jungler::SetJunglerFrequency (float fre)
{
	if (mMtlInst)
	{
		ShaderFloat *sf = mMtlInst->GetVertexConstant(0, "gUser");
		if (sf)
		{
			(*sf)[1] = fre;
		}
	}
}
//----------------------------------------------------------------------------
void Jungler::SetJunglerStrength (float strength)
{
	if (mMtlInst)
	{
		ShaderFloat *sf = mMtlInst->GetVertexConstant(0, "gUser");
		if (sf)
		{
			(*sf)[2] = strength;
		}
	}
}
//----------------------------------------------------------------------------
void Jungler::ReGenerate ()
{
	VertexBufferAccessor vba(GetVertexFormat(), GetVertexBuffer());
	int numPoses = (int)mPoses.size();

	for (int i = 0; i < numPoses; ++i)
	{
		APoint pos = mPoses[i];
		AVector normal = mNormals[i];
		float width = mWidths[i];
		float halfWidth = width/2.0f;
		float height = mHeights[i];

		AVector dir0 = AVector::UNIT_Z.Cross(normal);
		dir0.Normalize();

		if (JT_PLANE == mJunglerType)
		{
			vba.Position<Float3>(4*i) = pos - dir0*halfWidth;
			vba.Position<Float3>(4*i+1) = pos + dir0*halfWidth;
			vba.Position<Float3>(4*i+2) = pos + dir0*halfWidth
				+ AVector::UNIT_Z*height;
			vba.Position<Float3>(4*i+3) = pos - dir0*halfWidth 
				+ AVector::UNIT_Z*height;

			vba.Normal<Float3>(4*i) = normal;
			vba.Normal<Float3>(4*i+1) = normal;
			vba.Normal<Float3>(4*i+2) = normal;
			vba.Normal<Float3>(4*i+3) = normal;

			vba.TCoord<Float2>(0, 4*i) = Float2(0.0f, 0.0f);
			vba.TCoord<Float2>(0, 4*i+1) = Float2(1.0f, 0.0f);
			vba.TCoord<Float2>(0, 4*i+2) = Float2(1.0f, 1.0f);
			vba.TCoord<Float2>(0, 4*i+3) = Float2(0.0f, 1.0f);
		}
		else if (JT_CROSS == mJunglerType)
		{

		}
	}

	unsigned short* indices = (unsigned short*)GetIndexBuffer()->GetData();
	for (int i = 0; i < numPoses; ++i)
	{
		if (JT_PLANE == mJunglerType)
		{
			int v0 = i*4;
			int v1 = v0+1;
			int v2 = v0+3;

			int v3 = v0+2;
			int v4 = v0+3;
			int v5 = v0+1;

			*indices++ = (unsigned short)v0;
			*indices++ = (unsigned short)v1;
			*indices++ = (unsigned short)v2;
			*indices++ = (unsigned short)v3;
			*indices++ = (unsigned short)v4;
			*indices++ = (unsigned short)v5;
		}
		else if (JT_CROSS == mJunglerType)
		{

		}
	}

	GetVertexBuffer()->SetNumElements((int)mPoses.size()
		* msJunVertxNum[mJunglerType]);
	GetIndexBuffer()->SetNumElements((int)mPoses.size()
		* msJunIndexNum[mJunglerType]);

	Renderer::UpdateAll(GetVertexBuffer());
	Renderer::UpdateAll(GetIndexBuffer());
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
Jungler::Jungler (LoadConstructor value)
	:
TriMesh(value)
{
}
//----------------------------------------------------------------------------
void Jungler::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	TriMesh::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mJunglerType);
	source.Read(mMaxNum);
	source.ReadPointer(mMtlInst);
	source.ReadPointer(mTexture);
	int num = 0;
	source.Read(num);
	if (num > 0)
	{
		mPoses.resize(num);
		mNormals.resize(num);
		mWidths.resize(num);
		mHeights.resize(num);

		source.ReadAggregateVV(num, &mPoses[0]);
		source.ReadAggregateVV(num, &mNormals[0]);
		source.ReadVV(num, &mWidths[0]);
		source.ReadVV(num, &mHeights[0]);
	}

	PX2_END_DEBUG_STREAM_LOAD(Jungler, source);
}
//----------------------------------------------------------------------------
void Jungler::Link (InStream& source)
{
	TriMesh::Link(source);

	source.ResolveLink(mMtlInst);
	source.ResolveLink(mTexture);
}
//----------------------------------------------------------------------------
void Jungler::PostLink ()
{
	TriMesh::PostLink();
}
//----------------------------------------------------------------------------
bool Jungler::Register (OutStream& target) const
{
	if (TriMesh::Register(target))
	{
		target.Register(mMtlInst);
		target.Register(mTexture);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void Jungler::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	TriMesh::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mJunglerType);
	target.Write(mMaxNum);
	target.WritePointer(mMtlInst);
	target.WritePointer(mTexture);
	int num = (int)mPoses.size();
	target.Write<int>(num);
	if (num > 0)
	{
		target.WriteAggregateN<APoint>(num, &mPoses[0]);
		target.WriteAggregateN<AVector>(num, &mNormals[0]);
		target.WriteN<float>(num, &mWidths[0]);
		target.WriteN<float>(num, &mHeights[0]);
	}

	PX2_END_DEBUG_STREAM_SAVE(Jungler, target);
}
//----------------------------------------------------------------------------
int Jungler::GetStreamingSize (Stream &stream) const
{
	int size = TriMesh::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_ENUMSIZE(mJunglerType);
	size += sizeof(mMaxNum);
	size += PX2_POINTERSIZE(mMtlInst);
	size += PX2_POINTERSIZE(mTexture);
	int num = (int)mPoses.size();
	size += sizeof(num); // num
	size += num*sizeof(APoint);
	size += num*sizeof(AVector);
	size += num*sizeof(float);
	size += num*sizeof(float);

	return size;
}
//----------------------------------------------------------------------------
