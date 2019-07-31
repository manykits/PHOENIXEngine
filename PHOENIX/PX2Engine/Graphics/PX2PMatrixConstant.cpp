// PX2PMatrixConstant.cpp

#include "PX2PMatrixConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, PMatrixConstant);
PX2_IMPLEMENT_STREAM(PMatrixConstant);
PX2_IMPLEMENT_FACTORY(PMatrixConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, PMatrixConstant);
PX2_IMPLEMENT_DEFAULT_STREAM(ShaderFloat, PMatrixConstant);

//----------------------------------------------------------------------------
PMatrixConstant::PMatrixConstant ()
:
ShaderFloat(4)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
PMatrixConstant::~PMatrixConstant ()
{
}
//----------------------------------------------------------------------------
void PMatrixConstant::Update(const ShaderStruct *struc)
{
	const Camera *camera = struc->TheCamera;

	const HMatrix& projMatrix = camera->GetProjectionMatrix();

#ifdef PX2_USE_OPENGLES
	// you must trans on 0penGLES2
	HMatrix matTrans = projMatrix.Transpose();
	const float *source = (const float*)matTrans;
#else
	const float* source = (const float*)projMatrix;
#endif

	float* target = mData;
	for (int i = 0; i < 16; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------
