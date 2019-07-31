// PX2OpenGLESTexture1D.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESTexture1D.hpp"
#include "PX2OpenGLESMapping.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrTexture1D::PdrTexture1D (Renderer* renderer, const Texture1D* texture)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(texture);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------
PdrTexture1D::~PdrTexture1D ()
{
}
//----------------------------------------------------------------------------
void PdrTexture1D::Enable (Renderer* renderer, int textureUnit)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(textureUnit);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------
void PdrTexture1D::Disable (Renderer* renderer, int textureUnit)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(textureUnit);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------
void* PdrTexture1D::Lock (int level, Buffer::Locking mode)
{
	PX2_UNUSED(level);
	PX2_UNUSED(mode);

	assertion(false, "not realized.");

	return 0;
}
//----------------------------------------------------------------------------
void PdrTexture1D::Unlock (int level)
{
	PX2_UNUSED(level);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------

#endif

