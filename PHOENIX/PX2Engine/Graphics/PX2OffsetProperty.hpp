// PX2OffsetProperty.hpp

#ifndef PX2OFFSETPROPERTY_HPP
#define PX2OFFSETPROPERTY_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM OffsetProperty : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(OffsetProperty);

	public:
		OffsetProperty ();
		virtual ~OffsetProperty ();

		static bool IsEuqal(OffsetProperty *propA, OffsetProperty *propB);

		// Set whether offset should be enabled for the various polygon drawing
		// modes (fill, line, point).
		bool FillEnabled;   // default: false
		bool LineEnabled;   // default: false
		bool PointEnabled;  // default: false

		bool AllowRed;
		bool AllowGreen;
		bool AllowBlue;
		bool AllowAlpha;

		// The offset is Scale*dZ + Bias*r where dZ is the change in depth
		// relative to the screen space area of the poly, and r is the smallest
		// resolvable depth difference.  Negative values move polygons closer to
		// the eye.
		float Scale;  // default: 0
		float Bias;   // default: 0
	};

	PX2_REGISTER_STREAM(OffsetProperty);
	typedef Pointer0<OffsetProperty> OffsetPropertyPtr;

}

#endif
