// PX2BitHacks.inl

//----------------------------------------------------------------------------
inline int ScaledFloatToInt (float value, int power)
{
	int result;

	int shift = 150 - power - ((*(int*)(&value) >> 23) & 0xFF);
	if (shift < 24)
	{
		result = ((*(int*)(&value) & 0x007FFFFF) | 0x00800000) >> shift;
		if (result == (1 << power))
		{
			--result;
		}
	}
	else
	{
		result = 0;
	}

	return result;
}
//----------------------------------------------------------------------------