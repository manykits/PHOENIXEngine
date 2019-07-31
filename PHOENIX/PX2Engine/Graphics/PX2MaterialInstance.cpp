// PX2MaterialInstance.cpp

#include "PX2MaterialInstance.hpp"
#include "PX2StringHelp.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2MaterialManager.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2Log.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, MaterialInstance);
PX2_IMPLEMENT_STREAM(MaterialInstance);
PX2_IMPLEMENT_FACTORY(MaterialInstance);

//----------------------------------------------------------------------------
MaterialInstance::MaterialInstance (const Material* material,
											int techniqueIndex)
											:
mMaterial((Material*)material),  // conceptual constness
mIsShareMtl(false),
mTechniqueIndex(techniqueIndex),
mNumPasses(0),
mVertexParameters(0),
mPixelParameters(0),
mIsNeedUpdate(false)
{
	assertion(material != 0, "Material must be specified.\n");
	assertion(
		0 <= techniqueIndex && techniqueIndex < material->GetNumTechniques(),
		"Invalid technique index.\n");

	MaterialTechnique* technique = mMaterial->GetTechnique(mTechniqueIndex);
	mNumPasses = technique->GetNumPasses();
	mVertexParameters = new1<ShaderParametersPtr>(mNumPasses);
	mPixelParameters = new1<ShaderParametersPtr>(mNumPasses);
	int p;
	for (p = 0; p < mNumPasses; ++p)
	{
		MaterialPass* pass = technique->GetPass(p);
		mVertexParameters[p] =
			new0 ShaderParameters(pass->GetVertexShader());
		mVertexParameters[p]->SetName("VParams_" + StringHelp::IntToString(p));
		mPixelParameters[p] =
			new0 ShaderParameters(pass->GetPixelShader());
		mPixelParameters[p]->SetName("PParams_" + StringHelp::IntToString(p));
	}
}
//----------------------------------------------------------------------------
MaterialInstance::MaterialInstance(const std::string &mtlFilename, 
	const std::string &techName, bool shareMtl) :
	mIsShareMtl(shareMtl),
	mTechniqueIndex(0),
	mMaterialFilename(mtlFilename),
	mInstanceTechName(techName),
	mNumPasses(0),
	mVertexParameters(0),
	mPixelParameters(0),
	mIsNeedUpdate(false)
{
	mMaterialFilenameFString = mMaterialFilename.c_str();
	mInstanceTechNameFString = mInstanceTechName.c_str();

	MaterialTechnique *tech = _RefreshMaterial(mMaterialFilename, mInstanceTechName,
		mVertexParameters, mPixelParameters, mIsShareMtl, mMaterial, mTechniqueIndex);

	mNumPasses = tech->GetNumPasses();
}
//----------------------------------------------------------------------------
bool MaterialInstance::operator == (const MaterialInstance& otherMI) const
{
	if (this == &otherMI)
		return true;

	// fast return
	if (mTechniqueIndex != otherMI.GetTechniqueIndex())
		return false;

	if (mMaterialFilenameFString != otherMI.mMaterialFilenameFString)
		return false;

	if (mInstanceTechNameFString != otherMI.mInstanceTechNameFString)
		return false;

	ShaderParameters *pixelParams = GetPixelParameters(0);
	ShaderParameters *otherPixelParams = otherMI.GetPixelParameters(0);
	int numPixelParams = pixelParams->GetNumConstants();
	for (int i = 0; i < numPixelParams; i++)
	{
		ShaderFloat *sf = pixelParams->GetConstant(i);
		ShaderFloat *otherSF = otherPixelParams->GetConstant(i);
		if (0 != memcmp(sf->GetData(), otherSF->GetData(), sf->GetNumRegisters() * 4))
		{
			return false;
		}
	}
	int numTextures = pixelParams->GetNumTextures();
	for (int i = 0; i < numTextures; i++)
	{
		Texture *tex = pixelParams->GetTexture(i);
		Texture *otherTex = otherPixelParams->GetTexture(i);
		if (tex != otherTex)
			return false;

		if (tex->GetResourcePathFString() != otherTex->GetResourcePathFString())
			return false;
	}

	Material *thisMtl = GetMaterial();
	PX2_UNUSED(thisMtl);
	Material *otherMtl = otherMI.GetMaterial();
	PX2_UNUSED(otherMtl);

	for (int i = 0; i < GetNumPasses(); i++)
	{
		const MaterialPass *thisMtlPass = GetPass(i);
		const MaterialPass *otherMtlPass = otherMI.GetPass(i);

		if (!AlphaProperty::IsEuqal(thisMtlPass->GetAlphaProperty(),
			otherMtlPass->GetAlphaProperty()))
			return false;

		if (!CullProperty::IsEuqal(thisMtlPass->GetCullProperty(),
			otherMtlPass->GetCullProperty()))
			return false;

		if (!DepthProperty::IsEuqal(thisMtlPass->GetDepthProperty(),
			otherMtlPass->GetDepthProperty()))
			return false;

		if (!OffsetProperty::IsEuqal(thisMtlPass->GetOffsetProperty(),
			otherMtlPass->GetOffsetProperty()))
			return false;

		if (!StencilProperty::IsEuqal(thisMtlPass->GetStencilProperty(),
			otherMtlPass->GetStencilProperty()))
			return false;

		if (!WireProperty::IsEuqal(thisMtlPass->GetWireProperty(),
			otherMtlPass->GetWireProperty()))
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------
void MaterialInstance::SetUseMaterial(const std::string &mtlName,
	const std::string &techName)
{
	mMaterialFilename = mtlName;
	mMaterialFilenameFString = mMaterialFilename.c_str();
	mInstanceTechName = techName;
	mInstanceTechNameFString = mInstanceTechName.c_str();

	mIsNeedUpdate = true;
}
//----------------------------------------------------------------------------
void MaterialInstance::SetUseMaterialTechnique(const std::string &techName)
{
	SetUseMaterial(mMaterialFilename, techName);
}
//----------------------------------------------------------------------------
void MaterialInstance::_SetMaterialFilename(const std::string &filename)
{
	mMaterialFilename = filename;
	mMaterialFilenameFString = mMaterialFilename.c_str();
}
//----------------------------------------------------------------------------
MaterialTechnique *MaterialInstance::_RefreshMaterial(
	const std::string &mtlFilename,
	const std::string &intanceTechName, ShaderParametersPtr* &vp,
	ShaderParametersPtr* &pp, bool shareMtl, MaterialPtr &outMtl, 
	int &outTechIndex)
{
	// we reload the mtl
	outMtl = PX2_MATERIALMAN.GetMaterial(mtlFilename.c_str(), 
		intanceTechName.c_str(), shareMtl);
	assertion(outMtl != 0, "Material must be specified.\n");

	MaterialTechnique* technique = outMtl->GetTechnique(intanceTechName,
		outTechIndex);

	int newNumPass = technique->GetNumPasses();
	if (0 == vp)
		vp = new1<ShaderParametersPtr>(newNumPass);
	if (0 == pp)
		pp = new1<ShaderParametersPtr>(newNumPass);

	for (int p = 0; p < newNumPass; ++p)
	{
		MaterialPass* pass = technique->GetPass(p);
		VertexShader *vs = pass->GetVertexShader();
		PixelShader *ps = pass->GetPixelShader();

		_CreateConstants(vp[p], vs);
		_CreateConstants(pp[p], ps);
	}

	return technique;
}
//----------------------------------------------------------------------------
void MaterialInstance::_CreateConstants(ShaderParametersPtr &newParam,
	Shader *shader)
{
	if (0 == newParam)
		newParam = new0 ShaderParameters(shader);

	for (int i = 0; i < shader->GetNumConstants(); i++)
	{
		const std::string &constantName = shader->GetConstantName(i);
		int numRegister = shader->GetNumRegistersUsed(i);

		if (!newParam->GetConstant(constantName))
		{
			ShaderFloat *shaderFloat =
				PX2_MATERIALMAN.CreateShaderFloat(constantName.c_str(), 
				numRegister);
			if (shaderFloat)
			{
				newParam->SetConstant(constantName, shaderFloat);
			}
		}
	}
}
//----------------------------------------------------------------------------
MaterialInstance::~MaterialInstance ()
{
	delete1(mVertexParameters);
	delete1(mPixelParameters);
}
//----------------------------------------------------------------------------
const MaterialPass* MaterialInstance::GetPass (int pass) const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mMaterial->GetTechnique(mTechniqueIndex)->GetPass(pass);
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
ShaderParameters* MaterialInstance::GetVertexParameters (int pass) const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mVertexParameters[pass];
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
ShaderParameters* MaterialInstance::GetPixelParameters (int pass) const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mPixelParameters[pass];
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
int MaterialInstance::SetVertexConstant (int pass, const std::string& name, 
	ShaderFloat* sfloat)
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mVertexParameters[pass]->SetConstant(name, sfloat);
	}

	assertion(false, "Invalid pass index.\n");
	return -1;
}
//----------------------------------------------------------------------------
int MaterialInstance::SetPixelConstant (int pass, const std::string& name,
											ShaderFloat* sfloat)
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mPixelParameters[pass]->SetConstant(name, sfloat);
	}

	assertion(false, "Invalid pass index.\n");
	return -1;
}
//----------------------------------------------------------------------------
int MaterialInstance::SetVertexTexture (int pass, const std::string& name,
											Texture* texture)
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mVertexParameters[pass]->SetTexture(name, texture);
	}

	assertion(false, "Invalid pass index.\n");
	return -1;
}
//----------------------------------------------------------------------------
int MaterialInstance::SetPixelTexture (int pass, const std::string& name,
										   Texture* texture)
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mPixelParameters[pass]->SetTexture(name, texture);
	}

	assertion(false, "Invalid pass index.\n");
	return -1;
}
//----------------------------------------------------------------------------
int MaterialInstance::SetPixelTexture(int pass, const std::string& name,
	const std::string &filename)
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mPixelParameters[pass]->SetTexture(name, filename);
	}

	assertion(false, "Invalid pass index.\n");
	return -1;
}
//----------------------------------------------------------------------------
bool MaterialInstance::IsHasPixelSample(int pass, const std::string& name)
{
	int numTextures = mMaterial->GetPixelShader(mTechniqueIndex, pass)
		->GetNumSamplers();
	for (int i = 0; i < numTextures; i++)
	{
		const std::string &sampleName = mMaterial->GetPixelShader(
			mTechniqueIndex, pass)->GetSamplerName(i);
		if (name == sampleName)
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
ShaderFloat* MaterialInstance::GetVertexConstant (int pass,
													  const std::string& name) const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mVertexParameters[pass]->GetConstant(name);
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
ShaderFloat* MaterialInstance::GetPixelConstant (int pass,
													 const std::string& name) const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mPixelParameters[pass]->GetConstant(name);
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
Texture* MaterialInstance::GetVertexTexture (int pass,
												 const std::string& name) const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mVertexParameters[pass]->GetTexture(name);
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
Texture* MaterialInstance::GetPixelTexture (int pass,
												const std::string& name) const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mPixelParameters[pass]->GetTexture(name);
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
ShaderFloat* MaterialInstance::GetVertexConstant (int pass, int handle)
const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mVertexParameters[pass]->GetConstant(handle);
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
ShaderFloat* MaterialInstance::GetPixelConstant (int pass, int handle)
const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mPixelParameters[pass]->GetConstant(handle);
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
Texture* MaterialInstance::GetVertexTexture (int pass, int handle) const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mVertexParameters[pass]->GetTexture(handle);
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
Texture* MaterialInstance::GetPixelTexture (int pass, int handle) const
{
	if (0 <= pass && pass < mNumPasses)
	{
		return mPixelParameters[pass]->GetTexture(handle);
	}

	assertion(false, "Invalid pass index.\n");
	return 0;
}
//----------------------------------------------------------------------------
void MaterialInstance::Update(double appTime, double elapsedTime)
{
	PX2_UNUSED(appTime);
	PX2_UNUSED(elapsedTime);

	if (mIsNeedUpdate)
	{
		if (!mMaterialFilename.empty() && !mInstanceTechName.empty())
		{
			MaterialPtr newMaterial;
			int newTechIndex = 0;
			ShaderParametersPtr *newVertexParameters = 0;
			ShaderParametersPtr *newPixelParameters = 0;

			MaterialTechnique *tech = _RefreshMaterial(mMaterialFilename, mInstanceTechName,
				newVertexParameters, newPixelParameters, mIsShareMtl, newMaterial, newTechIndex);
			int newNumPasses = tech->GetNumPasses();

			if (newNumPasses <= mNumPasses)
			{
				for (int i = 0; i < newNumPasses; i++)
				{
					_CopyParams(mVertexParameters[i], newVertexParameters[i]);
					_CopyParams(mPixelParameters[i], newPixelParameters[i]);

					newMaterial->GetPass(newTechIndex, i)->SetAlphaProperty(mMaterial->GetAlphaProperty(mTechniqueIndex, i));
					newMaterial->GetPass(newTechIndex, i)->SetCullProperty(mMaterial->GetCullProperty(mTechniqueIndex, i));
					newMaterial->GetPass(newTechIndex, i)->SetDepthProperty(mMaterial->GetDepthProperty(mTechniqueIndex, i));
					newMaterial->GetPass(newTechIndex, i)->SetOffsetProperty(mMaterial->GetOffsetProperty(mTechniqueIndex, i));
					newMaterial->GetPass(newTechIndex, i)->SetStencilProperty(mMaterial->GetStencilProperty(mTechniqueIndex, i));
					newMaterial->GetPass(newTechIndex, i)->SetWireProperty(mMaterial->GetWireProperty(mTechniqueIndex, i));
				}
			}

			for (int i = 0; i < mNumPasses; i++) 
			{
				mVertexParameters[i] = 0;
				mPixelParameters[i] = 0;
			}
			delete1(mVertexParameters);
			delete1(mPixelParameters);

			mNumPasses = newNumPasses;
			mMaterial = newMaterial;
			mTechniqueIndex = newTechIndex;
			mVertexParameters = newVertexParameters;
			mPixelParameters = newPixelParameters;
		}

		mIsNeedUpdate = false;
	}

	_SetDepthTexture();
}
//----------------------------------------------------------------------------
void MaterialInstance::_SetDepthTexture()
{
	EnvirParamController *envParam = PX2_GR.GetCurEnvirParamController();
	for (int i = 0; i < GetNumPasses(); i++)
	{
		if (IsHasPixelSample(i, "SampleShadowDepth"))
		{
			SetPixelTexture(i, "SampleShadowDepth",
				envParam->GetLight_Dir_DepthTexture());
		}
	}
}
//----------------------------------------------------------------------------
void MaterialInstance::_CopyParams(ShaderParameters *from,
	ShaderParameters *to)
{
	int fromNumConstans = from->GetNumConstants();
	for (int i = 0; i < fromNumConstans; i++)
	{
		ShaderFloat *fromFloat = from->GetConstant(i);
		if (fromFloat)
		{
			const std::string &fromConstName = from->GetConstantName(i);

			ShaderFloat *toFloat = to->GetConstant(fromConstName);

			if (toFloat)
			{
				int fromNumRegisters = fromFloat->GetNumRegisters();
				if (fromNumRegisters == toFloat->GetNumRegisters())
				{
					for (int r = 0; r < fromNumRegisters; r++)
					{
						toFloat->SetRegister(r, fromFloat->GetRegister(r));
					}
				}
				else
				{
					assertion(false, "numRegist doest not be the same.\n");
				}
			}
		}
	}

	int fromNumSamples = from->GetNumTextures();
	for (int i = 0; i < fromNumSamples; i++)
	{
		const std::string &fromSampleName = from->GetSampleName(i);
		Texture *texture = from->GetTexture(i);
		if (texture)
		{
			to->_SetTexture(fromSampleName, texture);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Ãû³Æ
//----------------------------------------------------------------------------
Object* MaterialInstance::GetObjectByName (const std::string& name)
{
	Object* found = Object::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	PX2_GET_OBJECT_BY_NAME(mMaterial, name, found);
	for (int pass = 0; pass < mNumPasses; ++pass)
	{
		PX2_GET_OBJECT_BY_NAME(mVertexParameters[pass], name, found);
		PX2_GET_OBJECT_BY_NAME(mPixelParameters[pass], name, found);
	}

	return 0;
}
//----------------------------------------------------------------------------
void MaterialInstance::GetAllObjectsByName (const std::string& name,
												std::vector<Object*>& objects)
{
	Object::GetAllObjectsByName(name, objects);

	PX2_GET_ALL_OBJECTS_BY_NAME(mMaterial, name, objects);
	for (int pass = 0; pass < mNumPasses; ++pass)
	{
		PX2_GET_ALL_OBJECTS_BY_NAME(mVertexParameters[pass], name, objects);
		PX2_GET_ALL_OBJECTS_BY_NAME(mPixelParameters[pass], name, objects);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void MaterialInstance::RegistProperties ()
{
	Object::RegistProperties();

	AddPropertyClass("MaterialInstance");
	AddProperty("TechniqueIndex", PT_INT, GetTechniqueIndex(), false);
	AddProperty("NumPasses", PT_INT, GetNumPasses(), false);
}
//----------------------------------------------------------------------------
void MaterialInstance::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
MaterialInstance::MaterialInstance (LoadConstructor value) :
Object(value),
mIsShareMtl(false),
mTechniqueIndex(0),
mNumPasses(0),
mVertexParameters(0),
mPixelParameters(0),
mIsNeedUpdate(true)
{
}
//----------------------------------------------------------------------------
void MaterialInstance::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsShareMtl);

	source.ReadString(mMaterialFilename);
	mMaterialFilenameFString = mMaterialFilename.c_str();
	source.ReadString(mInstanceTechName);
	mInstanceTechNameFString = mInstanceTechName.c_str();

	source.ReadPointer(mMaterial);
	source.Read(mTechniqueIndex);

	source.ReadPointerRR(mNumPasses, mVertexParameters);
	source.ReadPointerVR(mNumPasses, mPixelParameters);

	PX2_END_DEBUG_STREAM_LOAD(MaterialInstance, source);
}
//----------------------------------------------------------------------------
void MaterialInstance::Link (InStream& source)
{
	Object::Link(source);

	source.ResolveLink(mMaterial);

	source.ResolveLink(mNumPasses, mVertexParameters);
	source.ResolveLink(mNumPasses, mPixelParameters);
}
//----------------------------------------------------------------------------
void MaterialInstance::PostLink ()
{
	Object::PostLink();

	if (mMaterial && !mMaterialFilename.empty() && !mInstanceTechName.empty())
	{
		PX2_MATERIALMAN.AddMaterial(mMaterialFilename.c_str(), 
			mInstanceTechName.c_str(), mMaterial);
	}
}
//----------------------------------------------------------------------------
bool MaterialInstance::Register (OutStream& target) const
{
	if (Object::Register(target))
	{
		target.Register(mMaterial);

		target.Register(mNumPasses, mVertexParameters);
		target.Register(mNumPasses, mPixelParameters);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void MaterialInstance::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsShareMtl);

	target.WriteString(mMaterialFilename);
	target.WriteString(mInstanceTechName);

	target.WritePointer(mMaterial);
	target.Write(mTechniqueIndex);

	target.WritePointerW(mNumPasses, mVertexParameters);
	target.WritePointerN(mNumPasses, mPixelParameters);

	PX2_END_DEBUG_STREAM_SAVE(MaterialInstance, target);
}
//----------------------------------------------------------------------------
int MaterialInstance::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsShareMtl);

	size += PX2_STRINGSIZE(mMaterialFilename);
	size += PX2_STRINGSIZE(mInstanceTechName);

	size += PX2_POINTERSIZE(mMaterial);
	size += sizeof(mTechniqueIndex);

	size += sizeof(mNumPasses);
	size += mNumPasses*PX2_POINTERSIZE(mVertexParameters[0]);
	size += mNumPasses*PX2_POINTERSIZE(mPixelParameters[0]);

	return size;
}
//----------------------------------------------------------------------------
