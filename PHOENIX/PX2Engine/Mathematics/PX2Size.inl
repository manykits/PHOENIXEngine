// PX2Size.inl

//----------------------------------------------------------------------------
template <typename T>
Size<T>::Size()
	:
Width((T)0),
Height((T)0)
{
}
//----------------------------------------------------------------------------
template <typename T>
Size<T>::Size(const T &width, const T &height)
	:
Width(width),
Height(height)
{
}
//----------------------------------------------------------------------------
template <typename T>
Size<T>::Size(const Size &size) 
	:
Width(size.Width),
Height(size.Height)
{
}
//----------------------------------------------------------------------------
template <typename T>
Size<T>::~Size()
{
}
//----------------------------------------------------------------------------
template <typename T>
Size<T> &Size<T>::operator-= (const Size &size)
{
	Width -= size.Width;
	Height -= size.Height;
	return *this;
}
//----------------------------------------------------------------------------
template <typename T>
Size<T> &Size<T>::operator+= (const Size &size)
{
	Width += size.Width;
	Height += size.Height;
	return *this;
}
//----------------------------------------------------------------------------
template <typename T>
Size<T> Size<T>::operator- (const Size &size) const
{
	return Size(Width - size.Width, Height - size.Height);
}
//----------------------------------------------------------------------------
template <typename T>
Size<T> Size<T>::operator+ (const Size &size) const
{
	return Size(Width + size.Width, Height + size.Height);
}
//----------------------------------------------------------------------------
template <typename T>
Size<T> &Size<T>::operator= (const Size &size)
{
	Width = size.Width;
	Height = size.Height;
	return *this;
}
//----------------------------------------------------------------------------
template <typename T>
bool Size<T>::operator== (const Size &size) const
{
	return ((Width == size.Width) && (Height == size.Height));
}
//----------------------------------------------------------------------------
template <typename T>
bool Size<T>::operator!= (const Size &size) const
{
	return !((Width == size.Width) && (Height == size.Height));
}
//----------------------------------------------------------------------------
template <typename T>
void Size<T>::Clear()
{
	Width = Height = 0;
}
//----------------------------------------------------------------------------
template <typename T>
void Size<T>::Set(const T &width, const T &height)
{
	Width = width;
	Height = height;
}
//----------------------------------------------------------------------------
template <typename T>
void Size<T>::Swap(Size &_value)
{
	Size tmp = _value;
	_value = *this;
	*this = tmp;
}
//----------------------------------------------------------------------------
template <typename T>
bool Size<T>::IsEmpty() const
{
	return ((Width == 0) && (Height == 0));
}
//----------------------------------------------------------------------------