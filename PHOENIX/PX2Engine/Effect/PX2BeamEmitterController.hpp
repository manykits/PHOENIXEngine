// PX2BeamEmitterControl.hpp

#ifndef PX2BEAMEMITTERCONTROL_HPP
#define PX2BEAMEMITTERCONTROL_HPP

#include "PX2EffectPre.hpp"
#include "PX2EffectableController.hpp"
#include "PX2BeamObject.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM BeamEmitterController : public EffectableController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(BeamEmitterController);

	public:
		BeamEmitterController ();
		virtual ~BeamEmitterController ();

		virtual void Reset ();

		virtual bool Update (double applicationTime, double elapsedTime);

public_internal:
		const std::vector<BeamObject> &GetBeamObjects () const;

	protected:
		void EmitABeam (float ctrlTime);
		virtual void OnNewAEffectObject (EffectObject *obj);

		void GernerateLFPoints (std::vector<float>& points, int frequency, 
			int type, float param0, float param1);
		void GernerateLFPointsSin (std::vector<float>& points, int frequency, 
			float param0, float param1, float param2);

		float mNumNewBeamExcess;
		std::vector<BeamObject> mBeamObjects;
	};

#include "PX2BeamEmitterController.inl"

	PX2_REGISTER_STREAM(BeamEmitterController);
	typedef Pointer0<BeamEmitterController> BeamEmitterControllerPtr;

}

#endif