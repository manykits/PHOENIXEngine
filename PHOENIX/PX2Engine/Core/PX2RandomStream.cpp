// PX2RandomStream.cpp

#include "PX2RandomStream.hpp"
#include "PX2Random.hpp"
#include "PX2SHA1Engine.hpp"

#if defined(_WIN32) || defined (WIN32)
#include <windows.h>
#include <wincrypt.h>
#elif defined (__LINUX__)
#include <stdio.h>  
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#endif

namespace PX2
{


	//----------------------------------------------------------------------------
	RandomBuf::RandomBuf() :
		BasicBufferedStreamBuf<char, std::char_traits<char> >(256, std::ios::in)
	{
	}
	//----------------------------------------------------------------------------
	RandomBuf::~RandomBuf()
	{
	}
	//----------------------------------------------------------------------------
	int RandomBuf::readFromDevice(char* buffer, std::streamsize length)
	{
		int n = 0;

#if defined(_WIN32) || defined (WIN32)
		HCRYPTPROV hProvider = 0;
		CryptAcquireContext(&hProvider, 0, 0, PROV_RSA_FULL,
			CRYPT_VERIFYCONTEXT);
		CryptGenRandom(hProvider, (DWORD)length, (BYTE*)buffer);
		CryptReleaseContext(hProvider, 0);
		n = static_cast<int>(length);
#else
#if defined(__LINUX__)
		int fd = open("/dev/urandom", O_RDONLY, 0);
		if (fd >= 0)
		{
			n = read(fd, buffer, length);
			close(fd);
		}
#endif
		if (n <= 0)
		{
			// x is here as a source of randomness, so it does not make
			// much sense to protect it with a Mutex.
			static uint32_t x = 0;
			Random rnd1(256);
			Random rnd2(64);
			x += rnd1.Next();

			n = 0;
			SHA1Engine engine;
			uint32_t t = (uint32_t)std::time(NULL);
			engine.update(&t, sizeof(t));
			void* p = this;
			engine.update(&p, sizeof(p));
			engine.update(buffer, length);
			uint32_t junk[32];
			engine.update(junk, sizeof(junk));
			while (n < length)
			{
				for (int i = 0; i < 100; ++i)
				{
					uint32_t r = rnd2.Next();
					engine.update(&r, sizeof(r));
					engine.update(&x, sizeof(x));
					x += rnd1.Next();
				}
				DigestEngine::Digest d = engine.digest();
				for (DigestEngine::Digest::const_iterator it = d.begin(); 
					it != d.end() && n < length; ++it, ++n)
				{
					engine.update(*it);
					*buffer++ = *it++;
				}
			}
		}
#endif
		return n;
	}
	//----------------------------------------------------------------------------
	RandomIOS::RandomIOS()
	{
	}
	//----------------------------------------------------------------------------
	RandomIOS::~RandomIOS()
	{
	}
	//----------------------------------------------------------------------------
	RandomBuf* RandomIOS::rdbuf()
	{
		return &_buf;
	}
	//----------------------------------------------------------------------------
	RandomInputStream::RandomInputStream() :
		std::istream(&_buf)
	{
		}
	//----------------------------------------------------------------------------
	RandomInputStream::~RandomInputStream()
	{
	}
	//----------------------------------------------------------------------------
}