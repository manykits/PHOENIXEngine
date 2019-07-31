// PX2HalfFloat.cpp

#include "PX2HalfFloat.hpp"

namespace PX2
{

//----------------------------------------------------------------------------
HalfFloat ToHalf (float value)
{
	unsigned int bits = *(unsigned int*)&value;
	unsigned short biasExp = (unsigned short)((bits & 0x7F800000) >> 23);
	if (biasExp >= 0x0071)
	{
		if (biasExp <= 0x008E)
		{
			if (biasExp != 0)
			{
				// Truncate 23-bit mantissa to 10 bits.
				unsigned short signBit =
					(unsigned short)((bits & 0x80000000) >> 16);
				unsigned int mantissa = (bits & 0x007FFFFF) >> 13;
				biasExp = (biasExp - 0x0070) << 10;
				return (HalfFloat)(signBit | biasExp | mantissa);
			}
			else
			{
				// E = 0 (alternate encoding of zero, M does not matter)
				return (unsigned short)0;
			}
		}
		else
		{
			// E = 30, M = 1023 (largest magnitude half-float)
			unsigned short signBit =
				(unsigned short)((bits & 0x80000000) >> 16);
			return signBit | (unsigned short)0x7BFF;
		}
	}
	else
	{
		// E = 1, M = 0 (smallest magnitude half-float)
		unsigned short signBit = (unsigned short)((bits & 0x80000000) >> 16);
		return signBit | (unsigned short)0x0400;
	}
}
//----------------------------------------------------------------------------
float ToFloat (HalfFloat value)
{
	unsigned int biasExp  = (unsigned int)(value & 0x7C00) >> 10;
	if (biasExp != 0)
	{
		unsigned int signBit  = (unsigned int)(value & 0x8000) << 16;
		unsigned int mantissa = (unsigned int)(value & 0x03FF) << 13;
		biasExp = (biasExp + 0x0070) << 23;
		unsigned int result = signBit | biasExp | mantissa;
		return *(float*)&result;
	}
	else
	{
		// E = 0 (alternate encoding of zero, M does not matter)
		return 0.0f;
	}
}
//----------------------------------------------------------------------------

}
