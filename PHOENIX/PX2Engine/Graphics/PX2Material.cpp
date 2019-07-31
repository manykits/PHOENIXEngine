// PX2Material.cpp

#include "PX2Material.hpp"
#include "PX2Renderable.hpp"
#include "PX2MaterialInstance.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, Material);
PX2_IMPLEMENT_STREAM(Material);
PX2_IMPLEMENT_FACTORY(Material);

//----------------------------------------------------------------------------
Material::Material ()
{
}
//----------------------------------------------------------------------------
Material::~Material ()
{
}
//----------------------------------------------------------------------------
void Material::InsertTechnique (MaterialTechnique* technique)
{
	if (technique)
	{
		mTechniques.push_back(technique);
	}
	else
	{
		assertion(false, "Input to InsertTechnique must be nonnull.\n");
	}
}
//----------------------------------------------------------------------------
MaterialTechnique* Material::GetTechnique (int techniqueIndex) const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex];
	}

	assertion(false, "Invalid index in GetTechnique.\n");
	return 0;
}
//----------------------------------------------------------------------------
MaterialTechnique* Material::GetTechnique(const std::string &techniqueName,
	int &index) const
{
	for (int i = 0; i < (int)mTechniques.size(); i++)
	{
		if (techniqueName == mTechniques[i]->GetName())
		{
			index = i;
			return mTechniques[i];
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
int Material::GetNumPasses (int techniqueIndex) const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex]->GetNumPasses();
	}

	assertion(false, "Invalid index in GetNumPasses.\n");
	return 0;
}
//----------------------------------------------------------------------------
MaterialPass* Material::GetPass (int techniqueIndex, int passIndex) const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex]->GetPass(passIndex);
	}

	assertion(false, "Invalid index in GetPass.\n");
	return 0;
}
//----------------------------------------------------------------------------
VertexShader* Material::GetVertexShader (int techniqueIndex,
											 int passIndex) const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex]->GetVertexShader(passIndex);
	}

	assertion(false, "Invalid index in GetVertexShader.\n");
	return 0;
}
//----------------------------------------------------------------------------
PixelShader* Material::GetPixelShader (int techniqueIndex,
										   int passIndex) const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex]->GetPixelShader(passIndex);
	}

	assertion(false, "Invalid index in GetPixelShader.\n");
	return 0;
}
//----------------------------------------------------------------------------
AlphaProperty* Material::GetAlphaProperty (int techniqueIndex,
										 int passIndex) const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex]->GetAlphaProperty(passIndex);
	}

	assertion(false, "Invalid index in GetAlphaProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------
CullProperty* Material::GetCullProperty (int techniqueIndex,
									   int passIndex) const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex]->GetCullProperty(passIndex);
	}

	assertion(false, "Invalid index in GetCullProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------
DepthProperty* Material::GetDepthProperty (int techniqueIndex,
										 int passIndex) const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex]->GetDepthProperty(passIndex);
	}

	assertion(false, "Invalid index in GetDepthProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------
OffsetProperty* Material::GetOffsetProperty (int techniqueIndex,
										   int passIndex) const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex]->GetOffsetProperty(passIndex);
	}

	assertion(false, "Invalid index in GetOffsetProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------
StencilProperty* Material::GetStencilProperty (int techniqueIndex,
											 int passIndex) const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex]->GetStencilProperty(passIndex);
	}

	assertion(false, "Invalid index in GetStencilProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------
WireProperty* Material::GetWireProperty (int techniqueIndex, int passIndex)
const
{
	if (0 <= techniqueIndex && techniqueIndex < (int)mTechniques.size())
	{
		return mTechniques[techniqueIndex]->GetWireProperty(passIndex);
	}

	assertion(false, "Invalid index in GetWireProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Material::RegistProperties ()
{
	Object::RegistProperties();

	AddPropertyClass("Material");

	int numTech = GetNumTechniques();
	AddProperty("NumTechniques", Object::PT_INT, numTech, false);
}
//----------------------------------------------------------------------------
void Material::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Ãû³Æ
//----------------------------------------------------------------------------
Object* Material::GetObjectByName (const std::string& name)
{
	Object* found = Object::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	std::vector<MaterialTechniquePtr>::iterator iter = mTechniques.begin();
	std::vector<MaterialTechniquePtr>::iterator end = mTechniques.end();
	for (/**/; iter != end; ++iter)
	{
		PX2_GET_OBJECT_BY_NAME((*iter), name, found);
	}

	return 0;
}
//----------------------------------------------------------------------------
void Material::GetAllObjectsByName (const std::string& name,
										std::vector<Object*>& objects)
{
	Object::GetAllObjectsByName(name, objects);

	std::vector<MaterialTechniquePtr>::iterator iter = mTechniques.begin();
	std::vector<MaterialTechniquePtr>::iterator end = mTechniques.end();
	for (/**/; iter != end; ++iter)
	{
		PX2_GET_ALL_OBJECTS_BY_NAME((*iter), name, objects);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
Material::Material (LoadConstructor value)
:
Object(value)
{
}
//----------------------------------------------------------------------------
void Material::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	int numTechniques;
	source.Read(numTechniques);
	mTechniques.resize(numTechniques);
	source.ReadPointerVV(numTechniques, &mTechniques[0]);

	PX2_END_DEBUG_STREAM_LOAD(Material, source);
}
//----------------------------------------------------------------------------
void Material::Link (InStream& source)
{
	Object::Link(source);

	std::vector<MaterialTechniquePtr>::iterator iter = mTechniques.begin();
	std::vector<MaterialTechniquePtr>::iterator end = mTechniques.end();
	for (/**/; iter != end; ++iter)
	{
		source.ResolveLink(*iter);
	}
}
//----------------------------------------------------------------------------
void Material::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool Material::Register (OutStream& target) const
{
	if (Object::Register(target))
	{
		std::vector<MaterialTechniquePtr>::const_iterator iter =
			mTechniques.begin();
		std::vector<MaterialTechniquePtr>::const_iterator end = mTechniques.end();
		for (/**/; iter != end; ++iter)
		{
			target.Register(*iter);
		}
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void Material::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointerW((int)mTechniques.size(), &mTechniques[0]);

	PX2_END_DEBUG_STREAM_SAVE(Material, target);
}
//----------------------------------------------------------------------------
int Material::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	int numTechniques = (int)mTechniques.size();
	size += sizeof(numTechniques);
	size += numTechniques*PX2_POINTERSIZE(mTechniques[0]);

	return size;
}
//----------------------------------------------------------------------------