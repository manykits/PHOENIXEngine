// PX2Sphere3.hpp

#ifndef PX2SPHERE3_HPP
#define PX2SPHERE3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector3.hpp"

namespace PX2
{

	template <typename Real>
	class Sphere3
	{
	public:
		Sphere3 ();  //< Î´³õÊ¼»¯
		~Sphere3 ();

		Sphere3 (const Vector3<Real>& center, Real radius);

		Vector3<Real> Center;
		Real Radius;
	};

#include "PX2Sphere3.inl"

	typedef Sphere3<float> Sphere3f;
	typedef Sphere3<double> Sphere3d;

}

#endif