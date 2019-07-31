// PX2ShadowMap_Std_Material.cpp

#include "PX2ShadowMapMaterial.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2TriMesh.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, GlobalMaterial, ShadowMap_Material);
PX2_IMPLEMENT_STREAM(ShadowMap_Material);
PX2_IMPLEMENT_FACTORY(ShadowMap_Material);
PX2_IMPLEMENT_DEFAULT_NAMES(GlobalMaterial, ShadowMap_Material);

//----------------------------------------------------------------------------
ShadowMap_Material::ShadowMap_Material()
{
	mInstanceStd = new0 MaterialInstance(
		"Data/engine_mtls/std/std.px2obj", "std_shadowmapdepth", false);

	mInstanceSkinSkeleton = new0 MaterialInstance(
		"Data/engine_mtls/skinskeleton/skinskeleton.px2obj",
		"skinskeleton_shadowmapdepth", false);

	mInstanceTerrain = new MaterialInstance(
		"Data/engine_mtls/terrain/terrain.px2obj",
		"terrain_shadowmapdepth", false);

	mOverRideOffsetProperty = new0 OffsetProperty();
	mOverRideOffsetProperty->FillEnabled = true;
	mOverRideOffsetProperty->Scale = 4.0f;
	mOverRideOffsetProperty->Bias = 100.0f;
}
//----------------------------------------------------------------------------
ShadowMap_Material::~ShadowMap_Material()
{
}
//----------------------------------------------------------------------------
void ShadowMap_Material::Draw(Renderer* renderer, 
	const VisibleSet& visibleSet)
{
	EnvirParamController *curEnvirParam = PX2_GR.GetCurEnvirParamController();

	mOverRideOffsetProperty->FillEnabled = true;
	mOverRideOffsetProperty->Scale = curEnvirParam->GetShadowMap_OffsetPropertyScale();
	mOverRideOffsetProperty->Bias = curEnvirParam->GetShadowMap_OffsetPropertyBias();
	renderer->SetOverrideOffsetProperty(mOverRideOffsetProperty);

	const int numVisible = visibleSet.GetNumVisible();
	for (int j = 0; j < numVisible; ++j)
	{
		Renderable *renderable = visibleSet.GetVisible(j);
		if (renderable->IsCastShadow())
		{
			MaterialInstancePtr save = renderable->GetMaterialInstance();
			if (save)
			{
				const std::string &mtlName = save->GetMaterial()->GetName();

				if ("std" == mtlName)
				{
					mInstanceStd->SetPixelTexture(0, "SampleBase", save->GetPixelTexture(0, "SampleBase"));

					renderable->SetMaterialInstance(mInstanceStd);
					renderer->Draw(renderable);

					renderable->SetMaterialInstance(save);
					save->Update(0.0f, 0.0f);
				}
				else if ("materialcolor" == mtlName)
				{
					renderer->Draw(renderable);
				}
				else if ("skinskeleton" == mtlName)
				{
					//mInstanceSkinSkeleton->SetPixelTexture(0, "SampleBase", save->GetPixelTexture(0, "SampleBase"));

					//renderable->SetMaterialInstance(mInstanceSkinSkeleton);
					renderer->Draw(renderable);

					//renderable->SetMaterialInstance(save);
					//save->Update(0.0f, 0.0f);
				}
				else if ("terrain" == mtlName)
				{
					renderable->SetMaterialInstance(mInstanceTerrain);
					renderer->Draw(renderable);

					renderable->SetMaterialInstance(save);
					save->Update(0.0f, 0.0f);
				}
			}
		}
	}

	renderer->SetOverrideOffsetProperty(0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Streaming support.
//----------------------------------------------------------------------------
ShadowMap_Material::ShadowMap_Material(LoadConstructor value):
GlobalMaterial(value)
{
}
//----------------------------------------------------------------------------
void ShadowMap_Material::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	GlobalMaterial::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(ShadowMap_Material, source);
}
//----------------------------------------------------------------------------
void ShadowMap_Material::Link(InStream& source)
{
	GlobalMaterial::Link(source);
}
//----------------------------------------------------------------------------
void ShadowMap_Material::PostLink()
{
	GlobalMaterial::PostLink();
}
//----------------------------------------------------------------------------
bool ShadowMap_Material::Register(OutStream& target) const
{
	if (GlobalMaterial::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void ShadowMap_Material::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	GlobalMaterial::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(ShadowMap_Material, target);
}
//----------------------------------------------------------------------------
int ShadowMap_Material::GetStreamingSize(Stream &stream) const
{
	int size = GlobalMaterial::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
