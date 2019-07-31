// PX2EffectModuleCallbacks_Effectable.hpp

#ifndef PX2EFFECTMODULECALLBACKS_EFFECTABLE_HPP
#define PX2EFFECTMODULECALLBACKS_EFFECTABLE_HPP

#include "PX2EffectPre.hpp"

namespace PX2
{
	
	class EffectModule;
	class Effectable;
	class EffectObject;

	// Effectable

	class PX2_ENGINE_ITEM EA_Callbacks
	{
	public:
		static void EA_EmitSizeX (EffectModule *module, Effectable *able, float ctrlTime);

		static void EA_EmitSizeY (EffectModule *module, Effectable *able, float ctrlTime);

		static void EA_EmitSizeZ (EffectModule *module, Effectable *able, float ctrlTime);

		static void EA_EmitSizeXYZ (EffectModule *module, Effectable *able, float ctrlTime);

		static void EA_EmitUV0_Offset (EffectModule *module, Effectable *able, float ctrlTime);
		static void EA_EmitUV0_Speed (EffectModule *module, Effectable *able, float ctrlTime);
		static void EA_EmitUV0_Repeat (EffectModule *module, Effectable *able, float ctrlTime);

		static void EA_EmitColor (EffectModule *module, Effectable *able, float ctrlTime);

		static void EA_EmitAlpha (EffectModule *module, Effectable *able, float ctrlTime);

		static void EA_EmitLife (EffectModule *module, Effectable *able, float ctrlTime);
	};

	// EffectObject

	class PX2_ENGINE_ITEM EO_Callbacks
	{
	public:
		static void EO_SizeXAtAge (EffectModule *module, EffectObject *obj);

		static void EO_SizeXByAge (EffectModule *module, EffectObject *obj);

		static void EO_SizeYAtAge (EffectModule *module, EffectObject *obj);

		static void EO_SizeYByAge (EffectModule *module, EffectObject *obj);

		static void EO_SizeZAtAge (EffectModule *module, EffectObject *obj);

		static void EO_SizeZByAge (EffectModule *module, EffectObject *obj);

		static void EO_SizeXYZAtAge (EffectModule *module, EffectObject *obj);

		static void EO_SizeXYZByAge (EffectModule *module, EffectObject *obj);

		static void EO_UV0_OffsetAtAge (EffectModule *module, EffectObject *obj);
		static void EO_UV0_SpeedAtAge (EffectModule *module, EffectObject *obj);
		static void EO_UV0_SpeedByAge (EffectModule *module, EffectObject *obj);
		static void EO_UV0_RepeatAtAge (EffectModule *module, EffectObject *obj);
		static void EO_UV0_RepeatByAge (EffectModule *module, EffectObject *obj);

		static void EO_ColorAtAge (EffectModule *module, EffectObject *obj);

		static void EO_AlphaAtAge (EffectModule *module, EffectObject *obj);
	};

}

#endif