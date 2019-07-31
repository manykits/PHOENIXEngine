// PX2TerrainPage.cpp

#include "PX2TerrainPage.hpp"
#include "PX2Culler.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, TriMesh, TerrainPage);
PX2_IMPLEMENT_STREAM(TerrainPage);
PX2_IMPLEMENT_FACTORY(TerrainPage);
PX2_IMPLEMENT_DEFAULT_NAMES(TriMesh, TerrainPage);

//----------------------------------------------------------------------------
TerrainPage::TerrainPage(int numVertexPage, float* heights, const Float2& origin,
	float spacing) :
	mNumVertexPage(numVertexPage),
	mNumVertexPageM1(numVertexPage - 1),
	mHeights(heights),
	mOrigin(origin),
	mSpacing(spacing)
{
	// numVertexPage = 2^p + 1, p <= 7
	assertion(numVertexPage ==  3 || numVertexPage ==  5 
		|| numVertexPage == 9 || numVertexPage == 17
		|| numVertexPage == 33 || numVertexPage == 65 
		|| numVertexPage == 129, "Invalid page numVertexPage\n");

	mInvSpacing = 1.0f/mSpacing;

	SetRenderLayer(Renderable::RL_TERRAIN);
}
//----------------------------------------------------------------------------
TerrainPage::~TerrainPage ()
{
	delete1(mHeights);
}
//----------------------------------------------------------------------------
float TerrainPage::GetHeight (float x, float y) const
{
	float xGrid = (x - mOrigin[0])*mInvSpacing;
	if (xGrid < 0.0f || xGrid >= (float)mNumVertexPageM1)
	{
		// 方位在地形页中
		return 0.0f;
	}

	float yGrid = (y - mOrigin[1])*mInvSpacing;
	if (yGrid < 0.0f || yGrid >= (float)mNumVertexPageM1)
	{
		// 方位不再地形页中
		return 0.0f;
	}

	float fCol = Mathf::Floor(xGrid);
	int iCol = (int)fCol;
	float fRow = Mathf::Floor(yGrid);
	int iRow = (int)fRow;

	int index = iCol + mNumVertexPage*iRow;
	float dx = xGrid - fCol;
	float dy = yGrid - fRow;
	float h00, h10, h01, h11, height;

	if ((iCol & 1) == (iRow & 1))
	{
		float diff = dx - dy;
		h00 = mHeights[index];
		h11 = mHeights[index + 1 + mNumVertexPage];
		if (diff > 0.0f)
		{
			h10 = mHeights[index + 1];
			height = (1.0f - diff - dy)*h00 + diff*h10 + dy*h11;
		}
		else
		{
			h01 = mHeights[index + mNumVertexPage];
			height = (1.0f + diff - dx)*h00 - diff*h01 + dx*h11;
		}
	}
	else
	{
		float sum = dx + dy;
		h10 = mHeights[index + 1];
		h01 = mHeights[index + mNumVertexPage];
		if (sum <= 1.0f)
		{
			h00 = mHeights[index];
			height = (1.0f - sum)*h00 + dx*h10 + dy*h01;
		}
		else
		{
			h11 = mHeights[index + 1 + mNumVertexPage];
			height = (sum - 1.0f)*h11 + (1.0f - dy)*h10 + (1.0f - dx)*h01;
		}
	}

	return height;
}
//----------------------------------------------------------------------------
void TerrainPage::ClearHoles ()
{
	mHoleIndexs.clear();
}
//----------------------------------------------------------------------------
void TerrainPage::AddHole (int index)
{
	mHoleIndexs.push_back(index);
}
//----------------------------------------------------------------------------
void TerrainPage::RemoveHole (int index)
{
	std::vector<int>::iterator it = mHoleIndexs.begin();
	for (; it!=mHoleIndexs.end(); it++)
	{
		int idx = *it;

		if (idx == index)
		{
			mHoleIndexs.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
void TerrainPage::UpdateHoles ()
{
}
//----------------------------------------------------------------------------
void TerrainPage::SetJunglerFrequency (float fre)
{
	for (int i=0; i<(int)mJunglers.size(); i++)
	{
		mJunglers[i]->SetJunglerFrequency(fre);
	}
}
//----------------------------------------------------------------------------
void TerrainPage::SetJunglerStrength (float strength)
{
	for (int i=0; i<(int)mJunglers.size(); i++)
	{
		mJunglers[i]->SetJunglerStrength(strength);
	}
}
//----------------------------------------------------------------------------
void TerrainPage::AddJunglers (Texture2D *tex, std::vector<JObj> objs)
{
	if (0 == (int)objs.size())
		return;

	JunglerPtr junglerP = mJunglersMap[tex];
	Jungler *jungler = junglerP;

	if (jungler)
	{
		jungler->Add(objs);
	}
	else
	{
		jungler = new0 Jungler(tex, 1000, Jungler::JT_PLANE);
		jungler->Add(objs);

		mJunglers.push_back(jungler);
		mJunglersMap[tex] = jungler;
	}
}
//----------------------------------------------------------------------------
void TerrainPage::RemoveJunglers (Texture2D *tex, APoint center, float radius, 
	int num)
{
	if (0 == (int)mJunglers.size())
		return;

	if (0 == tex)
	{
		std::vector<JunglerPtr>::iterator it = mJunglers.begin();
		for (; it!=mJunglers.end(); it++)
		{
			RemoveJunglerPoints((*it), center, radius, num);
		}
	}
	else
	{
		JunglerPtr junglerP = mJunglersMap[tex];
		Jungler *jungler = junglerP;

		if (jungler)
		{
			RemoveJunglerPoints(jungler, center, radius, num);
		}
	}

	std::vector<JunglerPtr>::iterator it = mJunglers.begin();
	for (; it!=mJunglers.end();)
	{
		if (0==(*it)->GetNum())
		{
			mJunglersMap.erase((*it)->GetTexture());
			it = mJunglers.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
void TerrainPage::RemoveJunglerPoints (Jungler *jungler, APoint center,
	float radius, int num)
{
	if (!jungler)
		return;

	// 范围内个数
	std::vector<int> indexs;
	for (int i=0; i<jungler->GetNum(); i++)
	{
		const APoint &pos = jungler->GetPos(i);
		AVector dir = pos - center;
		if (dir.Length() < radius)
		{
			indexs.push_back(i);
		}
	}

	std::vector<int> indexsRemoves;
	for (int i=0; i<(int)indexs.size(); i++)
	{
		float fRand = (float)num/(float)indexsRemoves.size();
		float fR = Mathf::IntervalRandom(0.0f, 1.0f);

		if (fR <= fRand)
		{
			indexsRemoves.push_back(indexs[i]);
		}
	}

	jungler->Remove(indexsRemoves);
}
//----------------------------------------------------------------------------
void TerrainPage::OnGetVisibleSet (Culler& culler, bool noCull)
{
	TriMesh::OnGetVisibleSet(culler, noCull);

	for (int i=0; i<(int)mJunglers.size(); i++)
	{
		culler.Insert(mJunglers[i]);
	}
}
//----------------------------------------------------------------------------
bool TerrainPage::IsHole (int index)
{
	std::vector<int>::iterator it = std::find(mHoleIndexs.begin(), 
		mHoleIndexs.end(), index);
	if (it != mHoleIndexs.end())
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void TerrainPage::RegistProperties ()
{
	TriMesh::RegistProperties();

	AddPropertyClass("TerrainPage");

	AddProperty("Size", Object::PT_INT, GetSize(), false);
	AddProperty("SizeM1", Object::PT_INT, mNumVertexPageM1, false);
	AddProperty("Spacing", Object::PT_FLOAT, mSpacing, false);
	AddProperty("Origin", Object::PT_FLOAT2, mOrigin, false);
	AddProperty("PageWidth", Object::PT_FLOAT, GetWidth(), false);
}
//----------------------------------------------------------------------------
void TerrainPage::OnPropertyChanged (const PropertyObject &obj)
{
	TriMesh::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
TerrainPage::TerrainPage (LoadConstructor value)
	:
TriMesh(value),
	mNumVertexPage(0),
	mNumVertexPageM1(0),
	mHeights(0),
	mOrigin(Float2(0.0f, 0.0f)),
	mSpacing(0.0f),
	mInvSpacing(0.0f)
{
}
//----------------------------------------------------------------------------
void TerrainPage::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	TriMesh::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mNumVertexPage);
	int numVertices = mNumVertexPage*mNumVertexPage;
	source.ReadVR(numVertices, mHeights);
	source.ReadAggregate(mOrigin);
	source.Read(mSpacing);

	int numHoles = 0;
	source.Read(numHoles);
	if (numHoles > 0)
	{
		mHoleIndexs.resize(numHoles);
		source.ReadVV(numHoles, &mHoleIndexs[0]);
	}

	mNumVertexPageM1 = mNumVertexPage - 1;
	mInvSpacing = 1.0f/mSpacing;

	int numJunglers = 0;
	source.Read(numJunglers);
	if (numJunglers > 0)
	{
		mJunglers.resize(numJunglers);
		source.ReadPointerVV(numJunglers, &mJunglers[0]);
	}

	PX2_END_DEBUG_STREAM_LOAD(TerrainPage, source);
}
//----------------------------------------------------------------------------
void TerrainPage::Link (InStream& source)
{
	TriMesh::Link(source);

	mJunglersMap.clear();
	for (int i=0; i<(int)mJunglers.size(); i++)
	{
		source.ResolveLink(mJunglers[i]);
		mJunglersMap.insert(std::pair<Texture2D*, JunglerPtr>(
			mJunglers[i]->GetTexture(), mJunglers[i]));
	}
}
//----------------------------------------------------------------------------
void TerrainPage::PostLink ()
{
	TriMesh::PostLink();
}
//----------------------------------------------------------------------------
bool TerrainPage::Register (OutStream& target) const
{
	if (TriMesh::Register(target))
	{
		for (int i=0; i<(int)mJunglers.size(); i++)
		{
			target.Register(mJunglers[i]);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TerrainPage::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	TriMesh::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mNumVertexPage);
	int numVertices = mNumVertexPage*mNumVertexPage;
	target.WriteN(numVertices, mHeights);
	target.WriteAggregate(mOrigin);
	target.Write(mSpacing);
	int numHoles = (int)mHoleIndexs.size();
	target.Write(numHoles);
	if (numHoles > 0)
	{
		target.WriteN<int>(numHoles, &mHoleIndexs[0]);
	}

	int numJunglers = (int)mJunglers.size();
	target.Write(numJunglers);
	for (int i=0; i<numJunglers; i++)
	{
		target.WritePointer(mJunglers[i]);
	}

	PX2_END_DEBUG_STREAM_SAVE(TerrainPage, target);
}
//----------------------------------------------------------------------------
int TerrainPage::GetStreamingSize (Stream &stream) const
{
	int size = TriMesh::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mNumVertexPage);
	size += mNumVertexPage*mNumVertexPage*sizeof(mHeights[0]);
	size += sizeof(mOrigin);
	size += sizeof(mSpacing);

	int numHoles = (int)mHoleIndexs.size();
	size += sizeof(numHoles);
	if (numHoles > 0)
	{
		size += sizeof(int)*numHoles;
	}

	int numJuns = (int)mJunglers.size();
	size += sizeof(numJuns);
	if (numJuns > 0)
	{
		size += numJuns * PX2_POINTERSIZE(mJunglers[0]);
	}

	return size;
}
//----------------------------------------------------------------------------
