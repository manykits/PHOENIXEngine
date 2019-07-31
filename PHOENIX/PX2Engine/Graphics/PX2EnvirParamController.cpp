// EnvirParamController.cpp

#include "PX2EnvirParamController.hpp"
#include "PX2Renderable.hpp"
#include "PX2GraphicsEventType.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, EnvirParamController);
PX2_IMPLEMENT_STREAM(EnvirParamController);
PX2_IMPLEMENT_FACTORY(EnvirParamController);

//----------------------------------------------------------------------------
EnvirParamController::EnvirParamController()
{
	mFogParam = Float4(-10.0f, 0.0f, 800.0f, 1120.0f);
	mFogColorHeightLast = Float4::RED;
	mFogColorDistLast = Float4::BLUE;

	mLight_Dir_Projector = new0 Projector(true);
	mLight_Dir_Projector->SetClearFlag(true, true, true);
	mLight_Dir_Projector->SetClearColor(Float4::WHITE);
	mLight_Dir_Projector->SetClearDepth(1.0f);

	mLight_Dir = new0 Light(Light::LT_DIRECTIONAL);
	mLight_Dir->Ambient = Float4(0.7f, 0.7f, 0.7f, 1.0f);
	mLight_Dir->Diffuse = Float4(0.3f, 0.3f, 0.3f, 1.0f);
	mLight_Dir->Specular = Float4(0.3f, 0.3f, 0.3f, 1.0f);
	AVector lightVec(1.0f, 1.0f, -1.0f);
	lightVec.Normalize();
	mLight_Dir->SetDirection(lightVec);

	mShadowOffsetProperty_Bias = 0.0f;
	mShadowOffsetProperty_Scale = 0.0f;

	mIsUseShadowMap = true;
	mShadowRenderTargetSize = Sizef(1024.0f, 1024.0f);
	mIsShadowRenderTargetSizeSameWithCanvas = true;

	mIsUseBloom = false;

	mBloomBrightWeight = 0.3f;
	mBloomBlurDeviation = 1.0f;
	mBloomBlurWeight = 1.0f;
	mBloomWeight = 1.0f;
	mBloomBrightParam = Float4::ZERO;
	mBloomParam = Float4::UNIT;

	SetName("EnvirParamController");
}
//----------------------------------------------------------------------------
EnvirParamController::~EnvirParamController()
{
	mLight_Dir_Projector = 0;

	mAllLights.clear();
	mLight_Dir = 0;
}
//----------------------------------------------------------------------------
void EnvirParamController::AddLight(Light *light)
{
	if (!light) return;

	bool bIn = false;
	for (int i = 0; i < (int)mAllLights.size(); i++)
	{
		if (light == mAllLights[i])
			bIn = true;
	}

	if (!bIn)
	{
		if (Light::LT_DIRECTIONAL == light->GetType())
		{
			mLight_Dir = light;
		}

		mAllLights.push_back(light);
	}
}
//----------------------------------------------------------------------------
void EnvirParamController::RemoveLight(Light *light)
{
	std::vector<Pointer0<Light> >::iterator it = mAllLights.begin();
	for (; it != mAllLights.end(); it++)
	{
		if (*it == light)
		{
			mAllLights.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
void EnvirParamController::ClearAllLights()
{
	mAllLights.clear();
}
//----------------------------------------------------------------------------
int EnvirParamController::GetNumLights()
{
	return (int)mAllLights.size();
}
//----------------------------------------------------------------------------
Light *EnvirParamController::GetLight(int index)
{
	int numLights = (int)mAllLights.size();
	if (index >= 0 && index < numLights)
	{
		return mAllLights[index];
	}

	return 0;
}
//----------------------------------------------------------------------------
void EnvirParamController::ComputeEnvironment(VisibleSet &vs)
{
	for (int i = 0; i < vs.GetNumVisible(); i++)
	{
		PX2::Renderable *renderable = vs.GetVisible(i);
		PX2::APoint renPos = renderable->WorldTransform.GetTranslate();
		renderable->ClearLights();

		for (int j = 0; j < (int)mAllLights.size(); j++)
		{
			PX2::Light *light = mAllLights[j];
			Bound lightBound;
			lightBound.SetCenter(light->Position);
			lightBound.SetRadius(light->Range);

			if (Light::LT_POINT == light->GetType() &&
				renderable->WorldBound.TestIntersection(lightBound))
			{
				renderable->AddLight(light);
			}
		}
	}
}
//----------------------------------------------------------------------------
void EnvirParamController::SetLight_Dir_DepthTexture(Texture *tex)
{
	mLight_Dir_DepthTexture = tex;
}
//----------------------------------------------------------------------------
void EnvirParamController::RegistProperties()
{
	Controller::RegistProperties();

	AddPropertyClass("EnvirParamController");
}
//----------------------------------------------------------------------------
void EnvirParamController::OnPropertyChanged(const PropertyObject &obj)
{
	Controller::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
EnvirParamController::EnvirParamController(LoadConstructor value) :
Controller(value),
mShadowOffsetProperty_Bias(0.0f),
mShadowOffsetProperty_Scale(0.0f)
{
	mIsUseShadowMap = true;
	mShadowRenderTargetSize = Sizef(1024.0f, 1024.0f);
	mIsShadowRenderTargetSizeSameWithCanvas = false;

	mIsUseBloom = false;

	mBloomBrightWeight = 0.3f;
	mBloomBlurDeviation = 1.0f;
	mBloomBlurWeight = 1.0f;
	mBloomWeight = 1.0f;
	mBloomBrightParam = Float4::ZERO;
	mBloomParam = Float4::UNIT;
}
//----------------------------------------------------------------------------
void EnvirParamController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mLight_Dir);
	source.ReadPointer(mLight_Dir_Projector);

	source.ReadAggregate(mFogParam);

	source.Read(mShadowOffsetProperty_Bias);
	source.Read(mShadowOffsetProperty_Scale);

	PX2_END_DEBUG_STREAM_LOAD(EnvirParamController, source);
}
//----------------------------------------------------------------------------
void EnvirParamController::Link(InStream& source)
{
	Controller::Link(source);

	if (mLight_Dir)
		source.ResolveLink(mLight_Dir);

	if (mLight_Dir_Projector)
		source.ResolveLink(mLight_Dir_Projector);
}
//----------------------------------------------------------------------------
void EnvirParamController::PostLink()
{
	Controller::PostLink();

	if (mLight_Dir)
	{
		mLight_Dir->PostLink();

		mLight_Dir->Ambient = Float4(1.0f, 1.0f, 1.0f, 1.0f);
		mLight_Dir->Diffuse = Float4(0.8f, 0.8f, 0.8f, 1.0f);
		mLight_Dir->Specular = Float4(1.9f, 1.9f, 1.9f, 1.0f);
		AVector lightVec(1.0f, 1.0f, -1.0f);
		lightVec.Normalize();
		mLight_Dir->SetDirection(lightVec);
	}

	if (mLight_Dir_Projector)
	{
		mLight_Dir_Projector->PostLink();

		mLight_Dir_Projector->SetClearFlag(true, true, true);
		mLight_Dir_Projector->SetClearColor(Float4::WHITE);
		mLight_Dir_Projector->SetClearDepth(1.0f);
	}
}
//----------------------------------------------------------------------------
bool EnvirParamController::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		if (mLight_Dir)
			target.Register(mLight_Dir);

		if (mLight_Dir_Projector)
			target.Register(mLight_Dir_Projector);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EnvirParamController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mLight_Dir);
	target.WritePointer(mLight_Dir_Projector);

	target.WriteAggregate(mFogParam);

	target.Write(mShadowOffsetProperty_Bias);
	target.Write(mShadowOffsetProperty_Scale);

	PX2_END_DEBUG_STREAM_SAVE(EnvirParamController, target);
}
//----------------------------------------------------------------------------
int EnvirParamController::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mLight_Dir);
	size += PX2_POINTERSIZE(mLight_Dir_Projector);

	size += sizeof(mFogParam);

	size += sizeof(mShadowOffsetProperty_Bias);
	size += sizeof(mShadowOffsetProperty_Scale);

	return size;
}
//----------------------------------------------------------------------------