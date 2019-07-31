// PX2CameraModelPositionConstant.cpp

#include "PX2CameraModelPositionConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, CameraModelPositionConstant);
PX2_IMPLEMENT_STREAM(CameraModelPositionConstant);
PX2_IMPLEMENT_FACTORY(CameraModelPositionConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, CameraModelPositionConstant);
PX2_IMPLEMENT_DEFAULT_STREAM(ShaderFloat, CameraModelPositionConstant);

//----------------------------------------------------------------------------
CameraModelPositionConstant::CameraModelPositionConstant ()
:
ShaderFloat(1)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
CameraModelPositionConstant::~CameraModelPositionConstant ()
{
}
//----------------------------------------------------------------------------
void CameraModelPositionConstant::Update(const ShaderStruct *struc)
{
	const Camera *camera = struc->TheCamera;
	const Renderable *renderable = struc->TheRenderable;

	const APoint& worldPosition = camera->GetPosition();
	const HMatrix& worldInvMatrix = renderable->WorldTransform.Inverse();
	APoint modelPosition = worldInvMatrix*worldPosition;

	const float* source = (const float*)modelPosition;
	float* target = mData;
	for (int i = 0; i < 4; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------