// PX2OutStream.cpp

#include "PX2OutStream.hpp"
#include "PX2FileIO.hpp"
#include "PX2Object.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
OutStream::OutStream ()
{
	mStreamType = ST_OUT;
}
//----------------------------------------------------------------------------
OutStream::~OutStream ()
{
}
//----------------------------------------------------------------------------
bool OutStream::Insert (Object* object)
{
	if (object)
	{
		// 同一个对象只能被插入到保存列表一次
		std::vector<Object*>::iterator iter = mTopLevel.begin();
		std::vector<Object*>::iterator end = mTopLevel.end();
		for (/**/; iter != end; ++iter)
		{
			if (object == *iter)
			{
				return false;
			}
		}

		if (object)
		{
			mTopLevel.push_back(object);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool OutStream::IsTopLevel (const Object* object) const
{
	std::vector<Object*>::const_iterator iter = mTopLevel.begin();
	std::vector<Object*>::const_iterator end = mTopLevel.end();
	for (/**/; iter != end; ++iter)
	{
		if (object == *iter)
		{
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------
void OutStream::Save (int& bufferSize, char*& buffer, int mode)
{
	// 插入指针和0索引对（保证每个对象的ID从0开始, 因为0指针需要）
	const Object* object = 0;
	unsigned int uniqueID = 0;
	mRegistered.insert(std::make_pair(object, uniqueID));
	mOrdered.push_back(object);

	// 需要保存的对象进行注册ct->Register(*this);
	std::vector<Object*>::iterator iterT = mTopLevel.begin();
	std::vector<Object*>::iterator endT = mTopLevel.end();
	for (/**/; iterT != endT; ++iterT)
	{
		object = *iterT;
		object->Register(*this);
	}

	// 需要的缓冲区大小
	RegisterArray::iterator iterO = mOrdered.begin();
	RegisterArray::iterator endO = mOrdered.end();
	bufferSize = 0;
	for (++iterO; iterO != endO; ++iterO)
	{
		object = *iterO;
		bufferSize += object->GetStreamingSize(*this);
	}

	// 为"Top Level"字符串调整缓冲区大小
	std::string topLevel("Top Level");
	int numTopLevelBytes = GetStreamingSize(topLevel);
	bufferSize += numTopLevelBytes*(int)mTopLevel.size();

	// 创建需要的缓冲区
	buffer = new1<char>(bufferSize);
	mTarget.Open(bufferSize, buffer, mode);

	// 将对象写入缓冲区
	iterO = mOrdered.begin();
	endO = mOrdered.end();
	for (++iterO, uniqueID = 1; iterO != endO; ++iterO, ++uniqueID)
	{
		object = *iterO;
		if (IsTopLevel(object))
		{
			WriteString(topLevel);
		}
		object->Save(*this);
	}

	// 清空容器，为其他保存作准备。
	mRegistered.clear();
	mOrdered.clear();
	mTarget.Close();
}
//----------------------------------------------------------------------------
bool OutStream::Save (const std::string& name, std::string version, int mode)
{
	// 保存文件的版本号
	if (!FileIO::Save(name, true, (int)version.length(), version.c_str()))
	{
		// 文件不能被打开
		return false;
	}

	// 将场景保存到内存buffer
	int bufferSize;
	char* buffer;
	Save(bufferSize, buffer, mode);

	// 将场景图从内存保存到硬盘
	if (!FileIO::Append(name, true, bufferSize, buffer))
	{
		// 文件不能被appended
		delete1(buffer);
		return false;
	}

	delete1(buffer);
	return true;
}
//----------------------------------------------------------------------------
bool OutStream::WriteBool (const bool datum)
{
	unsigned int value = (datum ? 0xFFFFFFFFu : 0u);
	return mTarget.Write(sizeof(unsigned int), &value);
}
//----------------------------------------------------------------------------
bool OutStream::WriteBoolW (int numElements, const bool* data)
{
	if (!mTarget.Write(sizeof(int), &numElements))
	{
		return false;
	}

	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!WriteBool(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool OutStream::WriteBoolN (int numElements, const bool* data)
{
	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!WriteBool(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool OutStream::WriteString (const std::string& datum)
{
	int length = (int)datum.length();
	if (!mTarget.Write(sizeof(int), &length))
	{
		return false;
	}

	if (length > 0)
	{
		if (!mTarget.Write(sizeof(char), length, datum.c_str()))
		{
			return false;
		}

		// 字符串被按照4字节倍数大小写入
		char zero[4] = { 0, 0, 0, 0 };
		int padding = (length % 4);
		if (padding > 0)
		{
			padding = 4 - padding;
			if (!mTarget.Write(sizeof(char), padding, zero))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool OutStream::WriteStringW (int numElements, const std::string* data)
{
	if (!mTarget.Write(sizeof(int), &numElements))
	{
		return false;
	}

	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!WriteString(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool OutStream::WriteStringN (int numElements, const std::string* data)
{
	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!WriteString(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
int OutStream::GetBytesWritten () const
{
	return mTarget.GetNumBytesProcessed();
}
//----------------------------------------------------------------------------
bool OutStream::RegisterRoot (const Object* object)
{
	if (mRegistered.find(object) == mRegistered.end())
	{
		// 这是第一次在场景中遇到该对象，我们需要讲此对象的独“一无二的标识（
		// unique identifier）”写入到硬盘，以支持对象的“加载时的链接机制（
		// loader-linker）”。为支持这种特性最简单的方式就是写入对象的地址。
		// 如果要写入内存地址的话，修要主要下面两点：
		//
		// 1. 随着应用程序的启动运行，对象的分配的地址是不一样的，这将导致对象
		//	  列表被写入的顺序不一样，除非我们不用map而用array作为存储容器。
		//
		// 2. 即使对象的顺序是确定的，对相同场景保存到文件的两次保存，字节和字
		//	  节是不匹配的。解决方案是，当那个注册对象时，为对象分配一个“虚拟
		//    地址（virtual address）”。我们使用数组中index+1作为这个虚地址。

		unsigned int uniqueID = (unsigned int)mOrdered.size();
		mRegistered.insert(std::make_pair(object, uniqueID));
		mOrdered.push_back(object);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void OutStream::WriteUniqueID (const Object* object)
{
	unsigned int uniqueID = mRegistered.find(object)->second;
	mTarget.Write(sizeof(unsigned int), &uniqueID);
}
//----------------------------------------------------------------------------