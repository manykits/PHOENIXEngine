// PX2ShineAmbientConstant.hpp

#ifndef PX2SHINEAMBIENTCONSTANT_HPP
#define PX2SHINEAMBIENTCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2Shine.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ShineAmbientConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ShineAmbientConstant);

	public:
		ShineAmbientConstant ();
		virtual ~ShineAmbientConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(ShineAmbientConstant);
	typedef Pointer0<ShineAmbientConstant> ShineAmbientConstantPtr;

}

#endif
