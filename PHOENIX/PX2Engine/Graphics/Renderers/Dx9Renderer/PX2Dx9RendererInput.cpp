// PX2Dx9RendererInput.cpp

#ifdef PX2_USE_DX9

#include "PX2Dx9RendererInput.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrRendererInput::PdrRendererInput() :
mDriver(0)
{
}
//----------------------------------------------------------------------------
PdrRendererInput::~PdrRendererInput()
{
}
//----------------------------------------------------------------------------
bool PdrRendererInput::Terminate()
{
	if (mDriver)
		mDriver->Release();

	mDriver = 0;

	return true;
}
//----------------------------------------------------------------------------

#endif