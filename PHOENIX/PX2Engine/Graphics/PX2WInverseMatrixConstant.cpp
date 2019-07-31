// PX2WInverseMatrixConstant.cpp

#include "PX2WInverseMatrixConstant.hpp"
#include "PX2Camera.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, ShaderFloat, WInverseMatrixConstant);
PX2_IMPLEMENT_STREAM(WInverseMatrixConstant);
PX2_IMPLEMENT_FACTORY(WInverseMatrixConstant);
PX2_IMPLEMENT_DEFAULT_NAMES(ShaderFloat, WInverseMatrixConstant);
PX2_IMPLEMENT_DEFAULT_STREAM(ShaderFloat, WInverseMatrixConstant);

//----------------------------------------------------------------------------
WInverseMatrixConstant::WInverseMatrixConstant() :
ShaderFloat(4)
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
WInverseMatrixConstant::~WInverseMatrixConstant()
{
}
//----------------------------------------------------------------------------
void WInverseMatrixConstant::Update(const ShaderStruct *struc)
{
	const Renderable *renderable = struc->TheRenderable;

	Transform inversTrans = renderable->WorldTransform.InverseTransform();
	const HMatrix &worldInverseMatrix = inversTrans.GetMatrix();

#ifdef PX2_USE_OPENGLES
	// you must trans on 0penGLES2
	HMatrix matTrans = worldInverseMatrix.Transpose();
	const float *source = (const float*)matTrans;
#else
	const float* source = (const float*)worldInverseMatrix;
#endif

	float* target = mData;
	for (int i = 0; i < 16; ++i)
	{
		*target++ = *source++;
	}
}
//----------------------------------------------------------------------------