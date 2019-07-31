// PX2Polygon2.hpp

#ifndef PX2POLYGON2_HPP
#define PX2POLYGON2_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Vector2.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	// 二维多边形类
	/**
	* 调用者必须确保，顶点顺序是逆时针顺序。
	*/
	template <typename Real>
	class Polygon2
	{
	public:
		Polygon2 (const int numVertices, Vector2<Real>* vertices);
		Polygon2 (const Polygon2& polygon);
		virtual ~Polygon2 ();

		// 赋值
		Polygon2& operator= (const Polygon2& polygon);

		// 成员访问
		int GetNumVertices () const;
		const Vector2<Real>* GetVertices () const;
		const Vector2<Real>& GetVertex (int i) const;

		virtual void SetVertex (int i, const Vector2<Real>& vertex);

		Vector2<Real> ComputeVertexAverage () const;
		Real ComputePerimeterLength () const;
		Real ComputeArea () const;

	protected:
		int mNumVertices;
		Pointer1<Vector2<Real> > mVertices;
	};

#include "PX2Polygon2.inl"

	typedef Polygon2<float> Polygon2f;
	typedef Polygon2<double> Polygon2d;

}

#endif