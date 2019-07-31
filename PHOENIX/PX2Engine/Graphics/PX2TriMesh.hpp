// PX2TriMesh.hpp

#ifndef PX2TRIMESH_HPP
#define PX2TRIMESH_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Triangles.hpp"

namespace PX2
{
	
	/// 三角形网格类
	class PX2_ENGINE_ITEM TriMesh : public Triangles
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(TriMesh);

	protected:
		TriMesh ();
	public:
		TriMesh (VertexFormat* vformat, VertexBuffer* vbuffer,
			IndexBuffer* ibuffer);

		virtual ~TriMesh ();

		virtual int GetNumTriangles () const;
		virtual bool GetTriangle (int i, int& v0, int& v1, int& v2) const;
	};

	PX2_REGISTER_STREAM(TriMesh);
	typedef Pointer0<TriMesh> TriMeshPtr;

}

#endif
