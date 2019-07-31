// PX2HMatrix.inl

//----------------------------------------------------------------------------
inline HMatrix::operator const float* () const
{
	return mEntry;
}
//----------------------------------------------------------------------------
inline HMatrix::operator float* ()
{
	return mEntry;
}
//----------------------------------------------------------------------------
inline const float* HMatrix::operator[] (int row) const
{
	return &mEntry[4*row];
}
//----------------------------------------------------------------------------
inline float* HMatrix::operator[] (int row)
{
	return &mEntry[4*row];
}
//----------------------------------------------------------------------------
inline float HMatrix::operator() (int row, int column) const
{
	return mEntry[column + 4*row];
}
//----------------------------------------------------------------------------
inline float& HMatrix::operator() (int row, int column)
{
	return mEntry[column + 4*row];
}
//----------------------------------------------------------------------------