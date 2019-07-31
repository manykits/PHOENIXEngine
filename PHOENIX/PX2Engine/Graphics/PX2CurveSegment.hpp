// PX2CurveSegment.hpp

#ifndef PX2CURVESEGMENT_HPP
#define PX2CURVESEGMENT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CurveSegment : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(CurveSegment);

	protected:
		CurveSegment (float umin, float umax);
	public:
		virtual ~CurveSegment ();

		// The parametric domain is umin <= u <= umax.
		float GetUMin () const;
		float GetUMax () const;

		// Position and derivatives up to third order.
		virtual APoint P (float u) const = 0;
		virtual AVector PU (float u) const = 0;
		virtual AVector PUU (float u) const = 0;
		virtual AVector PUUU (float u) const = 0;

		// Differential geometric quantities.
		AVector Tangent (float u) const;
		AVector Normal (float u) const;
		AVector Binormal (float u) const;
		void GetFrame (float u, APoint& position, AVector& tangent,
			AVector& normal, AVector& binormal) const;
		float Curvature (float u) const;
		float Torsion (float u) const;

	protected:
		float mUMin, mUMax;
	};

	PX2_REGISTER_STREAM(CurveSegment);
	typedef Pointer0<CurveSegment> CurveSegmentPtr;

}

#endif