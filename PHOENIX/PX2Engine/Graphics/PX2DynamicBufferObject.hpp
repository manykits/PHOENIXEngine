// PX2DynamicBufferObject.hpp

#ifndef PX2DYNAMICBUFFEROBJECT_HPP
#define PX2DYNAMICBUFFEROBJECT_HPP

#include "PX2VertexFormat.hpp"
#include "PX2VertexBuffer.hpp"
#include "PX2IndexBuffer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM DBObject_V
	{
	public:
		DBObject_V ();

		VertexBuffer *Buf;
		int Offset;
		int Num;
	};
	typedef Pointer0<DBObject_V> DBObject_VPtr;

	class PX2_ENGINE_ITEM DBObject_I
	{
	public:
		DBObject_I ();

		IndexBuffer *Buf;
		int Offset;
		int Num;
	};
	typedef Pointer0<DBObject_I> DBObject_IPtr;

}

#endif