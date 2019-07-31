// PX2Helps.hpp

#ifndef PX2HELPS_HPP
#define PX2HELPS_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	template<typename T>
	void Swap(T &a, T &b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

}

#endif