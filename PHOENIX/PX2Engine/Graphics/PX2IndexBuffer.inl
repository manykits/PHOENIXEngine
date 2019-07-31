// PX2IndexBuffer.inl

//----------------------------------------------------------------------------
inline char* IndexBuffer::GetData () const
{
    return mData;
}
//----------------------------------------------------------------------------
inline void IndexBuffer::SetOffset (int offset)
{
    if (offset >= 0)
    {
        mOffset = offset;
    }
    else
    {
        assertion(false, "The offset must be nonnegative\n");
    }
}
//----------------------------------------------------------------------------
inline int IndexBuffer::GetOffset () const
{
    return mOffset;
}
//----------------------------------------------------------------------------