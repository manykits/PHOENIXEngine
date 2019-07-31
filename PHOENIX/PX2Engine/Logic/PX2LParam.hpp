// PX2LParam.hpp

#ifndef PX2LPARAM_HPP
#define PX2LPARAM_HPP

#include "PX2LogicPre.hpp"
#include "PX2Object.hpp"
#include "PX2FunObject.hpp"
#include "PX2Float3.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2LogicDefine.hpp"
 
namespace PX2
{

	class LBlock;
	class LFile;

	class PX2_ENGINE_ITEM LParam : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LParam);

	public:
		enum LParamType
		{
			IPT_IN,
			IPT_OUT,
			IPT_IN_EXE,
			IPT_OUT_EXE,
			IPT_MAX_TYPE
		};

		LParam(LParamType lpt = IPT_IN);
		virtual ~LParam();

		virtual void SetName(const std::string &name);

		LParamType GetLParamType() const;

		void SetDataType(FunParamType type);
		FunParamType GetDataType() const;
		void SetDataTypeName(const std::string &typeName);
		const std::string &GetDataTypeName() const;
		bool IsOwnObjectParam() const;

		void SetEnum(bool isEnum);
		bool IsEnum() const;

		void SetValue(const Any &any);
		void SetValueFromString(const std::string &valueStr);
		const Any &GetValue() const;
		bool GetValueB() const;
		char GetValueChar() const;
		int GetValueI() const;
		float GetValueF() const;
		Float3 GetValueF3() const;
		APoint GetValueAPoint() const;
		AVector GetValueAVector() const;
		std::string GetValueStr() const;
		Object *GetValuePointer() const;

		std::string GetTypeScirptStr();
		std::string GetValueScriptStr(bool coverString);
		const std::string &GetOutputScriptVarString() const;

		LBlock *GetModule();
		const LBlock *GetModule() const;

		LBlock *GetLinkBlock();

	public_internal:
		void SetModule(LBlock *module);
		void SetLinkBlock(LBlock *block);

	protected:
		LParamType mLParamType;
		FunParamType mDataType;
		std::string mDataTypeName;
		Any mData;
		bool mIsEnum;
		std::string mOutputScriptVarString;

		LBlock *mModule;
		ObjectPtr mLinkBlock;

	public:
		void PreCompile(std::string &script, LFile *file);
		void Compile(std::string &script, int numTable, LFile *file);
	};

	PX2_REGISTER_STREAM(LParam);
	typedef Pointer0<LParam> LParamPtr;
#include "PX2LParam.inl"

}

#endif