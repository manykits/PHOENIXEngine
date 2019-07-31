// PX2Rect.hpp

#ifndef PX2RECT_HPP
#define PX2RECT_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Float2.hpp"

namespace PX2
{

	template<typename T>
	class Rect
	{
	public:
		T Left;
		T Bottom;
		T Right;
		T Top;
		
		Rect ();
		Rect (const T &left, const T &bottom, const T &right, const T &top);
		Rect (const Rect<T> &rct);
		~Rect ();

		T Width () const;
		T Height () const;

		void Clear ();				
		void Set(const T &left, const T &bottom, const T &right, const T &top);
		void Swap (Rect &rect);
		bool IsEmpty () const;
		bool IsInSide (const Rect<T> &rect) const;
		bool IsIntersect(const Rect<T> &rect) const;
		bool IsInsize (const Float2 &point) const;
		bool IsInsize (float x, float y) const;

		Rect &operator-= (const Rect &rect);
		Rect &operator+= (const Rect &rect);
		Rect operator- (const Rect &rect);
		Rect operator+ (const Rect &rect);
		Rect &operator= (const Rect &rect);

		template <typename U>
		Rect &operator= (const Rect<U> &rect)
		{
			Left = rect.Left;
			Top = rect.Top;
			Right = rect.Right;
			Bottom = rect.Bottom;
			return *this;
		}	

		bool operator== (const Rect &rect) const;
		bool operator!= (const Rect &rect) const;
	};

#include "PX2Rect.inl"

	typedef Rect<float> Rectf;
	typedef Rect<double> Rectd;
	typedef Rect<int> Recti;

}

#endif