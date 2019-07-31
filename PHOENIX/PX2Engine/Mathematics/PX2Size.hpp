// PX2Size.hpp

#ifndef PX2SIZE_HPP
#define PX2SIZE_HPP

#include "PX2MathematicsPre.hpp"

namespace PX2
{

	template<typename T>
	class Size
	{
	public:
		T Width;
		T Height;

		Size ();
		Size (const T &width, const T &height);
		Size (const Size &size);
		~Size ();

		Size &operator-= (const Size &size);
		Size &operator+= (const Size &size);
		Size operator- (const Size &size) const;
		Size operator+ (const Size &size) const;
		Size &operator= (const Size &size);

		template<typename U>
		Size &operator= (Size<U> const& size)
		{
			Width = size.Width;
			Height = size.Height;
			return *this;
		}

		bool operator== (const Size &size) const;
		bool operator!= (const Size &size) const;

		void Clear();
		void Set(T const &width, T const &height);
		void Swap(Size &val);
		bool IsEmpty() const;

	};

#include "PX2Size.inl"

	typedef Size<float> Sizef;
	typedef Size<double> Sized;
	typedef Size<int> Sizei;

}

#endif