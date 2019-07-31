// PX2RibbonObject.hpp

#ifndef PX2RIBBONOBJECT_HPP
#define PX2RIBBONOBJECT_HPP

#include "PX2EffectPre.hpp"
#include "PX2EffectObject.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM RibbonObject : public EffectObject
	{
	public:
		RibbonObject ();
		virtual ~RibbonObject ();

		virtual bool Update (Effectable *able, float elapsedTime);

		float		Length;
		float		LengthOnCompleteOneSegment;

		APoint		Pos;
		APoint		PrePos;

		AVector		DVector;
		AVector		MinusDepthVector;
	};

}

#endif