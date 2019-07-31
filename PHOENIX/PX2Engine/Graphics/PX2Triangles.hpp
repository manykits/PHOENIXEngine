// PX2Triangles.hpp

#ifndef PX2TRIANGLES_HPP
#define PX2TRIANGLES_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Renderable.hpp"
#include "PX2Float2.hpp"
#include "PX2APoint.hpp"
#include "PX2VertexBufferAccessor.hpp"

namespace PX2
{
	
	/// 三角形几何图形基类
	class PX2_ENGINE_ITEM Triangles : public Renderable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Triangles);

	protected:
		Triangles (PrimitiveType type = PT_TRIANGLES);
		Triangles (PrimitiveType type, VertexFormat* vformat,
			VertexBuffer* vbuffer, IndexBuffer* ibuffer);
	public:
		virtual ~Triangles ();

		virtual int GetNumTriangles () const = 0;
		virtual bool GetTriangle (int i, int& v0, int& v1, int& v2) const = 0;
		bool GetModelTriangle (int i, APoint* modelTriangle) const;
		bool GetWorldTriangle (int i, APoint* worldTriangle) const;

		inline int GetNumVertices () const;
		inline const Transform& GetWorldTransform () const;
		Float3 GetPosition (int v) const; //< 输入的索引v必须在顶点索引buffer中

		/// 几何图形跟新，在PX2Renderable.hpp中看参数说明。
		virtual void UpdateModelSpace (UpdateType type);

	private:
		// 用来支持几何图形更新
		void UpdateModelNormals (VertexBufferAccessor& vba);

		// tangent space更新
		void UpdateModelTangentsUseGeometry (VertexBufferAccessor& vba);
		void UpdateModelTangentsUseTCoords (VertexBufferAccessor& vba);

		///
		/**
		* 当使用UpdateModelTangentsUseTCoords更新时，该函数计算顶点position0处
		* 的tangent向量。所在的三角形的三个点为position0，position1，和
		* position2，按照逆时针顺序排列，三个顶点对应的纹理坐标分别为tcoord0, 
		* tcoord1, 和 tcoord2。
		*/
		static AVector ComputeTangent (
			const APoint& position0, const Float2& tcoord0,
			const APoint& position1, const Float2& tcoord1,
			const APoint& position2, const Float2& tcoord2);
	};

	PX2_REGISTER_STREAM(Triangles);
	typedef Pointer0<Triangles> TrianglesPtr;
#include "PX2Triangles.inl"

}

#endif
