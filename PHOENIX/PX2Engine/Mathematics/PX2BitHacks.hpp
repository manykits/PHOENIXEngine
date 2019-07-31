// PX2BitHacks.hpp

#ifndef PX2BITHACKS_HPP
#define PX2BITHACKS_HPP

#include "PX2MathematicsPre.hpp"

namespace PX2
{

	PX2_ENGINE_ITEM bool IsPowerOfTwo(unsigned int value);
	PX2_ENGINE_ITEM bool IsPowerOfTwo(int value);

	PX2_ENGINE_ITEM unsigned int Log2OfPowerOfTwo(unsigned int powerOfTwo);
	PX2_ENGINE_ITEM int Log2OfPowerOfTwo(int powerOfTwo);

	/// 快速将在区间[0,1]范围内的32bit的浮点数F，转换到在区间[0,2^P-1]的32bit的
	// 整数I。value表示F，power表示P，返回值表示I。
	inline int ScaledFloatToInt (float value, int power);

#include "PX2BitHacks.inl"

}

#endif
