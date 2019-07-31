// PX2Matrix3.cpp

#include "PX2Matrix3.hpp"

namespace PX2
{
	template<> const Matrix3<float> Matrix3<float>::ZERO(true);
	template<> const Matrix3<float> Matrix3<float>::IDENTITY(false);

	template<> const Matrix3<double> Matrix3<double>::ZERO(true);
	template<> const Matrix3<double> Matrix3<double>::IDENTITY(false);
}

