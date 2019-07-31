// PX2Vector4.cpp

#include "PX2Vector4.hpp"

namespace PX2
{
	template<> const Vector4<float> Vector4<float>::ZERO(0.0f,0.0f,0.0f,0.0f);
	template<> const Vector4<float> Vector4<float>::UNIT_X(1.0f,0.0f,0.0f,0.0f);
	template<> const Vector4<float> Vector4<float>::UNIT_Y(0.0f,1.0f,0.0f,0.0f);
	template<> const Vector4<float> Vector4<float>::UNIT_Z(0.0f,0.0f,1.0f,0.0f);
	template<> const Vector4<float> Vector4<float>::UNIT_W(0.0f,0.0f,0.0f,1.0f);
	template<> const Vector4<float> Vector4<float>::UNIT(1.0f, 1.0f, 1.0f, 1.0f);

	template<> const Vector4<double> Vector4<double>::ZERO(0.0,0.0,0.0,0.0);
	template<> const Vector4<double> Vector4<double>::UNIT_X(1.0,0.0,0.0,0.0);
	template<> const Vector4<double> Vector4<double>::UNIT_Y(0.0,1.0,0.0,0.0);
	template<> const Vector4<double> Vector4<double>::UNIT_Z(0.0,0.0,1.0,0.0);
	template<> const Vector4<double> Vector4<double>::UNIT_W(0.0,0.0,0.0,1.0);
	template<> const Vector4<double> Vector4<double>::UNIT(1.0, 1.0, 1.0, 1.0);
}
