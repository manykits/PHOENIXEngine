// PkgParser.hpp

#ifndef PKGPARSER_HPP
#define PKGPARSER_HPP

#include "PkgParsePre.hpp"
#include "PkgFunctionInfo.hpp"
#include "PkgClassInfo.hpp"
#include "PkgEnumInfo.hpp"

namespace PkgParse
{

	class PkgParser
	{
	public:
		PkgParser();
		~PkgParser();

		bool ParseFile(const std::string &filename);

	protected:
		void ParseLine(const std::string &filename);
		std::string StringToFilename(const std::string &str);
		void PraseEnumItem(const std::string &itemStr);
		void PraseFunction(const std::string &fun);
		ParamInfo *PraseFunctionParam(const std::string &paramStr);
		void PraseClassTemplate(const std::string &str);
		void PraseParam(const std::string &paramStr);

	protected:
		PkgClassInfo *mCurPkgClassInfo;

		enum FunState
		{
			FS_NONE,
			FS_BEGIN,
			FS_END,
			FS_MAX_TYPE
		};
		FunState mFunState;
		std::string mFunStr;
		std::vector<std::string> mFunTokens;

		std::string mParamStr;

		PkgEnumInfo *mCurEnumInfo;
		enum EnumState
		{
			ES_NONE,
			ES_BEGIN,
			ES_BEGIN_ITEMS,
			ES_END,
			ES_MAX_TYPE
		};
		EnumState mEnumState;
		std::string mEnumStr;
	};

}

#endif