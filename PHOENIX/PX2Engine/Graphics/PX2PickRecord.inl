// PX2PickRecord.inl

//----------------------------------------------------------------------------
inline bool PickRecord::operator== (const PickRecord& record) const
{
	return T == record.T;
}
//----------------------------------------------------------------------------
inline bool PickRecord::operator!= (const PickRecord& record) const
{
	return T != record.T;
}
//----------------------------------------------------------------------------
inline bool PickRecord::operator<  (const PickRecord& record) const
{
	return T < record.T;
}
//----------------------------------------------------------------------------
inline bool PickRecord::operator<= (const PickRecord& record) const
{
	return T <= record.T;
}
//----------------------------------------------------------------------------
inline bool PickRecord::operator>  (const PickRecord& record) const
{
	return T > record.T;
}
//----------------------------------------------------------------------------
inline bool PickRecord::operator>= (const PickRecord& record) const
{
	return T >= record.T;
}
//----------------------------------------------------------------------------