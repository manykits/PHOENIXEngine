// PX2Rect.inl

//----------------------------------------------------------------------------
template <typename T>
Rect<T>::Rect ():
Left((T)0),
Bottom((T)0),
Right((T)0),
Top((T)0)
{
}
//----------------------------------------------------------------------------
template <typename T>
Rect<T>::Rect (const T &Left, const T &Bottom, const T &Right, const T &Top)
	:
Left(Left),
Bottom(Bottom),
Right(Right),
Top(Top)
{
}
//----------------------------------------------------------------------------
template <typename T>
Rect<T>::Rect(const Rect<T> &rct) :
Left(rct.Left),
Top(rct.Top),
Right(rct.Right),
Bottom(rct.Bottom)
{
}
//----------------------------------------------------------------------------
template <typename T>
Rect<T>::~Rect()
{
}
//----------------------------------------------------------------------------
template <typename T>
T Rect<T>::Width() const
{
	return Right - Left;
}
//----------------------------------------------------------------------------
template <typename T>
T Rect<T>::Height() const
{
	return Top - Bottom;
}
//----------------------------------------------------------------------------
template <typename T>
void Rect<T>::Clear ()
{
	Left = Top = Right = Bottom = 0;
}
//----------------------------------------------------------------------------
template <typename T>
void Rect<T>::Set(const T &left, const T &bottom, const T &right, const T &top)
{
	Left = left;
	Bottom = bottom;
	Right = right;
	Top = top;
}
//----------------------------------------------------------------------------
template <typename T>
void Rect<T>::Swap (Rect &rect)
{
	Rect tmp = rect;
	rect = *this;
	*this = tmp;
}
//----------------------------------------------------------------------------
template <typename T>
bool Rect<T>::IsEmpty () const
{
	return ((Left == 0) && (Top == 0) && (Right == 0) && (Bottom == 0));
}
//----------------------------------------------------------------------------
template <typename T>
bool Rect<T>::IsInSide (const Rect<T> &rect) const
{
	return ((rect.Left >= Left) && (rect.Right <= Right) && (rect.Top <= Top)
		&& (rect.Bottom >= Bottom));
}
//----------------------------------------------------------------------------
template <typename T>
bool Rect<T>::IsIntersect(const Rect<T> &rect) const
{
	return ((rect.Left <= Right) && (rect.Right >= Left)
		&& (rect.Top >= Bottom) && (rect.Bottom <= Top));
}
//----------------------------------------------------------------------------
template <typename T>
bool Rect<T>::IsInsize (const Float2 &point) const
{
	return ((point[0] >= Left) && (point[0] <= Right) && (point[1] >= Bottom)
		&& (point[1] <= Top));
}
//----------------------------------------------------------------------------
template <typename T>
bool Rect<T>::IsInsize (float x, float y) const
{
	return ((x >= Left) && (x <= Right) && (y >= Bottom)
		&& (y <= Top));
}
//----------------------------------------------------------------------------
template <typename T>
Rect<T> &Rect<T>::operator-= (const Rect &rect)
{
	Left -= rect.Left;
	Top -= rect.Top;
	Right -= rect.Right;
	Bottom -= rect.Bottom;
	return *this;
}
//----------------------------------------------------------------------------
template <typename T>
Rect<T> &Rect<T>::operator+= (const Rect &rect)
{
	Left += rect.Left;
	Top += rect.Top;
	Right += rect.Right;
	Bottom += rect.Bottom;
	return *this;
}
//----------------------------------------------------------------------------
template <typename T>
Rect<T> Rect<T>::operator- (const Rect &rect)
{
	return Rect(Left - rect.Left, Top - rect.Top, Right - rect.Right,
		Bottom - rect.Bottom);
}
//----------------------------------------------------------------------------
template <typename T>
Rect<T> Rect<T>::operator+ (const Rect &rect)
{
	return Rect(Left + rect.Left, Top + rect.Top, Right + rect.Right,
		Bottom + rect.Bottom);
}
//----------------------------------------------------------------------------
template <typename T>
Rect<T> &Rect<T>::operator= (const Rect &rect)
{
	Left = rect.Left;
	Top = rect.Top;
	Right = rect.Right;
	Bottom = rect.Bottom;
	return *this;
}
//----------------------------------------------------------------------------
template <typename T>
bool Rect<T>::operator== (const Rect& rect) const
{
	return ((Left == rect.Left) && (Top == rect.Top) && (Right == rect.Right) 
		&& (Bottom == rect.Bottom));
}
//----------------------------------------------------------------------------
template <typename T>
bool Rect<T>::operator != (const Rect &rect) const
{
	return !((Left == rect.Left) && (Top == rect.Top) && (Right == rect.Right)
		&& (Bottom == rect.Bottom));
}
//----------------------------------------------------------------------------