// PX2LParam.cpp

#include "PX2LParam.hpp"
#include "PX2StringHelp.hpp"
#include "PX2LBlock.hpp"
#include "PX2LFile.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, LParam);
PX2_IMPLEMENT_STREAM(LParam);
PX2_IMPLEMENT_FACTORY(LParam);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, LParam);

//----------------------------------------------------------------------------
LParam::LParam(LParamType lpt):
mLParamType(lpt),
mDataType(FPT_NONE),
mModule(0),
mIsEnum(false)
{
}
//----------------------------------------------------------------------------
LParam::~LParam()
{
}
//----------------------------------------------------------------------------
void LParam::SetName(const std::string &name)
{
	Object::SetName(name);

	if ("SDT_NONE" == name)
	{
		int a = 0;
	}
}
//----------------------------------------------------------------------------
void LParam::SetDataType(FunParamType type)
{
	mDataType = type;

	if (FPT_NONE == type)
	{
		int a = 0;
	}
}
//----------------------------------------------------------------------------
void LParam::SetDataTypeName(const std::string &typeName)
{
	mDataTypeName = typeName;
}
//----------------------------------------------------------------------------
const std::string &LParam::GetDataTypeName() const
{
	return mDataTypeName;
}
//----------------------------------------------------------------------------
void LParam::SetEnum(bool isEnum)
{
	mIsEnum = isEnum;
}
//----------------------------------------------------------------------------
bool LParam::IsEnum() const
{
	return mIsEnum;
}
//----------------------------------------------------------------------------
void LParam::SetModule(LBlock *module)
{
	mModule = module;
}
//----------------------------------------------------------------------------
void LParam::SetLinkBlock(LBlock *block)
{
	if (GetLinkBlock())
	{
		GetLinkBlock()->SetParent(0);
	}

	mLinkBlock = (Object*)block;

	if (block)
	{
		block->SetParent(this);
	}
}
//----------------------------------------------------------------------------
LBlock *LParam::GetLinkBlock()
{
	return DynamicCast<LBlock>(mLinkBlock);
}
//----------------------------------------------------------------------------
void LParam::SetValue(const Any &any)
{
	mData = any;
}
//----------------------------------------------------------------------------
void LParam::SetValueFromString(const std::string &valueStr)
{
	if (FPT_BOOL == mDataType)
	{
		if ("true" == valueStr)
		{
			mData = true;
		}
		else if ("false" == valueStr)
		{
			mData = false;
		}
		else
		{
			mData = false;
		}
	}
	else if (FPT_CHAR == mDataType)
	{
		if (!valueStr.empty())
			mData = (int)valueStr[0];
		else
			mData = int(0);
	}
	else if (FPT_INT == mDataType)
	{
		if (!valueStr.empty())
			mData = StringHelp::StringToInt(valueStr);
		else
			mData = 0;
	}
	else if (FPT_FLOAT == mDataType)
	{
		if (!valueStr.empty())
			mData = StringHelp::StringToFloat(valueStr);
		else
			mData = 0.0f;
	}
	else if (FPT_STRING == mDataType)
	{
		mData = valueStr;
	}
}
//----------------------------------------------------------------------------
bool LParam::GetValueB() const
{
	if (FPT_BOOL == mDataType)
		return PX2_ANY_AS(mData, bool);

	return false;
}
//----------------------------------------------------------------------------
char LParam::GetValueChar() const
{
	if (FPT_CHAR == mDataType)
		return (char)PX2_ANY_AS(mData, int);

	return 0;
}
//----------------------------------------------------------------------------
int LParam::GetValueI() const
{
	if (FPT_INT == mDataType)
		return PX2_ANY_AS(mData, int);

	return 0;
}
//----------------------------------------------------------------------------
float LParam::GetValueF() const
{
	if (FPT_FLOAT == mDataType)
		return PX2_ANY_AS(mData, float);

	return 0.0f;
}
//----------------------------------------------------------------------------
Float3 LParam::GetValueF3() const
{
	if (FPT_FLOAT3 == mDataType)
		return PX2_ANY_AS(mData, Float3);

	return Float3::ZERO;
}
//----------------------------------------------------------------------------
APoint LParam::GetValueAPoint() const
{
	if (FPT_APOINT == mDataType)
		return PX2_ANY_AS(mData, APoint);

	return APoint::ORIGIN;
}
//----------------------------------------------------------------------------
AVector LParam::GetValueAVector() const
{
	if (FPT_AVECTOR == mDataType)
		return PX2_ANY_AS(mData, AVector);

	return AVector::ZERO;
}
//----------------------------------------------------------------------------
std::string LParam::GetValueStr() const
{
	if (FPT_STRING == mDataType)
		return PX2_ANY_AS(mData, std::string);

	return "";
}
//----------------------------------------------------------------------------
Object *LParam::GetValuePointer() const
{
	if (FPT_POINTER == mDataType || FPT_POINTER_THIS == mDataType)
		return PX2_ANY_AS(mData, Object*);

	return 0;
}
//----------------------------------------------------------------------------
std::string LParam::GetTypeScirptStr()
{
	std::string str;

	if (FPT_CHAR == mDataType)
	{
		str += "char";
	}
	else if (FPT_INT == mDataType)
	{
		str += "int";
	}
	else if (FPT_FLOAT == mDataType)
	{
		str += "float";
	}
	else if (FPT_BOOL == mDataType)
	{
		str += "bool";
	}
	else if (FPT_STRING == mDataType)
	{
		str += "string";
	}
	else if (FPT_POINTER == mDataType)
	{
		str += "obj";
	}
	else if (FPT_POINTER_THIS == mDataType)
	{
		str += "obj";
	}

	return str;
}
//----------------------------------------------------------------------------
std::string LParam::GetValueScriptStr(bool coverString)
{
	std::string str;

	if (FPT_CHAR == mDataType)
	{
		char cVal[2];
		cVal[0] = GetValueChar();
		cVal[1] = '\0';
		str += std::string(cVal);
	}
	else if (FPT_INT == mDataType)
	{
		str += StringHelp::IntToString(GetValueI());
	}
	else if (FPT_FLOAT == mDataType)
	{
		str += StringHelp::FloatToString(GetValueF());
	}
	else if (FPT_FLOAT3 == mDataType)
	{
		Float3 fVal3 = GetValueF3();
		str += "Float3("
			+ StringHelp::FloatToString(fVal3[0]) + ", "
			+ StringHelp::FloatToString(fVal3[1]) + ", "
			+ StringHelp::FloatToString(fVal3[2]) + ")";
	}
	else if (FPT_APOINT == mDataType)
	{
		APoint point = GetValueAPoint();
		str += "APoint("
			+ StringHelp::FloatToString(point[0]) + ", "
			+ StringHelp::FloatToString(point[1]) + ", "
			+ StringHelp::FloatToString(point[2]) + ")";
	}
	else if (FPT_AVECTOR == mDataType)
	{
		AVector vec = GetValueAVector();
		str += "AVector("
			+ StringHelp::FloatToString(vec[0]) + ", "
			+ StringHelp::FloatToString(vec[1]) + ", "
			+ StringHelp::FloatToString(vec[2]) + ")";
	}
	else if (FPT_BOOL == mDataType)
	{
		bool b = GetValueB();
		str += b ? "true" : "false";
	}
	else if (FPT_STRING == mDataType)
	{
		if (coverString)
		{
			str += "\"" + GetValueStr() + "\"";
		}
		else
		{
			str += GetValueStr();
		}
	}
	else if (FPT_POINTER == mDataType)
	{
		str += "Pointer_" + StringHelp::IntToString((int)(int64_t)GetValuePointer());
	}
	else if (FPT_POINTER_THIS == mDataType)
	{
		str += "Pointer_" + StringHelp::IntToString((int)(int64_t)GetValuePointer());
	}
	else if (FPT_POINTER_THIS_STATIC == mDataType)
	{
		str += GetName();
	}

	return str;
}
//----------------------------------------------------------------------------
void LParam::PreCompile(std::string &script, LFile *file)
{
	if (mLinkBlock)
	{
		((LBlock*)((Object*)mLinkBlock))->PreCompile(script, file, false);
	}
}
//----------------------------------------------------------------------------
void LParam::Compile(std::string &script, int numTable, LFile *file)
{
	if (mLinkBlock)
	{
		((LBlock*)((Object*)mLinkBlock))->CompileAll(script, numTable, file);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
LParam::LParam(LoadConstructor value) :
Object(value),
mModule(0),
mDataType(FPT_NONE)
{
}
//----------------------------------------------------------------------------
void LParam::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mLParamType);

	source.ReadEnum(mDataType);
	source.ReadString(mDataTypeName);

	if (FPT_CHAR == mDataType)
	{
		int v;
		source.Read(v);
		mData = v;
	}
	else if (FPT_INT == mDataType)
	{
		int v;
		source.Read(v);
		mData = v;
	}
	else if (FPT_FLOAT == mDataType)
	{
		float v;
		source.Read(v);
		mData = v;
	}
	else if (FPT_FLOAT3 == mDataType)
	{
		Float3 v;
		source.ReadAggregate(v);
		mData = v;
	}
	else if (FPT_APOINT == mDataType)
	{
		APoint v;
		source.ReadAggregate(v);
		mData = v;
	}
	else if (FPT_AVECTOR == mDataType)
	{
		AVector v;
		source.ReadAggregate(v);
		mData = v;
	}
	if (FPT_BOOL == mDataType)
	{
		bool b;
		source.ReadBool(b);
		mData = b;
	}
	else if (FPT_STRING == mDataType)
	{
		std::string v;
		source.ReadString(v);
		mData = v;
	}
	else if (FPT_POINTER == mDataType || FPT_POINTER_THIS == mDataType ||
		FPT_POINTER_THIS_STATIC == mDataType)
	{
		Object *p = 0;
		source.ReadPointer(p);
		mData = p;
	}

	source.ReadBool(mIsEnum);
	source.ReadPointer(mLinkBlock);

	PX2_END_DEBUG_STREAM_LOAD(LParam, source);
}
//----------------------------------------------------------------------------
void LParam::Link(InStream& source)
{
	Object::Link(source);

	if (mLinkBlock)
		source.ResolveLink(mLinkBlock);
}
//----------------------------------------------------------------------------
void LParam::PostLink()
{
	Object::PostLink();

	if (mLinkBlock)
	{
		mLinkBlock->PostLink();
	}
}
//----------------------------------------------------------------------------
bool LParam::Register(OutStream& target) const
{
	if (Object::Register(target))
	{
		if (mLinkBlock)
		{
			target.Register(mLinkBlock);
		}

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void LParam::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mLParamType);

	target.WriteEnum(mDataType);
	target.WriteString(mDataTypeName);

	if (FPT_CHAR == mDataType)
	{
		int v = PX2_ANY_AS(mData, int);
		target.Write((int)v);
	}
	else if (FPT_INT == mDataType)
	{
		int v = PX2_ANY_AS(mData, int);
		target.Write(v);
	}
	else if (FPT_FLOAT == mDataType)
	{
		float v = PX2_ANY_AS(mData, float);
		target.Write(v);
	}
	else if (FPT_FLOAT3 == mDataType)
	{
		Float3 v = PX2_ANY_AS(mData, Float3);
		target.WriteAggregate(v);
	}
	else if (FPT_APOINT == mDataType)
	{
		APoint v = PX2_ANY_AS(mData, APoint);
		target.WriteAggregate(v);
	}
	else if (FPT_AVECTOR == mDataType)
	{
		AVector v = PX2_ANY_AS(mData, AVector);
		target.WriteAggregate(v);
	}
	if (FPT_BOOL == mDataType)
	{
		bool b = PX2_ANY_AS(mData, bool);
		target.WriteBool(b);
	}
	else if (FPT_STRING == mDataType)
	{
		std::string v = PX2_ANY_AS(mData, std::string);
		target.WriteString(v);
	}
	else if (FPT_POINTER == mDataType || FPT_POINTER_THIS == mDataType ||
		FPT_POINTER_THIS_STATIC == mDataType)
	{
		Object *p = PX2_ANY_AS(mData, Object*);
		target.WritePointer(p);
	}

	target.WriteBool(mIsEnum);
	target.WritePointer(mLinkBlock);

	PX2_END_DEBUG_STREAM_SAVE(LParam, target);
}
//----------------------------------------------------------------------------
int LParam::GetStreamingSize(Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mLParamType);

	size += PX2_ENUMSIZE(mDataType);
	size += PX2_STRINGSIZE(mDataTypeName);

	if (FPT_CHAR == mDataType)
	{
		size += sizeof(int);
	}
	else if (FPT_BOOL == mDataType)
	{
		//bool b = PX2_ANY_AS(mData, bool);
		size += PX2_BOOLSIZE(b);
	}
	else if (FPT_INT == mDataType)
	{
		size += sizeof(int);
	}
	else if (FPT_FLOAT == mDataType)
	{
		size += sizeof(float);
	}
	else if (FPT_FLOAT3 == mDataType)
	{
		size += sizeof(Float3);
	}
	else if (FPT_APOINT == mDataType)
	{
		size += sizeof(APoint);
	}
	else if (FPT_AVECTOR == mDataType)
	{
		size += sizeof(AVector);
	}
	else if (FPT_STRING == mDataType)
	{
		std::string v = PX2_ANY_AS(mData, std::string);
		size += PX2_STRINGSIZE(v);
	}
	else if (FPT_POINTER == mDataType || FPT_POINTER_THIS == mDataType ||
		FPT_POINTER_THIS_STATIC == mDataType)
	{
		//Object *p = PX2_ANY_AS(mData, Object*);
		size += PX2_POINTERSIZE(0);
	}

	size += PX2_BOOLSIZE(mIsEnum);
	size += PX2_POINTERSIZE(mLinkBlock);

	return size;
}
//----------------------------------------------------------------------------