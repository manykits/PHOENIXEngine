// PX2TextIterator.inl

//----------------------------------------------------------------------------
inline bool TextIterator::operator == (const TextIterator& it) const
{
	return _it == it._it;
}
//----------------------------------------------------------------------------
inline bool TextIterator::operator != (const TextIterator& it) const
{
	return _it != it._it;
}
//----------------------------------------------------------------------------
inline void swap(TextIterator& it1, TextIterator& it2)
{
	it1.swap(it2);
}
//----------------------------------------------------------------------------
inline TextIterator TextIterator::end() const
{
	return TextIterator(_end);
}
//----------------------------------------------------------------------------