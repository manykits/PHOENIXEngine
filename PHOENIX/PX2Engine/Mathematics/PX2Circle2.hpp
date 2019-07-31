// PX2Circle2.hpp

#ifndef PX2CIRCLE2_HPP
#define PX2CIRCLE2_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector2.hpp"

namespace PX2
{

	template <typename Real>
	class Circle2
	{
	public:
		Circle2 ();  //< Î´³õÊ¼»¯
		~Circle2 ();

		Circle2 (const Vector2<Real>& center, Real radius);

		Vector2<Real> Center;
		Real Radius;
	};

#include "PX2Circle2.inl"

	typedef Circle2<float> Circle2f;
	typedef Circle2<double> Circle2d;

}

#endif