// PX2FString.cpp

#include "PX2FString.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Memory.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

struct stShareString
{
	stShareString ()
		:
	Next(0),
	RefCount(0),
	Str(0)
	{
	}

	~stShareString()
	{
		if (Str)
		{
			delete[] Str;
		}
	}

	stShareString *Next;
	volatile long RefCount;
#ifdef _DEBUG
	unsigned int HashValue;
#endif
	char *Str;
};
//----------------------------------------------------------------------------
const int STR_HASH_BUFLEN = 513;
static stShareString *sHashTable[STR_HASH_BUFLEN];
static Mutex *sTableMutex = 0;
static Mutex *sRefMutex = 0;
//----------------------------------------------------------------------------
inline unsigned int StringHash(const char *pstr, int length=-1)
{
	return StringHelp::Hash(pstr, 3, length);
}
//----------------------------------------------------------------------------
bool FString::Initlize ()
{
	for( int i=0; i<STR_HASH_BUFLEN; i++ )
	{
		sHashTable[i] = 0;
	}

	sTableMutex = new Mutex();
	sRefMutex = new Mutex();

	return true;
}
//----------------------------------------------------------------------------
bool FString::Ternimate ()
{
	for (int i=0; i<STR_HASH_BUFLEN; i++)
	{
		stShareString *cur = sHashTable[i];

		while (0 != cur)
		{
			stShareString *next = cur->Next;
			delete(cur);
			cur = next;
		}

		sHashTable[i] = 0;
	}

	delete(sTableMutex);
	sTableMutex = 0;
	delete(sRefMutex);
	sRefMutex = 0;

	return true;
}
//----------------------------------------------------------------------------
FStringHandle FString::Insert (const char *str, int length)
{
	if (!str)
		return 0;

	if (length < 0)
		length = (int)strlen(str);

	unsigned int i = StringHash(str, length) % STR_HASH_BUFLEN;

	ScopedCS sc(sTableMutex);

	stShareString *cur = sHashTable[i];

	while (cur)
	{
		if(length==(int)strlen(cur->Str)
			&& 0==(int)strncmp(str, cur->Str, length))
		{
			cur->RefCount++;
			return (FStringHandle)cur->Str;
		}

		cur = cur->Next;
	}

	cur = new stShareString();
	cur->RefCount = 1;
	cur->Next = sHashTable[i];
	sHashTable[i] = cur;
	cur->Str = new char[length+1];

#ifdef _DEBUG
	cur->HashValue = StringHash(str, length);
#endif

	char *dstStr = cur->Str;
	memcpy(dstStr, str, length);
	dstStr[length] = '\0';

	return (FStringHandle)dstStr;
}
//----------------------------------------------------------------------------
stShareString *GetCache (const char *str)
{
	unsigned int i = StringHash(str) % STR_HASH_BUFLEN;

	ScopedCS sc(sTableMutex);

	stShareString *cur = sHashTable[i];

	while (cur != 0)
	{
		if (cur->Str == str)
		{
			return cur;
		}

		cur = cur->Next;
	}

	return 0;
}
//----------------------------------------------------------------------------
void FString::AddRef (FStringHandle h)
{
	const char *str = Handler2Ptr(h);
	if (0 == str)
		return;

	ScopedCS sc(sRefMutex);

#ifdef _DEBUG
    int temp = 0;
	temp = (int)sizeof(unsigned int);
#endif

	stShareString *cur = GetCache(str);

#ifdef _DEBUG
	assertion(cur->HashValue == StringHash(str),
		"HashValue should be the same.");
#endif

	cur->RefCount++;
}
//----------------------------------------------------------------------------
void FString::Release (FStringHandle h)
{
	const char *str = Handler2Ptr(h);
	if (0 == str)
		return;

	ScopedCS sc(sRefMutex);

	stShareString *cur = GetCache(str);

#ifdef _DEBUG
	assertion(cur->HashValue == StringHash(str),
		"HashValue should be the same.");
#endif

	cur->RefCount--;

	long newRefCount = cur->RefCount;
	if (newRefCount > 0)
		return;

	assertion(0==newRefCount, "newRefCount should be zero.\n");
	return;
}
//----------------------------------------------------------------------------
const char *FString::Handler2Ptr (FStringHandle h)
{
	return (const char *)h;
}
//----------------------------------------------------------------------------
FString::FString ()
	:
mHandle(0)
{
}
//----------------------------------------------------------------------------
FString::FString (FStringHandle h)
	:
mHandle(h)
{
	AddRef(h);
}
//----------------------------------------------------------------------------
FString::FString (const FString &str)
	:
mHandle(str.mHandle)
{
	AddRef(mHandle);
}
//----------------------------------------------------------------------------
FString::FString (const char *str)
{
	mHandle = Insert(str);
}
//----------------------------------------------------------------------------
FString::FString (const char *str, int length)
{
	mHandle = Insert(str, (int)length); 
}
//----------------------------------------------------------------------------
FString::~FString ()
{
	Release(mHandle);
}
//----------------------------------------------------------------------------
FString::operator const char *() const
{
	return Handler2Ptr(mHandle);
}
//----------------------------------------------------------------------------
FStringHandle FString::GetHandle () const
{
	return mHandle;
}
//----------------------------------------------------------------------------
int FString::GetLength () const
{
	if (!mHandle)
		return 0;

	return (int)strlen(Handler2Ptr(mHandle));
}
//----------------------------------------------------------------------------
FString FString::SubStr (int start, int count) const
{
	if (!mHandle)
		return FString();

	const char *p = Handler2Ptr(mHandle);
	std::string str(p);
	str = str.substr((size_t)start, (size_t)count);

	return FString(str.c_str());
}
//----------------------------------------------------------------------------
FString &FString::operator= (const FString &str)
{
	AddRef(str.mHandle);
	Release(mHandle);

	mHandle = str.mHandle;

	return *this;
}
//----------------------------------------------------------------------------
FString &FString::operator= (const char *str)
{
	Release(mHandle);
	mHandle = Insert(str);

	return *this;
}
//----------------------------------------------------------------------------
bool FString::operator== (const FString &str) const
{
	const char *strBuf = (const char *)str;

	if (0==mStr || 0==strBuf)
	{
		return mStr==strBuf;
	}

	return 0==strcmp(mStr, strBuf);
}
//----------------------------------------------------------------------------
bool FString::operator== (const char *str) const
{
	if (0==mStr || 0==str)
	{
		return mStr==str;
	}

	return 0==strcmp(mStr, str);
}
//----------------------------------------------------------------------------
bool FString::operator!= (const FString &str) const
{
	const char *strBuf = (const char *)str;

	if (0==mStr || 0==strBuf)
	{
		return mStr!=strBuf;
	}

	return 0!=strcmp(mStr, strBuf);
}
//----------------------------------------------------------------------------
bool FString::operator!= (const char *str) const
{
	if (0==mStr || 0==str)
	{
		return mStr!=str;
	}

	return 0!=strcmp(mStr, str);
}
//----------------------------------------------------------------------------