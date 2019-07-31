// PX2StringTokenizer.inl

//----------------------------------------------------------------------------
inline StringTokenizer::Iterator StringTokenizer::Begin() const
{
	return mTokens.begin();
}
//----------------------------------------------------------------------------
inline StringTokenizer::Iterator StringTokenizer::End() const
{
	return mTokens.end();
}
//----------------------------------------------------------------------------
inline int StringTokenizer::Count()
{
	return (int)mTokens.size();
}
//----------------------------------------------------------------------------