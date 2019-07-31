// PX2URI.hpp

#ifndef PX2URI_HPP
#define PX2URI_HPP

#include "PX2NetPre.hpp"

namespace PX2
{


	class PX2_ENGINE_ITEM URI
	{
	public:
		URI();
		~URI();

		static void encode(const std::string& str,
			const std::string& reserved, std::string& encodedStr);
		static void decode(const std::string& str, std::string& decodedStr,
			bool plusAsSpace = false);

		static const std::string ILLEGAL;
	};


}

#endif