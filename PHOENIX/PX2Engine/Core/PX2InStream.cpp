// PX2InStream.cpp

#include "PX2InStream.hpp"
#include "PX2FileIO.hpp"
#include "PX2Object.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
InStream::InStream ()
{
	mStreamType = ST_IN;
}
//----------------------------------------------------------------------------
InStream::~InStream ()
{
}
//----------------------------------------------------------------------------
int InStream::GetNumObjects () const
{
	return (int)mTopLevel.size();
}
//----------------------------------------------------------------------------
Object* InStream::GetObjectAt (int i) const
{
	if (0 <= i && i < (int)mTopLevel.size())
	{
		return mTopLevel[i];
	}
	return 0;
}
//----------------------------------------------------------------------------
void InStream::Load (int bufferSize, char* buffer, int mode)
{
	// 为输入buffer创建读取器
	mSource.Open(bufferSize, buffer, mode);

	// 加载顶层级别物体列表
	std::string topLevel("Top Level");
	Object* object;
	while (mSource.GetNumBytesProcessed() < mSource.GetNumBytesTotal())
	{
		// 读取"Top Level"或者RTTI名称
		std::string name;
		ReadString(name);
		bool isTopLevel = (name == topLevel);
		if (isTopLevel)
		{
			// 读取RTTI名称
			ReadString(name);
		}

		// 获得该对象的工厂创建函数。如果Object::Find(name)调用失败，确信你已经
		// 在main或者WinMain开始时调用了InitTerm::ExecuteInitializers()。
		// InitTerm内的初始化函数创建工厂创建函数map。
		Object::FactoryFunction factory = Object::Find(name);
		if (!factory)
		{
			// 寻找不到工厂创建函数.确信你加了PX2_REGISTER_STREAM(someclass)宏
			// 在类'someclass'的头文件中。这个宏为这个类注册了工厂创建函数。
			assertion(false, "Cannot find factory for %s.\n", name.c_str());
			return;
		}

		// 创建对象，加载成员数据
		object = (*factory)(*this);

		// 如果是顶层级别物体，将其加入列表。
		if (isTopLevel)
		{
			mTopLevel.push_back(object);
		}
	}

	// 链接对象
	std::vector<Object* >::iterator iter = mOrdered.begin();
	std::vector<Object* >::iterator end = mOrdered.end();
	for (/**/; iter != end; ++iter)
	{
		(*iter)->Link(*this);
	}

	// 当对象被加载时，调用对象的缺省构造函数创建对象，PostLink()机制能做一些
	// 非缺省构造函数可以完成的工作。
	iter = mOrdered.begin();
	end = mOrdered.end();
	for (/**/; iter != end; ++iter)
	{
		(*iter)->PostLink();
	}

	// 清空列表，为其他的调用做准备。
	mLinked.clear();
	mOrdered.clear();
	mSource.Close();
}
//----------------------------------------------------------------------------
bool InStream::Load1 (int bufferSize, char* buffer, std::string version,
	int mode)
{
	int length = (int)version.length();
	if (bufferSize < length)
	{
		return false;
	}

	if (strncmp(version.c_str(), buffer, length) != 0)
	{
		return false;
	}

	bufferSize -= length;
	Load(bufferSize, buffer + length, mode);

	return true;
}
//----------------------------------------------------------------------------
bool InStream::Load (const std::string& name, std::string version, int mode)
{
	// 从硬盘加载到内存。
	int bufferSize;
	char* buffer;
	if (!FileIO::Load(name, true, bufferSize, buffer))
	{
		return false;
	}

	// 获得文件版本
	int length = (int)version.length();
	if (bufferSize < length)
	{
		// 文件连存储文件版本字符串的大小都不够
		delete1(buffer);
		return false;
	}

	// 你需要加载的版本号和文件版本做比较
	if (strncmp(version.c_str(), buffer, length) != 0)
	{
		// 版本不匹配
		delete1(buffer);
		return false;
	}

	// 从内存buffer中重新构建场景图
	bufferSize -= length;
	Load(bufferSize, buffer + length, mode);

	delete1(buffer);
	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadBool (bool& datum)
{
	unsigned int value;
	if (!mSource.Read(sizeof(unsigned int), &value))
	{
		return false;
	}
	datum = (value != 0);
	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadBoolVV (int numElements, bool* data)
{
	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadBool(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadBoolVR (int numElements, bool*& data)
{
	if (numElements > 0)
	{
		data = new1<bool>(numElements);
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadBool(data[i]))
			{
				return false;
			}
		}
	}
	else
	{
		data = 0;
	}

	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadBoolRR (int& numElements, bool*& data)
{
	if (!mSource.Read(sizeof(int), &numElements))
	{
		return false;
	}

	if (numElements > 0)
	{
		data = new1<bool>(numElements);
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadBool(data[i]))
			{
				return false;
			}
		}
	}
	else
	{
		data = 0;
	}

	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadString (std::string& datum)
{
	int length;
	if(!mSource.Read(sizeof(int), &length))
	{
		datum.clear();
		return false;
	}

	if(length == 0)
	{
		datum.resize(0);
		return true;
	}

	// 将字符串长度垫到四字节的倍数
	int padding = (length % 4);
	if (padding > 0)
	{
		padding = 4 - padding;
	}

	const char* text = mSource.GetBuffer() + mSource.GetNumBytesProcessed();
	datum.assign(text, length);
	mSource.IncrementNumBytesProcessed(length + padding);
	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadStringVV (int numElements, std::string* data)
{
	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadString(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadStringVR (int numElements, std::string*& data)
{
	if (numElements > 0)
	{
		data = new1<std::string>(numElements);
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadString(data[i]))
			{
				return false;
			} 
		}
	}
	else
	{
		data = 0;
	}

	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadStringRR (int& numElements, std::string*& data)
{
	if (!mSource.Read(sizeof(int), &numElements))
	{
		return false;
	}

	if (numElements > 0)
	{
		data = new1<std::string>(numElements);
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadString(data[i]))
			{
				return false;
			}
		}
	}
	else
	{
		data = 0;
	}

	return true;
}
//----------------------------------------------------------------------------
int InStream::GetBytesRead () const
{
	return mSource.GetNumBytesProcessed();
}
//----------------------------------------------------------------------------
void InStream::ReadUniqueID (Object* object)
{
	unsigned int uniqueID;
	if (mSource.Read(sizeof(unsigned int), &uniqueID))
	{
		mLinked.insert(std::make_pair((uint64_t)uniqueID, object));
		mOrdered.push_back(object);
	}
}
//----------------------------------------------------------------------------