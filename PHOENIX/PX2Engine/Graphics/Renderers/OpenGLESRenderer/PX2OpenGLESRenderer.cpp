// PX2OpenGLESRenderer.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2Renderer.hpp"
#include "PX2OpenGLESMapping.hpp"
#include "PX2OpenGLESRendererData.hpp"
#include "PX2Polypoint.hpp"
#include "PX2Polysegment.hpp"
#include "PX2TriFan.hpp"
#include "PX2TriStrip.hpp"
#include "PX2OpenGLESMaterialPass.hpp"
#include "PX2OpenGLESRendererInput.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Renderer::Renderer (RendererInput *input, int width, int height,
	Texture::Format colorFormat, Texture::Format depthStencilFormat,
	int numMultisamples)
{
	Initialize(width, height, colorFormat, depthStencilFormat,
		numMultisamples);

	mData = new0 RendererData();

	PdrRendererInput *pdrRenderInput = (PdrRendererInput *)(input);

#if defined(_WIN32) || defined(WIN32)  || defined (__LINUX__)
    
    mData->mWindowHandle = pdrRenderInput->mWindowHandle;
    mData->mDisplayType = pdrRenderInput->mRendererDC;
    
	EGLDisplay display = eglGetDisplay(pdrRenderInput->mRendererDC);
	if (display == EGL_NO_DISPLAY)
	{
		assertion(false, "eglGetDisplay error.\n");
		PX2_LOG_ERROR("eglGetDisplay error.");
	}
	mData->mDisplay = display;

	EGLint majorVersion;
	EGLint minorVersion;
	if (EGL_FALSE == eglInitialize(display, &majorVersion, &minorVersion))
	{
		assertion(false, "eglInitialize error.\n");
		PX2_LOG_ERROR("eglInitialize error.");
	}

	EGLint numConfigs;
	if (!eglGetConfigs(display, NULL, 0, &numConfigs) )
	{
		assertion(false, "eglGetConfigs error.\n");
		PX2_LOG_ERROR("eglGetConfigs error.");
	}

	//const EGLint attribList[] =
	//{
	//	EGL_RED_SIZE, 8,
	//	EGL_GREEN_SIZE, 8,
	//	EGL_BLUE_SIZE, 8,
	//	EGL_DEPTH_SIZE, 16,
	//	EGL_STENCIL_SIZE, 8,
	//	EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
	//	EGL_SAMPLE_BUFFERS, 1,
	//	EGL_SAMPLES, 0, // 在这里修改MSAA的倍数
	//	EGL_NONE
	//};

	EGLint attribList[5];
	attribList[0] = EGL_SURFACE_TYPE;
	attribList[1] = EGL_WINDOW_BIT;
	attribList[2] = EGL_RENDERABLE_TYPE;
	attribList[3] = EGL_OPENGL_ES2_BIT;
	attribList[4] = EGL_NONE;

	EGLConfig config;
	if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
	{
		assertion(false, "eglChooseConfig error.\n");
		PX2_LOG_ERROR("eglChooseConfig error.");
	}
	mData->mConfig = config;

	EGLSurface surface = eglCreateWindowSurface(display, config, mData->mWindowHandle, NULL);
	if (surface == EGL_NO_SURFACE)
	{
		assertion(false, "eglCreateWindowSurface error.\n");
		PX2_LOG_ERROR("eglCreateWindowSurface error.");

		EGLint eglError = eglGetError();
		if(eglError != EGL_SUCCESS)
		{
			PX2_LOG_ERROR("eglGetError() = %i (0x%.8x) at line %i\n", eglError, eglError, __LINE__);
		}
	}
	mData->mSurface = surface;

	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
	EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT)
	{
		assertion(false, "");
		PX2_LOG_ERROR("eglCreateContext error.");
	}
	mData->mContext = context;

	if (!eglMakeCurrent(display, surface, surface, context))
	{
		assertion(false, "eglMakeCurrent error.\n");
		PX2_LOG_ERROR("eglMakeCurrent error.");
	}
