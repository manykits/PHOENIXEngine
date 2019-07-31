// PX2FString.hpp

#ifndef PX2FSTRING_HPP
#define PX2FSTRING_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	typedef void *FStringHandle;

	class PX2_ENGINE_ITEM FString
	{
	public:
		static bool Initlize ();
		static bool Ternimate ();
		static FStringHandle Insert (const char *str, int length=-1);
		static void AddRef (FStringHandle h);
		static void Release (FStringHandle h);
		static const char *Handler2Ptr (FStringHandle h);

		FString ();
		FString (FStringHandle h);
		FString (const FString &str);
		FString (const char *str);
		FString (const char *str, int length);
		~FString ();

		operator const char *() const;
		FStringHandle GetHandle () const;
		int GetLength () const;
		FString SubStr (int start, int count=-1) const;

		FString &operator= (const FString &str);
		FString &operator= (const char *str);
		bool operator== (const FString &str) const;
		bool operator== (const char *str) const;
		bool operator!= (const FString &str) const;
		bool operator!= (const char *str) const;

	protected:
		union 
		{
			FStringHandle mHandle;
			char *mStr;
		};
	};

}

#endif