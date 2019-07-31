// PX2EdgeKey.inl

//----------------------------------------------------------------------------
inline EdgeKey::EdgeKey (int v0, int v1)
{
    if (v0 < v1)
    {
        // v0 is minimum
        V[0] = v0;
        V[1] = v1;
    }
    else
    {
        // v1 is minimum
        V[0] = v1;
        V[1] = v0;
    }
}
//----------------------------------------------------------------------------
inline bool EdgeKey::operator< (const EdgeKey& key) const
{
    if (V[1] < key.V[1])
    {
        return true;
    }

    if (V[1] > key.V[1])
    {
        return false;
    }

    return V[0] < key.V[0];
}
//----------------------------------------------------------------------------
inline EdgeKey::operator size_t () const
{
    return V[0] | (V[1] << 16);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
inline OrderedEdgeKey::OrderedEdgeKey(int v0, int v1)
{
	V[0] = v0;
	V[1] = v1;
}
//----------------------------------------------------------------------------
inline bool OrderedEdgeKey::operator< (const OrderedEdgeKey& key) const
{
	if (V[1] < key.V[1])
	{
		return true;
	}

	if (V[1] > key.V[1])
	{
		return false;
	}

	return V[0] < key.V[0];
}
//----------------------------------------------------------------------------