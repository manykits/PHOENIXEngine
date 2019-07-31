// PX2Terrain.cpp

#include "PX2Terrain.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, Terrain);
PX2_IMPLEMENT_STREAM(Terrain);
PX2_IMPLEMENT_ABSTRACT_FACTORY(Terrain);

//----------------------------------------------------------------------------
Terrain::~Terrain ()
{
}
//----------------------------------------------------------------------------
Terrain::Terrain () :
mRowFrom(0),
mRowTo(0),
mColFrom(0),
mColTo(0),
mNumVertexPage(0),
mMinElevation(0),
mMaxElevation(0),
mSpacing(1.0f),
mJunglerFrequency(1.0f),
mJunglerStrength(1.0f)
{
	mTerrainShine = new0 Shine();
	mTerrainShine->Emissive = Float4(0.0f, 0.0f, 0.0f, 0.0f);
	mTerrainShine->Ambient = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	mTerrainShine->Diffuse = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	mTerrainShine->Specular = Float4(1.0f, 1.0f, 1.0f, 1.0f);

	mVFormatEdit = VertexFormat::Create(3,
		VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0,
		VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0,
		VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
}
//----------------------------------------------------------------------------
void Terrain::OnChildDetach(Movable* child)
{
	Node::OnChildDetach(child);

	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		if (it->second == child)
		{
			mPages.erase(it);
		}
	}
}
//----------------------------------------------------------------------------
TerrainPage *Terrain::GetPage (int row, int col)
{
	std::pair<int, int> key(row, col);
	auto it = mPages.find(key);
	if (it != mPages.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
TerrainPage *Terrain::GetCurrentPage (float x, float y)
{
	float invLength = 1.0f/(mSpacing*(float)(mNumVertexPage - 1));

	int col = (int)Mathf::Floor(x*invLength);
	int row = (int)Mathf::Floor(y*invLength);

	return GetPage(row, col);
}
//----------------------------------------------------------------------------
bool Terrain::GetPageIndex (int &outRow, int &outCol, TerrainPage *page)
{
	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		if (it->second == page)
		{
			outRow = it->first.first;
			outCol = it->first.second;

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
TerrainPagePtr Terrain::ReplacePage (int row, int col, TerrainPage* newPage)
{
	std::pair<int, int> key(row, col);
	auto it = mPages.find(key);
	if (it != mPages.end())
	{
		it->second = newPage;
	}

	assertion(false, "Invalid row or column index\n");
	return 0;
}
//----------------------------------------------------------------------------
void Terrain::UpdateHoles ()
{
	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		if (it->second)
		{
			it->second->UpdateHoles();
		}
	}
}
//----------------------------------------------------------------------------
float Terrain::GetHeight (float x, float y)
{
	TerrainPage* page = GetCurrentPage(x, y);
	if (page)
	{
		x -= page->LocalTransform.GetTranslate().X();
		y -= page->LocalTransform.GetTranslate().Y();

		return page->GetHeight(x, y);
	}

	return 0.0f;
}
//----------------------------------------------------------------------------
AVector Terrain::GetNormal (float x, float y)
{
	float xp = x + mSpacing;
	float xm = x - mSpacing;
	float yp = y + mSpacing;
	float ym = y - mSpacing;

	float hpz = 0.0f;
	TerrainPage* page = GetCurrentPage(xp, y);
	if (page)
	{
		float xtmp = xp - page->LocalTransform.GetTranslate().X();
		float ytmp = y - page->LocalTransform.GetTranslate().Y();
		hpz = page->GetHeight(xtmp, ytmp);
	}

	float hmz = 0.0f;
	page = GetCurrentPage(xm, y);
	if (page)
	{
		float xtmp = xm - page->LocalTransform.GetTranslate().X();
		float ytmp = y - page->LocalTransform.GetTranslate().Y();
		hmz = page->GetHeight(xtmp, ytmp);
	}

	float hzp = 0.0f;
	page = GetCurrentPage(x, yp);
	if (page)
	{
		float xtmp = x - page->LocalTransform.GetTranslate().X();
		float ytmp = yp - page->LocalTransform.GetTranslate().Y();
		hzp = page->GetHeight(xtmp, ytmp);
	}

	float hzm = 0.0f;
	page = GetCurrentPage(x, ym);
	if (page)
	{
		float xtmp = x - page->LocalTransform.GetTranslate().X();
		float ytmp = ym - page->LocalTransform.GetTranslate().Y();
		hzm = page->GetHeight(xtmp, ytmp);
	}

	AVector normal(hmz - hpz, hzm - hzp, 1.0f);
	normal.Normalize();
	return normal;
}
//----------------------------------------------------------------------------
void Terrain::AddJunglers (Texture2D *tex, APoint center, float radius,
	int num, float width, float height, float lower)
{
	if (!tex)
		return;

	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		TerrainPage *page = it->second;
		if (page)
		{
			page->mAddingJObjs.clear();
		}
	}

	for (int i=0; i<num; i++)
	{
		float unitRadius = Mathf::UnitRandom();
		float fX0 = Mathf::SymmetricRandom();
		float fY0 = Mathf::SymmetricRandom();
		AVector normal0(fX0, fY0, 0.0f);
		normal0.Normalize();

		float fX1 = Mathf::SymmetricRandom();
		float fY1 = Mathf::SymmetricRandom();
		AVector normal1(fX1, fY1, 0.0f);
		normal1.Normalize();

		APoint pos = center + normal0*unitRadius*radius;
		pos.Z() = GetHeight(pos.X(), pos.Y()) - lower;

		JObj obj;
		obj.Pos = pos;
		obj.Normal = normal1;
		obj.Width = width;
		obj.Height = height;

		TerrainPage *page = GetCurrentPage(obj.Pos.X(), obj.Pos.Y());
		if (page)
		{
			page->mAddingJObjs.push_back(obj);
		}
	}

	auto it1 = mPages.begin();
	for (; it1 != mPages.end(); it1++)
	{
		TerrainPage *page = it1->second;
		if (page)
		{
			page->AddJunglers(tex, page->mAddingJObjs);
		}
	}
}
//----------------------------------------------------------------------------
void Terrain::RemoveJunglers (Texture2D *tex, APoint center, float radius,
	int num)
{
	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		TerrainPage *page = it->second;
		if (page)
		{
			page->RemoveJunglers(tex, center, radius, num);
		}
	}
}
//----------------------------------------------------------------------------
void Terrain::SetJunglerFrequency (float fre)
{
	mJunglerFrequency = fre;

	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		TerrainPage *page = it->second;
		if (page)
		{
			page->SetJunglerFrequency(fre);
		}
	}
}
//----------------------------------------------------------------------------
void Terrain::SetJunglerStrength (float strength)
{
	mJunglerStrength = strength;

	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		TerrainPage *page = it->second;
		if (page)
		{
			page->SetJunglerStrength(strength);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Terrain::RegistProperties ()
{
	Node::RegistProperties();

	AddPropertyClass("Terrain");

	AddProperty("NumRow", Object::PT_INT, GetRowQuantity(), true);
	AddProperty("NumCol", Object::PT_INT, GetColQuantity(), true);
	AddProperty("NumVertexPage", Object::PT_INT, GetNumVertexPage(), false);
	AddProperty("Spacing", Object::PT_FLOAT, GetSpacing(), false);

	Float3 emissive = Float3(mTerrainShine->Emissive[0], mTerrainShine->Emissive[1], mTerrainShine->Emissive[2]);
	Float3 ambient = Float3(mTerrainShine->Ambient[0], mTerrainShine->Ambient[1], mTerrainShine->Ambient[2]);
	Float3 diffuse = Float3(mTerrainShine->Diffuse[0], mTerrainShine->Diffuse[1], mTerrainShine->Diffuse[2]);
	Float3 specular = Float3(mTerrainShine->Specular[0], mTerrainShine->Specular[1], mTerrainShine->Specular[2]);
	float specularPow = mTerrainShine->Specular[3];

	AddProperty("S_Emissive", Object::PT_COLOR3FLOAT3, emissive);
	AddProperty("S_Ambient", Object::PT_COLOR3FLOAT3, ambient);
	AddProperty("S_Diffuse", Object::PT_COLOR3FLOAT3, diffuse);
	AddProperty("S_Specular", Object::PT_COLOR3FLOAT3, specular);
	AddProperty("S_SpecularPow", Object::PT_FLOAT, specularPow);
}
//----------------------------------------------------------------------------
void Terrain::OnPropertyChanged (const PropertyObject &obj)
{
	Node::OnPropertyChanged(obj);

	if ("S_Emissive" == obj.Name)
	{
		Float3 val = PX2_ANY_AS(obj.Data, Float3);
		mTerrainShine->Emissive =Float4(val[0], val[1], val[2], mTerrainShine->Emissive[3]);
	}
	else if ("S_Ambient" == obj.Name)
	{
		Float3 val = PX2_ANY_AS(obj.Data, Float3);
		mTerrainShine->Ambient =Float4(val[0], val[1], val[2], mTerrainShine->Ambient[3]);
	}
	else if ("S_Diffuse" == obj.Name)
	{
		Float3 val = PX2_ANY_AS(obj.Data, Float3);
		mTerrainShine->Diffuse =Float4(val[0], val[1], val[2], mTerrainShine->Diffuse[3]);
	}
	else if ("S_Specular" == obj.Name)
	{
		Float3 val = PX2_ANY_AS(obj.Data, Float3);
		mTerrainShine->Specular =Float4(val[0], val[1], val[2], mTerrainShine->Specular[3]);
	}
	else if ("S_SpecularPow" == obj.Name)
	{
		float pow = PX2_ANY_AS(obj.Data, float);
		mTerrainShine->Specular =Float4(mTerrainShine->Specular[0], mTerrainShine->Specular[1], 
			mTerrainShine->Specular[2], pow);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* Terrain::GetObjectByName (const std::string& name)
{
	Object* found = Node::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	return 0;
}
//----------------------------------------------------------------------------
void Terrain::GetAllObjectsByName (const std::string& name,
	std::vector<Object*>& objects)
{
	Node::GetAllObjectsByName(name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Terrain::Terrain (LoadConstructor value) :
Node(value),
mRowFrom(0),
mRowTo(0),
mColFrom(0),
mColTo(0),
mNumVertexPage(0),
mMinElevation(0.0f),
mMaxElevation(0.0f),
mSpacing(0.0f),
mJunglerFrequency(1.0f),
mJunglerStrength(1.0f)
{
}
//----------------------------------------------------------------------------
void Terrain::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mRowFrom);
	source.Read(mRowTo);
	source.Read(mColFrom);
	source.Read(mColTo);
	source.Read(mNumVertexPage);
	source.Read(mMinElevation);
	source.Read(mMaxElevation);
	source.Read(mSpacing);

	int numPages = 0;
	source.Read(numPages);
	for (int i = 0; i < numPages; i++)
	{
		int x, y;
		source.Read(x);
		source.Read(y);
		TerrainPage *tp = 0;
		source.ReadPointer(tp);
		mPages[std::pair<int, int>(x, y)] = tp;
	}

	source.ReadPointer(mVFormatEdit);
	source.ReadPointer(mTerrainShine);
	source.Read(mJunglerFrequency);
	source.Read(mJunglerStrength);

	PX2_END_DEBUG_STREAM_LOAD(Terrain, source);
}
//----------------------------------------------------------------------------
void Terrain::Link (InStream& source)
{
	Node::Link(source);

	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		source.ResolveLink(it->second);
	}

	source.ResolveLink(mVFormatEdit);
	source.ResolveLink(mTerrainShine);
}
//----------------------------------------------------------------------------
void Terrain::PostLink ()
{
	Node::PostLink();
}
//----------------------------------------------------------------------------
bool Terrain::Register (OutStream& target) const
{
	if (Node::Register(target))
	{
		auto it = mPages.begin();
		for (; it != mPages.end(); it++)
		{
			target.Register(it->second);
		}

		target.Register(mVFormatEdit);
		target.Register(mTerrainShine);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void Terrain::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mRowFrom);
	target.Write(mRowTo);
	target.Write(mColFrom);
	target.Write(mColTo);
	target.Write(mNumVertexPage);
	target.Write(mMinElevation);
	target.Write(mMaxElevation);
	target.Write(mSpacing);

	int numPages = mPages.size();
	target.Write(numPages);
	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		int row = it->first.first;
		int col = it->first.second;
		target.Write(row);
		target.Write(col);
		target.WritePointer(it->second);
	}

	target.WritePointer(mVFormatEdit);
	target.WritePointer(mTerrainShine);
	target.Write(mJunglerFrequency);
	target.Write(mJunglerStrength);

	PX2_END_DEBUG_STREAM_SAVE(Terrain, target);
}
//----------------------------------------------------------------------------
int Terrain::GetStreamingSize (Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mRowFrom);
	size += sizeof(mRowTo);
	size += sizeof(mColFrom);
	size += sizeof(mColTo);
	size += sizeof(mNumVertexPage);
	size += sizeof(mMinElevation);
	size += sizeof(mMaxElevation);
	size += sizeof(mSpacing);
	int numPages = mPages.size();
	size += sizeof(numPages);

	auto it = mPages.begin();
	for (; it != mPages.end(); it++)
	{
		int row = it->first.first;
		int col = it->first.second;
		size += sizeof(row);
		size += sizeof(col);
		size += PX2_POINTERSIZE(it->second);
	}

	size += PX2_POINTERSIZE(mVFormatEdit);
	size += PX2_POINTERSIZE(mTerrainShine);
	size += sizeof(mJunglerFrequency);
	size += sizeof(mJunglerStrength);

	return size;
}
//----------------------------------------------------------------------------
