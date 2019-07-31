// PX2MD5.hpp

#ifndef PX2MD5_HPP
#define PX2MD5_HPP

#include "PX2CorePre.hpp"

#ifdef __cplusplus
extern "C"{
#endif

/* MD5 context. */
typedef struct 
{
	unsigned long int state[4];                                   /* state (ABCD) */
	unsigned long int count[2];        /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

PX2_ENGINE_ITEM void MD5Init(MD5_CTX *);
PX2_ENGINE_ITEM void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
PX2_ENGINE_ITEM void MD5Final(unsigned char[16], MD5_CTX *);

/*
	输入const char *inBuffer、int length
	输出char *outBuffer
	其中length可为0,outBuffer的长度为MD5_DIGEST_LENGTH(16byte)
*/
PX2_ENGINE_ITEM void Md5HashBuffer(char *, const char *, int);
PX2_ENGINE_ITEM void Md5String(char *outBuffer, const char *inBuffer, int length);
PX2_ENGINE_ITEM void Md5File(char *outBuffer, const char *filename);
PX2_ENGINE_ITEM void Md5HexString(char in16[], char out32[]);

#ifdef __cplusplus
}
#endif

#endif
