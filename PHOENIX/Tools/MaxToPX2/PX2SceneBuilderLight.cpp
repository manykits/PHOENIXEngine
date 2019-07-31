// PX2SceneBuilderLight.cpp

#include "PX2SceneBuilder.hpp"

//----------------------------------------------------------------------------
void SceneBuilder::BuildAmbientLight()
{
	Interval valid = FOREVER;
	Color color = mMax->GetAmbient(0, valid);
	mAmbientLight = new0 PX2::Light(PX2::Light::LT_AMBIENT);
	mAmbientLight->Ambient = PX2::Float4(color.r,color.g,color.b, 1.0f);
}
//----------------------------------------------------------------------------
void SceneBuilder::BuildLight(INode *maxNode, PX2::Movable *movbale)
{
	// 根据Max场景图中的灯光建立相对应的Phoenix2中的灯光。支持Max的点光源，
	// 方向光，和自由聚光灯。
	// 
	// maxNode:
	//		代表灯光的Max节点
	// movbale:
	//		在Phoenix2中，灯光会绑定到的节点。

	PX2_UNUSED(movbale);

	// 从Max获得灯光
	ObjectState objectState = maxNode->EvalWorldState(mTimeStart);
	GenLight *genLight = (GenLight*)objectState.obj;
	LightState lightState;
	Interval valid = FOREVER;
	genLight->EvalLightState(mTimeStart, valid, &lightState);

	PX2::Light *light = 0;

	switch (genLight->Type())
	{
	case OMNI_LIGHT:
		light = BuildPointLight(maxNode);
		break;
	case TSPOT_LIGHT:
	case FSPOT_LIGHT:
		light = BuildSpotLight(maxNode, lightState);
		break;
	case TDIR_LIGHT:
	case DIR_LIGHT:
		light = BuildDirectionalLight(maxNode);
		break;
	default:
		assertion(false, "light type not supportted.");
		return;
	}
	
	light->Intensity = lightState.intens;

	if (lightState.affectDiffuse)
	{
		light->Diffuse = PX2::Float4(lightState.color.r, lightState.color.g, 
			lightState.color.b, 1.0f);
	}

	if (lightState.affectSpecular)
	{
		light->Specular = PX2::Float4(lightState.color.r, lightState.color.g,
			lightState.color.b, 1.0f);
	}

	// 衰退参数
	if (genLight->GetDecayType() != 0)
	{
		Interval valid;
		float decayRadius = genLight->GetDecayRadius(mTimeStart, valid);

		switch (genLight->GetDecayType())
		{
		case 0:
			light->Constant = ATTENUATION;
			light->Linear = 0.0f;
			light->Quadratic = 0.0f;
			break;
		case 1:
			light->Constant = 0.0f;
			light->Linear = ATTENUATION/decayRadius;
			light->Quadratic = 0.0f;
			break;
		case 2:
			light->Constant = 0.0f;
			light->Linear = 0.0f;
			light->Quadratic = ATTENUATION/(decayRadius*decayRadius);
			break;
		}
	}
	else if (genLight->GetUseAtten())
	{
		light->Constant = 0.0f;
		light->Linear = ATTENUATION/lightState.attenEnd;
		light->Quadratic = 0.0f;
	}

	mLights.push_back(light);
}
//----------------------------------------------------------------------------
PX2::APoint SceneBuilder::GetLightLocation(INode *node)
{
	Matrix3 local = node->GetNodeTM(mTimeStart) *
		Inverse(node->GetParentTM(mTimeStart));

	AffineParts affParts;
	decomp_affine(local, &affParts);

	return PX2::APoint(affParts.t.x, affParts.t.y, affParts.t.z);
}
//----------------------------------------------------------------------------
PX2::Light *SceneBuilder::BuildPointLight(INode *node)
{
	PX2::Light *light = new0 PX2::Light(PX2::Light::LT_POINT);

	light->Position = GetLightLocation(node);

	return light;
}
//----------------------------------------------------------------------------
PX2::Light *SceneBuilder::BuildSpotLight(INode *node, LightState &lightState)
{
	PX2::Light *light = new0 PX2::Light(PX2::Light::LT_SPOT);

	light->Position = GetLightLocation(node);
	light->SetDirection(-PX2::AVector::UNIT_Z);

	light->Angle = HALFPI * lightState.fallsize/180.0f; 

	light->Exponent = 128.0f*((lightState.fallsize - lightState.hotsize)
		/lightState.fallsize);

	return light;
}
//----------------------------------------------------------------------------
PX2::Light *SceneBuilder::BuildDirectionalLight(INode *node)
{
	PX2_UNUSED(node);

	PX2::Light *light = new0 PX2::Light(PX2::Light::LT_DIRECTIONAL);

	light->SetDirection(-PX2::AVector::UNIT_Z);

	return light;
}
//----------------------------------------------------------------------------