// PX2FunObject.hpp

#ifndef PX2FUNOBJECT_HPP
#define PX2FUNOBJECT_HPP

#include "PX2CorePre.hpp"
#include "PX2Any.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2XMLNode.hpp"

namespace PX2
{

	enum PX2_ENGINE_ITEM FunParamType
	{
		FPT_NONE,
		FPT_CHAR,
		FPT_INT,
		FPT_FLOAT,
		FPT_FLOAT3,
		FPT_APOINT,
		FPT_AVECTOR,
		FPT_BOOL,
		FPT_STRING,
		FPT_POINTER,
		FPT_POINTER_THIS,
		FPT_POINTER_THIS_STATIC,
		FPT_LINK,
		FPT_MAX_TYPE
	};

	struct PX2_ENGINE_ITEM FunParam
	{
		FunParam();
		~FunParam();

		std::string Name;
		FunParamType Type;
		std::string TypeName;
		bool IsEnum;
		bool IsEnumItem;
		Any Value;
	};

	class PX2_ENGINE_ITEM FunObject
	{
	public:
		FunObject();
		~FunObject();

		void Save(XMLNode node);
		void Clear();

		std::string CateName;

		bool IsClassCatalogue;
		std::string ParentClassName;

		bool IsEnum;
		bool IsStatic;
		std::string ClassName;
		std::string Name;
		std::string SigletonName;
		std::string Tag;

		void AddInput(const std::string &paramName, FunParamType type, const std::string &tName,
			const Any &paramValue, bool isEnumItem = false, bool isEnum = false);
		void RemoveInput(const std::string &paramName);
		void AddOutput(const std::string &paramName, FunParamType type, const std::string &tName,
			const Any &paramValue, bool isEnum = false);

		const std::vector<FunParam> &GetInParams() const;
		int GetNumInParams() const;
		const FunParam &GetInParam(int i) const;
		void ClearInParams();

		const std::vector<FunParam> &GetOutParams() const;
		int GetNumOutParams() const;
		const FunParam &GetOutParam(int i) const;
		void ClearOutParams();

		enum ParamType
		{
			PT_NONE,
			PT_CONST,
			PT_VARIABLE,
			PT_VALUE,
			PT_ENUM,
			PT_ENUMSTRING,
			PT_VALUESELECT,
			PT_VARIABLESELECT,
			PT_MAXTYPE
		};
		void SetParamType(ParamType pt);
		ParamType GetParamType() const;

	protected:
		std::vector<FunParam> mInParams;
		std::vector<FunParam> mOutParams;
		ParamType mParamType;

	public:
		FunObject *GetClass (const std::string &className);
		FunObject *AddClass(const std::string &className);

		FunObject *GetEnum(const std::string &enumName);
		FunObject *AddEnum(const std::string &enumName);

		FunObject *GetFunObject(const std::string &className,
			const std::string &funName);

		void AddFunObject(FunObject *funObj);
		bool IsHasFunObject(FunObject *funObj);

		std::vector<FunObject*> GetParentAndMeFunObjectList();

	public:
		std::vector<Pointer0<FunObject> > mChildFunObjectVec_Class;
		std::vector<Pointer0<FunObject> > mChildFunObjectVec_Enum;
		std::vector<Pointer0<FunObject> > mChildFunObjectVec;

		FunObject *ParentFunObject;
	};

	typedef Pointer0<FunObject> FunObjectPtr;

	class PX2_ENGINE_ITEM FunObjectManager : public Singleton<FunObjectManager>
	{
	public:
		FunObjectManager();
		virtual ~FunObjectManager();

		bool Initlize();
		bool Terminate();

		void SetObjectFunObject(FunObject *funObject);
		FunObject *GetObjectFunObject();

		std::map<std::string, FunObjectPtr> GetFunMap();
		FunObject *GetAddFunMap(const std::string &name);

	protected:
		FunObjectPtr mObjectFunObject;
		std::map<std::string, FunObjectPtr> mFunMap;
	};

}

#endif