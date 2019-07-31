// PX2OpenGLESRendererData.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESRendererData.hpp"
#include "PX2OpenGLESMapping.hpp"
using namespace PX2;

RendererData::RenderState::RenderState ()
{
}
//----------------------------------------------------------------------------
void RendererData::RenderState::Initialize (const AlphaProperty* astate,
											const CullProperty* cstate, 
											const DepthProperty* dstate,
											const OffsetProperty* ostate, 
											const StencilProperty* sstate,
											const WireProperty* wstate)
{
	// AlphaProperty
	mAlphaBlendEnabled = astate->BlendEnabled;
	mAlphaSrcBlend = gOGLAlphaSrcBlend[astate->SrcBlend];
	mAlphaDstBlend = gOGLAlphaDstBlend[astate->DstBlend];
	mAlphaCompareEnabled = astate->CompareEnabled;
	mCompareFunction = gOGLAlphaCompare[astate->Compare];
	mAlphaReference = astate->Reference;
	mBlendColor = astate->ConstantColor;

	mAlphaBlendEnabled ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
	glBlendFunc(mAlphaSrcBlend, GL_ONE_MINUS_SRC_ALPHA);
	//mAlphaCompareEnabled ? glEnable(GL_ALPHA_TEST) 
	//		: glDisable(GL_ALPHA_TEST); //< shader中支持
	//glAlphaFunc(mCompareFunction, mAlphaReference); //< shader中支持
	glBlendColor(mBlendColor[0], mBlendColor[1], mBlendColor[2],
		mBlendColor[3]);

	// CullProperty
	mCullEnabled = cstate->Enabled;
	mCCWOrder = cstate->CCWOrder;

	mCullEnabled ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(mCCWOrder ? GL_BACK : GL_FRONT);

	// DepthProperty
	mDepthEnabled = dstate->Enabled;
	mDepthWriteEnabled = dstate->Writable;
	mDepthCompareFunction = gOGLDepthCompare[dstate->Compare];

	mDepthEnabled ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	glDepthMask(mDepthWriteEnabled ? GL_TRUE : GL_FALSE);
	glDepthFunc(mDepthCompareFunction);

	// OffsetProperty
	mFillEnabled = ostate->FillEnabled;
	mLineEnabled = ostate->LineEnabled;
	mPointEnabled = ostate->PointEnabled;
	mOffsetScale = ostate->Scale;
	mOffsetBias = ostate->Bias;

	mFillEnabled ? glEnable(GL_POLYGON_OFFSET_FILL) :
		glDisable(GL_POLYGON_OFFSET_FILL);
	//mLineEnabled ? glEnable(GL_POLYGON_OFFSET_LINE) : //< 不支持
	//	glDisable(GL_POLYGON_OFFSET_LINE);
	//mPointEnabled ? glEnable(GL_POLYGON_OFFSET_POINT) : //< 不支持
	//	glDisable(GL_POLYGON_OFFSET_POINT);
	glPolygonOffset(mOffsetScale, mOffsetBias);

	// StencilProperty
	mStencilEnabled = sstate->Enabled;
	mStencilCompareFunction = gOGLStencilCompare[sstate->Compare];
	mStencilReference = sstate->Reference;
	mStencilMask = sstate->Mask;
	mStencilWriteMask = sstate->WriteMask;
	mStencilOnFail = gOGLStencilOperation[sstate->OnFail];
	mStencilOnZFail = gOGLStencilOperation[sstate->OnZFail];
	mStencilOnZPass = gOGLStencilOperation[sstate->OnZPass];

	mStencilEnabled ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
	glStencilFunc(mStencilCompareFunction, mStencilReference, mStencilMask);
	glStencilMask(mStencilWriteMask);
	glStencilOp(mStencilOnFail, mStencilOnZFail, mStencilOnZPass);

	// WireProperty
	mWireEnabled = wstate->Enabled;
	//glPolygonMode(GL_FRONT_AND_BACK,
		// mWireEnabled ? GL_LINE : GL_FILL); //< 不支持
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
RendererData::SamplerState::SamplerState ()
{
	mMagFilter = GL_NEAREST;
	mMinFilter = GL_NEAREST;
	mWrap[0] = GL_REPEAT;
	mWrap[1] = GL_REPEAT;
}
//----------------------------------------------------------------------------
void RendererData::SamplerState::GetCurrent (GLenum target)
{
	glGetTexParameteriv(target, GL_TEXTURE_MAG_FILTER, &mMagFilter);
	glGetTexParameteriv(target, GL_TEXTURE_MIN_FILTER, &mMinFilter);
	glGetTexParameteriv(target, GL_TEXTURE_WRAP_S, &mWrap[0]);
	glGetTexParameteriv(target, GL_TEXTURE_WRAP_T, &mWrap[1]);
}
//----------------------------------------------------------------------------

#endif
