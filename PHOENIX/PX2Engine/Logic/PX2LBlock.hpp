// PX2LModule.hpp

#ifndef PX2LMODULE_HPP
#define PX2LMODULE_HPP

#include "PX2LogicPre.hpp"
#include "PX2LParam.hpp"
#include "PX2LogicDefine.hpp"

namespace PX2
{

	class LFile;

	class PX2_ENGINE_ITEM LBlock : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LBlock);

	public:
		// Logic
		enum BlockType
		{
			MT_EVENT,
			MT_OBJECT,
			MT_FUNCTION_START,
			MT_FUNCTION_GENERAL,
			MT_FUNCTION_OUTPUT,
			MT_FUNCTION_OPERATOR,
			MT_PARAM,
			MT_CONTROL,
			MT_MAX_TYPE
		};
		LBlock(BlockType type = MT_EVENT);
		virtual ~LBlock();

		BlockType GetBlockType() const;

		void SetFunOutputConvertToGeneral(bool convert);
		bool IsFunOutputConvertToGeneral() const;

		void SetUseThisPointer(bool useThisPointer);
		bool IsUseThisPointer() const;

		void SetClassName(const std::string &className);
		const std::string &GetClsName() const;

		// regist
		void RegistFunObj(const FunObject &funObj);
		const FunObject &GetFunObj() const;
		enum CtrlType
		{
			CT_NONE,
			CT_IF,
			CT_IFELSE,
			CT_WHILE,
			CT_COROUTINE,
			CT_PROGRAM,
			CT_PROGRAMSTART,
			CT_PROGRAMFIXUPDATE,
			CT_MAX_TYPE
		};
		void SetCtrlType(CtrlType ct);
		CtrlType GetCtrlType() const;

		enum ParamType
		{
			PT_NONE,
			PT_VARIABLE,
			PT_CONST,
			PT_VALUE,
			PT_ENUM,
			PT_ENUMSTRING,
			PT_VALUESELECT,
			PT_VARIABLESELECT,
			PT_MAX_TYPE
		};
		void SetParamType(ParamType pt);
		ParamType GetParamType() const;

		LParam *GetLParamByName(const std::string &name);

		// Input Output
		std::vector<LParamPtr> &GetInputParamsVec();
		const std::map<std::string, LParam *> &GetInputParams() const;
		LParam *GetInputParam(int i);
		int GetNumInputs() const;
		const LParam *GetOwnerObjectParam() const;

		std::vector<LParamPtr> &GetOutParamsVec();
		const std::map<std::string, LParam *> &GetOutputParams() const;
		LParam *GetOutputParam(int i);
		int GetNumOutputs() const;

		void SetParent(LParam *parent);
		LParam *GetParent();

		void SetAnchorParam(float hor, float ver);
		const Vector3f &GetAnchorParam() const;

	protected:
		BlockType mBlockType;
		bool mIsFunOutputConvertToGeneral;
		bool mIsUseThisPointer;
		std::string mClassName;
		CtrlType mCtrlType;
		ParamType mParamType;
		FunObject mFunObject;
		std::string mFunObjectName;

		std::vector<LParamPtr> mInputParamsVec;
		std::map<std::string, LParam*> mInputParams;
		LParam *mOwnObjectParam;

		std::vector<LParamPtr> mOutputParamsVec;
		std::map<std::string, LParam*> mOutputParams;

		LParam *mParent;

	protected:
		LParamPtr mBeforeParam;
		LParamPtr mNextParam;
		std::vector<LParamPtr> mActOutButs_Operator;
		Vector3f mAnchorParam;

	public_internal:
		void SetLFile(LFile *lfile);
		LFile *GetLFile();

	public:
		void PreCompile(std::string &script, LFile *file, bool isOriginalStart = false);

		void CompileStart(std::string &script, int numTable, LFile *file);
		void CompileFixUpdate(std::string &script, int numTable, LFile *file);
		void CompileAll(std::string &script, int numTable, LFile *file);

		void Save(const std::string &filename);
		
	protected:
		void _WriteTables(std::string &script, int numTable);

		LFile *mLFile;
	};

#include "PX2LBlock.inl"
	PX2_REGISTER_STREAM(LBlock);
	typedef Pointer0<LBlock> LBlockPtr;

}

#endif