#endif

	 VertexShader::SetProfile(VertexShader::VP_OPENGLES2);
	 PixelShader::SetProfile(PixelShader::PP_OPENGLES2);

	 GLint maxVertexAttribs = 0;
	 glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
	 assertion(maxVertexAttribs>=8, "attribs num must >=8.\n");

	 mData->mMaxVShaderImages = 0;
	 mData->mMaxPShaderImages = 0;
	 mData->mMaxCombinedImages = 0;
	 mData->mMaxRenderBufferSize = 0;
	 glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		 &mData->mMaxVShaderImages);
	 glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,
		 &mData->mMaxPShaderImages);
	 glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		 &mData->mMaxCombinedImages);
	 glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &mData->mMaxRenderBufferSize);

	 // 设置缺省渲染状态和采样状态
	 mData->mCurrentRS.Initialize(mDefaultAlphaProperty, mDefaultCullProperty,
		 mDefaultDepthProperty, mDefaultOffsetProperty, mDefaultStencilProperty,
		 mDefaultWireProperty);

	 mNumDrawPrimitivePerFrame = 0;
}
//----------------------------------------------------------------------------
Renderer::~Renderer ()
{
#if defined(_WIN32) || defined(WIN32) || defined (__LINUX__)
	PX2_EGL_CHECK(eglSwapBuffers(mData->mDisplay, mData->mSurface));
#endif

	Terminate();

#if defined(_WIN32) || defined(WIN32) || defined (__LINUX__)
	PX2_EGL_CHECK(eglSwapBuffers(mData->mDisplay, mData->mSurface));
	PX2_EGL_CHECK(eglMakeCurrent(mData->mDisplay, EGL_NO_SURFACE,
		EGL_NO_SURFACE, EGL_NO_CONTEXT));
	PX2_EGL_CHECK(eglDestroyContext(mData->mDisplay, mData->mSurface));
	PX2_EGL_CHECK(eglDestroySurface(mData->mDisplay, mData->mSurface));
	PX2_EGL_CHECK(eglTerminate(mData->mDisplay));
#endif

	if (mData)
	{
		delete0(mData);
		mData = 0;
	}
}
//----------------------------------------------------------------------------
Renderer *Renderer::CreateRenderer(void *ptData, void *ptDataLinuxWindow,
	void *ptDataLinuxVisual, int width, int height,
	int numMultisamples, RendererInput* &renderInput)
{
	PX2_LOG_INFO("Begin Create Renderer width:%d, height:%d", width, height);

	PdrRendererInput *pdrRenderInput = new0 PdrRendererInput();
	renderInput = pdrRenderInput;

	Renderer *renderer = 0;
	Texture::Format colorFormat = Texture::TF_A8R8G8B8;
	Texture::Format depthStencilFormat = Texture::TF_D24S8;

#if defined(_WIN32) || defined(WIN32)

	HWND hWnd = (HWND)ptData;
	pdrRenderInput->mWindowHandle = hWnd;
	pdrRenderInput->mRendererDC = GetDC(hWnd);

#elif defined (__ANDROID__)

	pdrRenderInput->mWindowHandle = 0;
	pdrRenderInput->mRendererDC = EGL_DEFAULT_DISPLAY;

#elif defined (__LINUX__)

	pdrRenderInput->mWindowHandle = *((Window*)ptDataLinuxWindow);
	pdrRenderInput->mRendererDC = (EGLNativeDisplayType)ptData;

#endif

	renderer = new0 Renderer(pdrRenderInput, width, height,
		colorFormat, depthStencilFormat, numMultisamples);

	renderer->SetClearColor(Float4::WHITE);

	PX2_LOG_INFO("End Create Renderer.");

	return renderer;
}
//----------------------------------------------------------------------------
std::string Renderer::GetRenderTag()
{
	return "OpenGLES";
}
//----------------------------------------------------------------------------
void Renderer::InitRenderStates ()
{
	mData->mCurrentRS.Initialize(mDefaultAlphaProperty, mDefaultCullProperty,
		mDefaultDepthProperty, mDefaultOffsetProperty, mDefaultStencilProperty,
		mDefaultWireProperty);

	for (int i=0; i<RendererData::MAX_NUM_PSAMPLERS; i++)
	{
		mData->mCurrentSS[i] = RendererData::SamplerState();
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Pass 管理
//----------------------------------------------------------------------------
void Renderer::Bind (const MaterialPass *pass)
{
	if (mMaterialPasses.find(pass) == mMaterialPasses.end())
	{
		PdrMaterialPass *pdrMaterialPass = new0 PdrMaterialPass(this, pass);
		mMaterialPasses[pass] = pdrMaterialPass;
		((MaterialPass*)pass)->PdrPointer = pdrMaterialPass;
	}
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const MaterialPass *pass)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Bind(pass);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const MaterialPass *pass)
{
	MaterialPassMap::iterator iter = mMaterialPasses.find(pass);
	if (iter != mMaterialPasses.end())
	{
		PdrMaterialPass* pdrMaterialPass = iter->second;
		delete0(pdrMaterialPass);
		mMaterialPasses.erase(iter);
		((MaterialPass*)pass)->PdrPointer = 0;
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const MaterialPass *pass)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Unbind(pass);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const Renderable* renderable,
					   const MaterialInstance* instance, int index)
{
	const MaterialPass* pass = instance->GetPass(index);

	PdrMaterialPass *pdrMaterialPass = 0;

	if (1 == (int)msRenderers.size())
	{
		if (pass->PdrPointer)
		{
			pdrMaterialPass = (PdrMaterialPass*)pass->PdrPointer;
		}
		else
		{
			pdrMaterialPass = new0 PdrMaterialPass(this, pass);
			mMaterialPasses[pass] = pdrMaterialPass;
			((MaterialPass*)pass)->PdrPointer = pdrMaterialPass;
		}
	}
	else
	{
		MaterialPassMap::iterator iter = mMaterialPasses.find(pass);
		if (iter != mMaterialPasses.end())
		{
			pdrMaterialPass = iter->second;
		}
		else
		{
			pdrMaterialPass = new0 PdrMaterialPass(this, pass);
			mMaterialPasses[pass] = pdrMaterialPass;
		}
	}

	ShaderParameters* vparams = instance->GetVertexParameters(index);
	ShaderParameters* pparams = instance->GetPixelParameters(index);
	VertexShader* vshader = pass->GetVertexShader();
	PixelShader* pshader = pass->GetPixelShader();

	ShaderStruct struc;
	struc.TheRenderable = (Renderable *)renderable;
	struc.TheCamera = (Camera *)mCamera;
	struc.TheEnvirParam = PX2_GR.GetCurEnvirParamController();

	// 更新着色器常量
	vparams->UpdateConstants(&struc);
	pparams->UpdateConstants(&struc);

	// 设置渲染状态
	SetAlphaProperty(pass->GetAlphaProperty());
	SetCullProperty(pass->GetCullProperty());
	SetDepthProperty(pass->GetDepthProperty());
	SetOffsetProperty(pass->GetOffsetProperty());
	SetStencilProperty(pass->GetStencilProperty());
	SetWireProperty(pass->GetWireProperty());

	// 激活着色器
	pdrMaterialPass->Enable(this);
	Enable(vshader, vparams);
	Enable(pshader, pparams);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const Renderable* renderable,
						const MaterialInstance* instance, int index)
{
	PX2_UNUSED(renderable);

	const MaterialPass* pass = instance->GetPass(index);
	ShaderParameters* vparams = instance->GetVertexParameters(index);
	ShaderParameters* pparams = instance->GetPixelParameters(index);
	VertexShader* vshader = pass->GetVertexShader();
	PixelShader* pshader = pass->GetPixelShader();

	PdrMaterialPass *pdrMaterialPass = 0;

	if (1 == (int)msRenderers.size())
	{
		pdrMaterialPass = (PdrMaterialPass*)pass->PdrPointer;
		pdrMaterialPass->Disable(this);
	}
	else
	{
		MaterialPassMap::iterator iter = mMaterialPasses.find(pass);
		if (iter != mMaterialPasses.end())
		{
			pdrMaterialPass = iter->second;
			pdrMaterialPass->Disable(this);
		}
	}

	Disable(vshader, vparams);
	Disable(pshader, pparams);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// RenderProperty management.
//----------------------------------------------------------------------------
void Renderer::SetAlphaProperty (const AlphaProperty* AlphaProperty)
{
	if (!mOverrideAlphaProperty)
	{
		mAlphaProperty = AlphaProperty;
	}
	else	{
		mAlphaProperty = mOverrideAlphaProperty;
	}

	if (mAlphaProperty->BlendEnabled)
	{
		if (!mData->mCurrentRS.mAlphaBlendEnabled)
		{
			mData->mCurrentRS.mAlphaBlendEnabled = true;
			glEnable(GL_BLEND);
		}

		GLenum srcBlend = gOGLAlphaSrcBlend[mAlphaProperty->SrcBlend];
		GLenum dstBlend = gOGLAlphaDstBlend[mAlphaProperty->DstBlend];
		if (srcBlend != mData->mCurrentRS.mAlphaSrcBlend
			||  dstBlend != mData->mCurrentRS.mAlphaDstBlend)
		{
			mData->mCurrentRS.mAlphaSrcBlend = srcBlend;
			mData->mCurrentRS.mAlphaDstBlend = dstBlend;
			glBlendFunc(srcBlend, dstBlend);
		}

		if (mAlphaProperty->ConstantColor != mData->mCurrentRS.mBlendColor)
		{
			mData->mCurrentRS.mBlendColor = mAlphaProperty->ConstantColor;
			glBlendColor(
				mData->mCurrentRS.mBlendColor[0],
				mData->mCurrentRS.mBlendColor[1],
				mData->mCurrentRS.mBlendColor[1],
				mData->mCurrentRS.mBlendColor[3]);
		}
	}
	else
	{
		if (mData->mCurrentRS.mAlphaBlendEnabled)
		{
			mData->mCurrentRS.mAlphaBlendEnabled = false;
			glDisable(GL_BLEND);
		}
	}

	if (mAlphaProperty->CompareEnabled)
	{
		// 在shader中支持
	}
	else
	{
		// 在shader中支持
	}
}
//----------------------------------------------------------------------------
void Renderer::SetCullProperty (const CullProperty* cullState)
{
	if (!mOverrideCullProperty)
	{
		mCullProperty = cullState;
	}
	else
	{
		mCullProperty = mOverrideCullProperty;
	}

	if (mCullProperty->Enabled)
	{
		if (!mData->mCurrentRS.mCullEnabled)
		{
			mData->mCurrentRS.mCullEnabled = true;
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CCW);
		}

		bool order = mCullProperty->CCWOrder;
		if (mReverseCullOrder)
		{
			order = !order;
		}

		if (order != mData->mCurrentRS.mCCWOrder)
		{
			mData->mCurrentRS.mCCWOrder = order;
			if (mData->mCurrentRS.mCCWOrder)
			{
				glCullFace(GL_BACK);
			}
			else
			{
				glCullFace(GL_FRONT);
			}
		}
	}
	else
	{
		if (mData->mCurrentRS.mCullEnabled)
		{
			mData->mCurrentRS.mCullEnabled = false;
			glDisable(GL_CULL_FACE);
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::SetDepthProperty (const DepthProperty* depthState)
{
	if (!mOverrideDepthProperty)
	{
		mDepthProperty = depthState;
	}
	else
	{
		mDepthProperty = mOverrideDepthProperty;
	}

	if (mDepthProperty->Enabled)
	{
		if (!mData->mCurrentRS.mDepthEnabled)
		{
			mData->mCurrentRS.mDepthEnabled = true;
			glEnable(GL_DEPTH_TEST);
		}

		GLenum compare = gOGLDepthCompare[mDepthProperty->Compare];
		if (compare != mData->mCurrentRS.mDepthCompareFunction)
		{
			mData->mCurrentRS.mDepthCompareFunction = compare;
			glDepthFunc(compare);
		}
	}
	else
	{
		if (mData->mCurrentRS.mDepthEnabled)
		{
			mData->mCurrentRS.mDepthEnabled = false;
			glDisable(GL_DEPTH_TEST);
		}
	}

	if (mDepthProperty->Writable)
	{
		if (!mData->mCurrentRS.mDepthWriteEnabled)
		{
			mData->mCurrentRS.mDepthWriteEnabled = true;
			glDepthMask(GL_TRUE);
		}
	}
	else
	{
		if (mData->mCurrentRS.mDepthWriteEnabled)
		{
			mData->mCurrentRS.mDepthWriteEnabled = false;
			glDepthMask(GL_FALSE);
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::SetOffsetProperty (const OffsetProperty* offsetState)
{
	if (!mOverrideOffsetProperty)
	{
		mOffsetProperty = offsetState;
	}
	else
	{
		mOffsetProperty = mOverrideOffsetProperty;
	}

	SetColorMask(mOffsetProperty->AllowGreen, mOffsetProperty->AllowBlue,
		mOffsetProperty->AllowGreen, mOffsetProperty->AllowAlpha);

	if (mOffsetProperty->FillEnabled)
	{
		if (!mData->mCurrentRS.mFillEnabled)
		{
			mData->mCurrentRS.mFillEnabled = true;
			glEnable(GL_POLYGON_OFFSET_FILL);
		}
	}
	else
	{
		if (mData->mCurrentRS.mFillEnabled)
		{
			mData->mCurrentRS.mFillEnabled = false;
			glDisable(GL_POLYGON_OFFSET_FILL);
		}
	}

	// LineEnabled，PointEnabled不支持

	if (mOffsetProperty->Scale != mData->mCurrentRS.mOffsetScale
		||  mOffsetProperty->Bias != mData->mCurrentRS.mOffsetBias)
	{
		mData->mCurrentRS.mOffsetScale = mOffsetProperty->Scale;
		mData->mCurrentRS.mOffsetBias = mOffsetProperty->Bias;
		glPolygonOffset(mOffsetProperty->Scale, mOffsetProperty->Bias);
	}
}
//----------------------------------------------------------------------------
void Renderer::SetStencilProperty (const StencilProperty* stencilProperty)
{
	if (!mOverrideStencilProperty)
	{
		mStencilProperty = stencilProperty;
	}
	else
	{
		mStencilProperty = mOverrideStencilProperty;
	}

	if (mStencilProperty->Enabled)
	{
		if (!mData->mCurrentRS.mStencilEnabled)
		{
			mData->mCurrentRS.mStencilEnabled = true;
			glEnable(GL_STENCIL_TEST);
		}

		GLenum compare = gOGLStencilCompare[mStencilProperty->Compare];
		if (compare != mData->mCurrentRS.mStencilCompareFunction
			||  mStencilProperty->Reference != mData->mCurrentRS.mStencilReference
			||  mStencilProperty->Mask != mData->mCurrentRS.mStencilMask)
		{
			mData->mCurrentRS.mStencilCompareFunction = compare;
			mData->mCurrentRS.mStencilReference = mStencilProperty->Reference;
			mData->mCurrentRS.mStencilMask = mStencilProperty->Mask;
			glStencilFunc(compare, mStencilProperty->Reference,
				mStencilProperty->Mask);
		}

		if (mStencilProperty->WriteMask != mData->mCurrentRS.mStencilWriteMask)
		{
			mData->mCurrentRS.mStencilWriteMask = mStencilProperty->WriteMask;
			glStencilMask(mStencilProperty->WriteMask);
		}

		GLenum onFail = gOGLStencilOperation[mStencilProperty->OnFail];
		GLenum onZFail = gOGLStencilOperation[mStencilProperty->OnZFail];
		GLenum onZPass = gOGLStencilOperation[mStencilProperty->OnZPass];
		if (onFail != mData->mCurrentRS.mStencilOnFail
			||  onZFail != mData->mCurrentRS.mStencilOnZFail
			||  onZPass != mData->mCurrentRS.mStencilOnZPass)
		{
			mData->mCurrentRS.mStencilOnFail = onFail;
			mData->mCurrentRS.mStencilOnZFail = onZFail;
			mData->mCurrentRS.mStencilOnZPass = onZPass;
			glStencilOp(onFail, onZFail, onZPass);
		}
	}
	else
	{
		if (mData->mCurrentRS.mStencilEnabled)
		{
			mData->mCurrentRS.mStencilEnabled = false;
			glDisable(GL_STENCIL_TEST);
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::SetWireProperty (const WireProperty* wireState)
{
	if (!mOverrideWireProperty)
	{
		mWireProperty = wireState;
	}
	else
	{
		mWireProperty = mOverrideWireProperty;
	}

	if (mWireProperty->Enabled)
	{
		if (!mData->mCurrentRS.mWireEnabled)
		{
			mData->mCurrentRS.mWireEnabled = true;

			// assertion
			//assertion(false, "OpenglES2 doesn't support.");
		}
	}
	else
	{
		if (mData->mCurrentRS.mWireEnabled)
		{
			mData->mCurrentRS.mWireEnabled = false;

			// assertion
			//assertion(false, "OpenglES2 doesn't support.");
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Viewport management.
//----------------------------------------------------------------------------
void Renderer::SetViewport (int xPosition, int yPosition, int width,
							int height)
{
	glViewport(xPosition, yPosition, width, height);
}
//----------------------------------------------------------------------------
void Renderer::GetViewport (int& xPosition, int& yPosition,
							int& width, int& height) const
{
	int param[4];
	glGetIntegerv(GL_VIEWPORT, param);

	xPosition = param[0];
	yPosition = param[1];
	width = param[2];
	height = param[3];
}
//----------------------------------------------------------------------------
void Renderer::SetDepthRange (float zMin, float zMax)
{
	PX2_UNUSED(zMin);
	PX2_UNUSED(zMax);

	assertion(false, "PowerSDK does't support.");
}
//----------------------------------------------------------------------------
void Renderer::GetDepthRange (float& zMin, float& zMax) const
{
	GLfloat param[2];
	glGetFloatv(GL_DEPTH_RANGE, param);

	zMin = (float)param[0];
	zMax = (float)param[1];
}
//----------------------------------------------------------------------------
void Renderer::Resize (int width, int height)
{
	mWidth = width;
	mHeight = height;

	int param[4];
	glGetIntegerv(GL_VIEWPORT, param);
	glViewport(param[0], param[1], width, height);
}
//----------------------------------------------------------------------------
void Renderer::ResizeWindow (int width, int height)
{
	PX2_UNUSED(width);
	PX2_UNUSED(height);

	mData->mCurrentRS.Initialize(mDefaultAlphaProperty, mDefaultCullProperty,
		mDefaultDepthProperty, mDefaultOffsetProperty, mDefaultStencilProperty,
		mDefaultWireProperty);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Support for clearing the color, depth, and stencil buffers.
//----------------------------------------------------------------------------
void Renderer::ClearColorBuffer ()
{
	glClearColor(mClearColor[0], mClearColor[1], mClearColor[2],
		mClearColor[3]);

	glClear(GL_COLOR_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void Renderer::ClearDepthBuffer ()
{
	glClearDepthf((GLclampf)mClearDepth);

	glClear(GL_DEPTH_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void Renderer::ClearStencilBuffer ()
{
	glClearStencil((GLint)mClearStencil);

	glClear(GL_STENCIL_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void Renderer::ClearBuffers ()
{
	glClearColor(mClearColor[0], mClearColor[1], mClearColor[2],
		mClearColor[3]);

	glClearDepthf((GLclampf)mClearDepth);

	glClearStencil((GLint)mClearStencil);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |	GL_STENCIL_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void Renderer::ClearColorBuffer (int x, int y, int w, int h)
{
	glClearColor(mClearColor[0], mClearColor[1], mClearColor[2],
		mClearColor[3]);

	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void Renderer::ClearDepthBuffer (int x, int y, int w, int h)
{
	glClearDepthf((GLclampf)mClearDepth);

	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void Renderer::ClearStencilBuffer (int x, int y, int w, int h)
{
	glClearStencil((GLint)mClearStencil);

	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
	glClear(GL_STENCIL_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void Renderer::ClearBuffers (int x, int y, int w, int h)
{
	glClearColor(mClearColor[0], mClearColor[1], mClearColor[2],
		mClearColor[3]);

	glClearDepthf(mClearDepth);

	glClearStencil((GLint)mClearStencil);

	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |	GL_STENCIL_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void Renderer::DisplayColorBuffer ()
{
#if defined(_WIN32) || defined(WIN32)  || defined (__LINUX__)
	 PX2_EGL_CHECK(eglSwapBuffers(mData->mDisplay, mData->mSurface));
#endif
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Support for masking color channels.
//----------------------------------------------------------------------------
void Renderer::SetColorMask (bool allowRed, bool allowGreen,
							 bool allowBlue, bool allowAlpha)
{
	mAllowRed = allowRed;
	mAllowGreen = allowGreen;
	mAllowBlue = allowBlue;
	mAllowAlpha = allowAlpha;

	GLboolean rMask = (mAllowRed ? 1 : 0);
	GLboolean gMask = (mAllowGreen ? 1 : 0);
	GLboolean bMask = (mAllowBlue ? 1 : 0);
	GLboolean aMask = (mAllowAlpha ? 1 : 0);
	glColorMask(rMask, gMask, bMask, aMask);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Support for predraw and postdraw semantics.
//----------------------------------------------------------------------------
bool Renderer::PreDraw ()
{
	mNumDrawPrimitivePerFrame = 0;

	return true;
}
//----------------------------------------------------------------------------
void Renderer::PostDraw ()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Drawing routines.
//----------------------------------------------------------------------------
void Renderer::Draw (const unsigned char* screenBuffer, bool reflectY)
{
	PX2_UNUSED(reflectY);

	if (!screenBuffer)
	{
		return;
	}

	assertion(false, "OpenglES does't support.");
}
//----------------------------------------------------------------------------
void Renderer::Draw (int x, int y, const Float4& color,
					 const std::string& message)
{
	PX2_UNUSED(x);
	PX2_UNUSED(y);
	PX2_UNUSED(color);
	PX2_UNUSED(message);

	//assertion(false, "OpenglES does't support.");
}
//----------------------------------------------------------------------------
#ifdef PX2_QUERY_PIXEL_COUNT

#else
	#define PX2_BEGIN_QUERY(query)
	#define PX2_END_QUERY(query, numPixelsDrawn)
#endif
//----------------------------------------------------------------------------
void Renderer::DrawPrimitive (const Renderable* renderable)
{
	++mNumDrawPrimitivePerFrame;

	Renderable::PrimitiveType type = renderable->GetPrimitiveType();
	const VertexBuffer* vbuffer = renderable->GetVertexBuffer();
	const IndexBuffer* ibuffer = renderable->GetIndexBuffer();
	
	int indicesOffset = 0;
	int ibOffset = renderable->GetIBOffset();
	if (0 != ibOffset)
	{
		indicesOffset = ibOffset;
	}
	else
	{
		if (ibuffer)
		{
			indicesOffset = ibuffer->GetOffset();
		}
	}

	int numVertices = 0;
	numVertices = vbuffer->GetNumElements();
	
	int numIndices = 0;
	int ibNumElements = renderable->GetIBNumElements();
	if (0 != ibNumElements)
	{
		numIndices = ibNumElements;
	}
	else
	{
		if (ibuffer)
		{
			numIndices = ibuffer->GetNumElements();
		}
	}

	if (type == Renderable::PT_TRIMESH
		||  type == Renderable::PT_TRISTRIP
		||  type == Renderable::PT_TRIFAN)
	{
		if (numVertices > 0 && numIndices > 0)
		{
			GLenum indexType;
			const GLvoid* indexData;
			if (ibuffer->GetElementSize() == 2)
			{
				indexType = GL_UNSIGNED_SHORT;
				indexData = (unsigned short*)0 + indicesOffset;
			}
			else  // size is 4
			{
				indexType = GL_UNSIGNED_INT;
				indexData = (unsigned int*)0 + indicesOffset;
			}

			PX2_GL_CHECK(glDrawElements(gOGLPrimitiveType[type], numIndices, indexType, indexData));
		}
	}
	else if (type == Renderable::PT_POLYSEGMENTS_CONTIGUOUS)
	{
		int numSegments = StaticCast<Polysegment>(renderable)->GetNumSegments();
		if (numSegments > 0)
		{
			glDrawArrays(GL_LINE_STRIP, 0, numSegments + 1);
		}
	}
	else if (type == Renderable::PT_POLYSEGMENTS_DISJOINT)
	{
		int numSegments = StaticCast<Polysegment>(renderable)->GetNumSegments();
		if (numSegments > 0)
		{
			PX2_GL_CHECK(glDrawArrays(GL_LINES, 0, 2*numSegments));
		}
	}
	else if (type == Renderable::PT_POLYPOINT)
	{
		int numPoints = StaticCast<Polypoint>(renderable)->GetNumPoints();
		if (numPoints > 0)
		{
			PX2_GL_CHECK(glDrawArrays(GL_POINTS, 0, numPoints));
		}
	}
	else
	{
		assertion(false, "Invalid type.\n");
	}
}
//----------------------------------------------------------------------------
void Renderer::ClearSharePdrData ()
{
	std::map<int, SharePdrData>::iterator it = mSharePdrMaterialPasses.begin();
	for (; it!=mSharePdrMaterialPasses.end(); it++)
	{
		glDeleteProgram(it->second.DataInt);
	}

	mSharePdrMaterialPasses.clear();
}
//----------------------------------------------------------------------------

#endif
