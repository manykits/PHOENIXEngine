// PX2WMatrixConstant.cpp

#include "PX2WMatrixConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, WMatrixConstant);
PX2_IMPLEMENT_STREAM(WMatrixConstant);
PX2_IMPLEMENT_FACTORY(WMatrixConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, WMatrixConstant);
PX2_IMPLEMENT_DEFAULT_STREAM(ShaderFloat, WMatrixConstant);

//----------------------------------------------------------------------------
WMatrixConstant::WMatrixConstant ()
:
ShaderFloat(4)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
WMatrixConstant::~WMatrixConstant ()
{
}
//----------------------------------------------------------------------------
void WMatrixConstant::Update(const ShaderStruct *struc)
{
	const Renderable *renderable = struc->TheRenderable;

	const HMatrix& worldMatrix = renderable->WorldTransform.Matrix();

#ifdef PX2_USE_OPENGLES
	// you must trans on 0penGLES2
	HMatrix matTrans = worldMatrix.Transpose();
	const float *source = (const float*)matTrans;
#else
	const float* source = (const float*)worldMatrix;
#endif

	float* target = mData;
	for (int i = 0; i < 16; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------
