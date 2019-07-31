// PX2Object_Stream.cpp

#include "PX2Object.hpp"
#include "PX2EventHandler.hpp"
#include "PX2EventHandlerObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// 持久化
//----------------------------------------------------------------------------
Object::FactoryMap* Object::msFactories = 0;
bool Object::msStreamRegistered = false;
bool Object::msIsIOWriting = false;
//----------------------------------------------------------------------------
Object* Object::Factory(InStream&)
{
	assertion(false, "Abstract classes have no factory.\n");
	return 0;	
}
//----------------------------------------------------------------------------
bool Object::RegisterFactory()
{
	if (!msStreamRegistered)
	{
		InitTerm::AddInitializer(Object::InitializeFactory);
		InitTerm::AddTerminator(Object::TerminateFactory);
		msStreamRegistered = true;
	}
	return msStreamRegistered;
}
//----------------------------------------------------------------------------
void Object::InitializeFactory()
{
	if (!msFactories)
	{
		msFactories = new0 Object::FactoryMap();
	}
	//msFactories->insert(std::make_pair(TYPE.GetName(), Factory));
	(*msFactories)[TYPE.GetName()] = Factory;
}
//----------------------------------------------------------------------------
void Object::TerminateFactory()
{
	delete0(msFactories);
	msFactories = 0;
}
//----------------------------------------------------------------------------
Object::FactoryFunction Object::Find(const std::string& name)
{
	assertion(msFactories != 0, "FactoryMap was not yet allocated.\n");
	FactoryMap::iterator iter = msFactories->find(name);
	if (iter != msFactories->end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
Object::Object(LoadConstructor) :
mIsEnable(true),
mIsActivated(true),
mReadedVersion(0),
mCurStream(0),
mID(0),
mIsNameChangeable(true)
{
	mEventHandler = new0 ObjectEventHandler(this);
}
//----------------------------------------------------------------------------
void Object::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	// Rtti 名称已经被读取了

	// Rtti version
	source.Read(mReadedVersion);

	// 读取ID，提供对象的链接信息
	source.ReadUniqueID(this);

	source.ReadBool(mIsEnable);

	// 资源路径
	source.ReadString(mResourcePath);
	mResourcePathFString = mResourcePath.c_str();

	// 读取对象的名称
	source.ReadString(mName);
	mNameFString = mName.c_str();
	source.Read(mID);

	PX2_END_DEBUG_STREAM_LOAD(Object, source);
}
//----------------------------------------------------------------------------
void Object::Link(InStream&)
{
	// Object没有需要Link的成员
}
//----------------------------------------------------------------------------
void Object::PostLink()
{
	// Object没有需要PostLink的成员
}
//----------------------------------------------------------------------------
bool Object::Register(OutStream& target) const
{
	return target.RegisterRoot(this);
}
//----------------------------------------------------------------------------
void Object::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	// 写入Rtti 名称
	target.WriteString(GetRttiType().GetName());

	// 写入Rtti version
	target.Write(TYPE.GetVersion());

	// 写入ID
	target.WriteUniqueID(this);

	//是否有效
	target.WriteBool(mIsEnable);

	// 资源路径
	target.WriteString(mResourcePath);

	// 写入对象名称
	target.WriteString(mName);
	target.Write(mID);

	PX2_END_DEBUG_STREAM_SAVE(Object, target);
}
//----------------------------------------------------------------------------
int Object::GetStreamingSize(Stream &stream) const
{
	PX2_UNUSED(stream);

	// Rtti 名称
	int size = PX2_STRINGSIZE(GetRttiType().GetName());

	// Rtti version
	size += sizeof(GetRttiType().GetVersion());

	// 对象ID
	size += sizeof(unsigned int);

	// 是否有效
	size += PX2_BOOLSIZE(mIsEnable);

	// 资源路径
	size += PX2_STRINGSIZE(mResourcePath);

	// 对象的名称
	size += PX2_STRINGSIZE(mName);
	size += sizeof(mID);

	return size;
}
//----------------------------------------------------------------------------
unsigned short Object::GetReadedVersion() const
{
	return mReadedVersion;
}
//----------------------------------------------------------------------------
Object* Object::Copy(const std::string& uniqueNameAppend)
{
	// Save the object to a memory buffer.
	OutStream saveStream;

	saveStream.Insert((Object*)this);
	int bufferSize = 0;
	char* buffer = 0;
	saveStream.Save(bufferSize, buffer, BufferIO::BM_DEFAULT_WRITE);

	// Load the object from the memory buffer.
	InStream loadStream;
	loadStream.Load(bufferSize, buffer, BufferIO::BM_DEFAULT_READ);
	delete1(buffer);

	if (uniqueNameAppend != "")
	{
		int numObjects = loadStream.GetNumObjects();
		for (int i = 0; i < numObjects; i++)
		{
			PX2::Object *obj = loadStream.GetObjectAt(i);
			std::string name = obj->GetName();
			if (name.length() > 0)
			{
				name += uniqueNameAppend;
				obj->SetName(name);
			}
		}
	}

	return loadStream.GetObjectAt(0);
}
//----------------------------------------------------------------------------