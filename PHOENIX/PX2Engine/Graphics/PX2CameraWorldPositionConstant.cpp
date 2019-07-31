// PX2CameraWorldPositionConstant.cpp

#include "PX2CameraWorldPositionConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, CameraWorldPositionConstant);
PX2_IMPLEMENT_STREAM(CameraWorldPositionConstant);
PX2_IMPLEMENT_FACTORY(CameraWorldPositionConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, CameraWorldPositionConstant);
PX2_IMPLEMENT_DEFAULT_STREAM(ShaderFloat, CameraWorldPositionConstant);

//----------------------------------------------------------------------------
CameraWorldPositionConstant::CameraWorldPositionConstant ()
:
ShaderFloat(1)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
CameraWorldPositionConstant::~CameraWorldPositionConstant ()
{
}
//----------------------------------------------------------------------------
void CameraWorldPositionConstant::Update(const ShaderStruct *struc)
{
	const Camera *camera = struc->TheCamera;

	const APoint& worldPosition = camera->GetPosition();

	const float* source = (const float*)worldPosition;
	float* target = mData;
	for (int i = 0; i < 4; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------
