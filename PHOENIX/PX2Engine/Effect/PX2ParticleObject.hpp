// PX2ParticleObject.hpp

#ifndef PX2PARTICLEOBJECT_HPP
#define PX2PARTICLEOBJECT_HPP

#include "PX2EffectPre.hpp"
#include "PX2EffectObject.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ParticleObject : public EffectObject
	{
	public:
		ParticleObject ();
		virtual ~ParticleObject ();

		virtual bool Update (Effectable *able, float elapsedTime);

		APoint		Pos;
		APoint		PrePos;

		AVector		DVectorInit;
		AVector		UVectorInit;
		AVector		RVectorInit;
		AVector		DVector;
		AVector		UVector;
		AVector		RVector;

		float		RotSpeedInit;
		float		RotSpeed;
		float		RotDegreeInit;
		float		RotDegree;

		AVector		RotAxisInit;
		AVector		RotAxis;

		float		SpeedInit;
		float		Speed;
		AVector		SpeedDirInit;
		AVector		SpeedDir;

		float		AccelerateInit;
		float		Accelerate;
		AVector		AccelerateDirInit;
		AVector		AccelerateDir;

		AVector		Obstruct;

		int			BlastIndex;
	};

}

#endif