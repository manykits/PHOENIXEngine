// PkgClassInfo.hpp

#ifndef PKGCLASSINFO_HPP
#define PKGCLASSINFO_HPP

#include "PkgFunctionInfo.hpp"

namespace PkgParse
{

	class PkgEnumInfo;

	class PkgClassInfo
	{
	public:
		PkgClassInfo();
		~PkgClassInfo();

		std::string SingletonName;
		bool IsSingleton() const;

		bool IsRefObject;
		bool IsUseRefIncrease;

		bool IsTemplate;
		bool TemplateFloat;
		bool TemplateDouble;
		bool TemplateInt;

		bool IsValueType;

		std::string ClassName;
		std::string GetClassNameAS();

		bool IsVirtual;

		std::vector<PkgEnumInfo*> EnumInfos;
		bool IsContainEnumType(const std::string &type);

		void AddFunInfo(PkgFunInfo *info);
		std::vector<PkgFunInfo*> &GetFunctions();
		std::vector<PkgFunInfo*> &GetConstructorFunctions();
		bool IsHasConstruct() const;
		bool IsHasDestruct() const;
		PkgFunInfo *GetDestructorFunction();

		std::vector<ParamInfo*> MemberProps;

		std::string ParentClassName;
		PkgClassInfo *ParentClassInfo;

		void ClearRegistFun();
		bool IsFunRegisted(const std::string &funStrAS) const;
		void AddRegistFun(const std::string &funStrAS);

		void ClearRegistParam();
		bool IsParamRegisted(const std::string &paramStrAS) const;
		void AddRegistParam(const std::string &paramStrAS);

		std::vector<PkgClassInfo*> ChildClassInfos;

	protected:
		std::vector<PkgFunInfo*> mFunctions;
		std::vector<PkgFunInfo*> mConstructorFunctions;
		PkgFunInfo *mDestructorFunction;

		std::map<std::string, int> mRegistedFuns;
		std::map<std::string, int> mRegistedParams;
	};

}

#endif