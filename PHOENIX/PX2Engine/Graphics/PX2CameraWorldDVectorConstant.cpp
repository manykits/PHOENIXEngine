// PX2CameraWorldDVectorConstant.cpp

#include "PX2CameraWorldDVectorConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, CameraWorldDVectorConstant);
PX2_IMPLEMENT_STREAM(CameraWorldDVectorConstant);
PX2_IMPLEMENT_FACTORY(CameraWorldDVectorConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, CameraWorldDVectorConstant);
PX2_IMPLEMENT_DEFAULT_STREAM(ShaderFloat, CameraWorldDVectorConstant);

//----------------------------------------------------------------------------
CameraWorldDVectorConstant::CameraWorldDVectorConstant ()
:
ShaderFloat(1)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
CameraWorldDVectorConstant::~CameraWorldDVectorConstant ()
{
}
//----------------------------------------------------------------------------
void CameraWorldDVectorConstant::Update(const ShaderStruct *struc)
{
	const Camera *camera = struc->TheCamera;

	const AVector& worldDVector = camera->GetDVector();

	const float* source = (const float*)worldDVector;
	float* target = mData;
	for (int i = 0; i < 4; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------
