// PX2Endian.hpp

#ifndef PX2ENDIAN_H
#define PX2ENDIAN_H

#include "PX2CorePre.hpp"

namespace PX2
{

	/// 字节序类
	/**
	* 字节顺序是指占内存多于一个字节类型的数据在内存中的存放顺序，通常有小端、
	* 大端两种字节顺序。小端字节序指低字节数据存放在内存低地址处，高字节数据存
	* 放在内存高地址处；大端字节序是高字节数据存放在低地址处，低字节数据存放在
	* 高地址处。
	* 比如 int a = 0x05060708 　　
	* 在BIG-ENDIAN的情况下存放为： 
	*	字节号 0  1  2  3 　　
	*	数据   05 06 07 08 　
	* 在LITTLE-ENDIAN的情况下存放为： 
	*	字节号 0  1  2  3 
	*	数据   08 07 06 05 
	*/
	class PX2_ENGINE_ITEM Endian
	{
	public:
		/// 字节类型是否是大字节序
		static bool IsBig ();

		/// 字节类型是否是小字节序
		static bool IsLittle ();

		// 交换字节顺序
		static void Swap2 (void* datum);
		static void Swap2 (int numItems, void* data);
		static void Swap4 (void* datum);
		static void Swap4 (int numItems, void* data);
		static void Swap8 (void* datum);
		static void Swap8 (int numItems, void* data);
		static void Swap (size_t itemSize, void* datum);
		static void Swap (size_t itemSize, int numItems, void* data);

	private:
		static bool msIsLittle;
	};

}

#endif
