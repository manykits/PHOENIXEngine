// PX2Query2Rational.hpp

#ifndef PX2QUERY2RATIONAL_HPP
#define PX2QUERY2RATIONAL_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Query2.hpp"
#include "PX2Memory.hpp"
#include "PX2RVector2.hpp"

namespace PX2
{

template <typename Real>
class Query2Rational : public Query2<Real>
{
public:
    // The components of the input vertices are represented exactly as
    // rational values.
    Query2Rational (int numVertices, const Vector2<Real>* vertices);
    virtual ~Query2Rational ();

    // Run-time type information.
    virtual Query::Type GetType () const;

    // Queries about the relation of a point to various geometric objects.

    virtual int ToLine (int i, int v0, int v1) const;
    virtual int ToLine (const Vector2<Real>& test, int v0, int v1) const;

    virtual int ToTriangle (int i, int v0, int v1, int v2) const;
    virtual int ToTriangle (const Vector2<Real>& test, int v0, int v1,
        int v2) const;

    virtual int ToCircumcircle (int i, int v0, int v1, int v2) const;
    virtual int ToCircumcircle (const Vector2<Real>& test, int v0, int v1,
        int v2) const;

    // Helper functions.
    typedef Rational<4*sizeof(Real)> QRational;
    typedef RVector2<4*sizeof(Real)> QRVector;

    static QRational Dot (QRational& x0, QRational& y0, QRational& x1,
        QRational& y1);

    static QRational Det2 (QRational& x0, QRational& y0, QRational& x1,
        QRational& y1);

    static QRational Det3 (QRational& x0, QRational& y0, QRational& z0,
        QRational& x1, QRational& y1, QRational& z1, QRational& x2,
        QRational& y2, QRational& z2);

private:
    using Query2<Real>::mNumVertices;
    using Query2<Real>::mVertices;

    void Convert (int numIndices, int* indices) const;
    int ToLine (const QRVector& ratTest, int v0, int v1) const;
    int ToTriangle (const QRVector& ratTest, int v0, int v1, int v2) const;
    int ToCircumcircle (const QRVector& ratTest, int v0, int v1, int v2)
        const;

    // Caching for rational representations of the input.  The conversion of
    // floating-point numbers to Rational form is slow, so it is better to
    // keep track of which values have been converted.
    mutable QRVector* mRVertices;
    mutable bool* mEvaluated;
};

#include "PX2Query2Rational.inl"

typedef Query2Rational<float> Query2Rationalf;
typedef Query2Rational<double> Query2Rationald;

}

#endif
