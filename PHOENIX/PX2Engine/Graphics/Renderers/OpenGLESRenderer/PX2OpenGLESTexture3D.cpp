// PX2OpenGLESTexture3D.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESTexture3D.hpp"
#include "PX2OpenGLESMapping.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrTexture3D::PdrTexture3D (Renderer* renderer, const Texture3D* texture)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(texture);
	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------
PdrTexture3D::~PdrTexture3D ()
{
}
//----------------------------------------------------------------------------
void PdrTexture3D::Enable (Renderer* renderer, int textureUnit)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(textureUnit);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------
void PdrTexture3D::Disable (Renderer* renderer, int textureUnit)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(textureUnit);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------
void* PdrTexture3D::Lock (int level, Buffer::Locking mode)
{
	PX2_UNUSED(level);
	PX2_UNUSED(mode);

	assertion(false, "not realized.");

	return 0;
}
//----------------------------------------------------------------------------
void PdrTexture3D::Unlock (int level)
{
	PX2_UNUSED(level);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------

#endif
