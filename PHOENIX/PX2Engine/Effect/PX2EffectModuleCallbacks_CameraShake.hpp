// PX2EffectModuleCallbacks_CameraShake.hpp

#ifndef PX2EFFECTMODULECALLBACKS_CAMERASHAKE_HPP
#define PX2EFFECTMODULECALLBACKS_CAMERASHAKE_HPP

#include "PX2EffectPre.hpp"

namespace PX2
{

	class EffectModule;
	class Effectable;
	class EffectObject;

	class PX2_ENGINE_ITEM CSE_Callbacks
	{
	public:
		static void CSE_EmitStrength (EffectModule *module, Effectable *able, float ctrlTime);
	};

	class PX2_ENGINE_ITEM CSO_Callbacks
	{
	public:
		static void CSO_StrengthAtAge (EffectModule *module, EffectObject *obj);
		static void CSO_StrengthByAge (EffectModule *module, EffectObject *obj);
	};

}

#endif