// PX2AVector.cpp

#include "PX2AVector.hpp"
using namespace PX2;

const AVector AVector::ZERO(0.0f, 0.0f, 0.0f);
const AVector AVector::UNIT_X(1.0f, 0.0f, 0.0f);
const AVector AVector::UNIT_Y(0.0f, 1.0f, 0.0f);
const AVector AVector::UNIT_Z(0.0f, 0.0f, 1.0f);
const AVector AVector::UNIT(1.0f, 1.0f, 1.0f);

//----------------------------------------------------------------------------
AVector::AVector ()
{
	mTuple[0] = 0.0f;
	mTuple[1] = 0.0f;
	mTuple[2] = 0.0f;
	mTuple[3] = 0.0f;
}
//----------------------------------------------------------------------------
AVector::AVector (const AVector& vec)
{
	mTuple[0] = vec.mTuple[0];
	mTuple[1] = vec.mTuple[1];
	mTuple[2] = vec.mTuple[2];
	mTuple[3] = 0.0f;
}
//----------------------------------------------------------------------------
AVector::AVector (float x, float y, float z)
{
	mTuple[0] = x;
	mTuple[1] = y;
	mTuple[2] = z;
	mTuple[3] = 0.0f;
}
//----------------------------------------------------------------------------
AVector::AVector (const Float3& tuple)
{
	mTuple[0] = tuple[0];
	mTuple[1] = tuple[1];
	mTuple[2] = tuple[2];
	mTuple[3] = 0.0f;
}
//----------------------------------------------------------------------------
AVector::AVector (const Vector3f& tuple)
{
	mTuple[0] = tuple[0];
	mTuple[1] = tuple[1];
	mTuple[2] = tuple[2];
	mTuple[3] = 0.0f;
}
//----------------------------------------------------------------------------
AVector::~AVector ()
{
}
//----------------------------------------------------------------------------
AVector& AVector::operator= (const AVector& vec)
{
	mTuple[0] = vec.mTuple[0];
	mTuple[1] = vec.mTuple[1];
	mTuple[2] = vec.mTuple[2];
	mTuple[3] = 0.0f;
	return *this;
}
//----------------------------------------------------------------------------
AVector AVector::operator+ (const AVector& vec) const
{
	return AVector
		(
		mTuple[0] + vec.mTuple[0],
		mTuple[1] + vec.mTuple[1],
		mTuple[2] + vec.mTuple[2]
	);
}
//----------------------------------------------------------------------------
AVector AVector::operator- (const AVector& vec) const
{
	return AVector
		(
		mTuple[0] - vec.mTuple[0],
		mTuple[1] - vec.mTuple[1],
		mTuple[2] - vec.mTuple[2]
	);
}
//----------------------------------------------------------------------------
AVector AVector::operator* (float scalar) const
{
	return AVector
		(
		scalar*mTuple[0],
		scalar*mTuple[1],
		scalar*mTuple[2]
	);
}
//----------------------------------------------------------------------------
AVector AVector::operator/ (float scalar) const
{
	if (scalar != 0.0f)
	{
		float invScalar = 1.0f/scalar;
		return AVector
			(
			invScalar*mTuple[0],
			invScalar*mTuple[1],
			invScalar*mTuple[2]
		);
	}

	return AVector(FLT_MAX, FLT_MAX, FLT_MAX);
}
//----------------------------------------------------------------------------
AVector AVector::operator- () const
{
	return AVector(-mTuple[0], -mTuple[1], -mTuple[2]);
}
//----------------------------------------------------------------------------
AVector PX2::operator* (float scalar, const AVector& vec)
{
	return vec*scalar;
}
//----------------------------------------------------------------------------
AVector& AVector::operator+= (const AVector& vec)
{
	mTuple[0] += vec[0];
	mTuple[1] += vec[1];
	mTuple[2] += vec[2];
	return *this;
}
//----------------------------------------------------------------------------
AVector& AVector::operator-= (const AVector& vec)
{
	mTuple[0] -= vec[0];
	mTuple[1] -= vec[1];
	mTuple[2] -= vec[2];
	return *this;
}
//----------------------------------------------------------------------------
AVector& AVector::operator*= (float scalar)
{
	mTuple[0] *= scalar;
	mTuple[1] *= scalar;
	mTuple[2] *= scalar;
	return *this;
}
//----------------------------------------------------------------------------
AVector& AVector::operator/= (float scalar)
{
	if (scalar != 0.0f)
	{
		float invScalar = 1.0f/scalar;
		mTuple[0] *= invScalar;
		mTuple[1] *= invScalar;
		mTuple[2] *= invScalar;
	}
	else
	{
		mTuple[0] = FLT_MAX;
		mTuple[1] = FLT_MAX;
		mTuple[2] = FLT_MAX;
	}

	return *this;
}
//----------------------------------------------------------------------------
float AVector::Length () const
{
	float sqrLength = mTuple[0]*mTuple[0] + mTuple[1]*mTuple[1] +
		mTuple[2]*mTuple[2];

	return sqrtf(sqrLength);
}
//----------------------------------------------------------------------------
float AVector::SquaredLength () const
{
	float sqrLength = mTuple[0]*mTuple[0] + mTuple[1]*mTuple[1] +
		mTuple[2]*mTuple[2];

	return sqrLength;
}
//----------------------------------------------------------------------------
float AVector::Dot (const AVector& vec) const
{
	float dotProduct = mTuple[0]*vec.mTuple[0] + mTuple[1]*vec.mTuple[1] +
		mTuple[2]*vec.mTuple[2];

	return dotProduct;
}
//----------------------------------------------------------------------------
AVector AVector::Over(float val) const
{
	if (0.0f == val)
		val = 0.0001f;
	return *this / val;
}
//----------------------------------------------------------------------------
AVector AVector::Dot(float length) const
{
	return *this * length;
}
//----------------------------------------------------------------------------
AVector AVector::Add(const AVector &vec) const
{
	return *this + vec;
}
//----------------------------------------------------------------------------
float AVector::Normalize (const float epsilon)
{
	float length = Length();

	if (length > epsilon)
	{
		float invLength = 1.0f/length;
		mTuple[0] *= invLength;
		mTuple[1] *= invLength;
		mTuple[2] *= invLength;
	}
	else
	{
		length = 0.0f;
		mTuple[0] = 0.0f;
		mTuple[1] = 0.0f;
		mTuple[2] = 0.0f;
	}

	return length;
}
//----------------------------------------------------------------------------
AVector &AVector::Truncate (float max)
{
	if (Length() > max)
	{
		Normalize();

		*this *= max;
	}

	return *this;
}
//----------------------------------------------------------------------------
AVector AVector::Cross (const AVector& vec) const
{
	return AVector
		(
		mTuple[1]*vec.mTuple[2] - mTuple[2]*vec.mTuple[1],
		mTuple[2]*vec.mTuple[0] - mTuple[0]*vec.mTuple[2],
		mTuple[0]*vec.mTuple[1] - mTuple[1]*vec.mTuple[0]
	);
}
//----------------------------------------------------------------------------
AVector AVector::UnitCross (const AVector& vec) const
{
	AVector cross
		(
		mTuple[1]*vec.mTuple[2] - mTuple[2]*vec.mTuple[1],
		mTuple[2]*vec.mTuple[0] - mTuple[0]*vec.mTuple[2],
		mTuple[0]*vec.mTuple[1] - mTuple[1]*vec.mTuple[0]
	);

	cross.Normalize();
	return cross;
}
//----------------------------------------------------------------------------
Vector2f AVector::To2()
{
	return Vector2f(mTuple[0], mTuple[1]);
}
//----------------------------------------------------------------------------
void AVector::Orthonormalize (AVector& vec0, AVector& vec1, AVector& vec2)
{
	// 输入的向量为v0，v1，v2，按照下面的方式进行Gram-Schmidt正交化
	//
	//   u0 = v0/|v0|
	//   u1 = (v1-(u0*v1)u0)/|v1-(u0*v1)u0|
	//   u2 = (v2-(u0*v2)u0-(u1*v2)u1)/|v2-(u0*v2)u0-(u1*v2)u1|
	//
	// |A|代表向量A的长度；A*B代表两个向量“点乘”

	// 单位化
	vec0.Normalize();

	// 计算 u1
	float dot0 = vec0.Dot(vec1); 
	vec1 -= dot0*vec0;
	vec1.Normalize();

	// 计算 u2
	float dot1 = vec1.Dot(vec2);
	dot0 = vec0.Dot(vec2);
	vec2 -= dot0*vec0 + dot1*vec1;
	vec2.Normalize();
}
//----------------------------------------------------------------------------
void AVector::Orthonormalize (AVector* vec)
{
	Orthonormalize(vec[0], vec[1], vec[2]);
}
//----------------------------------------------------------------------------
void AVector::GenerateOrthonormalBasis (AVector& vec0, AVector& vec1,
										AVector& vec2)
{
	vec2.Normalize();
	GenerateComplementBasis(vec0, vec1, vec2);
}
//----------------------------------------------------------------------------
void AVector::GenerateComplementBasis (AVector& vec0, AVector& vec1,
									   const AVector& vec2)
{
	float invLength;

	if (fabsf(vec2.mTuple[0]) >= fabsf(vec2.mTuple[1]))
	{
		// vec2.x or vec2.z is the largest magnitude component, swap them
		invLength = 1.0f/sqrtf(vec2.mTuple[0]*vec2.mTuple[0] +
			vec2.mTuple[2]*vec2.mTuple[2]);
		vec0.mTuple[0] = -vec2.mTuple[2]*invLength;
		vec0.mTuple[1] = 0.0f;
		vec0.mTuple[2] = +vec2.mTuple[0]*invLength;
		vec1.mTuple[0] = vec2.mTuple[1]*vec0.mTuple[2];
		vec1.mTuple[1] = vec2.mTuple[2]*vec0.mTuple[0] -
			vec2.mTuple[0]*vec0.mTuple[2];
		vec1.mTuple[2] = -vec2.mTuple[1]*vec0.mTuple[0];
	}
	else
	{
		// vec2.y or vec2.z is the largest magnitude component, swap them
		invLength = 1.0f/sqrtf(vec2.mTuple[1]*vec2.mTuple[1] +
			vec2.mTuple[2]*vec2.mTuple[2]);
		vec0.mTuple[0] = 0.0f;
		vec0.mTuple[1] = +vec2.mTuple[2]*invLength;
		vec0.mTuple[2] = -vec2.mTuple[1]*invLength;
		vec1.mTuple[0] = vec2.mTuple[1]*vec0.mTuple[2] -
			vec2.mTuple[2]*vec0.mTuple[1];
		vec1.mTuple[1] = -vec2.mTuple[0]*vec0.mTuple[2];
		vec1.mTuple[2] = vec2.mTuple[0]*vec0.mTuple[1];
	}
}
//----------------------------------------------------------------------------
AVector AVector::AnglesToDirection(float angle0, float angle1)
{
	return AVector(-Mathf::Cos(angle1)*Mathf::Cos(angle0), 
		Mathf::Cos(angle1)*Mathf::Sin(angle0), -Mathf::Sin(angle1));
}
//----------------------------------------------------------------------------