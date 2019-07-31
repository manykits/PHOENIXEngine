// PX2RVector.hpp

#ifndef PX2RVECTOR_HPP
#define PX2RVECTOR_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Rational.hpp"

namespace PX2
{

template <int VSIZE, int ISIZE>
class RVector
{
public:
    // Construction.
    RVector ();
    RVector (const RVector& vec);

    // Coordinate access.
    inline operator const Rational<ISIZE>* () const;
    inline operator Rational<ISIZE>* ();
    inline const Rational<ISIZE>& operator[] (int i) const;
    inline Rational<ISIZE>& operator[] (int i);

    // Assignment.
    RVector& operator= (const RVector& vec);

    // Comparison.
    bool operator== (const RVector& vec) const;
    bool operator!= (const RVector& vec) const;
    bool operator<  (const RVector& vec) const;
    bool operator<= (const RVector& vec) const;
    bool operator>  (const RVector& vec) const;
    bool operator>= (const RVector& vec) const;

    // Arithmetic operations.
    RVector operator+ (const RVector& vec) const;
    RVector operator- (const RVector& vec) const;
    RVector operator* (const Rational<ISIZE>& scalar) const;
    RVector operator/ (const Rational<ISIZE>& scalar) const;
    RVector operator- () const;

    // Arithmetic updates.
    RVector& operator+= (const RVector& vec);
    RVector& operator-= (const RVector& vec);
    RVector& operator*= (const Rational<ISIZE>& scalar);
    RVector& operator/= (const Rational<ISIZE>& scalar);

    // Vector operations.
    Rational<ISIZE> SquaredLength () const;
    Rational<ISIZE> Dot (const RVector& vec) const;

protected:
    // Support for comparisons.
    int CompareArrays (const RVector& vec) const;

    Rational<ISIZE> mTuple[VSIZE];
};

template <int VSIZE, int ISIZE>
RVector<VSIZE,ISIZE> operator* (const Rational<ISIZE>& scalar,
    const RVector<VSIZE,ISIZE>& vec);

#include "PX2RVector.inl"

}

#endif
