// PX2MaterialTechnique.cpp

#include "PX2MaterialTechnique.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, MaterialTechnique);
PX2_IMPLEMENT_STREAM(MaterialTechnique);
PX2_IMPLEMENT_FACTORY(MaterialTechnique);

//----------------------------------------------------------------------------
MaterialTechnique::MaterialTechnique ()
{
	SetName("MT");
}
//----------------------------------------------------------------------------
MaterialTechnique::~MaterialTechnique ()
{
}
//----------------------------------------------------------------------------
void MaterialTechnique::InsertPass (MaterialPass* pass)
{
	if (pass)
	{
		mPasses.push_back(pass);
	}
	else
	{
		assertion(false, "Input to InsertPass must be nonnull.\n");
	}
}
//----------------------------------------------------------------------------
void MaterialTechnique::PopLastPass ()
{
	mPasses.pop_back();
}
//----------------------------------------------------------------------------
MaterialPass* MaterialTechnique::GetPass (int passIndex) const
{
	if (0 <= passIndex && passIndex < (int)mPasses.size())
	{
		return mPasses[passIndex];
	}

	assertion(false, "Invalid index in GetPass.\n");
	return 0;
}
//----------------------------------------------------------------------------
VertexShader* MaterialTechnique::GetVertexShader (int passIndex) const
{
	if (0 <= passIndex && passIndex < (int)mPasses.size())
	{
		return mPasses[passIndex]->GetVertexShader();
	}

	assertion(false, "Invalid index in GetVertexShader.\n");
	return 0;
}
//----------------------------------------------------------------------------
PixelShader* MaterialTechnique::GetPixelShader (int passIndex) const
{
	if (0 <= passIndex && passIndex < (int)mPasses.size())
	{
		return mPasses[passIndex]->GetPixelShader();
	}

	assertion(false, "Invalid index in GetPixelShader.\n");
	return 0;
}
//----------------------------------------------------------------------------
AlphaProperty* MaterialTechnique::GetAlphaProperty (int passIndex) const
{
	if (0 <= passIndex && passIndex < (int)mPasses.size())
	{
		return mPasses[passIndex]->GetAlphaProperty();
	}

	assertion(false, "Invalid index in GetAlphaProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------
CullProperty* MaterialTechnique::GetCullProperty (int passIndex) const
{
	if (0 <= passIndex && passIndex < (int)mPasses.size())
	{
		return mPasses[passIndex]->GetCullProperty();
	}

	assertion(false, "Invalid index in GetCullProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------
DepthProperty* MaterialTechnique::GetDepthProperty (int passIndex) const
{
	if (0 <= passIndex && passIndex < (int)mPasses.size())
	{
		return mPasses[passIndex]->GetDepthProperty();
	}

	assertion(false, "Invalid index in GetDepthProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------
OffsetProperty* MaterialTechnique::GetOffsetProperty (int passIndex) const
{
	if (0 <= passIndex && passIndex < (int)mPasses.size())
	{
		return mPasses[passIndex]->GetOffsetProperty();
	}

	assertion(false, "Invalid index in GetOffsetProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------
StencilProperty* MaterialTechnique::GetStencilProperty (int passIndex) const
{
	if (0 <= passIndex && passIndex < (int)mPasses.size())
	{
		return mPasses[passIndex]->GetStencilProperty();
	}

	assertion(false, "Invalid index in GetStencilProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------
WireProperty* MaterialTechnique::GetWireProperty (int passIndex) const
{
	if (0 <= passIndex && passIndex < (int)mPasses.size())
	{
		return mPasses[passIndex]->GetWireProperty();
	}

	assertion(false, "Invalid index in GetWireProperty.\n");
	return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void MaterialTechnique::RegistProperties ()
{
	Object::RegistProperties();

	AddPropertyClass("MaterialTechnique");
	AddProperty("NumPasses", PT_INT, GetNumPasses(), false);
}
//----------------------------------------------------------------------------
void MaterialTechnique::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* MaterialTechnique::GetObjectByName (const std::string& name)
{
	Object* found = Object::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	std::vector<MaterialPassPtr>::iterator iter = mPasses.begin();
	std::vector<MaterialPassPtr>::iterator end = mPasses.end();
	for (/**/; iter != end; ++iter)
	{
		PX2_GET_OBJECT_BY_NAME((*iter), name, found);
	}

	return 0;
}
//----------------------------------------------------------------------------
void MaterialTechnique::GetAllObjectsByName (const std::string& name,
										   std::vector<Object*>& objects)
{
	Object::GetAllObjectsByName(name, objects);

	std::vector<MaterialPassPtr>::iterator iter = mPasses.begin();
	std::vector<MaterialPassPtr>::iterator end = mPasses.end();
	for (/**/; iter != end; ++iter)
	{
		PX2_GET_ALL_OBJECTS_BY_NAME((*iter), name, objects);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
MaterialTechnique::MaterialTechnique (LoadConstructor value)
:
Object(value)
{
}
//----------------------------------------------------------------------------
void MaterialTechnique::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	int numPasses;
	source.Read(numPasses);
	mPasses.resize(numPasses);
	source.ReadPointerVV(numPasses, &mPasses[0]);

	PX2_END_DEBUG_STREAM_LOAD(MaterialTechnique, source);
}
//----------------------------------------------------------------------------
void MaterialTechnique::Link (InStream& source)
{
	Object::Link(source);

	std::vector<MaterialPassPtr>::iterator iter = mPasses.begin();
	std::vector<MaterialPassPtr>::iterator end = mPasses.end();
	for (/**/; iter != end; ++iter)
	{
		source.ResolveLink(*iter);
	}
}
//----------------------------------------------------------------------------
void MaterialTechnique::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool MaterialTechnique::Register (OutStream& target) const
{
	if (Object::Register(target))
	{
		std::vector<MaterialPassPtr>::const_iterator iter = mPasses.begin();
		std::vector<MaterialPassPtr>::const_iterator end = mPasses.end();
		for (/**/; iter != end; ++iter)
		{
			target.Register(*iter);
		}
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void MaterialTechnique::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointerW((int)mPasses.size(), &mPasses[0]);

	PX2_END_DEBUG_STREAM_SAVE(MaterialTechnique, target);
}
//----------------------------------------------------------------------------
int MaterialTechnique::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	int numPasses = (int)mPasses.size();
	size += sizeof(numPasses);
	size += numPasses*PX2_POINTERSIZE(mPasses[0]);
	return size;
}
//----------------------------------------------------------------------------
