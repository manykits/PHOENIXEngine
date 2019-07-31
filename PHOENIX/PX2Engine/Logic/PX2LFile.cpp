// PX2LFile.cpp

#include "PX2LFile.hpp"
#include "PX2FontManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2LogicManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, LFile);
PX2_IMPLEMENT_STREAM(LFile);
PX2_IMPLEMENT_FACTORY(LFile);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, LFile);

//----------------------------------------------------------------------------
LFile::LFile() :
mPlatformType(PT_ENGINE)
{
	LBlockPtr blockProgramStart = new0 LBlock(LBlock::MT_CONTROL);
	blockProgramStart->SetCtrlType(LBlock::CT_PROGRAMSTART);
	blockProgramStart->RegistFunObj(*(PX2_LOGICM.GetCtrl("ProgramStart")));
	blockProgramStart->SetAnchorParam(40.0f, -40.0f);
	AddBlockPrograms(blockProgramStart);

	LBlockPtr blockProgramFixUpdate = new0 LBlock(LBlock::MT_CONTROL);
	blockProgramFixUpdate->SetCtrlType(LBlock::CT_PROGRAMFIXUPDATE);
	blockProgramFixUpdate->RegistFunObj(*(PX2_LOGICM.GetCtrl("ProgramFixUpdate")));
	blockProgramFixUpdate->SetAnchorParam(480.0f, -40.0f);
	AddBlockPrograms(blockProgramFixUpdate);
}
//----------------------------------------------------------------------------
LFile::~LFile()
{
}
//----------------------------------------------------------------------------
bool LFile::AddBlockPrograms(LBlock *block)
{
	if (IsHasBlockProgram(block))
		return false;

	mBlockPrograms.push_back(block);
	block->SetLFile(this);

	return true;
}
//----------------------------------------------------------------------------
bool LFile::IsHasBlockProgram(LBlock *block)
{
	for (int i = 0; i < (int)mBlockPrograms.size(); i++)
	{
		if (block == mBlockPrograms[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LFile::RemoveBlockProgram(LBlock *block)
{
	auto it = mBlockPrograms.begin();
	for (; it != mBlockPrograms.end();)
	{
		if (block == *it)
		{
			it = mBlockPrograms.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
std::vector<LBlockPtr> &LFile::GetBlockPrograms()
{
	return mBlockPrograms;
}
//----------------------------------------------------------------------------
void LFile::SetPlatformType(LogicFilePlatformType pt)
{
	mPlatformType = pt;
}
//----------------------------------------------------------------------------
LogicFilePlatformType LFile::GetPlatformType() const
{
	return mPlatformType;
}
//----------------------------------------------------------------------------
bool LFile::IsPreCompiledParam(const std::string &paramName)
{
	for (int i = 0; i < (int)PreCompiledParams.size(); i++)
	{
		if (paramName == PreCompiledParams[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void LFile::_CompileFile(std::string &script)
{
	LogicFilePlatformType pt = GetPlatformType();
	const std::string &fileName = GetName();

	if (pt == PT_ENGINE)
	{
		script += fileName + " = class(LSController, \n";
		script += "{ \n";
		script += "    _name = " + std::string("\"") + fileName + "\"" + ",\n";
		script += "    _scriptControl = nil,\n";
		script += "    _ctrlable = nil,\n";
		script += "}); \n";
		script += "\n";
		script += "function " + fileName + ":OnAttached()\n";
		script += std::string("    PX2_LOGGER:LogInfo(\"script_lua\", ") +
			"\"\"..self._name..\" OnAttached\")\n";
		script += "    self._scriptControl = Cast:ToSC(self.__object)\n";
		script += "    self._ctrlable = self._scriptControl:GetControlledable()\n";
		script += "end\n";
		script += "\n";

		// start
		script += "function " + fileName + ":OnInitUpdate()\n";

		script += "    PX2_LOGICM:SetCurLogicObject(self._ctrlable)\n";

		for (int i = 0; i < (int)mBlockPrograms.size(); i++)
		{
			LBlock *lBlock = mBlockPrograms[i];
			if (lBlock)
			{
				lBlock->CompileStart(mCompiledString, 1, this);
			}
		}

		script += "end\n";

		script += "\n";

		// update
		script += "function " + fileName + ":OnFixUpdate()\n";

		script += "    PX2_LOGICM:SetCurLogicObject(self._ctrlable)\n";

		for (int i = 0; i < (int)mBlockPrograms.size(); i++)
		{
			LBlock *lBlock = mBlockPrograms[i];
			if (lBlock)
			{
				lBlock->CompileFixUpdate(mCompiledString, 0, this);
			}
		}

		script += "end\n";

		script += "\n";
	}
}
//----------------------------------------------------------------------------
const std::string &LFile::Compile()
{
	PreCompiledParams.clear();
	mCompiledString.clear();

	if (PT_ENGINE == mPlatformType)
	{
		mCompiledString += "-- " + GetName() + ".lua";
	}

	mCompiledString += "\n";

	for (int i= 0; i < (int)mBlockPrograms.size(); i++)
	{
		LBlock *lBlock = mBlockPrograms[i];
		if (lBlock)
		{
			lBlock->PreCompile(mCompiledString, this, true);
		}
	}

	mCompiledString += "\n";

	_CompileFile(mCompiledString);

	mCompiledString += "\n";

	return mCompiledString;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
LFile::LFile(LoadConstructor value) :
Object(value),
mPlatformType(PT_ENGINE)
{
}
//----------------------------------------------------------------------------
void LFile::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mPlatformType);
	int numProgram = 0;
	source.Read(numProgram);
	mBlockPrograms.resize(numProgram);
	for (int i = 0; i < numProgram; i++)
	{
		source.ReadPointer(mBlockPrograms[i]);
	}

	PX2_END_DEBUG_STREAM_LOAD(LFile, source);
}
//----------------------------------------------------------------------------
void LFile::Link(InStream& source)
{
	Object::Link(source);

	int numProgram = (int)mBlockPrograms.size();
	for (int i = 0; i < numProgram; i++)
	{
		source.ResolveLink(mBlockPrograms[i]);
	}
}
//----------------------------------------------------------------------------
void LFile::PostLink()
{
	Object::PostLink();

	int numProgram = (int)mBlockPrograms.size();
	for (int i = 0; i < numProgram; i++)
	{
		if (mBlockPrograms[i])
		{
			mBlockPrograms[i]->PostLink();
			mBlockPrograms[i]->SetLFile(this);
		}
	}
}
//----------------------------------------------------------------------------
bool LFile::Register(OutStream& target) const
{
	if (Object::Register(target))
	{
		int numProgram = (int)mBlockPrograms.size();
		for (int i = 0; i < numProgram; i++)
		{
			if (mBlockPrograms[i])
				target.Register(mBlockPrograms[i]);
		}

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LFile::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mPlatformType);

	int numPrograms = (int)mBlockPrograms.size();
	target.Write(numPrograms);
	for (int i = 0; i < numPrograms; i++)
	{
		target.WritePointer(mBlockPrograms[i]);
	}

	PX2_END_DEBUG_STREAM_SAVE(LFile, target);
}
//----------------------------------------------------------------------------
int LFile::GetStreamingSize(Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mPlatformType);

	int numPrograms = (int)mBlockPrograms.size();
	size += sizeof(numPrograms);
	for (int i = 0; i < numPrograms; i++)
	{
		size += PX2_POINTERSIZE(mBlockPrograms[i]);
	}

	return size;
}
//----------------------------------------------------------------------------