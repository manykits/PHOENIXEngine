// PX2RVector2.hpp

#ifndef PX2RVECTOR2_HPP
#define PX2RVECTOR2_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2RVector.hpp"

namespace PX2
{

template <int ISIZE>
class RVector2 : public RVector<2,ISIZE>
{
public:
    // Construction.
    RVector2 ();
    RVector2 (const RVector2& vec);
    RVector2 (const RVector<2,ISIZE>& vec);
    RVector2 (const Rational<ISIZE>& x, const Rational<ISIZE>& y);

    // Member access.
    inline Rational<ISIZE> X () const;
    inline Rational<ISIZE>& X ();
    inline Rational<ISIZE> Y () const;
    inline Rational<ISIZE>& Y ();

    // Assignment.
    RVector2& operator= (const RVector2& vec);
    RVector2& operator= (const RVector<2,ISIZE>& vec);

    // Returns Dot(this,V).
    Rational<ISIZE> Dot (const RVector2& vec) const;

    // Returns (y,-x).
    RVector2 Perp () const;

    // Returns Cross((x,y,0),(V.x,V.y,0)) = x*V.y - y*V.x
    Rational<ISIZE> DotPerp (const RVector2& vec) const;

protected:
    using RVector<2,ISIZE>::mTuple;
};

#include "PX2RVector2.inl"

}

#endif
