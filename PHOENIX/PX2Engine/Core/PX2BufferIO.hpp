// PX2BufferIO.hpp

#ifndef PX2BUFFERIO_HPP
#define PX2BUFFERIO_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	/// 内存缓冲区读写类
	/**
	* 支持二进制内存缓冲区读写操作。如果你以“只读”的方式打开内存缓冲区，调用写
	* 规则会导致错误；同样的，你以“只写”的方式打开内存缓冲区，调用读操作会出错。
	* 这个类不支持“读写”方式访问。
	* 通常我们使用FileIO::Load读取文件到内存缓冲区，然后使用BufferIO读取具体数值。
	* 保存过程则与此相反。
	*/
	class PX2_ENGINE_ITEM BufferIO
	{
	public:
		/// 缓冲区读写方式
		/**
		* BM_NONE
		* BM_READ			从缓冲区读取（没有字节交换）
		* BM_WRITE			写入缓冲区（没有字节交换）
		* BM_READ_AND_SWAP	从缓冲区读取并且交换（2-，4-和8-输入）
		* BM_WRITE_AND_SWAP	写入缓冲区并且交换（2-，4-和8-输入）
		*/
		enum
		{
			BM_NONE,
			BM_READ,
			BM_WRITE,
			BM_READ_AND_SWAP,
			BM_WRITE_AND_SWAP,

#ifdef PX2_LITTLE_ENDIAN
			BM_DEFAULT_READ = BM_READ,
			BM_DEFAULT_WRITE = BM_WRITE
#else
			BM_DEFAULT_READ = BM_READ_AND_SWAP,
			BM_DEFAULT_WRITE = BM_WRITE_AND_SWAP
#endif
		};

		BufferIO ();
		/// 构造函数
		/**
		* 调用者提供需要被读取或者写入的内存缓冲区，调用者自己负责管理内存。内
		* 存缓冲区不一定必须是动态分配的。
		* 注意：在将数据读取到外部的时候，需要调用GetNumBytes()获得数据量。
		*/
		BufferIO (int numBytesTotal, char* buffer, int mode);
		~BufferIO ();

		bool Open (int numBytesTotal, char* buffer, int mode);
		bool Close ();

		// 隐式转换，用来检测缓冲区打开是否成功。
		inline operator bool () const;

		// 成员访问
		inline const char* GetBuffer () const;
		inline int GetNumBytesTotal () const;
		inline int GetNumBytesProcessed () const;
		inline int GetMode () const;
		bool IncrementNumBytesProcessed (int numBytes);

		// 如果操作成功，返回'true'
		bool Read (size_t itemSize, void* datum);
		bool Read (size_t itemSize, int numItems, void* data);
		bool Write (size_t itemSize, const void* datum);
		bool Write (size_t itemSize, int numItems, const void* data);

	private:
		char* mBuffer;
		int mNumBytesTotal;
		int mNumBytesProcessed;
		int mMode;
	};

#include "PX2BufferIO.inl"

}

#endif