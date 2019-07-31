// PX2RawTerrain.cpp

#include "PX2RawTerrain.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Terrain, RawTerrain);
PX2_IMPLEMENT_STREAM(RawTerrain);
PX2_IMPLEMENT_FACTORY(RawTerrain);
PX2_IMPLEMENT_DEFAULT_NAMES(Terrain, RawTerrain)

//----------------------------------------------------------------------------
RawTerrain::RawTerrain ()
{
	SetName("RawTerrain");
}
//----------------------------------------------------------------------------
RawTerrain::~RawTerrain ()
{
}
//----------------------------------------------------------------------------
void RawTerrain::SetRowFromTo(int from, int to)
{
	mRowFrom = from;
	mRowTo = to;
}
//----------------------------------------------------------------------------
void RawTerrain::SetColFromTo(int from, int to)
{
	mColFrom = from;
	mColTo = to;
}
//----------------------------------------------------------------------------
void RawTerrain::SetSize (int size)
{
	mNumVertexPage = size;
}
//----------------------------------------------------------------------------
void RawTerrain::SetSpacing (float spacing)
{
	mSpacing = spacing;
}
//----------------------------------------------------------------------------
void RawTerrain::AllocateRawTerrainPages ()
{
	int row, col;
	for (row = mRowFrom; row < mRowTo; ++row)
	{
		for (col = mColFrom; col < mColTo; ++col)
		{
			CreatePage(row, col);
		}
	}

	int numRows = mRowTo - mRowFrom;
	int numCols = mColTo - mColFrom;
	mChild.resize(numRows*numCols);
	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		TerrainPage *tp = it->second;
		if (tp)
		{
			AttachChild(tp);
		}
	}
}
//----------------------------------------------------------------------------
RawTerrainPage *RawTerrain::CreatePage (int row, int col)
{
	std::pair<int, int> key(row, col);
	auto it = mPages.find(key);
	if (it != mPages.end())
	{
		DetachChild(it->second);
	}

	int numHeights = mNumVertexPage*mNumVertexPage;
	float *heights = new1<float>(numHeights);
	memset(heights, 0, numHeights*sizeof(float));

	float length = mSpacing*(float)(mNumVertexPage - 1);
	Float2 origin(col*length, row*length);

	RawTerrainPage* page = new0 RawTerrainPage(mVFormatEdit, mNumVertexPage, heights, origin, mSpacing);
	page->SetShine(mTerrainShine);

	page->SetName("[" + StringHelp::IntToString(row) + "]" + "[" + StringHelp::IntToString(col) + "]");
	mPages[std::pair<int, int>(row, col)] = page;

	return page;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void RawTerrain::RegistProperties ()
{
	Terrain::RegistProperties();

	AddPropertyClass("RawTerrain");
}
//----------------------------------------------------------------------------
void RawTerrain::OnPropertyChanged (const PropertyObject &obj)
{
	Terrain::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
RawTerrain::RawTerrain (LoadConstructor value) :
Terrain(value)
{
}
//----------------------------------------------------------------------------
void RawTerrain::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Terrain::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(RawTerrain, source);
}
//----------------------------------------------------------------------------
void RawTerrain::Link (InStream& source)
{
	Terrain::Link(source);
}
//----------------------------------------------------------------------------
void RawTerrain::PostLink ()
{
	Terrain::PostLink();
}
//----------------------------------------------------------------------------
bool RawTerrain::Register (OutStream& target) const
{
	if (Terrain::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void RawTerrain::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Terrain::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(RawTerrain, target);
}
//----------------------------------------------------------------------------
int RawTerrain::GetStreamingSize (Stream &stream) const
{
	int size = Terrain::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
