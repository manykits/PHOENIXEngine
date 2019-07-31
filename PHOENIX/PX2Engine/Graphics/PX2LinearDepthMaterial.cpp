// PX2LinearDepthMaterial.cpp

#include "PX2LinearDepthMaterial.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2TriMesh.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, GlobalMaterial, LinearDepth_Material);
PX2_IMPLEMENT_STREAM(LinearDepth_Material);
PX2_IMPLEMENT_FACTORY(LinearDepth_Material);
PX2_IMPLEMENT_DEFAULT_NAMES(GlobalMaterial, LinearDepth_Material);

//----------------------------------------------------------------------------
LinearDepth_Material::LinearDepth_Material()
{
	mInstanceStd = new0 MaterialInstance(
		"Data/engine_mtls/std/std.px2obj", "std_lineardepth", false);

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
LinearDepth_Material::~LinearDepth_Material()
{
}
//----------------------------------------------------------------------------
void LinearDepth_Material::Draw(Renderer* renderer,
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
		MaterialInstancePtr save = renderable->GetMaterialInstance();
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

	renderer->SetOverrideOffsetProperty(0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Streaming support.
//----------------------------------------------------------------------------
LinearDepth_Material::LinearDepth_Material(LoadConstructor value) :
GlobalMaterial(value)
{
}
//----------------------------------------------------------------------------
void LinearDepth_Material::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	GlobalMaterial::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LinearDepth_Material, source);
}
//----------------------------------------------------------------------------
void LinearDepth_Material::Link(InStream& source)
{
	GlobalMaterial::Link(source);
}
//----------------------------------------------------------------------------
void LinearDepth_Material::PostLink()
{
	GlobalMaterial::PostLink();
}
//----------------------------------------------------------------------------
bool LinearDepth_Material::Register(OutStream& target) const
{
	if (GlobalMaterial::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LinearDepth_Material::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	GlobalMaterial::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LinearDepth_Material, target);
}
//----------------------------------------------------------------------------
int LinearDepth_Material::GetStreamingSize(Stream &stream) const
{
	int size = GlobalMaterial::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
