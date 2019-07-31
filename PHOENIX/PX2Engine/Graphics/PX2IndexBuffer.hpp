// PX2IndexBuffer.hpp

#ifndef PX2INDEXBUFFER_HPP
#define PX2INDEXBUFFER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Buffer.hpp"

namespace PX2
{

	/// ¶¥µãË÷ÒýÀà
	class PX2_ENGINE_ITEM IndexBuffer : public Buffer
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(IndexBuffer);

	public:
		IndexBuffer (int numIndices, int indexSize, Usage usage = BU_STATIC);
		virtual ~IndexBuffer ();

		inline char* GetData () const;

		inline void SetOffset (int offset);
		inline int GetOffset () const;

	protected:
		int mOffset;
	};

	PX2_REGISTER_STREAM(IndexBuffer);
	typedef Pointer0<IndexBuffer> IndexBufferPtr;
#include "PX2IndexBuffer.inl"

}

#endif
