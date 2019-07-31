// PX2LightDiffuseConstant.hpp

#ifndef PX2LIGHTDIFFUSECONSTANT_HPP
#define PX2LIGHTDIFFUSECONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2LightConstant.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LightDiffuseConstant : public LightConstant
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LightDiffuseConstant);

	public:
		LightDiffuseConstant (Light* light);
		virtual ~LightDiffuseConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(LightDiffuseConstant);
	typedef Pointer0<LightDiffuseConstant> LightDiffuseConstantPtr;

}

#endif