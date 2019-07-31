// PX2Crypt.hpp

#ifndef PX2CRYPT_HPP
#define PX2CRYPT_HPP

#include "PX2CorePre.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM Crypt
	{
	public:
		Crypt ();
		~Crypt ();

		static bool Encrypt(unsigned char *source, unsigned char *destination, 
			unsigned long length);
		static bool Decrypt(unsigned char *source, unsigned char *destination, 
			unsigned long length);

		static std::string GetPasswdMd5(const std::string &readablePasswd);
	};

}

#endif