// PX2BitHacks.cpp

#include "PX2BitHacks.hpp"

namespace PX2
{

//----------------------------------------------------------------------------
bool IsPowerOfTwo (unsigned int value)
{
	return (value > 0) && ((value & (value - 1)) == 0);
}
//----------------------------------------------------------------------------
bool IsPowerOfTwo (int value)
{
	return (value > 0) && ((value & (value - 1)) == 0);
}
//----------------------------------------------------------------------------
unsigned int Log2OfPowerOfTwo (unsigned int powerOfTwo)
{
	unsigned int log2 = (powerOfTwo & 0xAAAAAAAA) != 0;
	log2 |= ((powerOfTwo & 0xFFFF0000) != 0) << 4;
	log2 |= ((powerOfTwo & 0xFF00FF00) != 0) << 3;
	log2 |= ((powerOfTwo & 0xF0F0F0F0) != 0) << 2;
	log2 |= ((powerOfTwo & 0xCCCCCCCC) != 0) << 1;
	return log2;
}
//----------------------------------------------------------------------------
int Log2OfPowerOfTwo (int powerOfTwo)
{
	unsigned int log2 = (powerOfTwo & 0xAAAAAAAA) != 0;
	log2 |= ((powerOfTwo & 0xFFFF0000) != 0) << 4;
	log2 |= ((powerOfTwo & 0xFF00FF00) != 0) << 3;
	log2 |= ((powerOfTwo & 0xF0F0F0F0) != 0) << 2;
	log2 |= ((powerOfTwo & 0xCCCCCCCC) != 0) << 1;
	return (int)log2;
}
//----------------------------------------------------------------------------

}