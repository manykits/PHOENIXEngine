// PkgFunInfo.hpp

#ifndef PKGFUNCTIONINFO_HPP
#define PKGFUNCTIONINFO_HPP

#include "PkgParsePre.hpp"

namespace PkgParse
{

	class PkgClassInfo;
	class PkgFunInfo;

	class ParamInfo
	{
	public:
		ParamInfo();
		virtual ~ParamInfo();

		enum ParamType
		{
			PT_VOID,
			PT_INT,
			PT_FLOAT,
			PT_BOOL,
			PT_BOOLCHECK,
			PT_COLOR3FLOAT3,
			PT_FLOAT2,
			PT_FLOAT3,
			PT_FLOAT4,
			PT_APOINT3,
			PT_APOINT4,
			PT_AVECTOR3,
			PT_AVECTOR4,
			PT_SIZE,
			PT_RECT,
			PT_STRING,
			PT_STRINGBUTTON,
			PT_ENUM,
			PT_TRANSFORM,
			PT_EMFLOAT,
			PT_EMFLOATRANGE,
			PT_EMFLOAT3,
			PT_EMFLOAT3RANGE,
			PT_EMFLOAT3COLOR,
			PT_EMFLOAT3COLORRANGE,
			PT_CLASS,
			PT_MAX_TYPE
		};

		std::string ParamStr;

		bool IsReturn;

		ParamType TheParamType;
		std::string TypeStr;
		std::string GetTypeStrAS() const;
		std::string ValueNameStr;
		std::string DefaultValStr;

		bool IsPointer;
		bool IsRef;
		bool IsConst;

		bool IsInternalEnumType() const;
		std::string GetParamStr_FunPtr() const;

		std::string GetASStr() const;

		PkgFunInfo *FunInfo;

		// 成员变量
		bool IsStatic;
	};


	class PkgFunInfo
	{
	public:
		PkgFunInfo();
		virtual ~PkgFunInfo();

		enum FunType
		{
			FT_CLASS_CONSTRUCTOR,
			FT_CLASS_DESTRUCTOR,
			FT_CLASS_MEMBER,
			PT_CLASS_STATIC,
			PT_STATIC,
			PT_MAX_TYPE
		};
		FunType TheFunType;

		std::string FunStr;
		std::string GetASFunStr() const;
		std::string GetASFactoryFunStr() const;
		std::string GetASParamStr() const;
		std::string GetASParamStr_C_NoValue () const;
		std::string GetASValueStr() const;
		
		bool IsOperator;
		std::string FunName;

		std::vector<ParamInfo *> Params;
		ParamInfo *ReturnVal;

		bool IsVirtual;
		bool IsPureVirtual;
		bool IsInline;
		bool IsConst;

		PkgClassInfo *ClassInfo;
	};

}

#endif