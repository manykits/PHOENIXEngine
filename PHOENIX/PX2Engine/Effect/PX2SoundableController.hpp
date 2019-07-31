// PX2SoundableController.hpp

#ifndef PX2SOUNDABLECONTROLLER_HPP
#define PX2SOUNDABLECONTROLLER_HPP

#include "PX2EffectPre.hpp"
#include "PX2EffectableController.hpp"
#include "PX2SoundableObject.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM SoundableController : public EffectableController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(SoundableController);

	public:
		SoundableController ();
		virtual ~SoundableController ();

		virtual void Reset ();

		const SoundableObject *GetSoundableObject () const;

		virtual bool Update (double applicationTime, double elapsedTime);

	protected:
		SoundableObjectPtr mSoundableObject;
	};

	PX2_REGISTER_STREAM(SoundableController);
	typedef Pointer0<SoundableController> SoundableControllerPtr;
#include "PX2SoundableController.inl"

}

#endif
