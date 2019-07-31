// PX2Rational.hpp

#ifndef PX2RATIONAL_HPP
#define PX2RATIONAL_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Integer.hpp"

namespace PX2
{

// N is the number of 32-bit words per Integer numerator/denominator
template <int N>
class Rational
{
public:
    // Construction.
    Rational ();  // default rational is 0/1
    Rational (const Rational& rat);
    Rational (const Integer<N>& numer);
    Rational (const Integer<N>& numer, const Integer<N>& denom);

    // Construction converters.
    Rational (int numer);
    Rational (int numer, int denom);
    Rational (float value);
    Rational (double value);

    // Member access.
    inline Integer<N>& Numer ();
    inline Integer<N>& Denom ();
    inline const Integer<N>& Numer () const;
    inline const Integer<N>& Denom () const;

    // Assignment.
    Rational& operator= (const Rational& rat);

    // Comparison.
    bool operator== (const Rational& rat) const;
    bool operator!= (const Rational& rat) const;
    bool operator<= (const Rational& rat) const;
    bool operator<  (const Rational& rat) const;
    bool operator>= (const Rational& rat) const;
    bool operator>  (const Rational& rat) const;

    // Arithmetic operations.
    Rational operator+ (const Rational& rat) const;
    Rational operator- (const Rational& rat) const;
    Rational operator* (const Rational& rat) const;
    Rational operator/ (const Rational& rat) const;
    Rational operator- () const;

    // Arithmetic updates.
    Rational& operator+= (const Rational& rat);
    Rational& operator-= (const Rational& rat);
    Rational& operator*= (const Rational& rat);
    Rational& operator/= (const Rational& rat);

    // Conversions to float and double.
    void ConvertTo (float& value) const;
    void ConvertTo (double& value) const;

    // Compute the absolute value of the rational number.
    Rational Abs () const;

private:
    // Cancel any powers of two common to the numerator and
    // denominator.
    void EliminatePowersOfTwo ();

    Integer<N> mNumer, mDenom;
};

template <int N>
Rational<N> operator+ (const Integer<N>& ival, const Rational<N>& rat);

template <int N>
Rational<N> operator- (const Integer<N>& ival, const Rational<N>& rat);

template <int N>
Rational<N> operator* (const Integer<N>& ival, const Rational<N>& rat);

template <int N>
Rational<N> operator/ (const Integer<N>& ival, const Rational<N>& rat);

#include "PX2Rational.inl"

}

#endif
