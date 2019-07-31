// PX2OpenGLRenderInput.cpp

#ifdef PX2_USE_OPENGL

#include "PX2OpenGLRendererInput.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrRendererInput::PdrRendererInput()
{
#if defined(_WIN32) || defined(WIN32)
	mWindowHandle = 0;
	mRendererDC = 0;
#elif defined (__LINUX__)
	mDisplay = 0;
	mVisual = 0;
	mContext = 0;
#endif

	mPixelFormat = 0;
	mDisableVerticalSync = true;
}
//----------------------------------------------------------------------------
PdrRendererInput::~PdrRendererInput()
{
}
//----------------------------------------------------------------------------
bool PdrRendererInput::Terminate()
{
	return true;
}
//----------------------------------------------------------------------------

#endif