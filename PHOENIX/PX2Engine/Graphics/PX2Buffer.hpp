// PX2Buffer.hpp

#ifndef PX2BUFFER_HPP
#define PX2BUFFER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class Renderer;

	/// 资源缓冲类
	/**
	* vertex buffers, index buffers, 和textures这些资源从此类派生。
	*/
	class PX2_ENGINE_ITEM Buffer : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Buffer);

	public:
		/// Buffer标记类型
		/**
		* 标记vertex buffers, index buffers, 和textures。
		*/
		enum Usage
		{
			BU_STATIC,
			BU_DYNAMIC,
			BU_RENDERTARGET,
			BU_DEPTHSTENCIL,
			BU_TEXTURE,
			BU_QUANTITY
		};

		// Buffer访问模式
		enum Locking
		{
			BL_READ_ONLY,
			BL_WRITE_ONLY,
			BL_READ_WRITE,
			BL_QUANTITY
		};

	protected:
		Buffer ();
		Buffer (int numElements, int elementSize, Usage usage);
	public:
		virtual ~Buffer ();

		// 成员访问
		inline int GetNumElements () const;
		inline int GetNumMaxElements() const;
		inline int GetElementSize () const;
		inline Usage GetUsage () const;

		/// 设置激活的buffer数量
		/**
		* 这个通常被用在顶点索引激活中。函数不会改变原有数据的存储，只改变
		* Buffer::mNumElements。调用者有职责保存原有的顶点索引数量，并且在完成
		* 顶点索引设置后置回原有的顶点索引数。调用者传递的numElements必须比原有
		* 的顶点索引总数少。
		*/
		inline void SetNumElements (int numElements);

		// 在VertexBuffer和IndexBuffer中需要访问缓存数据。GetData()返回的只能是
		// 只读数据。不要用"char*"进行强制转换，然后写入新的数据，否则渲染器不会
		// 知道数据被改变了。
		inline int GetNumBytes () const;
		inline const char* GetData () const;

		void ClearData ();

		// pdr
	public:
		void *PdrPointer;

	protected:
		int mNumElements;
		int mNumMaxElements;
		int mElementSize;
		Usage mUsage;
		int mNumBytes;
		char* mData;
	};

	PX2_REGISTER_STREAM(Buffer);
	typedef Pointer0<Buffer> BufferPtr;
#include "PX2Buffer.inl"

}

#endif