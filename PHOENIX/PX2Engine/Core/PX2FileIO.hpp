// PX2FileIO.hpp

#ifndef PX2FILEIO_HPP
#define PX2FILEIO_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	/// 文件读写类
	/**
	* 支持二进制文件读写。如果你以“只读”的方式打开文件，调用写操作会导致错误；
	* 同样的，如果你以“只写”方式打开文件，调用读操作会出现错误。这个类不同时
	* 支持“读写”访问。这个类封装标准文件操作。
	*/
	class PX2_ENGINE_ITEM FileIO
	{
	public:
		/// 文件读取方式
		/**
		* FM_READ			从磁盘读取（没有字节交换）
		* FM_WRITE			写入磁盘（没有字节交换）
		* FM_READ_AND_SWAP	从磁盘读取并且交换（2-，4-和8-输入）
		* FM_WRITE_AND_SWAP	写入磁盘并且交换（2-，4-和8-输入）
		*/
		enum
		{
			FM_NONE,
			FM_READ,
			FM_WRITE,
			FM_READ_AND_SWAP,
			FM_WRITE_AND_SWAP,

#ifdef PX2_LITTLE_ENDIAN
			FM_DEFAULT_READ = FM_READ,
			FM_DEFAULT_WRITE = FM_WRITE
#else
			FM_DEFAULT_READ = FM_READ_AND_SWAP,
			FM_DEFAULT_WRITE = FM_WRITE_AND_SWAP
#endif
		};

		FileIO ();
		FileIO (const std::string& filename, int mode);
		~FileIO ();

		bool Open (const std::string& filename, int mode);
		bool Close ();

		// 隐式转换，用来检测文件是否成功打开。
		operator bool () const;

		inline int GetMode () const;

		enum SeekMode
		{
			SM_SET = 0,
			SM_CUR = 1,
			SM_END = 2
		};

		int Seek (long offset, SeekMode mode);

		// 操作成功返回'true'
		bool Read (size_t itemSize, void* datum);
		bool Read (size_t itemSize, int numItems, void* data);
		bool Write (size_t itemSize, const void* datum);
		bool Write (size_t itemSize, int numItems, const void* data);

		// load to buffer
		static bool Load (const std::string& filename, bool binaryFile,
			int& bufferSize, char*& buffer);

		// save buffer to file
		static bool Save (const std::string& filename, bool binaryFile,
			int bufferSize, const char* buffer);

		// append buffer to file
		static bool Append (const std::string& filename, bool binaryFile,
			int bufferSize, const char* buffer);

	private:
		FILE* mFile;
		int mMode;
	};

#include "PX2FileIO.inl"

}

#endif