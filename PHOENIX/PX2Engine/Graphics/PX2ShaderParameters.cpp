// PX2ShaderParameters.cpp

#include "PX2ShaderParameters.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Texture2D.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, ShaderParameters);
PX2_IMPLEMENT_STREAM(ShaderParameters);
PX2_IMPLEMENT_FACTORY(ShaderParameters);

//----------------------------------------------------------------------------
ShaderParameters::ShaderParameters (Shader* shader)
:
mShader(shader)
{
	assertion(shader != 0, "Shader must be specified.\n");

	mNumConstants = mShader->GetNumConstants();
	if (mNumConstants > 0)
	{
		mConstants = new1<ShaderFloatPtr>(mNumConstants);
	}
	else
	{
		mConstants = 0;
	}

	mNumTextures = mShader->GetNumSamplers();
	if (mNumTextures > 0)
	{
		mTextureResPath.resize(mNumTextures);
		mTextures = new1<TexturePtr>(mNumTextures);
	}
	else
	{
		mTextures = 0;
	}
}
//----------------------------------------------------------------------------
ShaderParameters::~ShaderParameters ()
{
	delete1(mConstants);
	delete1(mTextures);
}
//----------------------------------------------------------------------------
int ShaderParameters::SetConstant (const std::string& name,
								   ShaderFloat* sfloat)
{
	for (int i = 0; i < mNumConstants; ++i)
	{
		if (mShader->GetConstantName(i) == name)
		{
			mConstants[i] = sfloat;

			return i;
		}
	}

	assertion(false, "Cannot find constant:%s\n", name.c_str());
	return -1;
}
//----------------------------------------------------------------------------
int ShaderParameters::SetTexture (const std::string& name, Texture* texture)
{
	for (int i = 0; i < mNumTextures; ++i)
	{
		if (mShader->GetSamplerName(i) == name)
		{
			mTextures[i] = texture;
			return i;
		}
	}

	//assertion(false, "Cannot find texture:%s\n", name.c_str());
	return -1;
}
//----------------------------------------------------------------------------
int ShaderParameters::SetTexture(const std::string& name, 
	const std::string &filename)
{
	Texture2D *tex = DynamicCast<Texture2D>(
		GraphicsRoot::msUserLoadFun(filename.c_str()));

	return _SetTexture(name, tex);
}
//----------------------------------------------------------------------------
int ShaderParameters::_SetTexture(const std::string& name, Texture* texture)
{
	for (int i = 0; i < mNumTextures; ++i)
	{
		if (mShader->GetSamplerName(i) == name)
		{
			mTextures[i] = texture;
			return i;
		}
	}

	return -1;
}
//----------------------------------------------------------------------------
ShaderFloat* ShaderParameters::GetConstant (const std::string& name) const
{
	for (int i = 0; i < mNumConstants; ++i)
	{
		if (mShader->GetConstantName(i) == name)
		{
			return mConstants[i];
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
const std::string &ShaderParameters::GetConstantName(int handle) const
{
	return mShader->GetConstantName(handle);
}
//----------------------------------------------------------------------------
Texture* ShaderParameters::GetTexture (const std::string& name) const
{
	for (int i = 0; i < mNumTextures; ++i)
	{
		if (mShader->GetSamplerName(i) == name)
		{
			return mTextures[i];
		}
	}

	assertion(false, "Cannot find texture.\n");
	return 0;
}
//----------------------------------------------------------------------------
ShaderFloat* ShaderParameters::GetConstant (int handle) const
{
	if (0 <= handle && handle < mNumConstants)
	{
		return mConstants[handle];
	}

	assertion(false, "Invalid constant handle.\n");
	return 0;
}
//----------------------------------------------------------------------------
Texture* ShaderParameters::GetTexture (int handle) const
{
	if (0 <= handle && handle < mNumTextures)
	{
		return mTextures[handle];
	}

	assertion(false, "Invalid texture handle.\n");
	return 0;
}
//----------------------------------------------------------------------------
const std::string &ShaderParameters::GetTextureResPath(int handle) const
{
	return mTextureResPath[handle];
}
//----------------------------------------------------------------------------
const std::string &ShaderParameters::GetSampleName(int handle) const
{
	return mShader->GetSamplerName(handle);
}
//----------------------------------------------------------------------------
void ShaderParameters::UpdateConstants(const ShaderStruct *shaderStruc)
{
	ShaderFloatPtr* constants = mConstants;
	for (int i = 0; i < mNumConstants; ++i, ++constants)
	{
		ShaderFloat* constant = *constants;
		if (constant && constant->AllowUpdater())
		{
			constant->Update(shaderStruc);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void ShaderParameters::RegistProperties ()
{
	Object::RegistProperties();
	AddPropertyClass("ShaderParameters");

	AddProperty("NumConstants", PT_INT, GetNumConstants(), false);
	for (int i=0; i<GetNumConstants(); i++)
	{
		ShaderFloat *sf = GetConstant(i);
		if (sf)
		{
			std::string name = mShader->GetConstantName(i);
			AddProperty("ShaderFloat", Object::PT_STRING, name, false);
		}
	}

	AddProperty("NumTextures", PT_INT, GetNumTextures(), false);
	for (int i=0; i<GetNumTextures(); i++)
	{
		Texture *tex = GetTexture(i);
		if (tex)
		{
			std::string iStr = StringHelp::IntToString(i);
			AddProperty("Texture_Name"+iStr, PT_STRING, tex->GetName(), false);
			AddProperty("Texture_Path"+iStr, PT_STRINGBUTTON, tex->GetResourcePath());
		}
	}
}
//----------------------------------------------------------------------------
void ShaderParameters::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if (Object::PT_STRINGBUTTON == obj.Type)
	{
		const std::string name = obj.Name;
		std::string path = PX2_ANY_AS(obj.Data, std::string);

		std::string key = "Texture_Path";
		size_t pos = name.find(key);	
		size_t len = key.length();
		size_t leftLen = name.length() - len;
		if (pos != std::string::npos && GraphicsRoot::msUserLoadFun)
		{
			std::string iStr = name.substr(len, leftLen);
			int i = atoi(iStr.c_str());

			mTextureResPath[i] = path;
			mTextures[i] = DynamicCast<Texture2D>(GraphicsRoot::msUserLoadFun(path.c_str()));
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* ShaderParameters::GetObjectByName (const std::string& name)
{
	Object* found = Object::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	PX2_GET_OBJECT_BY_NAME(mShader, name, found);

	int i;
	for (i = 0; i < mNumConstants; ++i)
	{
		PX2_GET_OBJECT_BY_NAME(mConstants[i], name, found);
	}

	for (i = 0; i < mNumTextures; ++i)
	{
		PX2_GET_OBJECT_BY_NAME(mTextures[i], name, found);
	}

	return 0;
}
//----------------------------------------------------------------------------
void ShaderParameters::GetAllObjectsByName (const std::string& name,
											std::vector<Object*>& objects)
{
	Object::GetAllObjectsByName(name, objects);

	PX2_GET_ALL_OBJECTS_BY_NAME(mShader, name, objects);

	int i;
	for (i = 0; i < mNumConstants; ++i)
	{
		PX2_GET_ALL_OBJECTS_BY_NAME(mConstants[i], name, objects);
	}

	for (i = 0; i < mNumTextures; ++i)
	{
		PX2_GET_ALL_OBJECTS_BY_NAME(mTextures[i], name, objects);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
ShaderParameters::ShaderParameters (LoadConstructor value)
:
Object(value),
mNumConstants(0),
mConstants(0),
mNumTextures(0),
mTextures(0)
{
}
//----------------------------------------------------------------------------
void ShaderParameters::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mShader);
	source.ReadPointerRR(mNumConstants, mConstants);

	source.Read(mNumTextures);

	if (mNumTextures > 0 )
	{
		mTextures = new1<TexturePtr>(mNumTextures);
		mTextureResPath.resize(mNumTextures);

		for (int i=0; i<mNumTextures; i++)
		{
			std::string resPath;
			source.ReadString(resPath);
			mTextureResPath[i] = resPath;

			if (GraphicsRoot::sTerResPath == resPath)
			{
				source.ReadPointer(mTextures[i]);
			}
		}
	}

	PX2_END_DEBUG_STREAM_LOAD(ShaderParameters, source);
}
//----------------------------------------------------------------------------
void ShaderParameters::Link (InStream& source)
{
	Object::Link(source);

	source.ResolveLink(mShader);
	source.ResolveLink(mNumConstants, mConstants);

	assertion(0!=GraphicsRoot::msUserLoadFun, "msUserLoadFun must not be 0.\n");

	for (int i=0; i<mNumTextures; i++)
	{
		std::string resPath = mTextureResPath[i];

		if (!resPath.empty())
		{
			if (GraphicsRoot::sEmptyResPath != resPath)
			{
				if (GraphicsRoot::sTerResPath == resPath)
				{
					source.ResolveLink(mTextures[i]);
				}
				else
				{
					mTextures[i] = DynamicCast<Texture>
						(GraphicsRoot::msUserLoadFun(resPath.c_str()));
				}
			}
			else
			{
				/*_*/
			}
		}
		else
		{
			mTextures[i] = 0;
		}
	}
}
//----------------------------------------------------------------------------
void ShaderParameters::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool ShaderParameters::Register (OutStream& target) const
{
	if (Object::Register(target))
	{
		target.Register(mShader);

		target.Register(mNumConstants, mConstants);

		for (int i=0; i<mNumTextures; i++)
		{
			if (mTextures[i])
			{
				const std::string &resPath = mTextures[i]->GetResourcePath();
				if (!resPath.empty())
				{
					if (GraphicsRoot::sTerResPath == resPath)
					{
						target.Register(mTextures[i]);
					}
				}
				else
				{
					/*_*/
				}
			}
		}

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void ShaderParameters::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mShader);
	target.WritePointerW(mNumConstants, mConstants);

	target.Write(mNumTextures);
	for (int i=0; i<mNumTextures; i++)
	{
		if (mTextures[i])
		{
			std::string resPath = mTextures[i]->GetResourcePath();
			if (!resPath.empty())
			{
				target.WriteString(resPath);

				if (resPath == GraphicsRoot::sTerResPath)
				{
					target.WritePointer(mTextures[i]);
				}
			}
			else
			{
				target.WriteString(GraphicsRoot::sEmptyResPath);
			}
		}
		else
		{
			target.WriteString("");
		}
	}

	PX2_END_DEBUG_STREAM_SAVE(ShaderParameters, target);
}
//----------------------------------------------------------------------------
int ShaderParameters::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_POINTERSIZE(mShader);
	size += sizeof(mNumConstants);
	size += mNumConstants*PX2_POINTERSIZE(mConstants[0]);
	size += sizeof(mNumTextures);

	if (Stream::ST_IN == stream.GetStreamType())
	{
		for (int i=0; i<mNumTextures; i++)
		{
			size += PX2_STRINGSIZE(mTextureResPath[i]);

			if (mTextureResPath[i] == GraphicsRoot::sTerResPath)
			{
				size += PX2_POINTERSIZE(mTextures[i]);
			}
		}
	}
	else
	{
		for (int i=0; i<mNumTextures; i++)
		{
			if (mTextures[i])
			{
				std::string resPath = mTextures[i]->GetResourcePath();
				if (!resPath.empty())
				{
					size += PX2_STRINGSIZE(resPath);

					if (resPath == GraphicsRoot::sTerResPath)
					{
						size += PX2_POINTERSIZE(mTextures[i]);
					}
				}
				else
				{
					size += PX2_STRINGSIZE(GraphicsRoot::sEmptyResPath);
				}
			}
			else 
			{
				size += PX2_STRINGSIZE("");
			}
		}
	}

	return size;
}
//----------------------------------------------------------------------------