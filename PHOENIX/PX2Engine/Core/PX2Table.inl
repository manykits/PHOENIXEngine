// PX2Table.inl

//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Table<NUMROWS,NUMCOLS,TYPE>::Table ()
{
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Table<NUMROWS,NUMCOLS,TYPE>::Table (const Table& table)
{
	for (int i = 0; i < NUMENTRIES; ++i)
	{
		mEntry[i] = table.mEntry[i];
	}
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Table<NUMROWS,NUMCOLS,TYPE>::~Table ()
{
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline Table<NUMROWS,NUMCOLS,TYPE>::operator const TYPE* () const
{
	return mEntry;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline Table<NUMROWS,NUMCOLS,TYPE>::operator TYPE* ()
{
	return mEntry;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline const TYPE* Table<NUMROWS,NUMCOLS,TYPE>::operator[] (int row) const
{
	return &mEntry[NUMCOLS*row];
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline TYPE* Table<NUMROWS,NUMCOLS,TYPE>::operator[] (int row)
{
	return &mEntry[NUMCOLS*row];
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline TYPE Table<NUMROWS,NUMCOLS,TYPE>::operator() (int row, int col) const
{
	return mEntry[col + NUMCOLS*row];
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
inline TYPE& Table<NUMROWS,NUMCOLS,TYPE>::operator() (int row, int col)
{
	return mEntry[col + NUMCOLS*row];
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
void Table<NUMROWS,NUMCOLS,TYPE>::SetRow (int row,
										  const Tuple<NUMROWS,TYPE>& tuple)
{
	for (int c = 0, i = NUMCOLS*row; c < NUMCOLS; ++c, ++i)
	{
		mEntry[i] = tuple[c];
	}
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Tuple<NUMROWS,TYPE> Table<NUMROWS,NUMCOLS,TYPE>::GetRow (int row) const
{
	Tuple<NUMROWS,TYPE> result;
	for (int c = 0, i = NUMCOLS*row; c < NUMCOLS; ++c, ++i)
	{
		result[c] = mEntry[i];
	}
	return result;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
void Table<NUMROWS,NUMCOLS,TYPE>::SetColumn (int col,
											 const Tuple<NUMCOLS,TYPE>& tuple)
{
	for (int r = 0, i = col; r < NUMROWS; ++r, i += NUMCOLS)
	{
		mEntry[i] = tuple[r];
	}
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Tuple<NUMCOLS,TYPE> Table<NUMROWS,NUMCOLS,TYPE>::GetColumn (int col) const
{
	Tuple<NUMCOLS,TYPE> result;
	for (int r = 0, i = col; r < NUMROWS; ++r, i += NUMCOLS)
	{
		result[r] = mEntry[i];
	}
	return result;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
Table<NUMROWS,NUMCOLS,TYPE>& Table<NUMROWS,NUMCOLS,TYPE>::operator= (
	const Table& table)
{
	for (int i = 0; i < NUMENTRIES; i++)
	{
		mEntry[i] = table.mEntry[i];
	}
	return *this;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator== (const Table& table) const
{
	return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) == 0;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator!= (const Table& table) const
{
	return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) != 0;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator<  (const Table& table) const
{
	return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) < 0;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator<= (const Table& table) const
{
	return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) <= 0;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator>  (const Table& table) const
{
	return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) > 0;
}
//----------------------------------------------------------------------------
template <int NUMROWS, int NUMCOLS, typename TYPE>
bool Table<NUMROWS,NUMCOLS,TYPE>::operator>= (const Table& table) const
{
	return memcmp(mEntry, table.mEntry, NUMENTRIES*sizeof(TYPE)) >= 0;
}
//----------------------------------------------------------------------------