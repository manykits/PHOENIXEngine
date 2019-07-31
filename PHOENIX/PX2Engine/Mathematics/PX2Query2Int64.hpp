// PX2Query2Int64.hpp

#ifndef PX2QUERY2INT64_HPP
#define PX2QUERY2INT64_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Query2.hpp"

namespace PX2
{

template <typename Real>
class Query2Int64 : public Query2<Real>
{
public:
    // The components of the input vertices are truncated to 64-bit integer
    // values, so you should guarantee that the vertices are sufficiently
    // large to give a good distribution of numbers.
    Query2Int64 (int numVertices, const Vector2<Real>* vertices);

    // Run-time type information.
    virtual Query::Type GetType () const;

    // Queries about the relation of a point to various geometric objects.

    virtual int ToLine (const Vector2<Real>& test, int v0, int v1) const;

    virtual int ToCircumcircle (const Vector2<Real>& test, int v0, int v1,
        int v2) const;

    // Helper functions.
    static int64_t Dot (int64_t x0, int64_t y0, int64_t x1, int64_t y1);
    static int64_t Det2 (int64_t x0, int64_t y0, int64_t x1, int64_t y1);
    static int64_t Det3 (int64_t x0, int64_t y0, int64_t z0, int64_t x1,
        int64_t y1, int64_t z1, int64_t x2, int64_t y2, int64_t z2);

private:
    using Query2<Real>::mVertices;
};

#include "PX2Query2Int64.inl"

typedef Query2Int64<float> Query2Int64f;
typedef Query2Int64<double> Query2Int64d;

}

#endif
