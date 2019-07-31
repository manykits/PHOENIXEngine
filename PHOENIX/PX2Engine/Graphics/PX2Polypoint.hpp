// PX2Polypoint.hpp

#ifndef PX2POLYPOINT_HPP
#define PX2POLYPOINT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Renderable.hpp"

namespace PX2
{
	
	/// 几何图形点类
	class PX2_ENGINE_ITEM Polypoint : public Renderable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Polypoint);

	public:
		Polypoint (VertexFormat* vformat, VertexBuffer* vbuffer);
		virtual ~Polypoint ();

		int GetMaxNumPoints () const;
		void SetNumPoints (int numPoints);
		inline int GetNumPoints () const;

	protected:
		int mNumPoints;
	};

	PX2_REGISTER_STREAM(Polypoint);
	typedef Pointer0<Polypoint> PolypointPtr;
#include "PX2Polypoint.inl"

}

#endif
