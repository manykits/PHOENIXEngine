// PX2Tuple.inl

//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
Tuple<DIMENSION,TYPE>::Tuple ()
{
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
Tuple<DIMENSION,TYPE>::Tuple (const Tuple& tuple)
{
    for (int i = 0; i < DIMENSION; ++i)
    {
        mTuple[i] = tuple.mTuple[i];
    }
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
Tuple<DIMENSION,TYPE>::~Tuple ()
{
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline Tuple<DIMENSION,TYPE>::operator const TYPE* () const
{
    return mTuple;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline Tuple<DIMENSION,TYPE>::operator TYPE* ()
{
    return mTuple;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline const TYPE& Tuple<DIMENSION,TYPE>::operator[] (int i) const
{
	return mTuple[i];
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
inline TYPE& Tuple<DIMENSION,TYPE>::operator[] (int i)
{
    return mTuple[i];
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
Tuple<DIMENSION,TYPE>& Tuple<DIMENSION,TYPE>::operator= (const Tuple& tuple)
{
    for (int i = 0; i < DIMENSION; ++i)
    {
        mTuple[i] = tuple.mTuple[i];
    }
    return *this;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator== (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) == 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator!= (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) != 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator< (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) < 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator<= (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) <= 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator> (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) > 0;
}
//----------------------------------------------------------------------------
template <int DIMENSION, typename TYPE>
bool Tuple<DIMENSION,TYPE>::operator>= (const Tuple& tuple) const
{
    return memcmp(mTuple, tuple.mTuple, DIMENSION*sizeof(TYPE)) >= 0;
}
//----------------------------------------------------------------------------