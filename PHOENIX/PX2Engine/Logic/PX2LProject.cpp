// PX2LFile.cpp

#include "PX2LProject.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, LProject);
PX2_IMPLEMENT_STREAM(LProject);
PX2_IMPLEMENT_FACTORY(LProject);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, LProject);

//----------------------------------------------------------------------------
LProject::LProject()
{
	LFile *file = new0 LFile();
	file->SetName("Main");
	mLFiles.push_back(file);
}
//----------------------------------------------------------------------------
LProject::~LProject()
{
}
//----------------------------------------------------------------------------
bool LProject::AddFile(LFile *file)
{
	if (IsHasFile(file))
		return false;

	mLFiles.push_back(file);

	return true;
}
//----------------------------------------------------------------------------
bool LProject::IsHasFile(LFile *file)
{
	auto it = mLFiles.begin();
	for (; it != mLFiles.end(); it++)
	{
		if (file == *it)
		{
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool LProject::RemoveFile(LFile *file)
{
	auto it = mLFiles.begin();
	for (; it != mLFiles.end(); it++)
	{
		if (file == *it)
		{
			it = mLFiles.erase(it);
			
			return true;
		}
		else
		{
			it++;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
std::vector<LFilePtr> &LProject::GetLFiles()
{
	return mLFiles;
}
//----------------------------------------------------------------------------
void LProject::Compile()
{
	auto it = mLFiles.begin();
	for (; it != mLFiles.end(); it++)
	{
		LFile *file = *it;
		if (file)
		{
			file->Compile();
		}
	}
}
//----------------------------------------------------------------------------
void LProject::Save()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
LProject::LProject(LoadConstructor value) :
Object(value)
{
}
//----------------------------------------------------------------------------
void LProject::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(LProject, source);
}
//----------------------------------------------------------------------------
void LProject::Link(InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void LProject::PostLink()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool LProject::Register(OutStream& target) const
{
	if (Object::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LProject::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(LProject, target);
}
//----------------------------------------------------------------------------
int LProject::GetStreamingSize(Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------