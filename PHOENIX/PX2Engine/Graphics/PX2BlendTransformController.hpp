// PX2BlendTransformController.hpp

#ifndef PX2BLENDTRANSFORMCONTROLLER_HPP
#define PX2BLENDTRANSFORMCONTROLLER_HPP

#include "PX2TransformController.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM BlendTransformController : public TransformController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(BlendTransformController);

	public:
		BlendTransformController (TransformController* controller0,
			TransformController* controller1, bool rsMatrices,
			bool geometricRotation = false, bool geometricScale = false);

		virtual ~BlendTransformController ();

		// Member access.  The weight w is a number for which 0 <= w <= 1.
		void SetController0 (TransformController *ctrl0);
		void SetController1 (TransformController *ctrl1);
		void SetController01 (TransformController *ctrl0, TransformController *ctrl1);
		void PushController (TransformController *ctrl);
		inline TransformController* GetController0 () const;
		inline TransformController* GetController1 () const;
		inline bool GetRSMatrices () const;
		inline void SetWeight (float weight);
		inline float GetWeight () const;

		// The animation update.  The application time is in milliseconds.
		virtual bool Update(double applicationTime, double elapsedTime);

	public_internal:
		virtual void SetControlledable(Controlledable* object);

	protected:
		TransformControllerPtr mController0, mController1;
		float mWeight;
		bool mRSMatrices, mGeometricRotation, mGeometricScale;
	};

	PX2_REGISTER_STREAM(BlendTransformController);
	typedef Pointer0<BlendTransformController> BlendTransformControllerPtr;
#include "PX2BlendTransformController.inl"

}

#endif
