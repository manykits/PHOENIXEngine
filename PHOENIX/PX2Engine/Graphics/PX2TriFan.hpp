// PX2TriFan.hpp

#ifndef PX2TRIFAN_HPP
#define PX2TRIFAN_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Triangles.hpp"

namespace PX2
{
	
	/// »˝Ω«–Œ…»¿‡
	class PX2_ENGINE_ITEM TriFan : public Triangles
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(TriFan);

	public:
		TriFan (VertexFormat* vformat, VertexBuffer* vbuffer, int indexSize);
		TriFan (VertexFormat* vformat, VertexBuffer* vbuffer,
			IndexBuffer* ibuffer);

		virtual ~TriFan ();

		virtual int GetNumTriangles () const;
		virtual bool GetTriangle (int i, int& v0, int& v1, int& v2) const;
	};

	PX2_REGISTER_STREAM(TriFan);
	typedef Pointer0<TriFan> TriFanPtr;

}

#endif
