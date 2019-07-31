// PX2Transform.cpp

#include "PX2Transform.hpp"
#include "PX2FunObject.hpp"
using namespace PX2;

const Transform Transform::IDENTITY;

//----------------------------------------------------------------------------
Transform::Transform ()
:
mHMatrix(false),  // 初始化到单位矩阵
mInvHMatrix(false),  // 初始化到单位矩阵
mMatrix(false),  // 初始化到单位矩阵
mTranslate(0.0f, 0.0f, 0.0f),
mScale(1.0f, 1.0f, 1.0f),
mIsIdentity(true),
mIsRSMatrix(true),
mIsUniformScale(true),
mInverseNeedsUpdate(false)
{
}
//----------------------------------------------------------------------------
Transform::~Transform ()
{
}
//----------------------------------------------------------------------------
void Transform::MakeIdentity ()
{
	mMatrix = HMatrix::IDENTITY;
	mTranslate = APoint(0.0f, 0.0f, 0.0f);
	mScale = APoint(1.0f, 1.0f, 1.0f);
	mIsIdentity = true;
	mIsRSMatrix = true;
	mIsUniformScale = true;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::MakeUnitScale ()
{
	//assertion(mIsRSMatrix, "Matrix is not a rotation\n");

	mScale = APoint(1.0f, 1.0f, 1.0f);
	mIsUniformScale = true;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::SetRotate (const HMatrix& rotate)
{
	mMatrix = rotate;
	mIsIdentity = false;
	mIsRSMatrix = true;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::SetRotate(float x, float y, float z)
{
	SetRotate(Matrix3f().MakeEulerZYX(z, y, x));
}
//----------------------------------------------------------------------------
void Transform::SetRotate(const APoint &rot)
{
	SetRotate(rot.X(), rot.Y(), rot.Z());
}
//----------------------------------------------------------------------------
void Transform::SetRotateDegree(float xDegree, float yDegree, float zDegree)
{
	SetRotate(xDegree*Mathf::DEG_TO_RAD, yDegree*Mathf::DEG_TO_RAD,
		zDegree*Mathf::DEG_TO_RAD);
}
//----------------------------------------------------------------------------
void Transform::SetRotateDegreeX(float degree)
{
	APoint rotate = GetRotateDegreeXYZ();
	SetRotateDegree(degree, rotate.Y(), rotate.Z());
}
//----------------------------------------------------------------------------
void Transform::SetRotateDegreeY(float degree)
{
	APoint rotate = GetRotateDegreeXYZ();
	SetRotateDegree(rotate.X(), degree, rotate.Z());
}
//----------------------------------------------------------------------------
void Transform::SetRotateDegreeZ(float degree)
{
	APoint rotate = GetRotateDegreeXYZ();
	SetRotateDegree(rotate.X(), rotate.Y(), degree);
}
//----------------------------------------------------------------------------
void Transform::SetRotate(const AVector &r, const AVector &d, const AVector &u)
{
	HMatrix mat(r, d, u, APoint::ORIGIN, true);
	SetRotate(mat);
}
//----------------------------------------------------------------------------
void Transform::SetMatrix (const HMatrix& matrix)
{
	mMatrix = matrix;
	mIsIdentity = false;
	mIsRSMatrix = false;
	mIsUniformScale = false;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::SetTranslate (const APoint& translate)
{
	mTranslate = translate;
	mIsIdentity = false;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::SetTranslate (float x, float y, float z)
{
	SetTranslate(APoint(x, y, z));
	mIsIdentity = false;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::SetTranslateX (float x)
{
	mTranslate.X() = x;
	mIsIdentity = false;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::SetTranslateY (float y)
{
	mTranslate.Y() = y;
	mIsIdentity = false;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::SetTranslateZ (float z)
{
	mTranslate.Z() = z;
	mIsIdentity = false;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::SetTranslateXY(float x, float z)
{
	mTranslate.X() = x;
	mTranslate.Y() = z;
	mIsIdentity = false;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::SetTranslateXZ(float x, float z)
{
	mTranslate.X() = x;
	mTranslate.Z() = z;
	mIsIdentity = false;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::SetScale (const APoint& scale)
{
	if (scale.X()==scale.Y() && scale.Y()==scale.Z())
	{
		SetUniformScale(scale.X());
	}
	else
	{
		//assertion(mIsRSMatrix, "Matrix is not a rotation\n");
		//assertion(scale[0] != 0.0f && scale[1] != 0.0f && scale[2] != 0.0f,
		//	"Scales must be nonzero\n");

		APoint lastScale = scale;
		if (lastScale == APoint::ORIGIN)
			lastScale = APoint(0.00001f, 0.00001f, 0.00001f);

		mScale = lastScale;
		mIsIdentity = false;
		mIsUniformScale = false;
		UpdateHMatrix();
	}
}
//----------------------------------------------------------------------------
void Transform::SetUniformScale (float scale)
{
	//assertion(mIsRSMatrix, "Matrix is not a rotation\n");

	if (0.0f == scale)
		scale = 0.00001f;

	mScale = APoint(scale, scale, scale);
	mIsIdentity = false;
	mIsUniformScale = true;
	UpdateHMatrix();
}
//----------------------------------------------------------------------------
void Transform::GetRotate(float &x, float &y, float &z) const
{
	Matrix3f mat = GetRotate();
	mat.ExtractEulerXYZ(x, y, z);
}
//----------------------------------------------------------------------------
APoint Transform::GetRotateXYZ() const
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	Matrix3f mat = GetRotate();
	mat.ExtractEulerXYZ(x, y, z);
	return APoint(x, y, z);
}
//----------------------------------------------------------------------------
float Transform::GetRotateX() const
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	Matrix3f mat = GetRotate();
	mat.ExtractEulerXYZ(x, y, z);

	return x;
}
//----------------------------------------------------------------------------
float Transform::GetRotateY() const
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	Matrix3f mat = GetRotate();
	mat.ExtractEulerXYZ(x, y, z);

	return y;
}
//----------------------------------------------------------------------------
float Transform::GetRotateZ() const
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	Matrix3f mat = GetRotate();
	mat.ExtractEulerXYZ(x, y, z);

	return z;
}
//----------------------------------------------------------------------------
APoint Transform::GetRotateDegreeXYZ() const
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	Matrix3f mat = GetRotate();
	mat.ExtractEulerXYZ(x, y, z);
	return APoint(x*Mathf::RAD_TO_DEG, y*Mathf::RAD_TO_DEG,
		z*Mathf::RAD_TO_DEG);
}
//----------------------------------------------------------------------------
float Transform::GetRotateDegreeX() const
{
	return GetRotateDegreeXYZ().X();
}
//----------------------------------------------------------------------------
float Transform::GetRotateDegreeY() const
{
	return GetRotateDegreeXYZ().Y();
}
//----------------------------------------------------------------------------
float Transform::GetRotateDegreeZ() const
{
	return GetRotateDegreeXYZ().Z();
}
//----------------------------------------------------------------------------
void Transform::SetDU(const AVector &dir, const AVector &uping)
{
	AVector right = dir.Cross(uping);
	right.Normalize();
	AVector up = right.Cross(dir);
	up.Normalize();

	Matrix3f matRot(right, dir, up, true);
	SetRotate(matRot);
}
//----------------------------------------------------------------------------
AVector Transform::GetDirection() const
{
	const HMatrix &matRot = GetRotate();
	HPoint dir;
	matRot.GetColumn(1, dir);

	AVector vecDir = AVector(dir[0], dir[1], dir[2]);
	vecDir.Normalize();

	return vecDir;
}
//----------------------------------------------------------------------------
void Transform::GetRDUVector(AVector &r, AVector &d, AVector &u) const
{
	const HMatrix &rotMat = GetRotate();
	HPoint r0, d0, u0;
	rotMat.GetColumn(0, r0);
	rotMat.GetColumn(1, d0);
	rotMat.GetColumn(2, u0);

	r = AVector(r0[0], r0[1], r0[2]);
	d = AVector(d0[0], d0[1], d0[2]);
	u = AVector(u0[0], u0[1], u0[2]);
}
//----------------------------------------------------------------------------
float Transform::GetNorm () const
{
	if (mIsRSMatrix)
	{
		float maxValue = Mathf::FAbs(mScale[0]);
		if (Mathf::FAbs(mScale[1]) > maxValue)
		{
			maxValue = Mathf::FAbs(mScale[1]);
		}
		if (Mathf::FAbs(mScale[2]) > maxValue)
		{
			maxValue = Mathf::FAbs(mScale[2]);
		}
		return maxValue;
	}

	// A general matrix.  Use the max-row-sum matrix norm.  The spectral
	// norm (the maximum absolute value of the eigenvalues) is smaller or
	// equal to this norm.  Therefore, this function returns an approximation
	// to the maximum scale.
	float maxRowSum =
		Mathf::FAbs(mMatrix[0][0]) +
		Mathf::FAbs(mMatrix[0][1]) +
		Mathf::FAbs(mMatrix[0][2]);

	float rowSum =
		Mathf::FAbs(mMatrix[1][0]) +
		Mathf::FAbs(mMatrix[1][1]) +
		Mathf::FAbs(mMatrix[1][2]);

	if (rowSum > maxRowSum)
	{
		maxRowSum = rowSum;
	}

	rowSum =
		Mathf::FAbs(mMatrix[2][0]) +
		Mathf::FAbs(mMatrix[2][1]) +
		Mathf::FAbs(mMatrix[2][2]);

	if (rowSum > maxRowSum)
	{
		maxRowSum = rowSum;
	}

	return maxRowSum;
}
//----------------------------------------------------------------------------
Transform Transform::operator* (const Transform& transform) const
{
	if (IsIdentity())
	{
		return transform;
	}

	if (transform.IsIdentity())
	{
		return *this;
	}

	Transform product;

	if (mIsRSMatrix && transform.mIsRSMatrix)
	{
		if (mIsUniformScale)
		{
			product.SetRotate(mMatrix*transform.mMatrix);

			product.SetTranslate(GetUniformScale()*(
				mMatrix*transform.mTranslate) + mTranslate);

			if (transform.IsUniformScale())
			{
				product.SetUniformScale(
					GetUniformScale()*transform.GetUniformScale());
			}
			else
			{
				product.SetScale(GetUniformScale()*transform.GetScale());
			}

			return product;
		}
	}

	// 在所有剩下的情况，变换不能被写成R*S*X+T的形式
	HMatrix matMA = (mIsRSMatrix ? mMatrix.TimesDiagonal(mScale) : mMatrix);
	HMatrix matMB = (transform.mIsRSMatrix ?
		transform.mMatrix.TimesDiagonal(transform.mScale) :
	transform.mMatrix);

	product.SetMatrix(matMA*matMB);

	product.SetTranslate(matMA*transform.mTranslate + mTranslate);
	return product;
}
//----------------------------------------------------------------------------
const HMatrix& Transform::Inverse () const
{
	if (mInverseNeedsUpdate)
	{
		if (mIsIdentity)
		{
			mInvHMatrix = HMatrix::IDENTITY;
		}
		else
		{
			if (mIsRSMatrix)
			{
				if (mIsUniformScale)
				{
					float invScale = 1.0f/mScale[0];
					mInvHMatrix[0][0] = invScale*mMatrix[0][0];
					mInvHMatrix[0][1] = invScale*mMatrix[1][0];
					mInvHMatrix[0][2] = invScale*mMatrix[2][0];
					mInvHMatrix[1][0] = invScale*mMatrix[0][1];
					mInvHMatrix[1][1] = invScale*mMatrix[1][1];
					mInvHMatrix[1][2] = invScale*mMatrix[2][1];
					mInvHMatrix[2][0] = invScale*mMatrix[0][2];
					mInvHMatrix[2][1] = invScale*mMatrix[1][2];
					mInvHMatrix[2][2] = invScale*mMatrix[2][2];
				}
				else
				{
					// Replace 3 reciprocals by 6 multiplies and 1 reciprocal.
					float s01 = mScale[0]*mScale[1];
					float s02 = mScale[0]*mScale[2];
					float s12 = mScale[1]*mScale[2];
					float invs012 = 1.0f/(s01*mScale[2]);
					float invS0 = s12*invs012;
					float invS1 = s02*invs012;
					float invS2 = s01*invs012;
					mInvHMatrix[0][0] = invS0*mMatrix[0][0];
					mInvHMatrix[0][1] = invS0*mMatrix[1][0];
					mInvHMatrix[0][2] = invS0*mMatrix[2][0];
					mInvHMatrix[1][0] = invS1*mMatrix[0][1];
					mInvHMatrix[1][1] = invS1*mMatrix[1][1];
					mInvHMatrix[1][2] = invS1*mMatrix[2][1];
					mInvHMatrix[2][0] = invS2*mMatrix[0][2];
					mInvHMatrix[2][1] = invS2*mMatrix[1][2];
					mInvHMatrix[2][2] = invS2*mMatrix[2][2];
				}
			}
			else
			{
				Invert3x3(mHMatrix, mInvHMatrix);
			}

			mInvHMatrix[0][3] = -(
				mInvHMatrix[0][0]*mTranslate[0] +
				mInvHMatrix[0][1]*mTranslate[1] +
				mInvHMatrix[0][2]*mTranslate[2]
			);

			mInvHMatrix[1][3] = -(
				mInvHMatrix[1][0]*mTranslate[0] +
				mInvHMatrix[1][1]*mTranslate[1] +
				mInvHMatrix[1][2]*mTranslate[2]
			);

			mInvHMatrix[2][3] = -(
				mInvHMatrix[2][0]*mTranslate[0] +
				mInvHMatrix[2][1]*mTranslate[1] +
				mInvHMatrix[2][2]*mTranslate[2]
			);

			// mHMatrix的最后一行总是(0,0,0,1)，只要在构造函数设置一次即可。不需要
			// 在这里重新设置。
		}

		mInverseNeedsUpdate = false;
	}

	return mInvHMatrix;
}
//----------------------------------------------------------------------------
Transform Transform::InverseTransform () const
{
	if (mIsIdentity)
	{
		return IDENTITY;
	}

	Transform inverse;
	APoint invTrn;
	if (mIsRSMatrix)
	{
		HMatrix invRot = mMatrix.Transpose();
		inverse.SetRotate(invRot);
		if (mIsUniformScale)
		{
			float invScale = 1.0f/mScale[0];
			inverse.SetUniformScale(invScale);
			invTrn = -invScale*(invRot*mTranslate);
		}
		else
		{
			APoint invScale(1.0f/mScale[0], 1.0f/mScale[1], 1.0f/mScale[2]);
			inverse.SetScale(invScale);
			invTrn = invRot*mTranslate;
			invTrn[0] *= -invScale[0];
			invTrn[1] *= -invScale[1];
			invTrn[2] *= -invScale[2];
		}
	}
	else
	{
		HMatrix invMat;
		Invert3x3(mMatrix, invMat);
		inverse.SetMatrix(invMat);
		invTrn = -(invMat*mTranslate);
	}
	inverse.SetTranslate(invTrn);

	return inverse;
}
//----------------------------------------------------------------------------
void Transform::UpdateHMatrix ()
{
	if (mIsIdentity)
	{
		mHMatrix = HMatrix::IDENTITY;
	}
	else
	{
		if (mIsRSMatrix)
		{
			mHMatrix[0][0] = mMatrix[0][0]*mScale[0];
			mHMatrix[0][1] = mMatrix[0][1]*mScale[1];
			mHMatrix[0][2] = mMatrix[0][2]*mScale[2];
			mHMatrix[1][0] = mMatrix[1][0]*mScale[0];
			mHMatrix[1][1] = mMatrix[1][1]*mScale[1];
			mHMatrix[1][2] = mMatrix[1][2]*mScale[2];
			mHMatrix[2][0] = mMatrix[2][0]*mScale[0];
			mHMatrix[2][1] = mMatrix[2][1]*mScale[1];
			mHMatrix[2][2] = mMatrix[2][2]*mScale[2];
		}
		else
		{
			mHMatrix[0][0] = mMatrix[0][0];
			mHMatrix[0][1] = mMatrix[0][1];
			mHMatrix[0][2] = mMatrix[0][2];
			mHMatrix[1][0] = mMatrix[1][0];
			mHMatrix[1][1] = mMatrix[1][1];
			mHMatrix[1][2] = mMatrix[1][2];
			mHMatrix[2][0] = mMatrix[2][0];
			mHMatrix[2][1] = mMatrix[2][1];
			mHMatrix[2][2] = mMatrix[2][2];
		}

		mHMatrix[0][3] = mTranslate[0];
		mHMatrix[1][3] = mTranslate[1];
		mHMatrix[2][3] = mTranslate[2];

		// mHMatrix的最后一行总是(0,0,0,1)，只要在构造函数设置一次即可。不需要
		// 在这里重新设置。
	}

	mInverseNeedsUpdate = true;
}
//----------------------------------------------------------------------------
void Transform::Invert3x3 (const HMatrix& mat, HMatrix& invMat)
{
	// 计算伴随矩阵adjoint(M)
	invMat[0][0] = mat[1][1]*mat[2][2] - mat[1][2]*mat[2][1];
	invMat[0][1] = mat[0][2]*mat[2][1] - mat[0][1]*mat[2][2];
	invMat[0][2] = mat[0][1]*mat[1][2] - mat[0][2]*mat[1][1];
	invMat[1][0] = mat[1][2]*mat[2][0] - mat[1][0]*mat[2][2];
	invMat[1][1] = mat[0][0]*mat[2][2] - mat[0][2]*mat[2][0];
	invMat[1][2] = mat[0][2]*mat[1][0] - mat[0][0]*mat[1][2];
	invMat[2][0] = mat[1][0]*mat[2][1] - mat[1][1]*mat[2][0];
	invMat[2][1] = mat[0][1]*mat[2][0] - mat[0][0]*mat[2][1];
	invMat[2][2] = mat[0][0]*mat[1][1] - mat[0][1]*mat[1][0];

	// 计算矩阵的质determinant(M)
	float invDet = 1.0f/(
		mat[0][0]*invMat[0][0] +
		mat[0][1]*invMat[1][0] +
		mat[0][2]*invMat[2][0]
	);

	// inverse(M) = adjoint(M)/determinant(M)
	invMat[0][0] *= invDet;
	invMat[0][1] *= invDet;
	invMat[0][2] *= invDet;
	invMat[1][0] *= invDet;
	invMat[1][1] *= invDet;
	invMat[1][2] *= invDet;
	invMat[2][0] *= invDet;
	invMat[2][1] *= invDet;
	invMat[2][2] *= invDet;
}
//----------------------------------------------------------------------------