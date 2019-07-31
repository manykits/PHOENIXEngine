// PX2EffectModuleCallbacks_CameraShake.cpp

#include "PX2EffectModuleCallbacks_CameraShake.hpp"
#include "PX2CameraShake.hpp"
#include "PX2CameraShakeObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// CSE_Callbacks
//----------------------------------------------------------------------------
void CSE_Callbacks::CSE_EmitStrength (EffectModule *module, Effectable *able,
	float ctrlTime)
{
	CameraShake *cameraShake = (CameraShake*)able;

	if (!module->IsRange())
	{
		cameraShake->SetEmitStrength(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(ctrlTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(ctrlTime, 0.0f);
		cameraShake->SetEmitStrength(Mathf::IntervalRandom(valMin, valMax));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// CSO_Callbacks
//----------------------------------------------------------------------------
void CSO_Callbacks::CSO_StrengthAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	CameraShakeObject *cso = (CameraShakeObject*)obj;

	if (!module->IsRange())
	{
		cso->Strength = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		cso->Strength = Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void CSO_Callbacks::CSO_StrengthByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	CameraShakeObject *cso = (CameraShakeObject*)obj;

	if (!module->IsRange())
	{
		cso->Strength = cso->StrengthInit * 
			module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		cso->Strength = cso->StrengthInit * Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------