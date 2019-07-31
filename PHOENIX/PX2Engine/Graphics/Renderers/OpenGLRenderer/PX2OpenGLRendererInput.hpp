// PX2OpenGLRendererInput.hpp

#ifdef PX2_USE_OPENGL

#ifndef PX2OPENGLRENDERERINPUT_HPP
#define PX2OPENGLRENDERERINPUT_HPP

#include "PX2OpenGLRendererPre.hpp"
#include "PX2RendererInput.hpp"

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#elif defined(__LINUX__)
#include "PX2GlxExtensions.hpp"
#endif

namespace PX2
{

	class PdrRendererInput : public RendererInput
	{
	public:
		PdrRendererInput();
		virtual ~PdrRendererInput();

		virtual bool Terminate();

#if defined(_WIN32) || defined(WIN32)
		HWND mWindowHandle;
		HDC mRendererDC;
#elif defined (__LINUX__)
		Display* mDisplay;
		Window mWindow;
		XVisualInfo* mVisual;
		GLXContext mContext;
#endif
		int mPixelFormat;
		bool mDisableVerticalSync;
	};

}

#endif

#endif