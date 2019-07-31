// PX2LModule.cpp

#include "PX2LBlock.hpp"
#include "PX2FontManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2LogicManager.hpp"
#include "PX2LFile.hpp"
#include "PX2XMLData.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, LBlock);
PX2_IMPLEMENT_STREAM(LBlock);
PX2_IMPLEMENT_FACTORY(LBlock);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, LBlock);

//----------------------------------------------------------------------------
LBlock::LBlock(BlockType type) :
mBlockType(type),
mIsFunOutputConvertToGeneral(false),
mIsUseThisPointer(false),
mCtrlType(CT_NONE),
mParamType(PT_NONE),
mOwnObjectParam(0),
mParent(0),
mLFile(0)
{
	mBeforeParam = new0 LParam(LParam::IPT_IN_EXE);
	mBeforeParam->SetName("Before");
	mBeforeParam->SetModule(this);

	mNextParam = new0 LParam(LParam::IPT_OUT_EXE);
	mNextParam->SetName("Next");
	mNextParam->SetModule(this);
}
//----------------------------------------------------------------------------
LBlock::~LBlock()
{
}
//----------------------------------------------------------------------------
void LBlock::SetFunOutputConvertToGeneral(bool convert)
{
	mIsFunOutputConvertToGeneral = convert;
}
//----------------------------------------------------------------------------
bool LBlock::IsFunOutputConvertToGeneral() const
{
	return mIsFunOutputConvertToGeneral;
}
//----------------------------------------------------------------------------
void LBlock::SetUseThisPointer(bool useThisPointer)
{
	mIsUseThisPointer = useThisPointer;
}
//----------------------------------------------------------------------------
bool LBlock::IsUseThisPointer() const
{
	return mIsUseThisPointer;
}
//----------------------------------------------------------------------------
void LBlock::SetClassName(const std::string &className)
{
	mClassName = className;
}
//----------------------------------------------------------------------------
const std::string &LBlock::GetClsName() const
{
	return mClassName;
}
//----------------------------------------------------------------------------
void LBlock::RegistFunObj(const FunObject &funObj)
{
	mFunObject = funObj;
	mFunObjectName = mFunObject.Name;

	SetName(funObj.Name);
	SetClassName(funObj.ClassName);

	mInputParams.clear();
	mInputParamsVec.clear(); 
	for (int i = 0; i < funObj.GetNumInParams(); i++)
	{
		const FunParam &funParam = funObj.GetInParam(i);

		std::map<std::string, LParam*>::iterator it = mInputParams.find(funParam.Name);
		if (it == mInputParams.end())
		{
			LParamPtr param = new0 LParam();
			param->SetName(funParam.Name);
			param->SetDataType(funParam.Type);
			param->SetDataTypeName(funParam.TypeName);
			param->SetEnum(funParam.IsEnum);
			param->SetValue(funParam.Value);

			mInputParams[funParam.Name] = param;
			mInputParamsVec.push_back(param);
			param->SetModule(this);

			if (FPT_POINTER_THIS == funParam.Type || FPT_POINTER_THIS_STATIC == funParam.Type)
			{
				mOwnObjectParam = param;
			}

			if (FPT_POINTER_THIS_STATIC == funParam.Type)
			{
				mOwnObjectParam->Enable(false);
			}
		}
	}

	mOutputParams.clear();
	mOutputParamsVec.clear();
	for (int i = 0; i < funObj.GetNumOutParams(); i++)
	{
		const FunParam &funParam = funObj.GetOutParam(i);

		std::map<std::string, LParam*>::iterator it = mOutputParams.find(funParam.Name);
		if (it == mOutputParams.end())
		{
			LParamPtr param = new0 LParam(LParam::IPT_OUT);
			param->SetName(funParam.Name);
			param->SetDataType(funParam.Type);
			param->SetDataTypeName(funParam.TypeName);
			param->SetEnum(funParam.IsEnum);
			param->SetValue(funParam.Value);
			mOutputParams[funParam.Name] = param;
			mOutputParamsVec.push_back(param);
			param->SetModule(this);
		}
	}

	FunObject::ParamType pt = funObj.GetParamType();
	if (pt == FunObject::PT_CONST)
	{
		SetParamType(PT_CONST);
	}
	else if (pt == FunObject::PT_VALUE)
	{
		SetParamType(PT_VALUE);
	}
	else if (pt == FunObject::PT_VARIABLE)
	{
		SetParamType(PT_VARIABLE);
	}
	else if (pt == FunObject::PT_ENUM)
	{
		SetParamType(PT_ENUM);
	}
	else if (pt == FunObject::PT_ENUMSTRING)
	{
		SetParamType(PT_ENUMSTRING);
	}
	else if (pt == FunObject::PT_VARIABLESELECT)
	{
		SetParamType(PT_VARIABLESELECT);
	}
	else if (pt == FunObject::PT_VALUESELECT)
	{
		SetParamType(PT_VALUESELECT);
	}
}
//----------------------------------------------------------------------------
const FunObject &LBlock::GetFunObj() const
{
	return mFunObject;
}
//----------------------------------------------------------------------------
void LBlock::SetCtrlType(CtrlType ct)
{
	mCtrlType = ct;
}
//----------------------------------------------------------------------------
void LBlock::SetParamType(ParamType pt)
{
	mParamType = pt;
}
//----------------------------------------------------------------------------
LParam *LBlock::GetLParamByName(const std::string &name)
{
	if (mBeforeParam->GetName() == name)
		return mBeforeParam;

	if (mNextParam->GetName() == name)
		return mNextParam;

	for (int i = 0; i < (int)mInputParamsVec.size(); i++)
	{
		if (name == mInputParamsVec[i]->GetName())
			return mInputParamsVec[i];
	}

	for (int i = 0; i < (int)mOutputParamsVec.size(); i++)
	{
		if (name == mOutputParamsVec[i]->GetName())
			return mOutputParamsVec[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
LParam *LBlock::GetInputParam(int i)
{
	if (0 <= i && i < (int)mInputParamsVec.size())
		return mInputParamsVec[i];

	return 0;
}
//----------------------------------------------------------------------------
LParam *LBlock::GetOutputParam(int i)
{
	if (0 <= i && i < (int)mOutputParamsVec.size())
		return mOutputParamsVec[i];

	return 0;
}
//----------------------------------------------------------------------------
void LBlock::SetParent(LParam *parent)
{
	mParent = parent;
}
//----------------------------------------------------------------------------
LParam *LBlock::GetParent()
{
	return mParent;
}
//----------------------------------------------------------------------------
void LBlock::SetAnchorParam(float hor, float ver)
{
	mAnchorParam.X() = hor;
	mAnchorParam.Y() = ver;
}
//----------------------------------------------------------------------------
const Vector3f &LBlock::GetAnchorParam() const
{
	return mAnchorParam;
}
//----------------------------------------------------------------------------
const LParam *LBlock::GetOwnerObjectParam() const
{
	return mOwnObjectParam;
}
//----------------------------------------------------------------------------
void LBlock::_WriteTables(std::string &script, int numTable)
{
	for (int i = 0; i < numTable; i++)
	{
		script += "    ";
	}
}
//----------------------------------------------------------------------------
void LBlock::SetLFile(LFile *lfile)
{
	mLFile = lfile;
}
//----------------------------------------------------------------------------
LFile *LBlock::GetLFile()
{
	return mLFile;
}
//----------------------------------------------------------------------------
void LBlock::PreCompile(std::string &script, LFile *file,
	bool isOriginalStart)
{
	PX2_UNUSED(isOriginalStart);

	LogicFilePlatformType pt = file->GetPlatformType();

	const std::string &name = GetName();
	BlockType bt = GetBlockType();
	CtrlType ct = GetCtrlType();

	if (MT_CONTROL == bt)
	{
		if (CT_PROGRAM == ct)
		{
			LParam *paramStart = GetLParamByName("Start");
			if (paramStart)
			{
				paramStart->PreCompile(script, file);
			}


			LParam *paramFixUpdate = GetLParamByName("FixUpdate");
			if (paramFixUpdate)
			{
				paramFixUpdate->PreCompile(script, file);
			}
		}
		if (CT_PROGRAMSTART == ct)
		{
			LParam *paramStart = GetLParamByName("Start");
			if (paramStart)
			{
				paramStart->PreCompile(script, file);
			}
		}
		if (CT_PROGRAMFIXUPDATE == ct)
		{
			LParam *paramStart = GetLParamByName("FixUpdate");
			if (paramStart)
			{
				paramStart->PreCompile(script, file);
			}
		}
		else if (CT_IF == ct)
		{
			LParam *paramIf = GetLParamByName("If");
			if (paramIf)
			{
				paramIf->PreCompile(script, file);
			}

			LParam *paramIfDo = GetLParamByName("IfDo");
			if (paramIfDo)
			{
				paramIfDo->PreCompile(script, file);
			}
		}
		else if (CT_IFELSE == ct)
		{
			LParam *paramIf = GetLParamByName("If");
			if (paramIf)
			{
				paramIf->PreCompile(script, file);
			}

			LParam *paramIfDo = GetLParamByName("IfDo");
			if (paramIfDo)
			{
				paramIfDo->PreCompile(script, file);
			}

			LParam *paramElseDo = GetLParamByName("ElseDo");
			if (paramElseDo)
			{
				paramElseDo->PreCompile(script, file);
			}
		}
		else if (CT_WHILE == ct)
		{
			LParam *paramWhile = GetLParamByName("While");
			if (paramWhile)
			{
				paramWhile->PreCompile(script, file);
			}

			LParam *paramWhileDo = GetLParamByName("WhileDo");
			if (paramWhileDo)
			{
				paramWhileDo->PreCompile(script, file);
			}
		}
		else if (CT_COROUTINE == ct)
		{
			LParam *paramCorDo = GetLParamByName("CorDo");
			if (paramCorDo)
			{
				paramCorDo->PreCompile(script, file);
			}
		}
	}
	else if (MT_FUNCTION_GENERAL == bt || MT_FUNCTION_OUTPUT == bt)
	{
		for (int i = 0; i < (int)mInputParamsVec.size(); i++)
		{
			LParam *inParam = mInputParamsVec[i];
			if (inParam)
			{
				inParam->PreCompile(script, file);
			}
		}

		for (int i = 0; i < (int)mOutputParamsVec.size(); i++)
		{
			LParam *outParam = mOutputParamsVec[i];
			if (outParam)
			{
				outParam->PreCompile(script, file);
			}
		}
	}
	else if (MT_FUNCTION_OPERATOR == bt)
	{
		if (1 == (int)mInputParamsVec.size())
		{
			LParam *paramA = mInputParamsVec[0];
			if (paramA)
			{
				paramA->PreCompile(script, file);
			}
		}
		else if (2 == (int)mInputParamsVec.size())
		{
			LParam *paramA = mInputParamsVec[0];
			if (paramA)
			{
				paramA->PreCompile(script, file);
			}

			LParam *paramB = mInputParamsVec[1];
			if (paramB)
			{
				paramB->PreCompile(script, file);
			}
		}
		else
		{
			assertion(false, "");
		}
	}
	else if (MT_EVENT == bt)
	{
		LParam *paramEvent = GetLParamByName("Event");
		if (paramEvent)
		{
			paramEvent->PreCompile(script, file);
		}

		LParam *paramEventDo = GetLParamByName("EventDo");
		if (paramEventDo)
		{
			paramEventDo->PreCompile(script, file);
		}
	}
	else if (MT_PARAM == bt)
	{
		ParamType paramType = GetParamType();

		if (PT_VARIABLE == paramType)
		{
			LParam *param = GetInputParam(0);
			if (param)
			{
				if (!file->IsPreCompiledParam(name))
				{
					file->PreCompiledParams.push_back(name);

					FunParamType dataType = param->GetDataType();

					if (PT_ENGINE == pt || PT_NODEMCU == pt)
					{
						if (FPT_CHAR == dataType)
						{
							script += name + " = 0\n";
						}
						else if (FPT_INT == dataType)
						{
							script += name + " = 0\n";
						}
						else if (FPT_FLOAT == dataType)
						{
							script += name + " = 0.0\n";
						}
						else if (FPT_BOOL == dataType)
						{
							script += name + " = true\n";
						}
						else if (FPT_STRING == dataType)
						{
							script += name + " = \"\"\n";
						}
						else if (FPT_POINTER == dataType)
						{
							script += name + " = nil\n";
						}
					}
					else if (PT_ARDUINO == pt)
					{
						if (FPT_CHAR == dataType)
						{
							script += "char " + name + " = 0;\n";
						}
						else if (FPT_INT == dataType)
						{
							script += "int " + name + " = 0;\n";
						}
						else if (FPT_FLOAT == dataType)
						{
							script += "float " + name + " = 0.0;\n";
						}
						else if (FPT_BOOL == dataType)
						{
							script += "bool " + name + " = true;\n";
						}
						else if (FPT_STRING == dataType)
						{
							script += "String" + name + " = \"\";\n";
						}
						else if (FPT_POINTER == dataType)
						{
							script += "FPT_POINTER not support;";
						}
					}
				}
			}
		}	
	}

	LParam *paramNext = GetLParamByName("Next");
	if (paramNext)
	{
		paramNext->PreCompile(script, file);
	}
}
//----------------------------------------------------------------------------
void LBlock::CompileStart(std::string &script, int numTable, LFile *file)
{
	LogicFilePlatformType pt = file->GetPlatformType();

	const std::string &fileName = file->GetName();
	const std::string &name = GetName();
	BlockType bt = GetBlockType();
	CtrlType ct = GetCtrlType();

	if (MT_CONTROL == bt)
	{
		if (CT_PROGRAM == ct)
		{
			LParam *paramStart = GetLParamByName("Start");
			if (paramStart)
			{
				if (pt == PT_ENGINE)
				{
					paramStart->Compile(script, numTable + 1, file);
				}
			}
		}
		else if (CT_PROGRAMSTART == ct)
		{
			LParam *paramStart = GetLParamByName("Start");
			if (paramStart)
			{
				if (pt == PT_ENGINE)
				{
					paramStart->Compile(script, numTable + 1, file);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void LBlock::CompileFixUpdate(std::string &script, int numTable, LFile *file)
{
	LogicFilePlatformType pt = file->GetPlatformType();

	const std::string &fileName = file->GetName();
	const std::string &name = GetName();
	BlockType bt = GetBlockType();
	CtrlType ct = GetCtrlType();

	if (MT_CONTROL == bt)
	{
		if (CT_PROGRAM == ct)
		{
			LParam *paramFixUpdate = GetLParamByName("FixUpdate");
			if (paramFixUpdate)
			{
				if (pt == PT_ENGINE)
				{
					paramFixUpdate->Compile(script, numTable + 1, file);
				}
			}
		}
		else if (CT_PROGRAMFIXUPDATE == ct)
		{
			LParam *paramFixUpdate = GetLParamByName("FixUpdate");
			if (paramFixUpdate)
			{
				if (pt == PT_ENGINE)
				{
					paramFixUpdate->Compile(script, numTable + 1, file);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void LBlock::CompileAll(std::string &script, int numTable, LFile *file)
{
	LogicFilePlatformType pt = file->GetPlatformType();

	const std::string &name = GetName();
	BlockType bt = GetBlockType();
	CtrlType ct = GetCtrlType();

	if (MT_CONTROL == bt)
	{	
		if (CT_IF == ct)
		{
			if (pt == PT_ENGINE)
			{
				_WriteTables(script, numTable);
				script += "if ";

				LParam *paramIf = GetLParamByName("If");
				if (paramIf)
				{
					paramIf->Compile(script, numTable + 1, file);
				}

				script += " then\n";

				LParam *paramIfDo = GetLParamByName("IfDo");
				if (paramIfDo)
				{
					paramIfDo->Compile(script, numTable + 1, file);
				}

				_WriteTables(script, numTable);
				script += "end\n";
			}
			else if (pt == PT_ARDUINO)
			{
				_WriteTables(script, numTable);
				script += "if (";

				LParam *paramIf = GetLParamByName("If");
				if (paramIf)
				{
					paramIf->Compile(script, numTable + 1, file);
				}

				script += ")\n";

				_WriteTables(script, numTable);
				script += "{\n";

				LParam *paramIfDo = GetLParamByName("IfDo");
				if (paramIfDo)
				{
					paramIfDo->Compile(script, numTable + 1, file);
				}

				_WriteTables(script, numTable);
				script += "}\n";
			}
		}
		else if (CT_IFELSE == ct)
		{
			if (pt == PT_ENGINE)
			{
				_WriteTables(script, numTable);
				script += "if ";

				LParam *paramIf = GetLParamByName("If");
				if (paramIf)
				{
					paramIf->Compile(script, numTable + 1, file);
				}

				script += " then\n";

				LParam *paramIfDo = GetLParamByName("IfDo");
				if (paramIfDo)
				{
					paramIfDo->Compile(script, numTable + 1, file);
				}

				_WriteTables(script, numTable);
				script += "else\n";

				LParam *paramElseDo = GetLParamByName("ElseDo");
				if (paramElseDo)
				{
					paramElseDo->Compile(script, numTable + 1, file);
				}

				_WriteTables(script, numTable);
				script += "end\n";
			}
			else if (pt == PT_ARDUINO)
			{
				_WriteTables(script, numTable);
				script += "if (";

				LParam *paramIf = GetLParamByName("If");
				if (paramIf)
				{
					paramIf->Compile(script, numTable + 1, file);
				}

				script += " then\n";

				_WriteTables(script, numTable);
				script += "{\n";

				LParam *paramIfDo = GetLParamByName("IfDo");
				if (paramIfDo)
				{
					paramIfDo->Compile(script, numTable + 1, file);
				}

				_WriteTables(script, numTable);
				script += "}\n";

				_WriteTables(script, numTable);
				script += "else\n";

				_WriteTables(script, numTable);
				script += "{\n";

				LParam *paramElseDo = GetLParamByName("ElseDo");
				if (paramElseDo)
				{
					paramElseDo->Compile(script, numTable + 1, file);
				}

				_WriteTables(script, numTable);
				script += "}\n";
			}
		}
		else if (CT_WHILE == ct)
		{
			if (pt == PT_ENGINE)
			{
				_WriteTables(script, numTable);
				script += "while ";

				LParam *paramWhile = GetLParamByName("While");
				if (paramWhile)
				{
					paramWhile->Compile(script, numTable + 1, file);
				}

				script += " do\n";

				LParam *paramWhileDo = GetLParamByName("WhileDo");
				if (paramWhileDo)
				{
					paramWhileDo->Compile(script, numTable + 1, file);
				}

				_WriteTables(script, numTable);
				script += "end\n";
			}
			else if (pt == PT_ARDUINO)
			{
				_WriteTables(script, numTable);
				script += "while (";

				LParam *paramWhile = GetLParamByName("While");
				if (paramWhile)
				{
					paramWhile->Compile(script, numTable + 1, file);
				}

				script += ")\n";

				_WriteTables(script, numTable);
				script += "{\n";

				LParam *paramWhileDo = GetLParamByName("WhileDo");
				if (paramWhileDo)
				{
					paramWhileDo->Compile(script, numTable + 1, file);
				}

				_WriteTables(script, numTable);
				script += "}\n";
			}
		}
		else if (CT_COROUTINE == ct)
		{
			_WriteTables(script, numTable);
			script += "coroutine.wrap(function() \n";

			_WriteTables(script, numTable + 1);
			script += "PX2_LOGICM:SetCurLogicObject(self._ctrlable) \n";

			LParam *paramCorDo = GetLParamByName("CorDo");
			if (paramCorDo)
			{
				paramCorDo->Compile(script, numTable + 1, file);
			}

			_WriteTables(script, numTable);
			script += "end)()\n";
		}
	}
	else if (MT_EVENT == bt)
	{
		_WriteTables(script, numTable);

		script += "RegistEventObjectFunction(";
		LParam *paramEvent = GetLParamByName("Event");
		if (paramEvent)
		{
			paramEvent->Compile(script, numTable + 1, file);
		}
		script += ", self._ctrlable, function(ptr)\n";

		_WriteTables(script, numTable);
		script += "    if nil~= ptr then\n";

		_WriteTables(script, numTable);
		script += "        local obj = Cast:ToO(ptr)\n";
		_WriteTables(script, numTable);
		script += "        PX2_LOGICM:SetCurLogicObject(obj)\n";

		LParam *paramEventDo = GetLParamByName("EventDo");
		if (paramEventDo)
		{
			_WriteTables(script, numTable);
			paramEventDo->Compile(script, numTable + 1, file);
		}

		_WriteTables(script, numTable);
		script += "    end\n";

		_WriteTables(script, numTable);
		script += "end)\n";
	}
	else if (MT_FUNCTION_GENERAL == bt || MT_FUNCTION_OUTPUT == bt)
	{
		if (MT_FUNCTION_GENERAL == bt ||
			(MT_FUNCTION_OUTPUT == bt && mIsFunOutputConvertToGeneral))
		{
			_WriteTables(script, numTable);
		}

		bool isSet = ("setInt" == name || "setFloat" == name ||
			"setChar" == name || "setString" == name || "setObject" == name);
		bool isSleep = ("sleep" == name);

		if (isSet)
		{
			if (MT_FUNCTION_GENERAL == bt)
			{
				LParam *inParam = mInputParamsVec[0];
				LParam *inParam1 = mInputParamsVec[1];
				inParam->Compile(script, numTable + 1, file);
				script += " = ";
				inParam1->Compile(script, numTable + 1, file);

				if (pt == PT_ARDUINO)
				{
					script += ";";
				}

				script += "\n";
			}
		}
		else
		{
			bool isUseThisPointer = IsUseThisPointer();
			if (isUseThisPointer)
			{
				if ((int)mInputParamsVec.size() > 0)
				{
					LParam *param = mInputParamsVec[0];
					param->Compile(script, numTable + 1, file);
				}
			}

			if (pt == PT_ENGINE || pt == PT_NODEMCU)
			{
				if (pt == PT_ENGINE)
				{
					if (!mFunObject.SigletonName.empty())
					{
						script += mFunObject.SigletonName + ":";
					}
					else if (mFunObject.IsStatic)
					{
						script += mFunObject.ClassName + ".";
					}
					else
					{
						if (mFunObject.ClassName.empty())
						{
							/*_*/
						}
						else
						{
							if (!isUseThisPointer)
							{
								script += "self._ctrlable:";
							}
							else
							{
								script += ":";
							}
						}
					}
				}

				script += name + "(";
			}
			else if (pt == PT_ARDUINO)
			{
				if (!mFunObject.SigletonName.empty())
				{
					script += mFunObject.SigletonName + "." + name + "(";
				}
				else if (mFunObject.IsStatic)
				{
					script += mFunObject.ClassName + "::" + name + "(";
				}
				else
				{
					script += name + "(";
				}
			}
			for (int i = 0; i < (int)mInputParamsVec.size(); i++)
			{
				LParam *inParam = mInputParamsVec[i];
				if (inParam)
				{
					if (FPT_POINTER_THIS == inParam->GetDataType())
					{
					}
					else
					{
						inParam->Compile(script, numTable + 1, file);

						if ((i + 1) < (int)mInputParamsVec.size())
						{
							script += ", ";
						}
					}
				}
			}

			if (MT_FUNCTION_GENERAL == bt ||
				(MT_FUNCTION_OUTPUT == bt && mIsFunOutputConvertToGeneral))
			{
				script += ")";

				if (pt == PT_ARDUINO)
				{
					script += ";";
				}

				script += "\n";
			}
			else if (MT_FUNCTION_OUTPUT == bt ||
				(MT_FUNCTION_OUTPUT == bt && mIsFunOutputConvertToGeneral))
			{
				script += ")";
			}
		}

		if (isSleep)
		{
			_WriteTables(script, numTable);
			script += "PX2_LOGICM:SetCurLogicObject(self._ctrlable) \n";
		}
	}
	else if (MT_FUNCTION_OPERATOR == bt)
	{
		if (1 == (int)mInputParamsVec.size())
		{
			std::string useStr = name;
			useStr = name.substr(0, useStr.find_first_of("_"));

			if (pt == PT_ENGINE || pt == PT_NODEMCU)
			{
				if ("!" == useStr)
				{
					useStr = "not ";
				}
			}
			script += useStr;

			LParam *paramA = mInputParamsVec[0];
			if (paramA)
			{
				paramA->Compile(script, 0, file);
			}
		}
		else if (2 == (int)mInputParamsVec.size())
		{
			LParam *paramA = mInputParamsVec[0];
			if (paramA)
			{
				paramA->Compile(script, 0, file);
			}

			std::string useStr = name;
			useStr = name.substr(0, useStr.find_first_of("_"));
			if (pt == PT_ENGINE || pt == PT_NODEMCU)
			{
				if ("!=" == useStr)
				{
					useStr = "~=";
				}
				else if ("&&" == useStr)
				{
					useStr = "and";
				}
				else if ("||" == useStr)
				{
					useStr = "or";
				}
			}
			script += " " + useStr + " ";

			LParam *paramB = mInputParamsVec[1];
			if (paramB)
			{
				paramB->Compile(script, 0, file);
			}
		}
		else
		{
			assertion(false, "");
		}
	}
	else if (MT_PARAM == bt)
	{
		ParamType pt = GetParamType();

		if (PT_VARIABLE == pt)
		{
			script += name;
		}
		else if (PT_CONST == pt)
		{
			if ("TRUE" == name)
			{
				script += "true";
			}
			else if ("FALSE" == name)
			{
				script += "false";
			}
			else
			{
				script += name;
			}
		}
		else if (PT_VALUE == pt)
		{
			LParam *lParam = GetLParamByName("val");
			if (lParam)
			{
				script += lParam->GetValueScriptStr(true);
			}
		}
		else if (PT_ENUM == pt)
		{
			LParam *lParam = GetLParamByName("val");
			if (lParam)
			{
				script += lParam->GetValueScriptStr(false);
			}
		}
		else if (PT_ENUMSTRING == pt)
		{
			LParam *lParam = GetLParamByName("val");
			if (lParam)
			{
				script += lParam->GetValueScriptStr(true);
			}
		}
		else if (PT_VARIABLESELECT == pt)
		{
			LParam *lParam = GetLParamByName("val");
			if (lParam)
			{
				std::string scirptStr = lParam->GetValueScriptStr(false);
				script += scirptStr;
			}
		}
		else if (PT_VALUESELECT == pt)
		{
			LParam *lParam = GetLParamByName("val");
			if (lParam)
			{
				std::string scirptStr = lParam->GetValueScriptStr(true);
				script += scirptStr;
			}
		}
	}

	LParam *paramNext = GetLParamByName("Next");
	if (paramNext)
	{
		paramNext->Compile(script, numTable, file);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
LBlock::LBlock(LoadConstructor value) :
Object(value),
mOwnObjectParam(0),
mCtrlType(CT_NONE),
mIsFunOutputConvertToGeneral(false),
mIsUseThisPointer(false),
mParamType(PT_NONE),
mParent(0),
mLFile(0)
{

}
//----------------------------------------------------------------------------
void LBlock::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mBlockType);
	source.ReadBool(mIsFunOutputConvertToGeneral);
	source.ReadBool(mIsUseThisPointer);
	source.ReadString(mClassName);
	source.ReadEnum(mCtrlType);
	source.ReadEnum(mParamType);

	source.ReadString(mFunObjectName);

	int inputSize = 0;
	source.Read(inputSize);
	mInputParamsVec.resize(inputSize);
	for (int i = 0; i < inputSize; i++)
	{
		source.ReadPointer(mInputParamsVec[i]);
	}

	int outputParamSize = 0;
	source.Read(outputParamSize);
	mOutputParamsVec.resize(outputParamSize);
	for (int i = 0; i < outputParamSize; i++)
	{
		source.ReadPointer(mOutputParamsVec[i]);
	}

	source.ReadPointer(mBeforeParam);
	source.ReadPointer(mNextParam);

	source.ReadAggregate(mAnchorParam);

	PX2_END_DEBUG_STREAM_LOAD(LBlock, source);
}
//----------------------------------------------------------------------------
void LBlock::Link(InStream& source)
{
	Object::Link(source);

	int inputSize = (int)mInputParamsVec.size();
	for (int i = 0; i < inputSize; i++)
	{
		if (mInputParamsVec[i])
		{
			source.ResolveLink(mInputParamsVec[i]);
		}
	}

	int outputSize = (int)mOutputParamsVec.size();
	for (int i = 0; i < outputSize; i++)
	{
		if (mOutputParamsVec[i])
		{
			source.ResolveLink(mOutputParamsVec[i]);
		}
	}

	if (mBeforeParam)
	{
		source.ResolveLink(mBeforeParam);
	}

	if (mNextParam)
	{
		source.ResolveLink(mNextParam);
	}
}
//----------------------------------------------------------------------------
void LBlock::PostLink()
{
	Object::PostLink();

	FunObject *funObj = 0;
	if (!mClassName.empty() && !mFunObjectName.empty())
	{
		funObj = PX2_LOGICM.GetClassFunObject(mClassName, mFunObjectName);
	}
	else if(!mFunObjectName.empty())
	{
		funObj = PX2_LOGICM.GetFunObject(mFunObjectName);
	}

	if (funObj)
	{
		mFunObject = *funObj;
	}

	int inputSize = (int)mInputParamsVec.size();
	for (int i = 0; i < inputSize; i++)
	{
		if (mInputParamsVec[i])
		{
			mInputParamsVec[i]->PostLink();
			mInputParamsVec[i]->SetModule(this);

			mInputParams[mInputParamsVec[i]->GetName()] = mInputParamsVec[i];

			if (FPT_POINTER_THIS == mInputParamsVec[i]->GetDataType() ||
				FPT_POINTER_THIS_STATIC == mInputParamsVec[i]->GetDataType())
			{
				mOwnObjectParam = mInputParamsVec[i];
			}
		}
	}

	int outputSize = (int)mOutputParamsVec.size();
	for (int i = 0; i < outputSize; i++)
	{
		if (mOutputParamsVec[i])
		{
			mOutputParamsVec[i]->PostLink();
			mOutputParamsVec[i]->SetModule(this);

			mOutputParams[mOutputParamsVec[i]->GetName()] = mOutputParamsVec[i];
		}
	}

	if (mBeforeParam)
	{
		mBeforeParam->SetModule(this);
		mBeforeParam->PostLink();
	}

	if (mNextParam)
	{
		mNextParam->SetModule(this);
		mNextParam->PostLink();
	}
}
//----------------------------------------------------------------------------
bool LBlock::Register(OutStream& target) const
{
	if (Object::Register(target))
	{
		int inputSize = (int)mInputParamsVec.size();
		for (int i = 0; i < inputSize; i++)
		{
			target.Register(mInputParamsVec[i]);
		}

		int outputSize = (int)mOutputParamsVec.size();
		for (int i = 0; i < outputSize; i++)
		{
			target.Register(mOutputParamsVec[i]);
		}

		if (mBeforeParam)
			target.Register(mBeforeParam);

		if (mNextParam)
			target.Register(mNextParam);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LBlock::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mBlockType);
	target.WriteBool(mIsFunOutputConvertToGeneral);
	target.WriteBool(mIsUseThisPointer);
	target.WriteString(mClassName);
	target.WriteEnum(mCtrlType);
	target.WriteEnum(mParamType);

	target.WriteString(mFunObjectName);

	int inputSize = (int)mInputParamsVec.size();
	target.Write(inputSize);
	for (int i = 0; i < inputSize; i++)
	{
		target.WritePointer(mInputParamsVec[i]);
	}

	int outputSize = (int)mOutputParamsVec.size();
	target.Write(outputSize);
	for (int i = 0; i < outputSize; i++)
	{
		target.WritePointer(mOutputParamsVec[i]);
	}

	target.WritePointer(mBeforeParam);
	target.WritePointer(mNextParam);

	target.WriteAggregate(mAnchorParam);

	PX2_END_DEBUG_STREAM_SAVE(LBlock, target);
}
//----------------------------------------------------------------------------
int LBlock::GetStreamingSize(Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mBlockType);
	size += PX2_BOOLSIZE(mIsFunOutputConvertToGeneral);
	size += PX2_BOOLSIZE(mIsUseThisPointer);
	size += PX2_STRINGSIZE(mClassName);
	size += PX2_ENUMSIZE(mCtrlType);
	size += PX2_ENUMSIZE(mParamType);

	size += PX2_STRINGSIZE(mFunObjectName);

	int inputSize = (int)mInputParamsVec.size();
	size += sizeof(inputSize);
	if (inputSize > 0)
		size += inputSize * PX2_POINTERSIZE(mInputParamsVec[0]);

	int outputSize = (int)mOutputParamsVec.size();
	size += sizeof(outputSize);
	if (outputSize > 0)
		size += outputSize * PX2_POINTERSIZE(mOutputParamsVec[0]);

	size += PX2_POINTERSIZE(mBeforeParam);
	size += PX2_POINTERSIZE(mNextParam);

	size += sizeof(mAnchorParam);

	return size;
}
//----------------------------------------------------------------------------