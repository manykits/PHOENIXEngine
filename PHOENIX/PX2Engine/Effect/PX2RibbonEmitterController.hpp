// PX2RibbonEmitterController.hpp

#ifndef PX2RIBBONEMITTERCONTROLLER_HPP
#define PX2RIBBONEMITTERCONTROLLER_HPP

#include "PX2EffectPre.hpp"
#include "PX2EffectableController.hpp"
#include "PX2RibbonObject.hpp"
#include "PX2Smoother.hpp"

 namespace PX2
{

	 class PX2_ENGINE_ITEM RibbonEmitterController : public EffectableController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(RibbonEmitterController);

	public:
		RibbonEmitterController ();
		virtual ~RibbonEmitterController ();

		virtual void Reset ();

		virtual bool Update (double applicationTime, double elapsedTime);

public_internal:
		const std::list<RibbonObject> &GetRibbonObjects () const;
		int StartRandomIndex;

	protected:
		APoint mLastPos;
		std::list<RibbonObject> mRibbonObjects;
		Smoother<AVector> *mSmoother;
	};

#include "PX2RibbonEmitterController.inl"
	PX2_REGISTER_STREAM(RibbonEmitterController);
	typedef Pointer0<RibbonEmitterController> RibbonEmitterControllerPtr;

}

#endif