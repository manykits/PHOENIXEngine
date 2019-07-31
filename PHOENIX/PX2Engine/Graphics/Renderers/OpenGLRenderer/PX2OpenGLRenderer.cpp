// PX2OpenGLRenderer.cpp

#ifdef PX2_USE_OPENGL

#include "PX2Renderer.hpp"
#include "PX2OpenGLBitmapFont.hpp"
#include "PX2OpenGLMapping.hpp"
#include "PX2OpenGLRendererData.hpp"
#include "PX2Polypoint.hpp"
#include "PX2Polysegment.hpp"
#include "PX2TriFan.hpp"
#include "PX2TriStrip.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2OpenGLRendererInput.hpp"
#include "PX2Log.hpp"
using namespace PX2;

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include "PX2WglExtensions.hpp"

#elif defined(__LINUX__)
#include "PX2GlExtensions.hpp"
#endif

//----------------------------------------------------------------------------
Renderer::Renderer(RendererInput *input, int width, int height,
	Texture::Format colorFormat, Texture::Format depthStencilFormat,
	int numMultisamples)
{
	Initialize(width, height, colorFormat, depthStencilFormat,
		numMultisamples);

	mData = new0 RendererData();

	PdrRendererInput *pdrRenderInput = (PdrRendererInput *)(input);

#if defined(_WIN32) || defined(WIN32)
	mData->mWindowHandle = pdrRenderInput->mWindowHandle;
	mData->mWindowDC = ::GetDC(pdrRenderInput->mWindowHandle);

	// The application layer needs to know the device context in case it has
	// to recreate a window to support multisampling.
	pdrRenderInput->mRendererDC = mData->mWindowDC;

	// Select the format for the drawing surface.
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags =
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_GENERIC_ACCELERATED;
	pfd.dwFlags |= PFD_DOUBLEBUFFER;

	// colorFormat is ignored, create 32-bit RGBA buffer.
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	// depthStencilFormat is ignored, create 24-8 depthstencil buffer.
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	// Set the pixel format for the rendering context.
	bool firstTime = (pdrRenderInput->mPixelFormat == 0 ? true : false);
	if (firstTime)
	{
		// First time creation.  If multisampling is requested, a second
		// creation will occur and the input pixel format will be supplied
		// directly to SetPixelFormat.
		pdrRenderInput->mPixelFormat = ChoosePixelFormat(mData->mWindowDC, &pfd);
		if (pdrRenderInput->mPixelFormat == 0)
		{
			assertion(false, "ChoosePixelFormat failed\n");
			return;
		}
	}

	BOOL success = SetPixelFormat(mData->mWindowDC, pdrRenderInput->mPixelFormat, &pfd);
	if (!success)
	{
		assertion(false, "SetPixelFormat failed\n");
		return;
	}

	// Create an OpenGL context.
	mData->mWindowRC = wglCreateContext(mData->mWindowDC);
	if (!mData->mWindowRC)
	{
		assertion(false, "wglCreateContext failed\n");
		return;
	}

	// Activate the context.
	success = wglMakeCurrent(mData->mWindowDC, mData->mWindowRC);
	if (!success)
	{
		assertion(false, "wglMakeCurrent failed\n");
		return;
	}

	if (firstTime)
	{
		// Load the function pointers for the OpenGL extensions.  This only
		// needs to occur once.  If you want a file to be created that stores
		// the OpenGL driver information, change the input filename to a
		// valid string; for example, "OpenGLDriverInfo.txt".
		//const char* infoFile = "OpenGLDriverInfo.txt";
		const char* infoFile = 0;
		GTInitWGL(infoFile);
	}

	if (pdrRenderInput->mDisableVerticalSync)
	{
		// Disable the wait for vertical sync.
		wglSwapIntervalEXT(0);
	}


#ifdef PX2_USE_TEXT_DISPLAY_LIST
	// Use the system font as default.
	int firstChar = int(' ');
	mData->mFont.mQuantity = 127 - firstChar;
	mData->mFont.mStart = glGenLists(mData->mFont.mQuantity);
	mData->mFont.mBase = mData->mFont.mStart - firstChar;
	SelectObject(mData->mWindowDC, GetStockObject(SYSTEM_FONT));
	wglUseFontBitmaps(mData->mWindowDC, firstChar, mData->mFont.mQuantity,
		mData->mFont.mStart);
#endif

#elif defined (__LINUX__)

	mData->mDisplay = pdrRenderInput->mDisplay;
	mData->mWindow = pdrRenderInput->mWindow;
	mData->mVisual = pdrRenderInput->mVisual;

	// Create an OpenGL rendering context.
	GLXContext sharedList = 0;
	Bool directRender = True;
	mData->mContext = glXCreateContext(mData->mDisplay, mData->mVisual,
		sharedList, directRender);
	pdrRenderInput->mContext = mData->mContext;
	if (!mData->mContext)
	{
		mData->mDisplay = 0;
		mData->mVisual = 0;
		mData->mContext = 0;

		PX2_LOG_ERROR("glXCreateContext");

		return;
	}

	if (!glXMakeCurrent(mData->mDisplay, mData->mWindow, mData->mContext))
	{
		PX2_LOG_ERROR("glXMakeCurrent");

		return;
	}

	// Load the function pointers for the OpenGL extensions.  This only
	// needs to occur once.  If you want a file to be created that stores
	// the OpenGL driver information, change the input filename to a
	// valid string; for example, "OpenGLDriverInfo.txt".
	const char* infoFile = 0;
	GTInitGLX(infoFile);

#endif

	// Select the shader profiles.
	VertexShader::SetProfile(VertexShader::VP_ARBVP1);
	PixelShader::SetProfile(PixelShader::PP_ARBFP1);

	// Query the device for its capabilities.  TODO:  The number of images
	// turns out to be larger than what Shader Model 3 supports.  Why is this?
	mData->mMaxVShaderImages = 0;
	mData->mMaxPShaderImages = 0;
	mData->mMaxCombinedImages = 0;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		&mData->mMaxVShaderImages);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,
		&mData->mMaxPShaderImages);
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		&mData->mMaxCombinedImages);

	// Set the default render states.
	mData->mCurrentRS.Initialize(mDefaultAlphaProperty, mDefaultCullProperty,
		mDefaultDepthProperty, mDefaultOffsetProperty, mDefaultStencilProperty,
		mDefaultWireProperty);

}
//----------------------------------------------------------------------------
Renderer::~Renderer()
{

#ifdef PX2_USE_TEXT_DISPLAY_LIST
	glDeleteLists(mData->mFont.mStart, mData->mFont.mQuantity);
#endif

#if defined(_WIN32) || defined(WIN32)
	if (mData->mWindowRC)
	{
		wglDeleteContext(mData->mWindowRC);
	}

	if (mData->mWindowDC)
	{
		::ReleaseDC(mData->mWindowHandle, mData->mWindowDC);
	}
#elif defined(__LINUX__)
	if (mData->mDisplay && mData->mContext)
	{
		glXDestroyContext(mData->mDisplay, mData->mContext);
	}
#endif

	delete0(mData);
	Terminate();
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
	pdrRenderInput->mRendererDC = ::GetDC(hWnd);
	PX2_UNUSED(ptDataLinuxWindow);
	PX2_UNUSED(ptDataLinuxVisual);

#elif defined (__LINUX__)
	pdrRenderInput->mDisplay = (Display*)ptData;
	pdrRenderInput->mWindow = *((Window*)ptDataLinuxWindow);
	pdrRenderInput->mVisual = (XVisualInfo*)ptDataLinuxVisual;
#endif

	renderer = new0 Renderer(pdrRenderInput, width, height,
		colorFormat, depthStencilFormat, numMultisamples);

	renderer->SetClearColor(Float4::WHITE);

	PX2_LOG_INFO("End Create Renderer.");

	return renderer;
}
//----------------------------------------------------------------------------
void Renderer::DisplayColorBuffer()
{
#if defined(_WIN32) || defined(WIN32)
	SwapBuffers(mData->mWindowDC);
#elif defined(__LINUX__)
	glXSwapBuffers(mData->mDisplay, mData->mWindow);
#endif
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Renderable state management.
//----------------------------------------------------------------------------
void Renderer::SetAlphaProperty(const AlphaProperty* alphaState)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (!mOverrideAlphaProperty)
	{
		mAlphaProperty = alphaState;
	}
	else
	{
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
			|| dstBlend != mData->mCurrentRS.mAlphaDstBlend)
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
				mData->mCurrentRS.mBlendColor[2],
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
		if (!mData->mCurrentRS.mAlphaCompareEnabled)
		{
			mData->mCurrentRS.mAlphaCompareEnabled = true;
			glEnable(GL_ALPHA_TEST);
		}

		GLenum compare = gOGLAlphaCompare[mAlphaProperty->Compare];
		float reference = mAlphaProperty->Reference;
		if (compare != mData->mCurrentRS.mCompareFunction
			|| reference != mData->mCurrentRS.mAlphaReference)
		{
			mData->mCurrentRS.mCompareFunction = compare;
			mData->mCurrentRS.mAlphaReference = reference;
			glAlphaFunc(compare, reference);
		}
	}
	else
	{
		if (mData->mCurrentRS.mAlphaCompareEnabled)
		{
			mData->mCurrentRS.mAlphaCompareEnabled = false;
			glDisable(GL_ALPHA_TEST);
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::SetCullProperty(const CullProperty* cullState)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

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
void Renderer::SetDepthProperty(const DepthProperty* depthState)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

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
void Renderer::SetOffsetProperty(const OffsetProperty* offsetState)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (!mOverrideOffsetProperty)
	{
		mOffsetProperty = offsetState;
	}
	else
	{
		mOffsetProperty = mOverrideOffsetProperty;
	}

	SetColorMask(offsetState->AllowGreen, offsetState->AllowBlue,
		offsetState->AllowGreen, offsetState->AllowAlpha);

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

	if (mOffsetProperty->LineEnabled)
	{
		if (!mData->mCurrentRS.mLineEnabled)
		{
			mData->mCurrentRS.mLineEnabled = true;
			glEnable(GL_POLYGON_OFFSET_LINE);
		}
	}
	else
	{
		if (mData->mCurrentRS.mLineEnabled)
		{
			mData->mCurrentRS.mLineEnabled = false;
			glDisable(GL_POLYGON_OFFSET_LINE);
		}
	}

	if (mOffsetProperty->PointEnabled)
	{
		if (!mData->mCurrentRS.mPointEnabled)
		{
			mData->mCurrentRS.mPointEnabled = true;
			glEnable(GL_POLYGON_OFFSET_POINT);
		}
	}
	else
	{
		if (mData->mCurrentRS.mPointEnabled)
		{
			mData->mCurrentRS.mPointEnabled = false;
			glDisable(GL_POLYGON_OFFSET_POINT);
		}
	}

	if (mOffsetProperty->Scale != mData->mCurrentRS.mOffsetScale
		|| mOffsetProperty->Bias != mData->mCurrentRS.mOffsetBias)
	{
		mData->mCurrentRS.mOffsetScale = mOffsetProperty->Scale;
		mData->mCurrentRS.mOffsetBias = mOffsetProperty->Bias;
		glPolygonOffset(mOffsetProperty->Scale, mOffsetProperty->Bias);
	}
}
//----------------------------------------------------------------------------
void Renderer::SetStencilProperty(const StencilProperty* stencilState)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (!mOverrideStencilProperty)
	{
		mStencilProperty = stencilState;
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
			|| mStencilProperty->Reference != mData->mCurrentRS.mStencilReference
			|| mStencilProperty->Mask != mData->mCurrentRS.mStencilMask)
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
			|| onZFail != mData->mCurrentRS.mStencilOnZFail
			|| onZPass != mData->mCurrentRS.mStencilOnZPass)
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
void Renderer::SetWireProperty(const WireProperty* wireState)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

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
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
	else
	{
		if (mData->mCurrentRS.mWireEnabled)
		{
			mData->mCurrentRS.mWireEnabled = false;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Pass 管理
//----------------------------------------------------------------------------
void Renderer::Bind(const MaterialPass *pass)
{
	PX2_UNUSED(pass);
}
//----------------------------------------------------------------------------
void Renderer::BindAll(const MaterialPass *pass)
{
	PX2_UNUSED(pass);
}
//----------------------------------------------------------------------------
void Renderer::Unbind(const MaterialPass *pass)
{
	PX2_UNUSED(pass);
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll(const MaterialPass *pass)
{
	PX2_UNUSED(pass);
}
//----------------------------------------------------------------------------
void Renderer::Enable(const Renderable* renderable,
	const MaterialInstance* instance, int index)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	const MaterialPass* pass = instance->GetPass(index);
	ShaderParameters* vparams = instance->GetVertexParameters(index);
	ShaderParameters* pparams = instance->GetPixelParameters(index);
	VertexShader* vshader = pass->GetVertexShader();
	PixelShader* pshader = pass->GetPixelShader();

	// 更新着色器常量
	ShaderStruct struc;
	struc.TheRenderable = (Renderable*)renderable;
	struc.TheCamera = (Camera*)mCamera;
	struc.TheEnvirParam = PX2_GR.GetCurEnvirParamController();

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
	Enable(vshader, vparams);
	Enable(pshader, pparams);
}
//----------------------------------------------------------------------------
void Renderer::Disable(const Renderable* renderable,
	const MaterialInstance* instance, int index)
{
	PX2_UNUSED(renderable);

	const MaterialPass* pass = instance->GetPass(index);
	ShaderParameters* vparams = instance->GetVertexParameters(index);
	ShaderParameters* pparams = instance->GetPixelParameters(index);
	VertexShader* vshader = pass->GetVertexShader();
	PixelShader* pshader = pass->GetPixelShader();

	// 取消激活着色器
	Disable(vshader, vparams);
	Disable(pshader, pparams);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Viewport management.
//----------------------------------------------------------------------------
void Renderer::SetViewport(int xPosition, int yPosition, int width,
	int height)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	glViewport(xPosition, yPosition, width, height);
}
//----------------------------------------------------------------------------
void Renderer::GetViewport(int& xPosition, int& yPosition,
	int& width, int& height) const
{
#if defined PX2_USE_OPENGL && (defined WIN32||defined _WIN32)
	if (Renderer::msRenderers.size() > 1)
	{
		bool success = wglMakeCurrent(mData->mWindowDC, mData->mWindowRC);
	}
#endif

	int param[4];
	glGetIntegerv(GL_VIEWPORT, param);

	xPosition = param[0];
	yPosition = param[1];
	width = param[2];
	height = param[3];
}
//----------------------------------------------------------------------------
void Renderer::SetDepthRange(float zMin, float zMax)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	glDepthRange((GLclampd)zMin, (GLclampd)zMax);
}
//----------------------------------------------------------------------------
void Renderer::GetDepthRange(float& zMin, float& zMax) const
{
#if defined PX2_USE_OPENGL && (defined WIN32||defined _WIN32)
	if (Renderer::msRenderers.size() > 1)
	{
		bool success = wglMakeCurrent(mData->mWindowDC, mData->mWindowRC);
	}
#endif

	GLclampd param[2];
	glGetDoublev(GL_DEPTH_RANGE, param);

	zMin = (float)param[0];
	zMax = (float)param[1];
}
//----------------------------------------------------------------------------
void Renderer::Resize(int width, int height)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	mWidth = width;
	mHeight = height;

	int param[4];
	glGetIntegerv(GL_VIEWPORT, param);
	glViewport(param[0], param[1], width, height);
}
//----------------------------------------------------------------------------
void Renderer::ResizeWindow(int width, int height)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PX2_UNUSED(width);
	PX2_UNUSED(height);

	mData->mCurrentRS.Initialize(mDefaultAlphaProperty, mDefaultCullProperty,
		mDefaultDepthProperty, mDefaultOffsetProperty, mDefaultStencilProperty,
		mDefaultWireProperty);
}
//----------------------------------------------------------------------------
std::string Renderer::GetRenderTag()
{
	return "OpenGL";
}
//----------------------------------------------------------------------------
void Renderer::InitRenderStates()
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	mData->mCurrentRS.Initialize(mDefaultAlphaProperty, mDefaultCullProperty,
		mDefaultDepthProperty, mDefaultOffsetProperty, mDefaultStencilProperty,
		mDefaultWireProperty);

	for (int i = 0; i < RendererData::MAX_NUM_PSAMPLERS; i++)
	{
		mData->mCurrentSS[i] = RendererData::SamplerState();
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Support for clearing the color, depth, and stencil buffers.
//----------------------------------------------------------------------------
void Renderer::ClearColorBuffer()
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	glClearColor(mClearColor[0], mClearColor[1], mClearColor[2],
		mClearColor[3]);

	glClear(GL_COLOR_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void Renderer::ClearDepthBuffer()
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	glClearDepth((GLclampd)mClearDepth);

	glClear(GL_DEPTH_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void Renderer::ClearStencilBuffer()
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	glClearStencil((GLint)mClearStencil);

	glClear(GL_STENCIL_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void Renderer::ClearBuffers()
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	glClearColor(mClearColor[0], mClearColor[1], mClearColor[2],
		mClearColor[3]);

	glClearDepth((GLclampd)mClearDepth);

	glClearStencil((GLint)mClearStencil);

	glClear(
		GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
}
//----------------------------------------------------------------------------
void Renderer::ClearColorBuffer(int x, int y, int w, int h)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	glClearColor(mClearColor[0], mClearColor[1], mClearColor[2],
		mClearColor[3]);

	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void Renderer::ClearDepthBuffer(int x, int y, int w, int h)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	glClearDepth((GLclampd)mClearDepth);

	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void Renderer::ClearStencilBuffer(int x, int y, int w, int h)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	glClearStencil((GLint)mClearStencil);

	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
	glClear(GL_STENCIL_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------
void Renderer::ClearBuffers(int x, int y, int w, int h)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	glClearColor(mClearColor[0], mClearColor[1], mClearColor[2],
		mClearColor[3]);

	glClearDepth((GLclampd)mClearDepth);

	glClearStencil((GLint)mClearStencil);

	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, w, h);
	glClear(
		GL_COLOR_BUFFER_BIT |
		GL_DEPTH_BUFFER_BIT |
		GL_STENCIL_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Support for masking color channels.
//----------------------------------------------------------------------------
void Renderer::SetColorMask(bool allowRed, bool allowGreen,
	bool allowBlue, bool allowAlpha)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

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
bool Renderer::PreDraw()
{
#if defined _WIN32 || defined WIN32
	if (Renderer::msRenderers.size() > 1)
	{
		bool success = wglMakeCurrent(mData->mWindowDC, mData->mWindowRC);
		if (!success)
		{
			assertion(false, "");
		}
	}
#endif

	return true;
}
//----------------------------------------------------------------------------
void Renderer::PostDraw()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Drawing routines.
//----------------------------------------------------------------------------
void Renderer::Draw(const unsigned char* screenBuffer, bool reflectY)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (!screenBuffer)
	{
		assertion(false, "Incoming screen buffer is null.\n");
		return;
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, (double)mWidth, 0.0, (double)mHeight, 0.0, 1.0);
	glRasterPos3f(0.0, 0.0, 0.0);
	if (!reflectY)
	{
		// Set raster position to window coord (0,H-1).  The hack here avoids
		// problems with invalid raster positions which would cause
		// glDrawPixels not to execute.  OpenGL uses right-handed screen
		// coordinates, so using (0,H-1) as the raster position followed by
		// glPixelZoom(1,-1) tells OpenGL to draw the screen in left-handed
		// coordinates starting at the top row of the screen and finishing
		// at the bottom row.
		GLubyte bitmap[1] = { 0 };
		glBitmap(0, 0, 0.0f, 0.0f, 0.0f, (float)mHeight, bitmap);
	}
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	if (!reflectY)
	{
		glPixelZoom(1.0f, -1.0f);
	}

	glDrawPixels(mWidth, mHeight, GL_BGRA, GL_UNSIGNED_BYTE, screenBuffer);

	if (!reflectY)
	{
		glPixelZoom(1.0f, 1.0f);
	}
}
//----------------------------------------------------------------------------
void Renderer::Draw(int x, int y, const Float4& color,
	const std::string& message)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	const char* text = message.c_str();
	assertion(text != 0, "Invalid message\n");
	int length = (int)strlen(text);

#ifdef PX2_USE_TEXT_DISPLAY_LIST
	GLint listBase;
	glGetIntegerv(GL_LIST_BASE, &listBase);
#endif

	// Switch to orthogonal view.
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-0.5f, mWidth - 0.5f, -0.5f, mHeight - 0.5f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// Set default render states, except for depth buffering that must be
	// disabled because text is always overlayed.
	SetAlphaProperty(mDefaultAlphaProperty);
	SetCullProperty(mDefaultCullProperty);
	SetOffsetProperty(mDefaultOffsetProperty);
	SetStencilProperty(mDefaultStencilProperty);
	SetWireProperty(mDefaultWireProperty);

	mData->mCurrentRS.mDepthEnabled = false;
	glDisable(GL_DEPTH_TEST);

	// Set the text color.
	glColor4fv((const float*)color);

	// Draw the text string (use right-handed coordinates).
	glRasterPos3i(x, mHeight - 1 - y, 0);
#ifdef PX2_USE_TEXT_DISPLAY_LIST
	glListBase(mData->mFont.mBase);
	glCallLists(length, GL_UNSIGNED_BYTE, text);
#else
	for (int i = 0; i < length; ++i)
	{
		mData->DrawCharacter(gVerdanaS16B0I0, text[i]);
	}
#endif

#ifdef PX2_RESET_STATE_AFTER_DRAW
	// Restore visual state.  Only depth buffering state varied from the
	// default state.
	mData->mCurrentRS.mDepthEnabled = true;
	glEnable(GL_DEPTH_TEST);
#endif

	// Restore matrices.
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

#ifdef PX2_USE_TEXT_DISPLAY_LIST
	glListBase(listBase);
#endif
}
//----------------------------------------------------------------------------
#ifdef PX2_QUERY_PIXEL_COUNT
static GLuint BeginQuery()
{
	GLuint query = 0;
	glGenQueriesARB(1, &query);
	glBeginQueryARB(GL_SAMPLES_PASSED_ARB, query);
	return query;
}

static GLuint EndQuery(GLuint query)
{
	glEndQueryARB(GL_SAMPLES_PASSED_ARB);
	GLint available = 0;
	while (!available)
	{
		// Wait until query finishes.
		glGetQueryObjectivARB(query, GL_QUERY_RESULT_AVAILABLE_ARB,
			&available);
	}

	GLuint numPixelsDrawn = 0;
	glGetQueryObjectuivARB(query, GL_QUERY_RESULT_ARB, &numPixelsDrawn);
	glDeleteQueriesARB(1, &query);
	return numPixelsDrawn;
}

#define PX2_BEGIN_QUERY(query) \
    GLuint query = BeginQuery()

#define PX2_END_QUERY(query, numPixelsDrawn) \
    numPixelsDrawn = EndQuery(query)
#else
#define PX2_BEGIN_QUERY(query)
#define PX2_END_QUERY(query, numPixelsDrawn)
#endif
//----------------------------------------------------------------------------
void Renderer::DrawPrimitive(const Renderable* visual)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Renderable::PrimitiveType type = visual->GetPrimitiveType();
	const VertexBuffer* vbuffer = visual->GetVertexBuffer();
	const IndexBuffer* ibuffer = visual->GetIndexBuffer();

	GLuint numPixelsDrawn;
	PX2_UNUSED(numPixelsDrawn);

	if (type == Renderable::PT_TRIMESH
		|| type == Renderable::PT_TRISTRIP
		|| type == Renderable::PT_TRIFAN)
	{
		int numVertices = vbuffer->GetNumElements();
		int numIndices = ibuffer->GetNumElements();
		if (numVertices > 0 && numIndices > 0)
		{
			GLenum indexType;
			const GLvoid* indexData;
			if (ibuffer->GetElementSize() == 2)
			{
				indexType = GL_UNSIGNED_SHORT;
				indexData = (unsigned short*)0 + ibuffer->GetOffset();
			}
			else  // size is 4
			{
				indexType = GL_UNSIGNED_INT;
				indexData = (unsigned int*)0 + ibuffer->GetOffset();
			}

			PX2_BEGIN_QUERY(query);

			glDrawRangeElements(gOGLPrimitiveType[type], 0, numVertices - 1,
				numIndices, indexType, indexData);

			PX2_END_QUERY(query, numPixelsDrawn);
		}
	}
	else if (type == Renderable::PT_POLYSEGMENTS_CONTIGUOUS)
	{
		int numSegments = StaticCast<Polysegment>(visual)->GetNumSegments();
		if (numSegments > 0)
		{
			PX2_BEGIN_QUERY(query);

			glDrawArrays(GL_LINE_STRIP, 0, numSegments + 1);

			PX2_END_QUERY(query, numPixelsDrawn);
		}
	}
	else if (type == Renderable::PT_POLYSEGMENTS_DISJOINT)
	{
		int numSegments = StaticCast<Polysegment>(visual)->GetNumSegments();
		if (numSegments > 0)
		{
			PX2_BEGIN_QUERY(query);

			glDrawArrays(GL_LINES, 0, 2 * numSegments);

			PX2_END_QUERY(query, numPixelsDrawn);
		}
	}
	else if (type == Renderable::PT_POLYPOINT)
	{
		int numPoints = StaticCast<Polypoint>(visual)->GetNumPoints();
		if (numPoints > 0)
		{
			PX2_BEGIN_QUERY(query);

			glDrawArrays(GL_POINTS, 0, numPoints);

			PX2_END_QUERY(query, numPixelsDrawn);
		}
	}
	else
	{
		assertion(false, "Invalid type.\n");
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SharePdrData
//----------------------------------------------------------------------------
void Renderer::ClearSharePdrData()
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	std::map<int, SharePdrData>::iterator it = mSharePdrVertexShaders.begin();
	for (; it != mSharePdrVertexShaders.end(); it++)
	{
		GLuint prog = it->second.DataInt;
		glDeleteProgramsARB(1, &prog);
	}

	mSharePdrVertexShaders.clear();

	std::map<int, SharePdrData>::iterator itPS = mSharePdrPixelShaders.begin();
	for (; itPS != mSharePdrPixelShaders.end(); itPS++)
	{
		GLuint prog = itPS->second.DataInt;
		glDeleteProgramsARB(1, &prog);
	}

	mSharePdrPixelShaders.clear();
}
//----------------------------------------------------------------------------

#endif