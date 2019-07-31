// PX2EffectModuleCallbacks_Effectable.cpp

#include "PX2EffectModuleCallbacks_Effectable.hpp"
#include "PX2EffectModule.hpp"
#include "PX2Effectable.hpp"
#include "PX2EffectObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// Effectable
//----------------------------------------------------------------------------
void EA_Callbacks::EA_EmitSizeX (EffectModule *module, Effectable *able, float ctrlTime)
{
	if (!module->IsRange())
	{
		able->SetEmitSizeX(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(ctrlTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(ctrlTime, 0.0f);
		able->SetEmitSizeX(Mathf::IntervalRandom(valMin, valMax));
	}
}
//----------------------------------------------------------------------------
void EA_Callbacks::EA_EmitSizeY (EffectModule *module, Effectable *able, float ctrlTime)
{
	if (!module->IsRange())
	{
		able->SetEmitSizeY(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(ctrlTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(ctrlTime, 0.0f);
		able->SetEmitSizeY(Mathf::IntervalRandom(valMin, valMax));
	}
}
//----------------------------------------------------------------------------
void EA_Callbacks::EA_EmitSizeZ (EffectModule *module, Effectable *able, float ctrlTime)
{
	if (!module->IsRange())
	{
		able->SetEmitSizeZ(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(ctrlTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(ctrlTime, 0.0f);
		able->SetEmitSizeZ(Mathf::IntervalRandom(valMin, valMax));
	}
}
//----------------------------------------------------------------------------
void EA_Callbacks::EA_EmitSizeXYZ (EffectModule *module, Effectable *able, float ctrlTime)
{
	if (!module->IsRange())
	{
		float size = module->GetCurveFloatMin().Eval(ctrlTime, 0.0f);
		able->SetEmitSizeX(size);
		able->SetEmitSizeY(size);
		able->SetEmitSizeZ(size);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(ctrlTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(ctrlTime, 0.0f);
		float size = Mathf::IntervalRandom(valMin, valMax);
		able->SetEmitSizeX(size);
		able->SetEmitSizeY(size);
		able->SetEmitSizeZ(size);
	}
}
//----------------------------------------------------------------------------
void EA_Callbacks::EA_EmitUV0_Offset (EffectModule *module, Effectable *able, float ctrlTime)
{
	if (!module->IsRange())
	{
		Float3 val = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO);
		able->SetEmitUV0_Offset(Float2(val[0], val[1]));
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(ctrlTime, Float3::ZERO);

		able->SetEmitUV0_Offset(Float2(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1])));
	}
}
//----------------------------------------------------------------------------
void EA_Callbacks::EA_EmitUV0_Speed (EffectModule *module, Effectable *able, float ctrlTime)
{
	if (!module->IsRange())
	{
		Float3 val = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO);
		able->SetEmitUV0_Speed(Float2(val[0], val[1]));
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(ctrlTime, Float3::ZERO);

		able->SetEmitUV0_Speed(Float2(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1])));
	}
}
//----------------------------------------------------------------------------
void EA_Callbacks::EA_EmitUV0_Repeat (EffectModule *module, Effectable *able, float ctrlTime)
{
	if (!module->IsRange())
	{
		Float3 val = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::UNIT);
		able->SetEmitUV0_Repeat(Float2(val[0], val[1]));
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::UNIT);
		Float3 valMax = module->GetCurveFloat3Max().Eval(ctrlTime, Float3::UNIT);

		able->SetEmitUV0_Repeat(Float2(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1])));
	}
}
//----------------------------------------------------------------------------
void EA_Callbacks::EA_EmitColor (EffectModule *module, Effectable *able, float ctrlTime)
{
	if (!module->IsRange())
	{
		able->SetEmitColor(module->GetCurveFloat3Min().Eval(ctrlTime, Float3::WHITE));
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(ctrlTime, Float3::WHITE);
		Float3 valMax = module->GetCurveFloat3Max().Eval(ctrlTime, Float3::WHITE);

		able->SetEmitColor(Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2])));
	}
}
//----------------------------------------------------------------------------
void EA_Callbacks::EA_EmitAlpha (EffectModule *module, Effectable *able, float ctrlTime)
{
	if (!module->IsRange())
	{
		able->SetEmitAlpha(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(ctrlTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(ctrlTime, 0.0f);
		able->SetEmitAlpha(Mathf::IntervalRandom(valMin, valMax));
	}
}
//----------------------------------------------------------------------------
void EA_Callbacks::EA_EmitLife (EffectModule *module, Effectable *able, float ctrlTime)
{
	if (!module->IsRange())
	{
		able->SetEmitLife(module->GetCurveFloatMin().Eval(ctrlTime, 0.0f));
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(ctrlTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(ctrlTime, 0.0f);
		able->SetEmitLife(Mathf::IntervalRandom(valMin, valMax));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// EffectObject
//----------------------------------------------------------------------------
void EO_Callbacks::EO_SizeXAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		obj->SizeX = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		obj->SizeX = Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_SizeXByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		obj->SizeX = obj->SizeXInit * module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		obj->SizeX = obj->SizeXInit * Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_SizeYAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		obj->SizeY = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		obj->SizeY = Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_SizeYByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		obj->SizeY = obj->SizeYInit * module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		obj->SizeY = obj->SizeYInit * Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_SizeZAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		obj->SizeZ = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		obj->SizeZ = Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_SizeZByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		obj->SizeZ = obj->SizeZInit * module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		obj->SizeZ = obj->SizeZInit * Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_SizeXYZAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		obj->SizeX = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		obj->SizeY = obj->SizeX;
		obj->SizeZ = obj->SizeX;
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		obj->SizeX = Mathf::IntervalRandom(valMin, valMax);
		obj->SizeY = obj->SizeX;
		obj->SizeZ = obj->SizeX;
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_SizeXYZByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		float val = module->GetCurveFloatMin().Eval(lastTime, 0.0f);

		obj->SizeX = obj->SizeXInit * val;
		obj->SizeY = obj->SizeYInit * val;
		obj->SizeZ = obj->SizeZInit * val;
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		float val = Mathf::IntervalRandom(valMin, valMax);

		obj->SizeX = obj->SizeXInit * val;
		obj->SizeY = obj->SizeYInit * val;
		obj->SizeZ = obj->SizeZInit * val;
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_UV0_OffsetAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		Float3 val = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
		obj->UV0Offset = Float2(val[0], val[1]);
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(lastTime, Float3::ZERO);

		obj->UV0Offset = Float2(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]));
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_UV0_SpeedAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		Float3 val = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
		obj->UV0Speed = Float2(val[0], val[1]);
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(lastTime, Float3::ZERO);

		obj->UV0Speed = Float2(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]));
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_UV0_SpeedByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		Float3 val = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
		obj->UV0Speed[0] = obj->UV0SpeedInit[0] * val[0];
		obj->UV0Speed[1] = obj->UV0SpeedInit[1] * val[1];
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(lastTime, Float3::ZERO);
		Float3 valMax = module->GetCurveFloat3Max().Eval(lastTime, Float3::ZERO);

		obj->UV0Speed[0] = obj->UV0SpeedInit[0] * Mathf::IntervalRandom(valMin[0], valMax[0]);
		obj->UV0Speed[1] = obj->UV0SpeedInit[1] * Mathf::IntervalRandom(valMin[1], valMax[1]);
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_UV0_RepeatAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		Float3 val = module->GetCurveFloat3Min().Eval(lastTime, Float3::UNIT);
		obj->UV0Repeat = Float2(val[0], val[1]);
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(lastTime, Float3::UNIT);
		Float3 valMax = module->GetCurveFloat3Max().Eval(lastTime, Float3::UNIT);

		obj->UV0Repeat = Float2(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]));
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_UV0_RepeatByAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		Float3 val = module->GetCurveFloat3Min().Eval(lastTime, Float3::UNIT);
		obj->UV0Repeat[0] = obj->UV0RepeatInit[0] * val[0];
		obj->UV0Repeat[1] = obj->UV0RepeatInit[1] * val[1];
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(lastTime, Float3::UNIT);
		Float3 valMax = module->GetCurveFloat3Max().Eval(lastTime, Float3::UNIT);

		obj->UV0Repeat[0] = obj->UV0RepeatInit[0] * Mathf::IntervalRandom(valMin[0], valMax[0]);
		obj->UV0Repeat[1] = obj->UV0RepeatInit[1] * Mathf::IntervalRandom(valMin[1], valMax[1]);
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_ColorAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);
	
	if (!module->IsRange())
	{
		obj->Color = module->GetCurveFloat3Min().Eval(lastTime, Float3::WHITE);
	}
	else
	{
		Float3 valMin = module->GetCurveFloat3Min().Eval(lastTime, Float3::WHITE);
		Float3 valMax = module->GetCurveFloat3Max().Eval(lastTime, Float3::WHITE);

		obj->Color = Float3(
			Mathf::IntervalRandom(valMin[0], valMax[0]),
			Mathf::IntervalRandom(valMin[1], valMax[1]),
			Mathf::IntervalRandom(valMin[2], valMax[2]));
	}
}
//----------------------------------------------------------------------------
void EO_Callbacks::EO_AlphaAtAge (EffectModule *module, EffectObject *obj)
{
	float lastTime = module->GetModuleTime(obj->Age);

	if (!module->IsRange())
	{
		obj->Alpha = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
	}
	else
	{
		float valMin = module->GetCurveFloatMin().Eval(lastTime, 0.0f);
		float valMax = module->GetCurveFloatMax().Eval(lastTime, 0.0f);
		obj->Alpha = Mathf::IntervalRandom(valMin, valMax);
	}
}
//----------------------------------------------------------------------------