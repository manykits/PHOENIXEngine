// PX2MathHelp.hpp

#ifndef PX2MATHHELP_HPP
#define PX2MATHHELP_HPP

#include "PX2Float3.hpp"
#include "PX2Float4.hpp"
#include "PX2Rect.hpp"
#include "PX2Size.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM MathHelp
	{
	public:
		static Float3 Float4ToFloat3(const Float4 &from);
		static Float4 Float3ToFloat4(const Float3 &from, float w);
		static Sizef RectToSize(const Rectf &rect);
		static Rectf SizeToRect(const Sizef &size, float left, float bottom);
	};

}

#endif