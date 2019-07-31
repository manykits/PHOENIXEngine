// PX2FogColorHeightConstant.hpp

#ifndef PX2FOGCOLORHEIGHTCONSTANT_HPP
#define PX2FOGCOLORHEIGHTCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM FogColorHeightConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(FogColorHeightConstant);

	public:
		FogColorHeightConstant ();
		virtual ~FogColorHeightConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(FogColorHeightConstant);
	typedef Pointer0<FogColorHeightConstant> FogColorHeightConstantPtr;

}

#endif
