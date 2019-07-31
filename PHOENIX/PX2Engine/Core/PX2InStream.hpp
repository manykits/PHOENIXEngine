// PX2InStream.hpp

#ifndef PX2INSTREAM_H
#define PX2INSTREAM_H

#include "PX2CorePre.hpp"
#include "PX2Stream.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class Object;

	class PX2_ENGINE_ITEM InStream : public Stream
	{
	public:
		InStream ();
		virtual ~InStream ();

		// 加载的顶层级别（top-level）对象
		int GetNumObjects () const;
		Object* GetObjectAt (int i) const;

		// 从硬盘加载对象列表。调用者必须自己管理输入的缓冲区buffer。调用者一般
		// 不知需要开辟多大的缓冲区，这个函数为下面的加载函数服务。
		void Load (int bufferSize, char* buffer,
			int mode = BufferIO::BM_DEFAULT_READ);

		bool Load1 (int bufferSize, char* buffer,
			std::string version = "PX2_VERSION_1_0",
			int mode = BufferIO::BM_DEFAULT_READ);

		// 从文件读取对象。文件内部版本字符串会和参数version进行比较，如果字符串一致
		// 对象被加载到列表。如果操作成功，函数返回真。
		bool Load (const std::string& name,
			std::string version = "PX2_VERSION_1_0",
			int mode = BufferIO::BM_DEFAULT_READ);

public_internal:
		// 函数按照下面3中形式读取数组：VV，VR和RR。V表示按值读取，R表示按照引
		// 用读取。
		//	numElements:
		//		V = 调用者传递大小
		//		R = 从缓冲区中读取
		//	data:
		//		V = 由调用者事先非配内存
		//		R = 有函数从buffer中读取内存大小，然后进行分配

		// 原始类型读函数
		template <typename T> bool Read (T& datum);
		template <typename T> bool ReadVV (int numElements, T* data);
		template <typename T> bool ReadVR (int numElements, T*& data);
		template <typename T> bool ReadRR (int& numElements, T*& data);

		// 按照4-byte大小读bool值
		bool ReadBool (bool& datum);
		bool ReadBoolVV (int numElements, bool* data);
		bool ReadBoolVR (int numElements, bool*& data);
		bool ReadBoolRR (int& numElements, bool*& data);

		// 读取字符串，用4-byte表示字符串长度，仅跟着的是字符集
		bool ReadString (std::string& datum);
		bool ReadStringVV (int numElements, std::string* data);
		bool ReadStringVR (int numElements, std::string*& data);
		bool ReadStringRR (int& numElements, std::string*& data);

		// 按照一个枚举变量4-byte读取枚举值
		template <typename T> bool ReadEnum (T& datum);
		template <typename T> bool ReadEnumVV (int numElements, T* data);
		template <typename T> bool ReadEnumVR (int numElements, T*& data);
		template <typename T> bool ReadEnumRR (int& numElements, T*& data);

		// 读取对象的指针（指针被作为对象的独一无二的ID）
		template <typename T> bool ReadPointer (T*& object);
		template <typename T> bool ReadPointerVV (int numElements, T** objects);
		template <typename T> bool ReadPointerVR (int numElements, T**& objects);
		template <typename T> bool ReadPointerRR (int& numElements, T**& objects);

		template <typename T> bool ReadPointer (Pointer0<T>& object);
		template <typename T> bool ReadPointerVV (int numElements,
			Pointer0<T>* objects);
		template <typename T> bool ReadPointerVR (int numElements,
			Pointer0<T>*& objects);
		template <typename T> bool ReadPointerRR (int& numElements,
			Pointer0<T>*& objects);

		// 支持扩展类型的读取。这些扩展类型必须在扩展类型的组件中专门写，在
		// TDCore中不会被实现。
		template <typename T> bool ReadAggregate (T& datum);
		template <typename T> bool ReadAggregateVV (int numElements, T* data);
		template <typename T> bool ReadAggregateVR (int numElements, T*& data);
		template <typename T> bool ReadAggregateRR (int& numElements, T*& data);

		// 帮助函数
		int GetBytesRead () const;
		void ReadUniqueID (Object* object);
		template <typename T> void ResolveLink (T*& object);
		template <typename T> void ResolveLink (int numElements, T** object);
		template <typename T> void ResolveLink (Pointer0<T>& object);
		template <typename T> void ResolveLink (int numElements,
			Pointer0<T>* objects);

	private:
		typedef std::map<uint64_t, Object*> LinkMap; //< uint64_t记录在mOrdered中的顺序号，Object*记录当前对象指针
		typedef std::vector<Object*> LinkArray;

		// 需要做持久化的对象列表
		std::vector<Object*> mTopLevel;

		// 加载场景的所有对象的列表
		LinkMap mLinked;
		LinkArray mOrdered;
		BufferIO mSource;
	};

#include "PX2InStream.inl"

}

#endif