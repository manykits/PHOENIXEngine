// PX2EffectModuleCallbacks_ParticleEmitter.hpp

#ifndef PX2EFFECTMODULECALLBACKS_PARTICLEEMITTER_HPP
#define PX2EFFECTMODULECALLBACKS_PARTICLEEMITTER_HPP

#include "PX2EffectPre.hpp"

namespace PX2
{

	class EffectModule;
	class Effectable;
	class EffectObject;

	// ParticleEmitter

	class PX2_ENGINE_ITEM PE_Callbacks
	{
	public:
		static void PE_EmitRate (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_EmitDir (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_EmitSpeed (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_EmitAccelerateDir (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_EmitAccelerate (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_EmitRotateAxis (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_EmitRotateDegree (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_EmitRotateSpeed (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_PlacerInLength (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_PlacerOutLength (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_PlacerInWidth (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_PlacerOutWidth (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_PlacerInHeight (EffectModule *module, Effectable *able, float ctrlTime);

		static void PE_PlacerOutHeight (EffectModule *module, Effectable *able, float ctrlTime);
	};

	// ParticleObject

	class PX2_ENGINE_ITEM PO_Callbacks
	{
	public:
		static void PO_RotAxisAtAge (EffectModule *module, EffectObject *obj);

		static void PO_RotDegreeAtAge (EffectModule *module, EffectObject *obj);

		static void PO_RotDegreeByAge (EffectModule *module, EffectObject *obj);

		static void PO_RotSpeedAtAge (EffectModule *module, EffectObject *obj);

		static void PO_RotSpeedByAge (EffectModule *module, EffectObject *obj);

		static void PO_SpeedAtAge (EffectModule *module, EffectObject *obj);

		static void PO_SpeedByAge (EffectModule *module, EffectObject *obj);

		static void PO_SpeedDirAtAge (EffectModule *module, EffectObject *obj);

		static void PO_AccelerateAtAge (EffectModule *module, EffectObject *obj);

		static void PO_AccelerateByAge (EffectModule *module, EffectObject *obj);

		static void PO_AccelerateDirAtAge (EffectModule *module, EffectObject *obj);

		static void PO_ObstructByAtAge (EffectModule *module, EffectObject *obj);
	};

}

#endif