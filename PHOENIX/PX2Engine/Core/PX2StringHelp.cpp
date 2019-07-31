// PX2StringHelp.cpp

#include "PX2StringHelp.hpp"
#include "PX2Assert.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2Log.hpp"
using namespace PX2;
using namespace std;

#ifdef WIN32
	#include <Windows.h>
#endif

//----------------------------------------------------------------------------
#define MPQ_TOOLS_BUFSIZE 0x500
static unsigned int	mpqBuffer[MPQ_TOOLS_BUFSIZE];
static void InitMPQBuffer()
{
	unsigned int seed   = 0x00100001;
	unsigned int index1 = 0;
	unsigned int index2 = 0;
	int i;

	for (index1 = 0; index1 < 0x100; index1++)
	{
		for(index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
		{
			unsigned int temp1, temp2;
			seed  = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;

			seed  = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);

			mpqBuffer[index2] = (temp1 | temp2);
		}
	}
}
//----------------------------------------------------------------------------
StringHelp::StringHelp()
{
}
//----------------------------------------------------------------------------
StringHelp::~StringHelp()
{
}
//----------------------------------------------------------------------------
bool StringHelp::Initlize()
{
	InitMPQBuffer();

	return true;
}
//----------------------------------------------------------------------------
std::string StringHelp::StandardiseFilename(const std::string &path)
{
	std::string strPath = path;

	std::replace(strPath.begin(), strPath.end(), '\\', '/');

	return strPath;
}
//----------------------------------------------------------------------------
std::string StringHelp::StandardisePath (const std::string &path)
{
	std::string strPath = path;
	
	std::replace(strPath.begin(), strPath.end(), '\\', '/');

	if (strPath[strPath.length() - 1] != '/')
		strPath += '/';

	return strPath;
}
//----------------------------------------------------------------------------
void StringHelp::SplitFilename (const std::string &fullName,
						   std::string &outPath, std::string &outBaseFileName)
{
	std::string path = fullName;

	// Replace \ with / first
	std::replace(path.begin(), path.end(), '\\', '/');
	// split based on final /

	size_t i = path.find_last_of('/');

	if (i == std::string::npos)
	{
		outPath.clear();
		outBaseFileName = fullName;
	}
	else
	{
		outPath = path.substr(0, i+1);
		outBaseFileName = path.substr(i+1, path.size()-i-1);
	}
}
//----------------------------------------------------------------------------
void StringHelp::SplitFullFilename (const std::string &fullName,
							   std::string &outPath,
							   std::string &outBaseName,
							   std::string &outExtention)
{
	std::string outName;
	SplitFilename(fullName, outPath, outName);
	SplitBaseFilename(outName, outBaseName, outExtention);
}
//----------------------------------------------------------------------------
void StringHelp::SplitBaseFilename (const std::string &baseFileName,
							   std::string &outBaseName,
							   std::string &outExtention)
{
	size_t i = baseFileName.find_last_of(".");

	if (i == std::string::npos)
	{
		outExtention.clear();
		outBaseName = baseFileName;
	}
	else
	{
		outExtention = baseFileName.substr(i+1);
		outBaseName = baseFileName.substr(0, i);
	}
}
//----------------------------------------------------------------------------
std::string StringHelp::PathToWindowStyle (const std::string &fromPath)
{
	std::string path = fromPath;
	std::replace(path.begin(), path.end(), '/', '\\');

	return path;
}
//----------------------------------------------------------------------------
const size_t MAX_CONVERT = 4096*2;
static char s_DBCSBuffer[MAX_CONVERT];
static wchar_t s_UnicodeBuffer[MAX_CONVERT];
//----------------------------------------------------------------------------
int StringHelp::UnicodeToAnsi (char *dest, int maxChar, const wchar_t *src,
						  int numSrcChar)
{
#ifdef WIN32

	int nchar;
#ifdef _DEBUG
	nchar = WideCharToMultiByte(CP_ACP, WC_SEPCHARS|WC_COMPOSITECHECK, src, 
		numSrcChar, dest, 0, NULL, NULL );
	assertion(nchar<maxChar, "");
#endif
	nchar = WideCharToMultiByte(
		CP_ACP,
		WC_SEPCHARS | WC_COMPOSITECHECK,
		src,
		numSrcChar,
		dest,
		maxChar,
		NULL,
		NULL);
	assertion (nchar>0, "");
	return nchar;
#else

	assertion(false, "Not supported on other platform.");

	return 0;

#endif
}
//----------------------------------------------------------------------------
int StringHelp::AnsiToUnicode (wchar_t *dest, int maxChar, const char *src, 
						  int numSrcChar)
{
	#ifdef WIN32

	int nchar;
#ifdef _DEBUG
	nchar = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, src, numSrcChar, dest, 0);
	assertion(nchar<=maxChar, "");
