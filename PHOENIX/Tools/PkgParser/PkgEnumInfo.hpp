// PkgEnumInfo.hpp

#ifndef PKGENUMINFO_HPP
#define PKGENUMINFO_HPP

#include "PkgParsePre.hpp"

namespace PkgParse
{

	class PkgClassInfo;

	class PkgEnumInfo
	{
	public:
		PkgEnumInfo();
		~PkgEnumInfo();

		std::string EnumName;
		std::vector<std::string> EnumItemStrs;
		bool IsContainEnumType(const std::string &type);

		PkgClassInfo *ClassInfo;
	};

}

#endif