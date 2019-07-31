// PX2PVWMatrixConstant.cpp

#include "PX2PVWMatrixConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, PVWMatrixConstant);
PX2_IMPLEMENT_STREAM(PVWMatrixConstant);
PX2_IMPLEMENT_FACTORY(PVWMatrixConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, PVWMatrixConstant);
PX2_IMPLEMENT_DEFAULT_STREAM(ShaderFloat, PVWMatrixConstant);

//----------------------------------------------------------------------------
PVWMatrixConstant::PVWMatrixConstant ()
:
ShaderFloat(4)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
PVWMatrixConstant::~PVWMatrixConstant ()
{
}
//----------------------------------------------------------------------------
void PVWMatrixConstant::Update(const ShaderStruct *struc)
{
	const Camera *camera = struc->TheCamera;
	const Renderable *renderable = struc->TheRenderable;

	const HMatrix& projViewMatrix = camera->GetProjectionViewMatrix();
	const HMatrix& worldMatrix = renderable->WorldTransform.Matrix();
	HMatrix projViewWorldMatrix = projViewMatrix*worldMatrix;

#ifdef PX2_USE_OPENGLES
	// you must trans on 0penGLES2
	HMatrix matTrans = projViewWorldMatrix.Transpose();
	const float *source = (const float*)matTrans;
#else
	const float* source = (const float*)projViewWorldMatrix;
#endif

	float* target = mData;
	for (int i = 0; i < 16; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------