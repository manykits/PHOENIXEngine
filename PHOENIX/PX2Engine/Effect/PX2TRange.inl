// PX2TRange.inl

//----------------------------------------------------------------------------
template <class T>
TRange<T>::TRange()
{
	Value0 = T(); Value1 = T();
}
//----------------------------------------------------------------------------
template <class T>
TRange<T>::TRange (T value0, T value1)
{
	Value0 = value0; Value1 = value1; 
}
//----------------------------------------------------------------------------
template <class T>
TRange<T>::~TRange()
{
}
//----------------------------------------------------------------------------
template <class T>
inline T TRange<T>::GetRandomInRange()
{
	return RandomNumber(Value0, Value1);
}
//----------------------------------------------------------------------------
template <class T>
inline T TRange<T>::GetRange()
{
	return (Value1 - Value0);
}
//----------------------------------------------------------------------------
inline int RandomNumber (int value0, int value1)
{
	if (value0 == value1)
		return value0;

	return ((rand() % (abs(value1-value0)+1))+value0);
}
//----------------------------------------------------------------------------
inline float RandomNumber (float value0, float value1)
{
	if (value0 == value1)
		return value0;

	float fRandom = (float)rand() / (float)RAND_MAX;
	return ((fRandom * (float)fabs(value1-value0))+value0);
}
//----------------------------------------------------------------------------
inline Float1 RandomNumber (Float1 value0, Float1 value1)
{
	float val = RandomNumber(value0[0], value1[0]);

	return Float1(val);
}
//----------------------------------------------------------------------------
inline Float2 RandomNumber (Float2 value0, Float2 value1)
{
	float x = RandomNumber(value0[0], value1[0]);
	float y = RandomNumber(value0[1], value1[1]);

	return Float2(x, y);
}
//----------------------------------------------------------------------------
inline Float3 RandomNumber (Float3 value0, Float3 value1)
{
	float x = RandomNumber(value0[0], value1[0]);
	float y = RandomNumber(value0[1], value1[1]);
	float z = RandomNumber(value0[2], value1[2]);

	return Float3(x, y, z);
}
//----------------------------------------------------------------------------
inline Float4 RandomNumber (Float4 value0, Float4 value1)
{
	float x = RandomNumber(value0[0], value1[0]);
	float y = RandomNumber(value0[1], value1[1]);
	float z = RandomNumber(value0[2], value1[2]);
	float w = RandomNumber(value0[3], value1[3]);

	return Float4(x, y, z, w);
}
//----------------------------------------------------------------------------