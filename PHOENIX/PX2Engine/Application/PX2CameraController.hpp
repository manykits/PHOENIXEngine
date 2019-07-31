// PX2CameraController.hpp

#ifndef PX2CAMERACONTROLLER_HPP
#define PX2CAMERACONTROLLER_HPP

#include "PX2Controller.hpp"
#include "PX2Movable.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CameraController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(CameraController);

	public:
		CameraController();
		virtual ~CameraController();

		void SetFocusMovable(Movable *mov);
		Movable *GetFocursMovable() const;

		void SetDistance(float distance);
		float GetDistance() const;

		void SetHorAngle(float degree);
		float GetHorAngle() const;
		
		virtual void OnEvent(Event *ent);

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);

		Movable *mFocusMovable;
		float mDistance;
		float mHorAngle;
		float mVerAngle;
		bool mIsPressed;
		APoint mLastMouseButtonPos;
	};

	PX2_REGISTER_STREAM(CameraController);
	typedef Pointer0<CameraController> CameraControllerPtr;

}

#endif