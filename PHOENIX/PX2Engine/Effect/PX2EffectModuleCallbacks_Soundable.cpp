// PX2EffectModuleCallbacks_Soundable.cpp

#include "PX2EffectModuleCallbacks_Soundable.hpp"
#include "PX2EffectModule.hpp"
#include "PX2Soundable.hpp"
#include "PX2SoundableController.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// SE_Callback
//----------------------------------------------------------------------------
void SE_Callbacks::SE_EmitVolume (EffectModule *module, Effectable *able, 
	float ctrlTime)
{
	Soundable *soundable = (Soundable*)able;

	if (!module->IsRange())
	{
		soundable->SetVolume(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(ctrlTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(ctrlTime, 0.0f);
		soundable->SetVolume(Mathf::IntervalRandom(valMin, valMax));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SO_Callback
//----------------------------------------------------------------------------
void SO_Callbacks::SO_VolumeAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	SoundableObject *po = (SoundableObject*)obj;

	if (!module->IsRange())
	{
		po->Volume = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		po->Volume = Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void SO_Callbacks::SO_VolumeByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	SoundableObject *po = (SoundableObject*)obj;

	if (!module->IsRange())
	{
		po->Volume = po->VolumeInit * 
			module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		po->Volume = po->VolumeInit * Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------