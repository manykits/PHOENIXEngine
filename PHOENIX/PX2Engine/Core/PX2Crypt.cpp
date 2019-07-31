// PX2Crypt.cpp

#include "PX2Crypt.hpp"
#include "PX2MD5.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
const int C1 = 52845;
const int C2 = 22719;
const int KEY = 72957;
//----------------------------------------------------------------------------
bool Crypt::Encrypt(unsigned char *source, unsigned char *destination, 
	unsigned long length)
{
	unsigned long	i;
	int Key = KEY;

	if (!source || !destination || length <= 0)
		return false;

	for (i=0; i<length; i++)
	{
		destination[i] = (unsigned char)(source[i]^Key >> 8);
		Key = (destination[i] + Key) * C1 + C2;
	}

	return true;
}
//----------------------------------------------------------------------------
bool Crypt::Decrypt(unsigned char *source, unsigned char *destination, 
	unsigned long length)
{
	unsigned long i;
	unsigned char PreviousBlock;
	int Key	= KEY;

	if (!source || !destination || length <= 0)
		return false;

	for (i=0; i<length; i++)
	{
		PreviousBlock = source[i];
		destination[i] = (unsigned char)(source[i] ^ Key >> 8);
		Key = (PreviousBlock + Key) * C1 + C2;
	}

	return true;
}
//----------------------------------------------------------------------------
std::string Crypt::GetPasswdMd5(const std::string &readablePasswd)
{
	char buffer[17], out[33];
	memset(buffer, 0, sizeof(buffer));
	memset(out, 0, sizeof(out));

	Md5HashBuffer(buffer, readablePasswd.c_str(), (int)readablePasswd.length());
	Md5HexString(buffer, out);

	memmove(out, out + 8, 16);
	out[16] = 0;

	return std::string(out);
}
//----------------------------------------------------------------------------