// PX2Float3.cpp

#include "PX2Float3.hpp"
using namespace PX2;

const Float3 Float3::ZERO(0.0f, 0.0f, 0.0f);
const Float3 Float3::UNIT(1.0f, 1.0f, 1.0f);
const Float3 Float3::UNIT_X(1.0f, 0.0f, 0.0f);
const Float3 Float3::UNIT_Y(0.0f, 1.0f, 0.0f);
const Float3 Float3::UNIT_Z(0.0f, 0.0f, 1.0f);
const Float3 Float3::BLACK(0.0f, 0.0f, 0.0f);
const Float3 Float3::WHITE(1.0f, 1.0f, 1.0f);
const Float3 Float3::RED(1.0f, 0.0f, 0.0f);
const Float3 Float3::GREEN(0.0f, 1.0f, 0.0f);
const Float3 Float3::BLUE(0.0f, 0.0f, 1.0f);
const Float3 Float3::YELLOW(1.0f, 1.0f, 0.0f);

//----------------------------------------------------------------------------
Float3::Float3 ()
{
	mTuple[0] = 0.0f;
	mTuple[1] = 0.0f;
	mTuple[2] = 0.0f;
}
//----------------------------------------------------------------------------
Float3::~Float3 ()
{
}
//----------------------------------------------------------------------------
float Float3::X() const
{
	return mTuple[0];
}
//----------------------------------------------------------------------------
float& Float3::X()
{
	return mTuple[0];
}
//----------------------------------------------------------------------------
float Float3::Y() const
{
	return mTuple[1];
}
//----------------------------------------------------------------------------
float& Float3::Y()
{
	return mTuple[1];
}
//----------------------------------------------------------------------------
float Float3::Z() const
{
	return mTuple[2];
}
//----------------------------------------------------------------------------
float& Float3::Z()
{
	return mTuple[2];
}
//----------------------------------------------------------------------------
Float3::Float3 (float f0, float f1, float f2)
{
    mTuple[0] = f0;
    mTuple[1] = f1;
    mTuple[2] = f2;
}
//----------------------------------------------------------------------------
Float3::Float3 (const Float3& tuple)
{
    mTuple[0] = tuple.mTuple[0];
    mTuple[1] = tuple.mTuple[1];
    mTuple[2] = tuple.mTuple[2];
}
//----------------------------------------------------------------------------
Float3& Float3::operator= (const Float3& tuple)
{
    mTuple[0] = tuple.mTuple[0];
    mTuple[1] = tuple.mTuple[1];
    mTuple[2] = tuple.mTuple[2];
    return *this;
}
//----------------------------------------------------------------------------
Float3 Float3::operator+ (const Float3& tuple) const
{
	Float3 tupleTemp;

	for (int i=0; i<3; ++i)
	{
		tupleTemp[i] = mTuple[i] + tuple[i];
	}

	return tupleTemp;
}
//----------------------------------------------------------------------------
Float3 Float3::operator- (const Float3& tuple) const
{
	Float3 tupleTemp;

	for (int i=0; i<3; ++i)
	{
		tupleTemp[i] = mTuple[i] - tuple[i];
	}

	return tupleTemp;
}
//----------------------------------------------------------------------------
Float3 Float3::operator* (float scalar) const
{
	Float3 tupleTemp;

	for (int i=0; i<3; ++i)
	{
		tupleTemp[i] = mTuple[i] * scalar;
	}

	return tupleTemp;
}
//----------------------------------------------------------------------------
Float3 Float3::operator/ (float scalar) const
{
	Float3 tupleTemp;

	for (int i=0; i<3; ++i)
	{
		tupleTemp[i] = mTuple[i] / scalar;
	}

	return tupleTemp;
}
//----------------------------------------------------------------------------
Float3& Float3::operator+= (const Float3& tuple)
{
	for (int i=0; i<3; ++i)
	{
		mTuple[i] += tuple[i];
	}

	return *this;
}
//----------------------------------------------------------------------------
Float3& Float3::operator-= (const Float3& tuple)
{
	for (int i=0; i<3; ++i)
	{
		mTuple[i] -= tuple[i];
	}

	return *this;
}
//----------------------------------------------------------------------------
Float3& Float3::operator*= (float scalar)
{
	for (int i=0; i<3; ++i)
	{
		mTuple[i] *= scalar;
	}

	return *this;
}
//----------------------------------------------------------------------------
Float3& Float3::operator/= (float scalar)
{
	for (int i=0; i<3; ++i)
	{
		mTuple[i] /= scalar;
	}

	return *this;
}
//----------------------------------------------------------------------------
Float3& Float3::operator- ()
{
	for (int i=0; i<3; ++i)
	{
		mTuple[i] = -mTuple[i];
	}

	return *this;
}
//----------------------------------------------------------------------------
Float3 Float3::MakeColor (unsigned int r, unsigned int g, unsigned int b)
{
	return Float3((float)r/255.0f, (float)g/255.0f, (float)b/255.0f);
}
//----------------------------------------------------------------------------
#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))
//----------------------------------------------------------------------------
Float3 Float3::RGB2HSL(const Float3 &rgb)
{
	float h = 0, s = 0, l = 0;
	// normalizes red-green-blue values
	float r = rgb[0];
	float g = rgb[1];
	float b = rgb[2];
	float maxVal = max3v(r, g, b);
	float minVal = min3v(r, g, b);

	// hue

	if (maxVal == minVal)
	{
		h = 0; // undefined
	}

	else if (maxVal == r && g >= b)
	{
		h = 60.0f*(g - b) / (maxVal - minVal);
	}

	else if (maxVal == r && g < b)
	{
		h = 60.0f*(g - b) / (maxVal - minVal) + 360.0f;
	}

	else if (maxVal == g)
	{
		h = 60.0f*(b - r) / (maxVal - minVal) + 120.0f;
	}
	else if (maxVal == b)
	{
		h = 60.0f*(r - g) / (maxVal - minVal) + 240.0f;
	}

	// luminance

	l = (maxVal + minVal) / 2.0f;
	// saturation

	if (l == 0 || maxVal == minVal)
	{
		s = 0;
	}

	else if (0 < l && l <= 0.5f)
	{
		s = (maxVal - minVal) / (maxVal + minVal);
	}
	else if (l > 0.5f)
	{
		s = (maxVal - minVal) / (2 - (maxVal + minVal)); //(maxVal-minVal > 0)?
	}

	Float3 hsl;
	hsl[0] = (h > 360) ? 360 : ((h < 0) ? 0 : h);
	hsl[1] = ((s > 1) ? 1 : ((s < 0) ? 0 : s)) * 100;
	hsl[2] = ((l > 1) ? 1 : ((l < 0) ? 0 : l)) * 100;

	return hsl;
}
//----------------------------------------------------------------------------
Float3 Float3::HSL2RGB(const Float3 &hsl)
{
	float h = hsl[0];			// h must be [0, 360]
	float s = hsl[1] / 100.f;	// s must be [0, 1]
	float l = hsl[2] / 100.f;	// l must be [0, 1]
	float R, G, B;

	if (s == 0.0f)
	{
		// achromatic color (gray scale)
		R = G = B = l*255.f;
	}
	else
	{
		float q = (l < 0.5f) ? (l * (1.0f + s)) : (l + s - (l*s));
		float p = (2.0f * l) - q;
		float Hk = h / 360.0f;
		float T[3];
		T[0] = Hk + 0.3333333f; // Tr   0.3333333f=1.0/3.0
		T[1] = Hk;              // Tb
		T[2] = Hk - 0.3333333f; // Tg

		for (int i = 0; i < 3; i++)
		{
			if (T[i] < 0) T[i] += 1.0f;
			if (T[i] > 1) T[i] -= 1.0f;

			if ((T[i] * 6) < 1)
			{
				T[i] = p + ((q - p)*6.0f*T[i]);
			}
			else if ((T[i] * 2.0f) < 1) //(1.0/6.0)<=T[i] && T[i]<0.5
			{
				T[i] = q;
			}
			else if ((T[i] * 3.0f) < 2) // 0.5<=T[i] && T[i]<(2.0/3.0)
			{
				T[i] = p + (q - p) * ((2.0f / 3.0f) - T[i]) * 6.0f;
			}
			else T[i] = p;
		}
		R = T[0] * 255.0f;
		G = T[1] * 255.0f;
		B = T[2] * 255.0f;

	}
	int r = (int)((R > 255) ? 255 : ((R < 0) ? 0 : R));
	int g = (int)((G > 255) ? 255 : ((G < 0) ? 0 : G));
	int b = (int)((B > 255) ? 255 : ((B < 0) ? 0 : B));

	return Float3::MakeColor(r, g, b);
}
//----------------------------------------------------------------------------