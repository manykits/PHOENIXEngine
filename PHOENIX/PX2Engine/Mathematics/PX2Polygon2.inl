// PX2Polygon2.inl

//----------------------------------------------------------------------------
template <typename Real>
Polygon2<Real>::Polygon2 (int numVertices, Vector2<Real>* vertices)
:
mNumVertices(numVertices)
{
	// 多边形至少是一个三角形
	assertion(numVertices >= 4 && vertices,
		"Invalid input to Polygon2 constructor\n");

	mVertices = vertices;
}
//----------------------------------------------------------------------------
template <typename Real>
Polygon2<Real>::Polygon2 (const Polygon2& polygon)
{
	mVertices = 0;
	*this = polygon;
}
//----------------------------------------------------------------------------
template <typename Real>
Polygon2<Real>::~Polygon2 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
Polygon2<Real>& Polygon2<Real>::operator= (const Polygon2& polygon)
{
	mNumVertices = polygon.mNumVertices;
	mVertices = polygon.mVertices;
	return *this;
}
//----------------------------------------------------------------------------
template <typename Real>
int Polygon2<Real>::GetNumVertices () const
{
	return mNumVertices;
}
//----------------------------------------------------------------------------
template <typename Real>
const Vector2<Real>* Polygon2<Real>::GetVertices () const
{
	return mVertices;
}
//----------------------------------------------------------------------------
template <typename Real>
const Vector2<Real>& Polygon2<Real>::GetVertex (int i) const
{
	assertion(0 <= i && i < mNumVertices, "Invalid index in GetVertex\n");
	return mVertices[i];
}
//----------------------------------------------------------------------------
template <typename Real>
void Polygon2<Real>::SetVertex (int i, const Vector2<Real>& vertex)
{
	assertion(0 <= i && i < mNumVertices, "Invalid index in SetVertex\n");
	mVertices[i] = vertex;
}
//----------------------------------------------------------------------------
template <typename Real>
Vector2<Real> Polygon2<Real>::ComputeVertexAverage () const
{
	Vector2<Real> average = mVertices[0];
	for (int i = 1; i < mNumVertices; ++i)
	{
		average += mVertices[i];
	}
	average /= (Real)mNumVertices;
	return average;
}
//----------------------------------------------------------------------------
template <typename Real>
Real Polygon2<Real>::ComputePerimeterLength () const
{
	Real perimeterLength = (Real)0;
	for (int i0 = 0, i1 = 1; i1 < mNumVertices; ++i0, ++i1)
	{
		Vector2<Real> edge = mVertices[i1] - mVertices[i0];
		perimeterLength += edge.Length();
	}
	return perimeterLength;
}
//----------------------------------------------------------------------------
template <typename Real>
Real Polygon2<Real>::ComputeArea () const
{
	const int last = mNumVertices - 1;
	Real area = mVertices[0][0]*(mVertices[1][1] - mVertices[last][1]) +
		mVertices[last][0]*(mVertices[0][1] - mVertices[last-1][1]);

	for (int im1 = 0, i = 1, ip1 = 2; i < last; ++im1, ++i, ++ip1)
	{
		area += mVertices[i][0]*(mVertices[ip1][1] - mVertices[im1][1]);
	}

	area *= (Real)0.5;

	return area;
}
//----------------------------------------------------------------------------
