// PX2OpenGLESTextureCube.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESTextureCube.hpp"
#include "PX2OpenGLESMapping.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrTextureCube::PdrTextureCube (Renderer* renderer,
	const TextureCube* texture)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(texture);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------
PdrTextureCube::~PdrTextureCube ()
{
}
//----------------------------------------------------------------------------
void PdrTextureCube::Enable (Renderer* renderer, int textureUnit)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(textureUnit);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------
void PdrTextureCube::Disable (Renderer* renderer, int textureUnit)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(textureUnit);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------
void* PdrTextureCube::Lock (int face, int level, Buffer::Locking mode)
{
	PX2_UNUSED(face);
	PX2_UNUSED(level);
	PX2_UNUSED(mode);

	assertion(false, "not realized.");

	return 0;
}
//----------------------------------------------------------------------------
void PdrTextureCube::Unlock (int face, int level)
{
	PX2_UNUSED(face);
	PX2_UNUSED(level);

	assertion(false, "not realized.");
}
//----------------------------------------------------------------------------

#endif
