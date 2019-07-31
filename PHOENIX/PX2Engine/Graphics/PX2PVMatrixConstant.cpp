// PX2PVMatrixConstant.cpp

#include "PX2PVMatrixConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, PVMatrixConstant);
PX2_IMPLEMENT_STREAM(PVMatrixConstant);
PX2_IMPLEMENT_FACTORY(PVMatrixConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, PVMatrixConstant);
PX2_IMPLEMENT_DEFAULT_STREAM(ShaderFloat, PVMatrixConstant);

//----------------------------------------------------------------------------
PVMatrixConstant::PVMatrixConstant ()
:
ShaderFloat(4)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
PVMatrixConstant::~PVMatrixConstant ()
{
}
//----------------------------------------------------------------------------
void PVMatrixConstant::Update(const ShaderStruct *struc)
{
	const Camera *camera = struc->TheCamera;

	const HMatrix& projViewMatrix = camera->GetProjectionViewMatrix();

#ifdef PX2_USE_OPENGLES
	// you must trans on 0penGLES2
	HMatrix matTrans = projViewMatrix.Transpose();
	const float *source = (const float*)matTrans;
#else
	const float* source = (const float*)projViewMatrix;
#endif

	float* target = mData;
	for (int i = 0; i < 16; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------