#endif
	nchar = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		src,
		numSrcChar,
		dest,
		maxChar);
	assertion (nchar>0, "");
	return nchar;

#else

	assertion(false, "Not supported on other platform.");

	return 0;

#endif
}
//----------------------------------------------------------------------------
int StringHelp::UnicodeToUTF8 (char *dest, int maxChar, const wchar_t *src, 
	int numSrcChar)
{
	unsigned char *destTemp = (unsigned char *)dest;
	const wchar_t *cur = src;
	int srcCount = numSrcChar<0 ? 2147483647 : numSrcChar;

	while (*cur && srcCount>0)
	{
		srcCount--;
		wchar_t c = *cur++;

		if( c < 0x80 )
		{
			*destTemp++ = (unsigned char)c;
		}
		else if( c < 0x800 )
		{
			*destTemp++ = (unsigned char)((c>>6) | 0xC0);
			*destTemp++ = (unsigned char)((c&0x3F) | 0x80);
		}
		else
		{
			*destTemp++ = (unsigned char)((c>>12) | 0xE0);
			*destTemp++ = (unsigned char)(((c>>6)&0x3F) | 0x80);
			*destTemp++ = (unsigned char)((c&0x3F) | 0x80);
		}
	}
	*destTemp++ = 0;

	int nunChar = int((char *)destTemp - (char *)dest - 1);

	assertion(nunChar<maxChar, "nunChar<maxChar\n");

	return nunChar;
}
//----------------------------------------------------------------------------
int StringHelp::UTF8ToUnicode (wchar_t *dest, int maxChar, const char *src, 
	int numSrcChar)
{
	wchar_t *destTemp = dest;
	const unsigned char *cur = (unsigned char *)src;
	int srcCount = numSrcChar<0 ? 2147483647 : numSrcChar;

	while( *cur && srcCount>0 )
	{
		srcCount--;
		unsigned char head = *cur++;
		unsigned char mask = 0x80;
		wchar_t  c = 0;

		int bitshift = 0;
		if( head&mask )
		{
			mask >>= 1;
			while( head&mask )
			{
				assert( *cur!=0 && srcCount>0 );
				srcCount--;

				mask >>= 1;
				c = (c<<6) | (*cur++ & 0x3F);
				bitshift += 6;
			}
		}

		c |= (head&(mask-1)) << bitshift;
		*destTemp++ = c;
	}
	*destTemp++ = 0;

	int numChar = int((char *)destTemp - (char *)dest - 1);

	assertion(numChar<maxChar, "nchar<maxChar");

	return numChar;
}
//----------------------------------------------------------------------------
const char *StringHelp::UnicodeToAnsi (const wchar_t *src, int numSrcChar)
{
	StringHelp::UnicodeToAnsi(s_DBCSBuffer, MAX_CONVERT, src, numSrcChar);
	return s_DBCSBuffer;
}
//----------------------------------------------------------------------------
wchar_t *StringHelp::AnsiToUnicode (const char *src, int numSrcChar)
{
	StringHelp::AnsiToUnicode(s_UnicodeBuffer, MAX_CONVERT, src, numSrcChar);
	return s_UnicodeBuffer;
}
//----------------------------------------------------------------------------
const size_t gStringHelpMaxConvert = 4096*2;
static char gsStringHelpDBCSBuffer[gStringHelpMaxConvert];
static wchar_t gsStringHelpUnicodeBuffer[gStringHelpMaxConvert];
//----------------------------------------------------------------------------
const char *StringHelp::UnicodeToUTF8( const wchar_t *src, int numSrcChar)
{
	UnicodeToUTF8(gsStringHelpDBCSBuffer, gStringHelpMaxConvert, src,
		numSrcChar);

	return gsStringHelpDBCSBuffer;
}
//----------------------------------------------------------------------------
const wchar_t *StringHelp::UTF8ToUnicode( const char *src, int numSrcChar)
{
	UTF8ToUnicode(gsStringHelpUnicodeBuffer, gStringHelpMaxConvert, src, 
		numSrcChar);

	return gsStringHelpUnicodeBuffer;
}
//----------------------------------------------------------------------------
std::string StringHelp::IntToString (int val)
{
	char c[24];
	memset(c, 0, 24* sizeof(char));
	string str;
	sprintf(c, "%d", val);
	str = c;
	return str;
}
//----------------------------------------------------------------------------
int StringHelp::StringToInt (const std::string &val)
{
	return atoi(val.c_str());
}
//----------------------------------------------------------------------------
std::string StringHelp::FloatToString (float val, int numPoint)
{
	char c[24] = { 0 };
	string str;
	if (1 == numPoint)
		sprintf(c, "%.1f", val);
	else if (2 == numPoint)
		sprintf(c, "%.2f", val);
	else if (3 == numPoint)
		sprintf(c, "%.3f", val);
	else if (4 == numPoint)
		sprintf(c, "%.4f", val);
	else if (5 == numPoint)
		sprintf(c, "%.5f", val);
	else if (6 == numPoint)
		sprintf(c, "%.6f", val);
	else if (7 == numPoint)
		sprintf(c, "%.7f", val);
	else if (8 == numPoint)
		sprintf(c, "%.8f", val);
	str = c;
	return str;
}
//----------------------------------------------------------------------------
float StringHelp::StringToFloat(const std::string &val)
{
	return (float)atof(val.c_str());
}
//----------------------------------------------------------------------------
std::string StringHelp::LongToString(long val)
{
	string str;
#if defined WIN32 || defined _WIN32
	char c[64];
	memset(c, 0, 64 * sizeof(char));
	ltoa(val, c, 10);
	str = c;
#endif
	return str;
}
//----------------------------------------------------------------------------
long StringHelp::StringToLong(const std::string &val)
{
	return atol(val.c_str());
}
//----------------------------------------------------------------------------
void StringHelp::StringToXYZ(const std::string &val, float &x, float &y, float &z)
{
	StringTokenizer st(val, ",");
	x = StringToFloat(st[0]);
	y = StringToFloat(st[1]);
	z = StringToFloat(st[2]);
}
//----------------------------------------------------------------------------
void StringHelp::StringToVal01(const std::string &val, int &val0, int &val1)
{
	StringTokenizer st(val, ",");
	val0 = StringToInt(st[0]);
	val1 = StringToInt(st[1]);
}
//----------------------------------------------------------------------------
std::string StringHelp::FormatStr (const std::string &str,
	const std::vector<std::string> &params)
{
	std::string resStr = str;
	std::string strFind("%s");

	int paramIndex = 0;
	bool doWhile = true;
	while (doWhile)   
	{
		std::string::size_type pos(0);     
		if ((pos=resStr.find(strFind))!=string::npos)
		{
			std::string val;
			if (paramIndex < (int)params.size())
				val = params[paramIndex];

			resStr.replace(pos, strFind.length(), val);   
		}  
		else
		{
			doWhile = false;
		}
	}     

	return resStr;
}
//----------------------------------------------------------------------------
std::string StringHelp::FormatStr (const std::string &str, int val)
{
	std::string valStr = StringHelp::IntToString(val);
	std::vector<std::string> valStrs;
	valStrs.push_back(valStr);

	return FormatStr(str, valStrs);
}
//----------------------------------------------------------------------------
unsigned int StringHelp::Hash(const std::string &str, unsigned int type)
{
	return Hash(str.c_str(), type);
}
//----------------------------------------------------------------------------
unsigned int StringHelp::Hash(const char *str, unsigned int type,
	int length)
{
	unsigned int seed1 = 0x7FED7FED;
	unsigned int seed2 = 0xEEEEEEEE;
	unsigned int ch;

	if (length < 0)
	{
		while (*str != 0) 
		{
			ch = toupper(*str++);
			seed1 = mpqBuffer[(type<<8) + ch] ^ (seed1 + seed2);
			seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
		}
	}
	else
	{
		while(length > 0) 
		{
			ch = toupper(*str++);
			seed1 = mpqBuffer[(type<<8) + ch] ^ (seed1 + seed2);
			seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;

			length--;
		}
	}

	return seed1;
}
//----------------------------------------------------------------------------
std::string StringHelp::FormatSeconds (int seconds)
{
	int min = seconds/60;
	int sec = seconds%60;

	std::string text = "";
	if (seconds <= 60)
	{
		if (seconds < 10)
		{
			text = "0" + StringHelp::IntToString(seconds);
		}
		else
		{
			text = StringHelp::IntToString(seconds);
		}
	}
	else
	{
		if (sec < 10)
		{
			text = StringHelp::IntToString(min) + ":" + "0" 
				+ StringHelp::IntToString(sec);
		}
		else
		{
			text = StringHelp::IntToString(min) + ":" + 
				StringHelp::IntToString(sec);
		}
	}

	return text;
}
//----------------------------------------------------------------------------
std::string StringHelp::FormatColor(int r, int g, int b)
{
	std::string colorStr =
		StringHelp::IntToString(r) + "," +
		StringHelp::IntToString(g) + "," +
		StringHelp::IntToString(b);

	return colorStr;
}
//----------------------------------------------------------------------------
int StringHelp::StringToCharArray(char arr[], int lengthArr, 
	const std::string &fromStr)
{
	if (fromStr.empty())
	{
		memset(arr, 0, lengthArr);
		return 0;
	}

	int lengthStr = (int)fromStr.length();
	int copyStrLen = lengthStr;
	if (lengthStr > lengthArr - 1) copyStrLen = lengthArr - 1;

	memcpy(arr, fromStr.c_str(), copyStrLen);
	arr[copyStrLen] = '\0';

	return copyStrLen;
}
//----------------------------------------------------------------------------
/*----------------------------------------
函数名称：int StrToHex(const char *str, char *hex, UINT nLength)
功        能：将字符串（不一定是以'/0'结尾的）转化为十六进制表示的字符串,如："HELLO"-->"48 45 4C 4C 4F"
nLength可以指定为字符串的实际长度
参数说明：str--------需要转换的字符串
hex -------转换后的字符串
nLength----需要转换的字符串的长度(对不以'/0'结束的无符号数组，需要指明长度)
返 回 值：零，成功
制 作 人：yaodebo
时       间：2006-10-21
----------------------------------------*/
//----------------------------------------------------------------------------
std::string StringHelp::StrToHex(const std::string &str)
{
	if (str.empty())
		return "";

	std::string strRet;
	strRet.resize(str.length() * 2);
	StrToHex(str.c_str(), str.length(), (char*)strRet.c_str());

	return strRet;
}
//----------------------------------------------------------------------------
int StringHelp::StrToHex(const char *str, int nLength, char *hex)
{
	int i;
	for (i = 0; i<(int)nLength; i++)
	{
		sprintf(hex + i * 2, "%02X", str[i] & 0xFF);
	}
	return 0;
}
//----------------------------------------------------------------------------
int StringHelp::HexToStr(const char *hex, char *str, int &length)
{
	int i, hlen, iData, iFlag;
	char ch;

	length = 0;
	iData = 0;
	iFlag = 0;
	hlen = (int)strlen(hex);

	for (i = 0; i<hlen; i++)
	{
		ch = hex[i];
		if (' ' == ch)
		{
			continue;
		}

		if ((ch >= '0') && (ch <= '9'))
		{
			ch -= 0x30;
		}
		else if ((ch >= 'a') && (ch <= 'f'))
		{
			ch -= 0x57;
		}
		else if ((ch >= 'A') && (ch <= 'F'))
		{
			ch -= 0x37;
		}

		if (0 == iFlag)
		{
			iData = ch;
			iFlag = 1;
		}
		else
		{
			iData = (iData << 4) + ch;
			iFlag = 0;
			*str++ = (char)iData;
			length++;
		}//enf if
	}//enf for

	return 0;

}
//----------------------------------------------------------------------------
int char2Init(char chara)
{
	if ('0' == chara)
		return 0;
	else if ('1' == chara)
		return 1;
	else if ('2' == chara)
		return 2;
	else if ('3' == chara)
		return 3;
	else if ('4' == chara)
		return 4;
	else if ('5' == chara)
		return 5;
	else if ('6' == chara)
		return 6;
	else if ('7' == chara)
		return 7;
	else if ('8' == chara)
		return 8;
	else if ('9' == chara)
		return 9;
	else if ('A' == chara)
		return 10;
	else if ('B' == chara)
		return 11;
	else if ('C' == chara)
		return 12;
	else if ('D' == chara)
		return 13;
	else if ('E' == chara)
		return 14;
	else if ('F' == chara)
		return 15;
	else if ('a' == chara)
		return 10;
	else if ('b' == chara)
		return 11;
	else if ('c' == chara)
		return 12;
	else if ('d' == chara)
		return 13;
	else if ('e' == chara)
		return 14;
	else if ('f' == chara)
		return 15;

	return 0;
}
//----------------------------------------------------------------------------
int StringHelp::HexToInt(const std::string &str)
{
	int ret = 0;

	int length = (int)str.length();
	for (int i = 0; i<= length-1; i++)
	{
		char chara = str[i];
		int iVal = char2Init(chara);

		ret += iVal * (int)(pow(16, length - 1 - i));
	}

	return ret;
}
//----------------------------------------------------------------------------
int StringHelp::HexToInt(const char *str)
{
	int ret = 0;

	int length = strlen(str);
	for (int i = length - 1; i >= 0; i--)
	{
		char chara = str[i];
		int iVal = char2Init(chara);

		ret += iVal * (int)(pow(16, i));
	}

	return ret;
}
//----------------------------------------------------------------------------
