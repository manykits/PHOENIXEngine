// PX2StringHelp.hpp

#ifndef PX2STRINGHELP_HPP
#define PX2STRINGHELP_HPP

#include "PX2CorePre.hpp"
#include "PX2Ascii.hpp"
#include <stdlib.h>

namespace PX2
{

	class PX2_ENGINE_ITEM StringHelp
	{
	public:
		StringHelp();
		~StringHelp();

		static bool Initlize();

		// 将文件路径标准化（将"\\"换成'/'）
		static std::string StandardiseFilename(const std::string &path);

		// 将文件路径标准化（将"\\"换成'/'）, 如果末尾没有'/',添加'/'
		static std::string StandardisePath (const std::string &path);

		/// 将文件路径拆成文件夹名和文件名
		/**
		* 例如"E:\\Phoenix\\Data\\pxtf\\normal.pxtf"拆成的结果为
		* outPath = "E:/Phoenix/Data/pxtf/"
		* outBaseFileName = "normal.pxtf"
		*/
		static void SplitFilename (const std::string &fullName,
			std::string &outPath, std::string &outBaseFileName);

		/// 将文件名拆成文件名称和扩展名
		/**
		* 例如"normal.pxtf"拆成的结果为
		* outBaseName = "normal"
		* outExtention = "pxtf"
		*/
		static void SplitBaseFilename (const std::string &baseFileName,
			std::string &outBaseName, std::string &outExtention);

		/// 将文件路径拆成文件夹名，文件名称和扩展名称
		static void SplitFullFilename (const std::string &fullName,
			std::string &outPath, std::string &outBaseName, 
			std::string &outExtention);

		static std::string PathToWindowStyle (const std::string &fromPath);

		static int UnicodeToAnsi (char *dest, int maxChar, const wchar_t *src,
			int numSrcChar=-1);
		static int AnsiToUnicode (wchar_t *dest, int maxChar, const char *src, 
			int numSrcChar=-1);
		static int UnicodeToUTF8 (char *destBuf, int maxChar, const wchar_t *src, 
			int numSrcChar=-1);
		static int UTF8ToUnicode (wchar_t *destBuf, int maxChar, const char *src, 
			int numSrcChar=-1);

		static const char *UnicodeToAnsi (const wchar_t *src, int numSrcChar=-1);
		static wchar_t *AnsiToUnicode (const char *src, int numSrcChar=-1);
		static const char *UnicodeToUTF8( const wchar_t *src, int numSrcChar=-1);
		static const wchar_t *UTF8ToUnicode( const char *src, int numSrcChar=-1);

		static std::string IntToString (int val);
		static int StringToInt (const std::string &val);
		static std::string FloatToString (float val, int numPoint=1);
		static float StringToFloat(const std::string &val);
		static std::string LongToString(long val);
		static long StringToLong(const std::string &val);
		static void StringToXYZ(const std::string &val, float &x, float &y, float &z);
		static void StringToVal01(const std::string &val, int &val0, int &val1);

		static std::string FormatStr (const std::string &str, const std::vector<std::string> &params);
		static std::string FormatStr (const std::string &str, int val);
		static std::string FormatSeconds(int seconds);
		static std::string FormatColor(int r, int g, int b);

		static int StringToCharArray(char arr[], int lengthArr, const std::string &fromStr);

		static std::string StrToHex(const std::string &str);
		static int StrToHex(const char *str, int nLength, char *hex);
		static int HexToStr(const char *hex, char *str, int &length);
		static int HexToInt(const std::string &str);
		static int HexToInt(const char *str);

		// Hash
		static unsigned int Hash(const std::string &str, unsigned int type);
		static unsigned int Hash(const char *str, unsigned int type,
			int length=-1); //< 如果length<0,表示传入的字符串是已\0结尾
	};

