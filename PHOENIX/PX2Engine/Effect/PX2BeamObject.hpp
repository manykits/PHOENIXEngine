// PX2BeamObject.hpp

#ifndef PX2BEAMOBJECT_HPP
#define PX2BEAMOBJECT_HPP

#include "PX2EffectPre.hpp"
#include "PX2EffectObject.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM BeamObject : public EffectObject
	{
	public:
		BeamObject ();
		virtual ~BeamObject ();

		APoint StartPos;
		APoint EndPos;
		std::vector<APoint> Points;
	};

}

#endif