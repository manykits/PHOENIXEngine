// PX2MathHelp.hpp

#include "PX2MathHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Float3 MathHelp::Float4ToFloat3(const Float4 &from)
{
	return Float3(from[0], from[1], from[2]);
}
//----------------------------------------------------------------------------
Float4 MathHelp::Float3ToFloat4(const Float3 &from, float w)
{
	return Float4(from[0], from[1], from[2], w);
}
//----------------------------------------------------------------------------
Sizef MathHelp::RectToSize(const Rectf &rect)
{
	return Sizef(rect.Width(), rect.Height());
}
//----------------------------------------------------------------------------
Rectf MathHelp::SizeToRect(const Sizef &size, float left, float bottom)
{
	return Rectf(left, bottom, size.Width, size.Height);
}
//----------------------------------------------------------------------------