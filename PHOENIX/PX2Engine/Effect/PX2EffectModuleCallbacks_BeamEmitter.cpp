// PX2EffectModuleCallbacks_BeamEmitter.cpp

#include "PX2EffectModuleCallbacks_BeamEmitter.hpp"
#include "PX2BeamEmitter.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void BE_Callbacks::BE_EmitRate (EffectModule *module, Effectable *able, float ctrlTime)
{
	BeamEmitter *be = (BeamEmitter*)able;

	if (!module->IsRange())
	{
		be->SetEmitRate(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		be->SetEmitRate(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void BE_Callbacks::BE_EmitStartPos (EffectModule *module, Effectable *able, float ctrlTime)
{
	BeamEmitter *be = (BeamEmitter*)able;

	if (!module->IsRange())
	{
		be->SetEmitStartPos(module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO));
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(ctrlTime, Float3::ZERO);

		be->SetEmitStartPos(Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2])));
	}
}
//----------------------------------------------------------------------------
void BE_Callbacks::BE_EmitEndPos (EffectModule *module, Effectable *able, float ctrlTime)
{
	BeamEmitter *be = (BeamEmitter*)able;

	if (!module->IsRange())
	{
		be->SetEmitEndPos(module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO));
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(ctrlTime, Float3::ZERO);

		be->SetEmitEndPos(Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2])));
	}
}
//----------------------------------------------------------------------------