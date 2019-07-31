// PX2CharCoding.hpp

#ifndef PX2CHARCODING_HPP
#define PX2CHARCODING_HPP

#include "PX2UnityPre.hpp"
#include "PX2FontDefine.hpp"

namespace PX2
{

	class CharCoding
	{
	public:
		virtual ~CharCoding () {}

		/// 跳过字符串头部的空白，反回空白的字节长度
		virtual int JumpOverSpaces (const char *text) = 0;

		/// 从字符串头部取出一个字符，反回字符的字节长度
		virtual int GetAChar (const char *text, unsigned char *character) = 0;

		/// 取得指定字符的字节长度
		virtual int GetCharBytes (const unsigned char *character) = 0;

		/// 从字符串头部取得控制码，如果是控制码，返回控制码的字节长度，
		/// 否则返回0
		virtual int GetControlCode (const char *text, CharCtrlCode &ctrlCode, 
			bool doTransfer) = 0;

		/// 是否半角英文，包括标点
		virtual bool IsEnglish (const unsigned char *character) = 0;

		/// 是否半角英文标点符号
		virtual bool IsPunctuation (const unsigned char *character) = 0;
		virtual bool IsPoint(const unsigned char *character) = 0;

		/// 将字符转换为Unicode
		virtual unsigned short ToUnicode (const unsigned char *character) = 0;

		/// 将字符转换为在本字符编码方案范围内的唯一ID，用于hash key
		virtual unsigned int ToUniqueID (const unsigned char* character) = 0;
	};

}

#endif