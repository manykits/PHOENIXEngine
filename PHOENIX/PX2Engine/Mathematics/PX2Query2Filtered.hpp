// PX2Query2Filtered.hpp

#ifndef PX2QUERY2FILTERED_HPP
#define PX2QUERY2FILTERED_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Query2Rational.hpp"
#include "PX2Vector2.hpp"

namespace PX2
{

template <typename Real>
class Query2Filtered : public Query2<Real>
{
public:
    // The base class handles floating-point queries.  Each query involves
    // comparing a determinant to zero.  If the determinant is sufficiently
    // close to zero, numerical round-off errors may cause the determinant
    // sign to be misclassified.  To avoid this, the query is repeated with
    // exact rational arithmetic.  You specify the closeness to zero for the
    // switch to rational arithmetic via 'uncertainty', a value in the
    // interval [0,1].  The uncertainty of 0 causes the class to behave
    // as if it were Query2.  The uncertainty of 1 causes the class to
    // behave as if it were Query2Rational.
    Query2Filtered (int numVertices, const Vector2<Real>* vertices,
        Real uncertainty);
    virtual ~Query2Filtered ();

    // Run-time type information.
    virtual Query::Type GetType () const;

    // Queries about the relation of a point to various geometric objects.

    virtual int ToLine (const Vector2<Real>& test, int v0, int v1) const;

    virtual int ToCircumcircle (const Vector2<Real>& test, int v0, int v1,
        int v2) const;

private:
    using Query2<Real>::mVertices;
    using Query2<Real>::Sort;
    using Query2<Real>::Det2;
    using Query2<Real>::Det3;

    Query2Rational<Real> mRQuery;
    Real mUncertainty;
};

#include "PX2Query2Filtered.inl"

typedef Query2Filtered<float> Query2Filteredf;
typedef Query2Filtered<double> Query2Filteredd;

}

#endif
