// PX2GVector.hpp

#ifndef PX2GVECTOR_HPP
#define PX2GVECTOR_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Math.hpp"
#include "PX2Memory.hpp"
#include <cstring>

namespace PX2
{

	template <typename Real>
	class GVector
	{
	public:
		// Construction and destruction.
		GVector (int size = 0);
		GVector (int size, const Real* tuple);
		GVector (const GVector& vec);
		~GVector ();

		// Coordinate access.
		void SetSize (int size);
		inline int GetSize () const;
		inline operator const Real* () const;
		inline operator Real* ();
		inline const Real& operator[] (int i) const;
		inline Real& operator[] (int i);

		// Assignment.
		GVector& operator= (const GVector& vec);

		// Comparison (for use by STL containers).
		bool operator== (const GVector& vec) const;
		bool operator!= (const GVector& vec) const;
		bool operator<  (const GVector& vec) const;
		bool operator<= (const GVector& vec) const;
		bool operator>  (const GVector& vec) const;
		bool operator>= (const GVector& vec) const;

		// Arithmetic operations.
		GVector operator+ (const GVector& vec) const;
		GVector operator- (const GVector& vec) const;
		GVector operator* (Real scalar) const;
		GVector operator/ (Real scalar) const;
		GVector operator- () const;

		PX2_ENGINE_ITEM
		friend GVector operator* (Real scalar, const GVector& vec)
		{
			return vec*scalar;
		}

		// Arithmetic updates.
		GVector& operator+= (const GVector& vec);
		GVector& operator-= (const GVector& vec);
		GVector& operator*= (Real scalar);
		GVector& operator/= (Real scalar);

		// Vector operations.
		Real Length () const;
		Real SquaredLength () const;
		Real Dot (const GVector& vec) const;
		Real Normalize (Real epsilon = Math<Real>::ZERO_TOLERANCE);

	protected:
		int mSize;
		Real* mTuple;
	};

#include "PX2GVector.inl"

	typedef GVector<float> GVectorf;
	typedef GVector<double> GVectord;

}

#endif