// PX2AmbientRegionController.cpp

#include "PX2AmbientRegionController.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2AVector.hpp"
#include "PX2Project.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, TriggerController, AmbientRegionController);
PX2_IMPLEMENT_STREAM(AmbientRegionController);
PX2_IMPLEMENT_FACTORY(AmbientRegionController);

//----------------------------------------------------------------------------
AmbientRegionController::AmbientRegionController() :
mHorAngle(45.0f),
mVerAngle(45.0f),
mLightCameraExtent(10.0f),
mSpecularPow(10.0f),
mIntensity(1.0f),
mLightCameraLookDistance(50.0f),
_IsNeedUpdateLight(true),
_IsNeedUpdateFog(true)
{
	SetName("AmbientRegionController");

	mAmbientColor = Float3(0.4f, 0.4f, 0.4f);
	mDiffuseColor = Float3(1.0f, 1.0f, 1.0f);
	mSpecularColor = Float3(1.0f, 1.0f, 1.0f);

	mFogColorHeight = Float3::WHITE;
	mFogParamHeight = Float2(-20.0f, -10.0f);

	mFogColorDist = Float3::WHITE;
	mFogParamDist = Float2(600.0f, 800.0f);
}
//----------------------------------------------------------------------------
AmbientRegionController::~AmbientRegionController()
{
}
//----------------------------------------------------------------------------
void AmbientRegionController::_Update(double applicationTime,
	double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);

	SetFogColorHeight(mFogColorHeight);
	SetFogParamHeight(mFogParamHeight);
	SetFogColorDistance(mFogColorDist);
	SetFogParamDistance(mFogParamDist);

	if (_IsNeedUpdateLight)
	{
		_UpdateDirLightCamera();
		_IsNeedUpdateLight = false;
	}

	if (_IsNeedUpdateFog)
	{
		_UpdateFog();
		_IsNeedUpdateFog = false;
	}
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetLightCameraLookPostion(const APoint &pos)
{
	mLightCameraLookPosition = pos;

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetLightCameraLookDistance(float dist)
{
	mLightCameraLookDistance = dist;

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetLightCameraExtent(float extent)
{
	mLightCameraExtent = extent;

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetControlledable(Controlledable* object)
{
	TriggerController::SetControlledable(object);

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetHorAngle(float angle)
{
	mHorAngle = angle;

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetVerAngle(float angle)
{
	mVerAngle = angle;

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetAmbientColor(const Float3 &color)
{
	mAmbientColor = color;

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetDiffuseColor(const Float3 &color)
{
	mDiffuseColor = color;

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetSpecularColor(const Float3 &color)
{
	mSpecularColor = color;

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetSpecularPow(float pow)
{
	mSpecularPow = pow;

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetIntensity(float intensity)
{
	mIntensity = intensity;

	_IsNeedUpdateLight = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetFogColorHeight(const Float3 &color)
{
	mFogColorHeight = color;

	_IsNeedUpdateFog = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetFogParamHeight(const Float2 &param)
{
	mFogParamHeight = param;

	_IsNeedUpdateFog = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetFogColorDistance(const Float3 &dist)
{
	mFogColorDist = dist;

	_IsNeedUpdateFog = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::SetFogParamDistance(const Float2 &param)
{
	mFogParamDist = param;

	_IsNeedUpdateFog = true;
}
//----------------------------------------------------------------------------
void AmbientRegionController::_UpdateDirLightCamera()
{
	Movable *mov = DynamicCast<Movable>(GetControlledable());
	if (!mov) return;

	Scene *scene = DynamicCast<Scene>(mov);
	if (!scene)
		scene = DynamicCast<Scene>(mov->GetTopestParent());

	AVector dir = AVector::AnglesToDirection(Mathf::DEG_TO_RAD*mHorAngle,
		Mathf::DEG_TO_RAD*mVerAngle);
	dir.Normalize();

	if (scene)
	{
		EnvirParamController *envirParam = scene->GetEnvirParamController();

		Light *lightDir = envirParam->GetLight_Dir();
		Projector *projector = envirParam->GetLight_Dir_Projector();

		lightDir->Ambient = Float4(mAmbientColor[0], mAmbientColor[1],
			mAmbientColor[2], mIntensity);
		lightDir->Intensity = mIntensity;
		lightDir->Diffuse = Float4(mDiffuseColor[0], mDiffuseColor[1],
			mDiffuseColor[2], 1.0f);
		lightDir->Specular = Float4(mSpecularColor[0], mSpecularColor[1],
			mSpecularColor[2], mSpecularPow);

		float upDot = dir.Dot(-AVector::UNIT_Z);
		if (upDot >= 0.99f)
		{
		}
		else
		{
			AVector upTemp = AVector::UNIT_Z;
			AVector right = dir.UnitCross(upTemp);
			AVector up = right.UnitCross(dir);

			lightDir->DVector = dir;
			lightDir->UVector = up;
			lightDir->RVector = right;

			APoint camPos = mLightCameraLookPosition - dir*mLightCameraLookDistance;
			projector->SetFrame(camPos, lightDir->DVector,
				lightDir->UVector, lightDir->RVector);
		}

		if (!projector->IsPerspective())
		{
			projector->SetFrustum(0.1f, 100.0f,
				-mLightCameraExtent, mLightCameraExtent, -mLightCameraExtent,
				mLightCameraExtent);
		}
		else
		{
			projector->SetFrustum(mLightCameraExtent, 1.0f, 1.0f, 100.0f);
		}
	}
}
//----------------------------------------------------------------------------
void AmbientRegionController::_UpdateFog()
{
	Movable *mov = DynamicCast<Movable>(GetControlledable());
	if (!mov) return;

	Scene *scene = DynamicCast<Scene>(mov);
	if (!scene)
		scene = DynamicCast<Scene>(mov->GetTopestParent());

	if (scene)
	{
		EnvirParamController *envirParamCtrl =
			scene->GetEnvirParamController();
		if (envirParamCtrl)
		{
			envirParamCtrl->SetFogColorHeight(Float4(mFogColorHeight[0], 
				mFogColorHeight[1], mFogColorHeight[2], 1.0f));

			envirParamCtrl->SetFogColorDist(Float4(mFogColorDist[0], 
				mFogColorDist[1], mFogColorDist[2], 1.0f));

			Float4 fogParam = Float4(mFogParamHeight[0], mFogParamHeight[1],
				mFogParamDist[0], mFogParamDist[1]);
			envirParamCtrl->SetFogParam(fogParam);
		}
	}
	else
	{
		assertion(false, "there must be a scene.");
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void AmbientRegionController::RegistProperties()
{
	TriggerController::RegistProperties();

	AddPropertyClass("AmbientRegionController");

	AddProperty("HorAngle", Object::PT_INT, (int)mHorAngle);
	AddProperty("VerAngle", Object::PT_INT, (int)mVerAngle);
	AddProperty("LightCameraLookPosition", Object::PT_APOINT3, mLightCameraLookPosition);
	AddProperty("LightCameraLookDistance", Object::PT_FLOAT, mLightCameraLookDistance);
	AddProperty("LightCameraExtent", Object::PT_FLOAT, mLightCameraExtent);

	AddProperty("AmbientColor", Object::PT_COLOR3FLOAT3, mAmbientColor);
	AddProperty("DiffuseColor", Object::PT_COLOR3FLOAT3, mDiffuseColor);
	AddProperty("SpecularColor", Object::PT_COLOR3FLOAT3, mSpecularColor);
	AddProperty("SpecularPow", Object::PT_FLOAT, mSpecularPow);
	AddProperty("Intensity", Object::PT_FLOAT, mIntensity);

	AddProperty("FogColorHeight", PT_COLOR3FLOAT3, mFogColorHeight);
	AddProperty("FogParamHeight", PT_FLOAT2, mFogParamHeight);
	AddProperty("FogColorDistance", PT_COLOR3FLOAT3, mFogColorDist);
	AddProperty("FogParamDistance", PT_FLOAT2, mFogParamDist);
}
//----------------------------------------------------------------------------
void AmbientRegionController::OnPropertyChanged(const PropertyObject &obj)
{
	TriggerController::OnPropertyChanged(obj);

	if ("HorAngle" == obj.Name)
	{
		SetHorAngle((float)PX2_ANY_AS(obj.Data, int));
	}
	else if ("VerAngle" == obj.Name)
	{
		SetVerAngle((float)PX2_ANY_AS(obj.Data, int));
	}
	else if ("LightCameraLookPosition" == obj.Name)
	{
		SetLightCameraLookPostion(PX2_ANY_AS(obj.Data, APoint));
	}
	else if ("LightCameraLookDistance" == obj.Name)
	{
		SetLightCameraLookDistance(PX2_ANY_AS(obj.Data, float));
	}
	else if ("LightCameraExtent" == obj.Name)
	{
		SetLightCameraExtent(PX2_ANY_AS(obj.Data, float));
	}
	else if ("AmbientColor" == obj.Name)
	{
		SetAmbientColor(PX2_ANY_AS(obj.Data, Float3));
	}
	else if ("DiffuseColor" == obj.Name)
	{
		SetDiffuseColor(PX2_ANY_AS(obj.Data, Float3));
	}
	else if ("SpecularColor" == obj.Name)
	{
		SetSpecularColor(PX2_ANY_AS(obj.Data, Float3));
	}
	else if ("SpecularPow" == obj.Name)
	{
		SetSpecularPow(PX2_ANY_AS(obj.Data, float));
	}
	else if ("Intensity" == obj.Name)
	{
		SetIntensity(PX2_ANY_AS(obj.Data, float));
	}
	else if ("FogColorHeight" == obj.Name)
	{
		SetFogColorHeight(PX2_ANY_AS(obj.Data, Float3));
	}
	else if ("FogParamHeight" == obj.Name)
	{
		SetFogParamHeight(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("FogColorDistance" == obj.Name)
	{
		SetFogColorDistance(PX2_ANY_AS(obj.Data, Float3));
	}
	else if ("FogParamDistance" == obj.Name)
	{
		SetFogParamDistance(PX2_ANY_AS(obj.Data, Float2));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* AmbientRegionController::GetObjectByName(const std::string& name)
{
	Object* found = TriggerController::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	return 0;
}
//----------------------------------------------------------------------------
void AmbientRegionController::GetAllObjectsByName(const std::string& name,
	std::vector<Object*>& objects)
{
	TriggerController::GetAllObjectsByName(name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
AmbientRegionController::AmbientRegionController(LoadConstructor value) :
TriggerController(value),
_IsNeedUpdateLight(true),
_IsNeedUpdateFog(true)
{
}
//----------------------------------------------------------------------------
void AmbientRegionController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	TriggerController::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mHorAngle);
	source.Read(mVerAngle);

	source.ReadAggregate(mAmbientColor);
	source.ReadAggregate(mDiffuseColor);
	source.ReadAggregate(mSpecularColor);
	source.Read(mSpecularPow);
	source.Read(mIntensity);

	source.ReadAggregate(mLightCameraLookPosition);
	source.Read(mLightCameraLookDistance);
	source.Read(mLightCameraExtent);

	source.ReadAggregate(mFogColorHeight);
	source.ReadAggregate(mFogParamHeight);
	source.ReadAggregate(mFogColorDist);
	source.ReadAggregate(mFogParamDist);

	PX2_END_DEBUG_STREAM_LOAD(AmbientRegionController, source);
}
//----------------------------------------------------------------------------
void AmbientRegionController::Link(InStream& source)
{
	TriggerController::Link(source);
}
//----------------------------------------------------------------------------
void AmbientRegionController::PostLink()
{
	TriggerController::PostLink();

	SetHorAngle(mHorAngle);
	SetVerAngle(mVerAngle);
	SetAmbientColor(mAmbientColor);
	SetDiffuseColor(mDiffuseColor);
	SetSpecularColor(mSpecularColor);
	SetSpecularPow(mSpecularPow);

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
bool AmbientRegionController::Register(OutStream& target) const
{
	if (TriggerController::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void AmbientRegionController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	TriggerController::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mHorAngle);
	target.Write(mVerAngle);

	target.WriteAggregate(mAmbientColor);
	target.WriteAggregate(mDiffuseColor);
	target.WriteAggregate(mSpecularColor);
	target.Write(mSpecularPow);
	target.Write(mIntensity);

	target.WriteAggregate(mLightCameraLookPosition);
	target.Write(mLightCameraLookDistance);
	target.Write(mLightCameraExtent);

	target.WriteAggregate(mFogColorHeight);
	target.WriteAggregate(mFogParamHeight);
	target.WriteAggregate(mFogColorDist);
	target.WriteAggregate(mFogParamDist);

	PX2_END_DEBUG_STREAM_SAVE(AmbientRegionController, target);
}
//----------------------------------------------------------------------------
int AmbientRegionController::GetStreamingSize(Stream &stream) const
{
	int size = TriggerController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mHorAngle);
	size += sizeof(mVerAngle);

	size += sizeof(mAmbientColor);
	size += sizeof(mDiffuseColor);
	size += sizeof(mSpecularColor);
	size += sizeof(mSpecularPow);
	size += sizeof(mIntensity);

	size += sizeof(mLightCameraLookPosition);
	size += sizeof(mLightCameraLookDistance);
	size += sizeof(mLightCameraExtent);

	size += sizeof(mFogColorHeight);
	size += sizeof(mFogParamHeight);
	size += sizeof(mFogColorDist);
	size += sizeof(mFogParamDist);

	return size;
}
//----------------------------------------------------------------------------