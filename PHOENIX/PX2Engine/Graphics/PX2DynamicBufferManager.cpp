// PX2DynamicBufferManager.hpp

#include "PX2DynamicBufferManager.hpp"
#include "PX2Renderer.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
DynamicBufferManager::DynamicBufferManager ()
	:
mCurVBOffset(0),
mCurIBOffset(0),
mIsInited(false)
{
	mNumMaxVertex = 1024;
	mNumMaxIndex = 1024;
}
//----------------------------------------------------------------------------
DynamicBufferManager::~DynamicBufferManager ()
{
	Terminate ();
}
//----------------------------------------------------------------------------
bool DynamicBufferManager::Initlize ()
{
	VertexFormat *vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PCT1);

	mVB = new0 VertexBuffer(mNumMaxVertex, vf->GetStride(), 
		Buffer::BU_DYNAMIC);
	
	mIB = new0 IndexBuffer(mNumMaxIndex, 2, Buffer::BU_DYNAMIC);

	mIsInited = true;

	return true;
}
//----------------------------------------------------------------------------
bool DynamicBufferManager::Terminate ()
{
	Reset();

	for (int i=0; i<(int)mFreeIBs.size(); i++)
	{
		delete0(mFreeIBs[i]);
	}
	mFreeIBs.clear();

	for (int i=0; i<(int)mFreeVBs.size(); i++)
	{
		delete0(mFreeVBs[i]);
	}
	mFreeVBs.clear();

	mVB = 0;
	mIB = 0;

	mIsInited = false;

	return true;
}
//----------------------------------------------------------------------------
void DynamicBufferManager::Reset ()
{
	for (int i=0; i<(int)mUsedVBs.size(); i++)
	{
		mFreeVBs.push_back(mUsedVBs[i]);
	}
	mUsedVBs.resize(0);

	for (int i=0; i<(int)mUsedIBs.size(); i++)
	{
		mFreeIBs.push_back(mUsedIBs[i]);
	}
	mUsedIBs.resize(0);

	mCurVBOffset = 0;
	mCurIBOffset = 0;
}
//----------------------------------------------------------------------------
void DynamicBufferManager::Update ()
{
	//if (mIsInited)
	//{
	//	Renderer::UpdateAll(mVB);
	//	Renderer::UpdateAll(mIB);
	//}
}
//----------------------------------------------------------------------------
DBObject_V *DynamicBufferManager::AllocVertexBuffer (int numVertexs)
{
	if (mFreeVBs.empty())
	{
		DBObject_V *v = new0 DBObject_V();
		mFreeVBs.push_back(v);
	}

	DBObject_V *objV = mFreeVBs.back();
	mFreeVBs.pop_back();

	objV->Buf = mVB;
	objV->Offset = mCurVBOffset;
	objV->Num = numVertexs;

	mCurVBOffset += numVertexs;

	if (mCurVBOffset >= mNumMaxVertex)
	{
		assertion(false, "mCurVBOffset >= mNumMaxVertex");
	}

	mUsedVBs.push_back(objV);

	return objV;
}
//----------------------------------------------------------------------------
DBObject_I *DynamicBufferManager::AllocIndexBuffer (int numIndexs)
{
	if (mFreeIBs.empty())
	{
		DBObject_I *i = new0 DBObject_I();
		mFreeIBs.push_back(i);
	}

	DBObject_I *objI = mFreeIBs.back();
	mFreeIBs.pop_back();

	objI->Buf = mIB;
	objI->Offset = mCurIBOffset;
	objI->Num = numIndexs;

	mCurIBOffset += numIndexs;

	if (mCurIBOffset >= mNumMaxIndex)
	{
		assertion(false, "mCurIBOffset >= mNumMaxIndex");
	}

	mUsedIBs.push_back(objI);

	return objI;
}
//----------------------------------------------------------------------------