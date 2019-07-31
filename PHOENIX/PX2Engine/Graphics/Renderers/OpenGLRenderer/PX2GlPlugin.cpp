// PX2GlPlugin.cpp

#ifdef PX2_USE_OPENGL

#include "PX2GlExtensions.hpp"
#include <cassert>
#include <cstdio>

// Disable the MSVC8 warnings about deprecated functions.
#pragma warning( push )
#pragma warning( disable : 4996 )

//----------------------------------------------------------------------------
void ReportNullFunction(const char*)
{
	assert(false);
}
//----------------------------------------------------------------------------
void ReportGLError(const char* acFunction)
{
	GLenum code = glGetError();
	while (code != GL_NO_ERROR)
	{
		const char* errorString = GTGetErrorString(code);
		(void)errorString;
		assert(false);
		code = glGetError();
	}
	(void)acFunction;
}
//----------------------------------------------------------------------------

#pragma warning( pop )

#endif
