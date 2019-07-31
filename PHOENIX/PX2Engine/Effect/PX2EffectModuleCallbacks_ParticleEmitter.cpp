// PX2EffectModuleCallbacks_ParticleEmitter.cpp

#include "PX2EffectModuleCallbacks_ParticleEmitter.hpp"
#include "PX2EffectModule.hpp"
#include "PX2ParticleObject.hpp"
#include "PX2ParticleEmitter.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// ParticleEmitter
//----------------------------------------------------------------------------
void PE_Callbacks::PE_EmitRate (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetEmitRate(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetEmitRate(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_EmitDir (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetEmitDir(module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO));
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(ctrlTime, Float3::ZERO);

		em->SetEmitDir(Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2])));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_EmitSpeed (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetEmitSpeed(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetEmitSpeed(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_EmitAccelerateDir (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetEmitAccelerateDir(module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO));
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(ctrlTime, Float3::ZERO);

		em->SetEmitAccelerateDir(Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2])));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_EmitAccelerate (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetEmitAccelerate(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetEmitAccelerate(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_EmitRotateAxis (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetEmitRotateAxis(module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO));
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(ctrlTime, Float3::ZERO);

		em->SetEmitRotateAxis(Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2])));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_EmitRotateDegree (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetEmitRotateDegree(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetEmitRotateDegree(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_EmitRotateSpeed (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetEmitRotateSpeed(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetEmitRotateSpeed(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_PlacerInLength (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetPlacerInLength(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetPlacerInLength(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_PlacerOutLength (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetPlacerOutLength(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetPlacerOutLength(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_PlacerInWidth (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetPlacerInWidth(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetPlacerInWidth(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_PlacerOutWidth (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetPlacerOutWidth(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetPlacerOutWidth(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_PlacerInHeight (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetPlacerInHeight(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetPlacerInHeight(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------
void PE_Callbacks::PE_PlacerOutHeight (EffectModule *module, Effectable *able, float ctrlTime)
{
	ParticleEmitter *em = (ParticleEmitter*)able;

	if (!module->IsRange())
	{
		em->SetPlacerOutHeight(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		em->SetPlacerOutHeight(Mathf::IntervalRandom(
			module->GetCurveFloatMin().Eval(ctrlTime, 0.0f), 
			module->GetCurveFloatMax().Eval(ctrlTime, 0.0f)));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ParticleObject
//----------------------------------------------------------------------------
void PO_Callbacks::PO_RotSpeedAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->RotSpeed = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		po->RotSpeed = Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_RotSpeedByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->RotSpeed = po->RotSpeedInit * module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		po->RotSpeed = po->RotSpeedInit * Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_RotDegreeAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->RotDegree = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		po->RotDegree = Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_RotAxisAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->RotAxis = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(lastTime, Float3::ZERO);

		po->RotAxis = Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2]));
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_RotDegreeByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->RotDegree = po->RotDegreeInit * module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		po->RotDegree = po->RotDegreeInit * Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_SpeedAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->Speed = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		po->Speed = Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_SpeedByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->Speed = po->SpeedInit * module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		po->Speed = po->SpeedInit * Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_SpeedDirAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->SpeedDir = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(lastTime, Float3::ZERO);

		po->SpeedDir = Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2]));
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_AccelerateAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);
	
	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->Accelerate = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		po->Accelerate = Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_AccelerateByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->Accelerate = po->AccelerateInit * module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		po->Accelerate = po->AccelerateInit * Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_AccelerateDirAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->AccelerateDir = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(lastTime, Float3::ZERO);

		po->AccelerateDir = Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2]));
	}
}
//----------------------------------------------------------------------------
void PO_Callbacks::PO_ObstructByAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	ParticleObject *po = (ParticleObject*)obj;

	if (!module->IsRange())
	{
		po->Obstruct = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(lastTime, Float3::ZERO);
		po->Obstruct = Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2]));
	}
}
//----------------------------------------------------------------------------