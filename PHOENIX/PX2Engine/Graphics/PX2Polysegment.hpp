// PX2Polysegment.hpp

#ifndef PX2POLYSEGMENT_HPP
#define PX2POLYSEGMENT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Renderable.hpp"

namespace PX2
{

	/// 几何图形线类
	/**
	* 如果contiguous为真，几何图形为连接的线段。举例来说，{V0,V1,V2,V3}所组成的
	* 几何图形为<V0,V1>,<V1,V2>,和<V2,V3>；如果你希望线段是闭合的，最后一个顶点
	* 必须和第一个顶点重复。举例来说，{V0,V1,V2,V3=V0}组成的图形为<V0,V1>,
	* <V1,V2>,和<V2,V0>。如果contiguous为假，几何图形为线段列。举例来说，{V0,V1,
	* V2,V3}组成的几何图形为<V0,V1>，和<V2,V3>，在这样的情况下，顶点个数必须是
	* 偶数个。
	*/
	class PX2_ENGINE_ITEM Polysegment : public Renderable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Polysegment);

	public:
		Polysegment (VertexFormat* vformat, VertexBuffer* vbuffer,
			bool contiguous);

		virtual ~Polysegment ();

		/**
		* 如果是连接线段几何图形，0 <= numSegments <= numVertices-1；如果为线段
		* 集，0 <= numSegments <= numVertices/2。
		*/
		int GetMaxNumSegments () const;
		void SetNumSegments (int numSegments);
		inline int GetNumSegments () const;

		/**
		* 如果线段是连接的，返回true。否则返回false。
		*/
		inline bool GetContiguous () const;

	protected:
		Polysegment () {};
		
		int mNumSegments;
		bool mContiguous;
	};

	PX2_REGISTER_STREAM(Polysegment);
	typedef Pointer0<Polysegment> PolysegmentPtr;
#include "PX2Polysegment.inl"

}

#endif
