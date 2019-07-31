// PX2Matrix4.cpp

#include "PX2Matrix4.hpp"

namespace PX2
{
	template<> const Matrix4<float> Matrix4<float>::ZERO(true);
	template<> const Matrix4<float> Matrix4<float>::IDENTITY(false);

	template<> const Matrix4<double> Matrix4<double>::ZERO(true);
	template<> const Matrix4<double> Matrix4<double>::IDENTITY(false);
}
