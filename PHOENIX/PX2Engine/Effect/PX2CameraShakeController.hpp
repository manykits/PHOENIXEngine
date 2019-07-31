// PX2CameraShakeController.hpp

#ifndef PX2CAMERASHAKECONTROLLER_HPP
#define PX2CAMERASHAKECONTROLLER_HPP

#include "PX2EffectPre.hpp"
#include "PX2EffectableController.hpp"
#include "PX2CameraShakeObject.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CameraShakeController : public EffectableController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(CameraShakeController);

	public:
		CameraShakeController ();
		virtual ~CameraShakeController ();

		virtual void Reset ();

		const CameraShakeObject *GetCameraShakeObject () const;

		virtual bool Update (double applicationTime, double elapsedTime);

	protected:
		CameraShakeObjectPtr mCameraShakeObject;
	};

#include "PX2CameraShakeController.inl"
	PX2_REGISTER_STREAM(CameraShakeController);
	typedef Pointer0<CameraShakeController> CameraShakeControllerPtr;

}

#endif