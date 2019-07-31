// PX2Matrix2.cpp

#include "PX2Matrix2.hpp"

namespace PX2
{
	template<> const Matrix2<float> Matrix2<float>::ZERO(true);
	template<> const Matrix2<float> Matrix2<float>::IDENTITY(false);

	template<> const Matrix2<double> Matrix2<double>::ZERO(true);
	template<> const Matrix2<double> Matrix2<double>::IDENTITY(false);
}
