// PX2CameraModelDVectorConstant.cpp

#include "PX2CameraModelDVectorConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, CameraModelDVectorConstant);
PX2_IMPLEMENT_STREAM(CameraModelDVectorConstant);
PX2_IMPLEMENT_FACTORY(CameraModelDVectorConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, CameraModelDVectorConstant);
PX2_IMPLEMENT_DEFAULT_STREAM(ShaderFloat, CameraModelDVectorConstant);

//----------------------------------------------------------------------------
CameraModelDVectorConstant::CameraModelDVectorConstant ()
:
ShaderFloat(1)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
CameraModelDVectorConstant::~CameraModelDVectorConstant ()
{
}
//----------------------------------------------------------------------------
void CameraModelDVectorConstant::Update(const ShaderStruct *struc)
{
	const Camera *camera = struc->TheCamera;
	const Renderable *renderable = struc->TheRenderable;

	const AVector& worldDVector = camera->GetDVector();
	const HMatrix& worldInvMatrix = renderable->WorldTransform.Inverse();
	AVector modelDVector = worldInvMatrix*worldDVector;

	const float* source = (const float*)modelDVector;
	float* target = mData;
	for (int i = 0; i < 4; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------