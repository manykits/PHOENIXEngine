// PX2SoundableObject.hpp

#ifndef PX2SOUNDABLEOBJECT_HPP
#define PX2SOUNDABLEOBJECT_HPP

#include "PX2EffectPre.hpp"
#include "PX2EffectObject.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM SoundableObject : public EffectObject
	{
	public:
		SoundableObject ();
		virtual ~SoundableObject ();

		virtual bool Update (Effectable *able, float elapsedTime);

		float VolumeInit;
		float Volume;
	};
	typedef Pointer0<SoundableObject> SoundableObjectPtr;

}

#endif