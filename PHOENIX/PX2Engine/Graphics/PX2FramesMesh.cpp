// PX2FramesMesh.cpp

#include "PX2FramesMesh.hpp"
#include "PX2VBIBManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2TexPackData.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, TriMesh, FramesMesh);
PX2_IMPLEMENT_STREAM(FramesMesh);
PX2_IMPLEMENT_FACTORY(FramesMesh);
PX2_IMPLEMENT_DEFAULT_NAMES(TriMesh, FramesMesh);

//----------------------------------------------------------------------------
FramesMesh::FramesMesh() :
mIsNeedRecal(true),
mNumDir(8),
mAnimPlayedTime(0.0f),
mPlayOnceTime(1.0f),
mIsPlayOnce(false),
mIsPlaying(false)
{
	mModelBound.SetCenter(APoint::ORIGIN);
	mModelBound.SetRadius(3.0f);

	mSize = 100.0f;

	mCurDir = AVector::UNIT_Y;
}
//----------------------------------------------------------------------------
FramesMesh::~FramesMesh()
{
}
//----------------------------------------------------------------------------
void FramesMesh::Play()
{
	mIsPlaying = true;
}
//----------------------------------------------------------------------------
bool FramesMesh::IsPlaying() const
{
	return mIsPlaying;
}
//----------------------------------------------------------------------------
void FramesMesh::Pause()
{
	mIsPlaying = false;
}
//----------------------------------------------------------------------------
void FramesMesh::SetSize(float size)
{
	mSize = size;
	mIsNeedRecal = true;
}
//----------------------------------------------------------------------------
int FramesMesh::SetTexPack(const std::string &texPackFilename)
{
	mTexPackFilename = texPackFilename;

	AddTexPackFun addFun = PX2_GR.msAddTexPackFun;
	GetTexPackFun getFun = PX2_GR.msGetTexPackFun;

	addFun(texPackFilename);
	const TexPack &texPack = getFun(texPackFilename);
	if (!texPack.IsValid()) return 0;

	mIsNeedRecal = true;

	mNumAllFrames = (int)texPack.Elements.size();
	mNumFramesPerDir = mNumAllFrames / mNumDir;

	return mNumFramesPerDir;
}
//----------------------------------------------------------------------------
void FramesMesh::SetPlayOnce(bool once)
{
	mIsPlayOnce = once;
}
//----------------------------------------------------------------------------
void FramesMesh::SetPlayOnceTime(float playOnceTime)
{
	mPlayOnceTime = playOnceTime;
}
//----------------------------------------------------------------------------
void FramesMesh::SetWorldRunDir(const AVector &vec)
{
	mCurDir = vec;
}
//----------------------------------------------------------------------------
void FramesMesh::UpdateWorldData(double applicationTime, double elapsedTime)
{
	TriMesh::UpdateWorldData(applicationTime, elapsedTime);

	if (!IsPlaying()) return;

	mAnimPlayedTime += (float)elapsedTime;

	if (mAnimPlayedTime > mPlayOnceTime)
	{
		if (mIsPlayOnce)
		{
			Pause();
			mAnimPlayedTime = mPlayOnceTime;
		}
		else
		{
			mAnimPlayedTime = 0.0f;
		}
	}

	if (mIsNeedRecal)
	{
		_Cal(mTexPackFilename);
	}

	_CalFrame();
}
//----------------------------------------------------------------------------
void FramesMesh::_Cal(const std::string &texPackFilename)
{
	GetTexPackFun texPackFun = PX2_GR.msGetTexPackFun;
	const TexPack &texPack = texPackFun(texPackFilename);
	if (!texPack.IsValid()) return;

	std::string outPath;
	std::string outBaseFilename;
	StringHelp::SplitFilename(texPackFilename, outPath, outBaseFilename);

	std::string outBaseName;
	std::string outExt;
	StringHelp::SplitBaseFilename(outBaseFilename, outBaseName, outExt);

	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PCT1);
	SetVertexFormat(vf);

	mNumAllFrames = (int)texPack.Elements.size();
	mNumFramesPerDir = mNumAllFrames / mNumDir;
	if (0 == mNumFramesPerDir)
	{
		//assertion(false, "no frames");
		//return;
	}

	VBIBObj &obj = VBIBManager::GetSingleton().GetVBID(texPackFilename);
	if (obj.IsValued)
	{
		SetVertexBuffer(obj.mVB);
		SetIndexBuffer(obj.mIB);
	}
	else
	{
		int numVertex = mNumAllFrames * 4;
		int numIndex = mNumAllFrames * 6;

		int frameIndex = 0;
		VertexBuffer *vb = new0 VertexBuffer(numVertex, vf->GetStride());
		VertexBufferAccessor vba(vf, vb);
		for (int i = 0; i < mNumDir; i++)
		{
			for (int j = 0; j < mNumFramesPerDir; j++)
			{
				std::string eleName = outBaseName + "_" + StringHelp::IntToString(i * 45) + "_" + StringHelp::IntToString(j + 1);

				GetTexPackElementFun texPackElementFun = PX2_GR.msGetTexPackElementFun;
				const TexPackElement &ele = texPackElementFun(texPackFilename, eleName);

				float xPlusPer = 0.0f;
				float widthPer = 0.0f;
				float zPlusPer = 0.0f;
				float heightPer = 0.0f;
				if (0 != ele.OW)
				{
					xPlusPer = (float)ele.OX / (float)ele.OW;
					widthPer = (float)ele.W / (float)ele.OW;
				}
				if (0 != ele.OH)
				{
					zPlusPer = 1.0f - (float)(ele.OY + ele.H) / (float)ele.OH;
					heightPer = (float)ele.H / (float)ele.OH;
				}

				float width = mSize;
				float height = mSize;

				float xPos = 0.0f - width * 0.5f;
				float zPos = 0.0f - height * 0.5f;
				xPos += xPlusPer * width;
				zPos += zPlusPer * height;

				width *= widthPer;
				height *= heightPer;

				Float3 p0 = Float3(xPos, zPos, 0.0f);
				Float3 p1 = Float3(xPos + width, zPos, 0.0f);
				Float3 p2 = Float3(xPos, zPos + height, 0.0f);
				Float3 p3 = Float3(xPos + width, zPos + height, 0.0f);

				float uBegin = (float)ele.X / (float)ele.TexWidth;
				float uEnd = (float)(ele.X + ele.W) / (float)ele.TexWidth;
				float vBegin = (float)(ele.TexHeight - ele.Y - ele.H) / (float)ele.TexHeight;
				float vEnd = (float)(ele.TexHeight - ele.Y) / (float)ele.TexHeight;

				vba.Position<Float3>(frameIndex * 4 + 0) = p0;
				vba.Position<Float3>(frameIndex * 4 + 1) = p1;
				vba.Position<Float3>(frameIndex * 4 + 2) = p2;
				vba.Position<Float3>(frameIndex * 4 + 3) = p3;

				vba.TCoord<Float2>(0, frameIndex * 4 + 0) = Float2(uBegin, vBegin);
				vba.TCoord<Float2>(0, frameIndex * 4 + 1) = Float2(uEnd, vBegin);
				vba.TCoord<Float2>(0, frameIndex * 4 + 2) = Float2(uBegin, vEnd);
				vba.TCoord<Float2>(0, frameIndex * 4 + 3) = Float2(uEnd, vEnd);

				frameIndex++;
			}
		}

		IndexBuffer *ib = new0 IndexBuffer(numIndex, 2);
		for (int i = 0; i < mNumAllFrames; i++)
		{
			unsigned short *indices = (unsigned short*)ib->GetData();
			indices[i * 6 + 0] = (uint16_t)(i * 4 + 0);
			indices[i * 6 + 1] = (uint16_t)(i * 4 + 1);
			indices[i * 6 + 2] = (uint16_t)(i * 4 + 2);
			indices[i * 6 + 3] = (uint16_t)(i * 4 + 1);
			indices[i * 6 + 4] = (uint16_t)(i * 4 + 3);
			indices[i * 6 + 5] = (uint16_t)(i * 4 + 2);
		}

		SetVertexBuffer(vb);
		SetIndexBuffer(ib);

		VBIBManager::GetSingleton().AddVBIB(texPackFilename, vb, ib);
	}

	ObjectLoadFun loadFun = PX2_GR.msUserLoadFun;
	Texture2D *tex = (Texture2D*)loadFun(texPack.ImageFullPath.c_str());

	MaterialInstance *mi = new0 MaterialInstance(
		"Data/engine_mtls/tex2d/tex2d.px2obj", "tex2d", false);
	mi->SetPixelTexture(0, "SampleBase", tex);
	SetMaterialInstance(mi);

	mi->GetMaterial()->GetCullProperty(0, 0)->Enabled = false;

	mIsNeedRecal = false;
}
//----------------------------------------------------------------------------
int FramesMesh::_GetDirIndex(const AVector &dir)
{
	AVector animVec = dir;
	animVec.Normalize();

	AVector up = AVector::UNIT_Y;
	float dotVal = up.Dot(animVec);

	float rad = Mathf::ACos(dotVal);
	float degree = Mathf::RAD_TO_DEG * rad;
	bool isRight = (dir.X() > 0.0f);

	if (0 <= degree && degree < 22.5f)
	{
		return 0;
	}
	else if (22.5f <= degree && degree < 67.5f)
	{
		if (isRight)
			return 1;
		else
			return 7;
	}
	else if (67.5 <= degree && degree < 112.5f)
	{
		if (isRight)
			return 2;
		else
			return 6;
	}
	else if (112.5f <= degree && degree < 157.5f)
	{
		if (isRight)
			return 3;
		else
			return 5;
	}
	else if (157.5f <= degree && degree <= 180.0f)
	{
		return 4;
	}

	return 0;
}
//----------------------------------------------------------------------------
void FramesMesh::_CalFrame()
{
	IndexBuffer *ib = GetIndexBuffer();
	if (!ib)
		return;

	int index = _GetDirIndex(mCurDir);

	float interval = mPlayOnceTime / mNumFramesPerDir;

	int index1 = (int)(mAnimPlayedTime / interval);
	if (index1 >= mNumFramesPerDir)
		index1 = mNumFramesPerDir - 1;

	int frameIndex = index * mNumFramesPerDir + index1;

	SetIBOffset(6 * frameIndex);
	SetIBNumElements(6);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
FramesMesh::FramesMesh(LoadConstructor value) :
TriMesh(value),
mIsNeedRecal(true),
mNumDir(8),
mAnimPlayedTime(0.0f),
mPlayOnceTime(1.0f),
mIsPlayOnce(false),
mIsPlaying(false)
{
}
//----------------------------------------------------------------------------
void FramesMesh::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	TriMesh::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(FramesMesh, source);
}
//----------------------------------------------------------------------------
void FramesMesh::Link(InStream& source)
{
	TriMesh::Link(source);
}
//----------------------------------------------------------------------------
void FramesMesh::PostLink()
{
	TriMesh::PostLink();
}
//----------------------------------------------------------------------------
bool FramesMesh::Register(OutStream& target) const
{
	if (TriMesh::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void FramesMesh::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	TriMesh::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(FramesMesh, target);
}
//----------------------------------------------------------------------------
int FramesMesh::GetStreamingSize(Stream &stream) const
{
	int size = TriMesh::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------