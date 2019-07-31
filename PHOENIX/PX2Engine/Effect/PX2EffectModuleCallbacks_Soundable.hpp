// PX2EffectModuleCallbacks_Soundable.hpp

#ifndef PX2EFFECTMODULECALLBACKS_SOUNDABLE_HPP
#define PX2EFFECTMODULECALLBACKS_SOUNDABLE_HPP

#include "PX2EffectPre.hpp"

namespace PX2
{

	class EffectModule;
	class Effectable;
	class EffectObject;
	
	class PX2_ENGINE_ITEM SE_Callbacks
	{
	public:
		static void SE_EmitVolume (EffectModule *module, Effectable *able, float ctrlTime);
	};

	class PX2_ENGINE_ITEM SO_Callbacks
	{
	public:
		static void SO_VolumeAtAge (EffectModule *module, EffectObject *obj);
		static void SO_VolumeByAge (EffectModule *module, EffectObject *obj);
	};

}

#endif