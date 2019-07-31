// PX2OutStream.hpp

#ifndef PX2OUTSTREAM_HPP
#define PX2OUTSTREAM_HPP

#include "PX2CorePre.hpp"
#include "PX2Stream.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class Object;

	class PX2_ENGINE_ITEM OutStream : public Stream
	{
	public:
		OutStream ();
		virtual ~OutStream ();

		// 加载的顶层级别（top-level）对象
		bool Insert (Object* object);
		bool IsTopLevel (const Object* object) const;

		// 将对象写入内存，输入buffer是用new1进行分配的，所以你必须使用delete1
		// 进行释放。调用者一般不知需要开辟多大的缓冲区，这个函数为下面的加载函
		// 数服务。如果操作成功，函数返回'true'。
		void Save (int& bufferSize, char*& buffer,
			int mode = BufferIO::BM_DEFAULT_WRITE);

		/**
		* 将需要保存的对象写入到磁盘.版本字符串首先被写入，其次写入你需要保存
		* 的对象.如果操作成功，函数返回'true'。
		*/
		bool Save (const std::string& name,
			std::string version = "PX2_VERSION_1_0",
			int mode = BufferIO::BM_DEFAULT_WRITE);

public_internal:
		// 函数用来写入数组，按照下面两种性质写入：W和N。
		// W：将numElements写入buffer。
		// N: do not将numElements写入buffer。

		// 原始类型数据写函数
		template <typename T> bool Write (T datum);
		template <typename T> bool WriteW (int numElements, const T* data);
		template <typename T> bool WriteN (int numElements, const T* data);

		// 以4-byte大小写入bool值
		bool WriteBool (const bool datum);
		bool WriteBoolW (int numElements, const bool* data);
		bool WriteBoolN (int numElements, const bool* data);

		// 写入字符串，用4-byte表示字符串长度，仅跟着的是字符集
		bool WriteString (const std::string& datum);
		bool WriteStringW (int numElements, const std::string* data);
		bool WriteStringN (int numElements, const std::string* data);

		// 按照一个枚举变量4-byte写入枚举值
		template <typename T> bool WriteEnum (const T datum);
		template <typename T> bool WriteEnumW (int numElements, const T* data);
		template <typename T> bool WriteEnumN (int numElements, const T* data);

		// 对象指针写入
		template <typename T> bool WritePointer (const T* object);
		template <typename T> bool WritePointerW (int numElements,
			T* const* objects);
		template <typename T> bool WritePointerN (int numElements,
			T* const* objects);

		template <typename T> bool WritePointer (const Pointer0<T>& object);

		template <typename T> bool WritePointerW (int numElements,
			Pointer0<T> const* objects);

		template <typename T> bool WritePointerN (int numElements,
			Pointer0<T> const* objects);

		// 支持扩展类型的写入。这些扩展类型必须在扩展类型的组件中专门写，在
		// TDCore中不会被实现。
		template <typename T> bool WriteAggregate (const T& datum);
		template <typename T> bool WriteAggregateW (int numElements,
			const T* data);
		template <typename T> bool WriteAggregateN (int numElements,
			const T* data);

		// 帮助函数
		int GetBytesWritten () const;
		bool RegisterRoot (const Object* object);
		void WriteUniqueID (const Object* object);

		template <typename T> void Register(const T* object);
		template <typename T> void Register(int numElements, T* const* objects);
		template <typename T> void Register(const Pointer0<T>& object);
		template <typename T> void Register(int numElements,
			Pointer0<T> const* objects);

	private:
		typedef std::map<const Object*, unsigned int> RegisterMap;
		typedef std::vector<const Object*> RegisterArray;

		// 用来进行持久化的对象列表
		std::vector<Object*> mTopLevel;

		// 在保存场景时，进行深度优先遍历时，用来存储对象的列表
		RegisterMap mRegistered;
		RegisterArray mOrdered;
		BufferIO mTarget;
	};

#include "PX2OutStream.inl"

}

#endif
