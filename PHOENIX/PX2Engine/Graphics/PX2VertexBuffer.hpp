// PX2VertexBuffer.hpp

#ifndef PX2VERTEXBUFFER_HPP
#define PX2VERTEXBUFFER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Buffer.hpp"

namespace PX2
{
	
	/// 顶点缓冲区类
	class PX2_ENGINE_ITEM VertexBuffer : public Buffer
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(VertexBuffer);

	public:
		VertexBuffer (int numVertices, int vertexSize, Usage usage = BU_STATIC);
		virtual ~VertexBuffer ();

		// 访问buffer
		inline char* GetData () const;
	};

	PX2_REGISTER_STREAM(VertexBuffer);
	typedef Pointer0<VertexBuffer> VertexBufferPtr;
#include "PX2VertexBuffer.inl"

}

#endif
