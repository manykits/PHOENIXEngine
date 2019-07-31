// PX2ParticleObject.cpp

#include "PX2ParticleObject.hpp"
#include "PX2Renderable.hpp"
#include "PX2Renderer.hpp"
#include "PX2ParticleEmitter.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ParticleObject::ParticleObject () :
RotSpeedInit(0.0f),
RotSpeed(0.0f),
RotDegreeInit(0.0f),
RotDegree(0.0f),
SpeedInit(0.0f),
Speed(0.0f),
AccelerateInit(0.0f),
Accelerate(0.0f),
BlastIndex(0)
{
}
//----------------------------------------------------------------------------
ParticleObject::~ParticleObject ()
{
}
//----------------------------------------------------------------------------
bool ParticleObject::Update (Effectable *able, float elapsedTime)
{
	if (!EffectObject::Update(able, elapsedTime))
		return false;

	Camera *camera = PX2_GR.GetCurUpdateCamera();
	if (!camera)
		return true;
	
	ParticleEmitter *pe = DynamicCast<ParticleEmitter>(able);
	ParticleEmitter::FaceType faceType = pe->GetFaceType();
	ParticleEmitter::RotateAxisType rotateAxisType = pe->GetRotateAxisType();
	const APoint &attactPoint = pe->GetEmitAttactPoint();
	const AVector &attactStrength = pe->GetEmitAttackStrength();
	ParticleEmitter::CollisionType ct = pe->GetCollisionType();
	ParticleEmitter::CollisionOption co = pe->GetCollisionOption();
	float collisionHeight = pe->GetCollisionFaceHeight();
	float collisionSpeedPercent = pe->GetCollisionSpeedPercent();

	APoint worldAttactPoint = attactPoint;
	if (!pe->IsLocal())
		worldAttactPoint = pe->WorldTransform * attactPoint;
	AVector worldAttactStrength = attactStrength;

	AVector attactDir = worldAttactPoint - Pos;
	attactDir.Normalize();
	attactDir.X() *= worldAttactStrength.X();
	attactDir.Y() *= worldAttactStrength.Y();
	attactDir.Z() *= worldAttactStrength.Z();

	PrePos			=  Pos;

	// speed
	AVector spdir	= Speed * SpeedDir;
	AVector acdir	= Accelerate * AccelerateDir;
	AVector speedDir= spdir + acdir * elapsedTime + attactDir * elapsedTime;

	if (Obstruct != AVector::ZERO)
	{
		AVector obs;
		obs.X() = Mathf::FAbs(speedDir.X()) * Obstruct.X() * elapsedTime;
		obs.Y() = Mathf::FAbs(speedDir.Y()) * Obstruct.Y() * elapsedTime;
		obs.Z() = Mathf::FAbs(speedDir.Z()) * Obstruct.Z() * elapsedTime;
		if (speedDir.X() < 0.0f)
		{
			speedDir.X() += obs.X();

			if (speedDir.X() > 0.0f)
				speedDir.X() = 0.0f;
		}
		else if (speedDir.X() > 0.0f)
		{
			speedDir.X() -= obs.X();

			if (speedDir.X() < 0.0f)
				speedDir.X() = 0.0f;
		}
		if (speedDir.Y() < 0.0f)
		{
			speedDir.Y() += obs.Y();

			if (speedDir.Y() > 0.0f)
				speedDir.Y() = 0.0f;
		}
		else if (speedDir.Y() > 0.0f)
		{
			speedDir.Y() -= obs.Y();

			if (speedDir.Y() < 0.0f)
				speedDir.Y() = 0.0f;
		}
		if (speedDir.Z() < 0.0f)
		{
			speedDir.Z() += obs.Z();

			if (speedDir.Z() > 0.0f)
				speedDir.Z() = 0.0f;
		}
		else if (speedDir.Z() > 0.0f)
		{
			speedDir.Z() -= obs.Z();

			if (speedDir.Z() < 0.0f)
				speedDir.Z() = 0.0f;
		}
	}

	if (ParticleEmitter::CT_FACE_HEIGHTUP == ct)
	{
		APoint toPos = Pos;
		toPos += speedDir * elapsedTime;
		if (toPos.Z() < collisionHeight)
		{
			if (ParticleEmitter::CO_REFLECT == co)
			{
				toPos.Z() = collisionHeight;
				speedDir.Z() *= -1.0f;

				speedDir *= collisionSpeedPercent;
			}
			else
			{
				toPos.Z() = collisionHeight;
				Age = Life;
			}
		}
		Pos = toPos;
	}
	else
	{
		Pos	+= speedDir * elapsedTime;
	}

	float spLength	= speedDir.Normalize();
	Speed			= spLength;
	SpeedDir		= speedDir;

	// rotate
	RotDegree		+= RotSpeed * elapsedTime;

	// d u r
	if (ParticleEmitter::FT_X == faceType)
	{
		DVector = AVector::UNIT_X;
		UVector = AVector::UNIT_Z;
		RVector = -AVector::UNIT_Y;
	}
	else if (ParticleEmitter::FT_NX == faceType)
	{
		DVector = -AVector::UNIT_X;
		UVector = AVector::UNIT_Z;
		RVector = AVector::UNIT_Y;
	}
	else if (ParticleEmitter::FT_Y == faceType)
	{
		DVector = AVector::UNIT_Y;
		UVector = AVector::UNIT_Z;
		RVector = AVector::UNIT_X;
	}
	else if (ParticleEmitter::FT_NY == faceType)
	{
		DVector = -AVector::UNIT_Y;
		UVector = AVector::UNIT_Z;
		RVector = -AVector::UNIT_X;
	}
	else if (ParticleEmitter::FT_Z == faceType)
	{
		DVector = AVector::UNIT_Z;
		UVector = AVector::UNIT_Y;
		RVector = -AVector::UNIT_X;
	}
	else if (ParticleEmitter::FT_NZ == faceType)
	{
		DVector = -AVector::UNIT_Z;
		UVector = AVector::UNIT_Y;
		RVector = AVector::UNIT_X;
	}
	else if (ParticleEmitter::FT_CAMERA == faceType)
	{
		DVector = -camera->GetDVector();
		UVector = camera->GetUVector();
		RVector = -camera->GetRVector();
	}
	else if (ParticleEmitter::FT_SPEEDDIR == faceType)
	{
		DVector = SpeedDir;

		if (DVector != AVector::UNIT_Z)
		{
			RVector = AVector::UNIT_Z.Cross(DVector);
			RVector.Normalize();

			UVector = DVector.Cross(RVector);
			UVector.Normalize();
		}
		else
		{
			UVector = AVector::UNIT_Y;
			RVector = -AVector::UNIT_X;
		}
	}
	else if (ParticleEmitter::FT_CAMERA_ALIGN_EMITDIR == faceType)
	{
		DVector = -camera->GetDVector();
		RVector = SpeedDir;
		UVector = DVector.Cross(RVector);
		UVector.Normalize();
	}

	DVectorInit = DVector;
	UVectorInit = UVector;
	RVectorInit = RVector;

	// 有旋转才进行计算
	if (RotDegree != 0.0f)
	{
		if (rotateAxisType == ParticleEmitter::RAT_D)
		{
			RotAxis = DVector;

			HMatrix matRot(RotAxis, RotDegree);
			UVector = matRot * UVectorInit;
			RVector = matRot * RVectorInit;
		}
		else if (rotateAxisType == ParticleEmitter::RAT_U)
		{
			RotAxis = UVector;

			HMatrix matRot(RotAxis, RotDegree);
			DVector = matRot * DVectorInit;
			RVector = matRot * RVectorInit;
		}
		else if (rotateAxisType == ParticleEmitter::RAT_R)
		{
			RotAxis = RVector;

			HMatrix matRot(RotAxis, RotDegree);
			DVector = matRot * DVectorInit;
			UVector = matRot * UVectorInit;
		}
		else if (rotateAxisType == ParticleEmitter::RAT_FREE)
		{
			HMatrix matRot(RotAxis, RotDegree);
			DVector = matRot * DVectorInit;
			UVector = matRot * UVectorInit;
			RVector = matRot * RVectorInit;
		}
	}

	return true;
}
//----------------------------------------------------------------------------