	//----------------------------------------------------------------------------
	template <class S>
	S ToUpper(const S& str)
	{
		typename S::const_iterator it  = str.begin();
		typename S::const_iterator end = str.end();

		S result;
		result.reserve(str.size());
		while (it != end) result += Ascii::ToUpper(*it++);
		return result;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S& ToUpperInPlace(S& str)
	{
		typename S::iterator it  = str.begin();
		typename S::iterator end = str.end();

		while (it != end) { *it = Ascii::ToUpper(*it); ++it; }
		return str;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S ToLower(const S& str)
	{
		typename S::const_iterator it  = str.begin();
		typename S::const_iterator end = str.end();

		S result;
		result.reserve(str.size());
		while (it != end) result += Ascii::ToLower(*it++);
		return result;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S& ToLowerInPlace(S& str)
	{
		typename S::iterator it  = str.begin();
		typename S::iterator end = str.end();

		while (it != end) { *it = Ascii::ToLower(*it); ++it; }
		return str;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S& TrimRightInPlace(S& str)
		/// Removes all trailing whitespace in str.
	{
		int pos = int(str.size()) - 1;

		while (pos >= 0 && Ascii::IsSpace(str[pos])) --pos;
		str.resize(pos + 1);

		return str;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S& TrimInPlace(S& str)
		/// Removes all leading and trailing whitespace in str.
	{
		int first = 0;
		int last = int(str.size()) - 1;

		while (first <= last && Ascii::IsSpace(str[first])) ++first;
		while (last >= first && Ascii::IsSpace(str[last])) --last;

		str.resize(last + 1);
		str.erase(0, first);

		return str;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S Trim(const S& str)
	{
		int first = 0;
		int last = int(str.size()) - 1;

		while (first <= last && Ascii::IsSpace(str[first])) ++first;
		while (last >= first && Ascii::IsSpace(str[last])) --last;

		return S(str, first, last - first + 1);
	}
	//----------------------------------------------------------------------------
	template <class S>
	S& ReplaceInPlace(S& str, const S& from, const S& to, typename S::size_type start = 0)
	{
		assert(from.size() > 0);

		S result;
		typename S::size_type pos = 0;
		result.append(str, 0, start);
		do
		{
			pos = str.find(from, start);
			if (pos != S::npos)
			{
				result.append(str, start, pos - start);
				result.append(to);
				start = pos + from.length();
			}
			else result.append(str, start, str.size() - start);
		} while (pos != S::npos);
		str.swap(result);
		return str;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S& ReplaceInPlace(S& str, const typename S::value_type* from, const typename S::value_type* to, typename S::size_type start = 0)
	{
		assert(*from);

		S result;
		typename S::size_type pos = 0;
		typename S::size_type fromLen = std::strlen(from);
		result.append(str, 0, start);
		do
		{
			pos = str.find(from, start);
			if (pos != S::npos)
			{
				result.append(str, start, pos - start);
				result.append(to);
				start = pos + fromLen;
			}
			else result.append(str, start, str.size() - start);
		} while (pos != S::npos);
		str.swap(result);
		return str;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S& ReplaceInPlace(S& str, const typename S::value_type from, const typename S::value_type to = 0, typename S::size_type start = 0)
	{
		if (from == to) return str;

		typename S::size_type pos = 0;
		do
		{
			pos = str.find(from, start);
			if (pos != S::npos)
			{
				if (to) str[pos] = to;
				else str.erase(pos, 1);
			}
		} while (pos != S::npos);

		return str;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S& RemoveInPlace(S& str, const typename S::value_type ch, typename S::size_type start = 0)
	{
		return ReplaceInPlace(str, ch, 0, start);
	}
	//----------------------------------------------------------------------------
	template <class S>
	S Replace(const S& str, const S& from, const S& to, typename S::size_type start = 0)
		/// Replace all occurences of from (which must not be the empty string)
		/// in str with to, starting at position start.
	{
		S result(str);
		ReplaceInPlace(result, from, to, start);
		return result;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S Replace(const S& str, const typename S::value_type* from, const typename S::value_type* to, typename S::size_type start = 0)
	{
		S result(str);
		ReplaceInPlace(result, from, to, start);
		return result;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S Replace(const S& str, const typename S::value_type from, const typename S::value_type to = 0, typename S::size_type start = 0)
	{
		S result(str);
		ReplaceInPlace(result, from, to, start);
		return result;
	}
	//----------------------------------------------------------------------------
	template <class S>
	S Remove(const S& str, const typename S::value_type ch, typename S::size_type start = 0)
	{
		S result(str);
		ReplaceInPlace(result, ch, 0, start);
		return result;
	}
	//----------------------------------------------------------------------------
	template <class S, class It>
	int Icompare(
		const S& str,
		typename S::size_type pos,
		typename S::size_type n,
		It it2,
		It end2)
		/// Case-insensitive string comparison
	{
		typename S::size_type sz = str.size();
		if (pos > sz) pos = sz;
		if (pos + n > sz) n = sz - pos;
		It it1 = str.begin() + pos;
		It end1 = str.begin() + pos + n;
		while (it1 != end1 && it2 != end2)
		{
			typename S::value_type c1(static_cast<typename S::value_type>(Ascii::ToLower(*it1)));
			typename S::value_type c2(static_cast<typename S::value_type>(Ascii::ToLower(*it2)));
			if (c1 < c2)
				return -1;
			else if (c1 > c2)
				return 1;
			++it1; ++it2;
		}

		if (it1 == end1)
			return it2 == end2 ? 0 : -1;
		else
			return 1;
	}
	//----------------------------------------------------------------------------
	template <class S>
	int Icompare(const S& str1, const S& str2)
		// A special optimization for an often used case.
	{
		typename S::const_iterator it1(str1.begin());
		typename S::const_iterator end1(str1.end());
		typename S::const_iterator it2(str2.begin());
		typename S::const_iterator end2(str2.end());
		while (it1 != end1 && it2 != end2)
		{
			typename S::value_type c1(static_cast<typename S::value_type>(Ascii::ToLower(*it1)));
			typename S::value_type c2(static_cast<typename S::value_type>(Ascii::ToLower(*it2)));
			if (c1 < c2)
				return -1;
			else if (c1 > c2)
				return 1;
			++it1; ++it2;
		}

		if (it1 == end1)
			return it2 == end2 ? 0 : -1;
		else
			return 1;
	}
	//----------------------------------------------------------------------------
	template <class S>
	int Icompare(const S& str1, typename S::size_type n1, const S& str2, typename S::size_type n2)
	{
		if (n2 > str2.size()) n2 = str2.size();
		return Icompare(str1, 0, n1, str2.begin(), str2.begin() + n2);
	}
	//----------------------------------------------------------------------------
	template <class S>
	int Icompare(const S& str1, typename S::size_type n, const S& str2)
	{
		if (n > str2.size()) n = str2.size();
		return icompare(str1, 0, n, str2.begin(), str2.begin() + n);
	}
	//----------------------------------------------------------------------------
	template <class S>
	int Icompare(const S& str1, typename S::size_type pos, typename S::size_type n, const S& str2)
	{
		return Icompare(str1, pos, n, str2.begin(), str2.end());
	}
	//----------------------------------------------------------------------------
	template <class S>
	int Icompare(
		const S& str1,
		typename S::size_type pos1,
		typename S::size_type n1,
		const S& str2,
		typename S::size_type pos2,
		typename S::size_type n2)
	{
		typename S::size_type sz2 = str2.size();
		if (pos2 > sz2) pos2 = sz2;
		if (pos2 + n2 > sz2) n2 = sz2 - pos2;
		return icompare(str1, pos1, n1, str2.begin() + pos2, str2.begin() + pos2 + n2);
	}
	//----------------------------------------------------------------------------
	template <class S>
	int Icompare(
		const S& str1,
		typename S::size_type pos1,
		typename S::size_type n,
		const S& str2,
		typename S::size_type pos2)
	{
		typename S::size_type sz2 = str2.size();
		if (pos2 > sz2) pos2 = sz2;
		if (pos2 + n > sz2) n = sz2 - pos2;
		return icompare(str1, pos1, n, str2.begin() + pos2, str2.begin() + pos2 + n);
	}
	//----------------------------------------------------------------------------
	template <class S>
	int Icompare(
		const S& str,
		typename S::size_type pos,
		typename S::size_type n,
		const typename S::value_type* ptr)
	{
		typename S::size_type sz = str.size();
		if (pos > sz) pos = sz;
		if (pos + n > sz) n = sz - pos;
		typename S::const_iterator it = str.begin() + pos;
		typename S::const_iterator end = str.begin() + pos + n;
		while (it != end && *ptr)
		{
			typename S::value_type c1(static_cast<typename S::value_type>(Ascii::ToLower(*it)));
			typename S::value_type c2(static_cast<typename S::value_type>(Ascii::ToLower(*ptr)));
			if (c1 < c2)
				return -1;
			else if (c1 > c2)
				return 1;
			++it; ++ptr;
		}

		if (it == end)
			return *ptr == 0 ? 0 : -1;
		else
			return 1;
	}
	//----------------------------------------------------------------------------
	template <class S>
	int Icompare(
		const S& str,
		typename S::size_type pos,
		const typename S::value_type* ptr)
	{
		return icompare(str, pos, str.size() - pos, ptr);
	}
	//----------------------------------------------------------------------------
	template <class S>
	int Icompare(
		const S& str,
		const typename S::value_type* ptr)
	{
		return Icompare(str, 0, str.size(), ptr);
	}
	//----------------------------------------------------------------------------

}

#endif