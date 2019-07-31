// PX2OpenGLESRendererPre.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2RENDERERPRE_HPP
#define PX2OPENGLES2RENDERERPRE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Assert.hpp"

#if defined(_WIN32) || defined(WIN32)

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#elif defined (__ANDROID__)

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#elif defined (__IOS__)

#if defined (PX2_USE_OPENGLES3)

#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#else

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#endif

#else

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#endif

enum AttribLocationPos
{
	ALP_POSITION = 0,
	ALP_NORMAL,
	ALP_COLOR0,
	ALP_TEXCOORD0,
	ALP_TEXCOORD1,
	ALP_TEXCOORD2,
	ALP_TEXCOORD3,
	ALP_TEXCOORD4
};

#if defined(PX2_USE_GL_CHECK)

#define PX2_GL_CHECK(x) \
	x; \
		{ \
		GLenum glError = glGetError(); \
		if(glError != GL_NO_ERROR) { \
		assertion(false, "glGetError() = %i (0x%.8x) at line %i\n", glError, glError, __LINE__); \
		} \
		}

#define PX2_EGL_CHECK(x) \
	x; \
	{ \
	EGLint eglError = eglGetError(); \
	if(eglError != EGL_SUCCESS) { \
	assertion(false, "eglGetError() = %i (0x%.8x) at line %i\n", eglError, eglError, __LINE__); \
	} \
	}

#else

#define PX2_GL_CHECK(x) x;

#define PX2_EGL_CHECK(x) x;

#endif

//----------------------------------------------------------------------------
static bool IsGLExtensionSupported(const char * const extension)
{
	// The recommended technique for querying OpenGL extensions;
	// from http://opengl.org/resources/features/OGLextensions/
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *)strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;

	extensions = glGetString(GL_EXTENSIONS);

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings, etc. */
	start = extensions;
	for (;;) {
		where = (GLubyte *)strstr((const char *)start, extension);
		if (!where)
			break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return true;
		start = terminator;
	}

	return false;
}
//----------------------------------------------------------------------------

#endif

#endif