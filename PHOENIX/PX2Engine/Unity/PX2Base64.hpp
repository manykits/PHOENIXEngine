// PX2Base64.hpp

#ifndef PX2BASE64_HPP
#define PX2BASE64_HPP

#include "PX2UnityPre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Base64
	{
	public:
		Base64();
		~Base64();

		static std::string Encode(const char *str, int bytes);
		static std::string Encode(const std::string &s);
		static std::string Decode(const std::string &encoded_string, int &outLength);
		static std::string Decode(const char *str, int length, int &outLength);
	};

}

#endif /* BASE64_H_C0CE2A47_D10E_42C9_A27C_C883944E704A */