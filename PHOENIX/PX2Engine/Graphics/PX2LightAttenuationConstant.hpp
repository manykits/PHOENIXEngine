// PX2LightAttenuationConstant.hpp

#ifndef PX2LIGHTATTENUATIONCONSTANT_HPP
#define PX2LIGHTATTENUATIONCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2LightConstant.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LightAttenuationConstant : public LightConstant
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LightAttenuationConstant);

	public:
		LightAttenuationConstant (Light* light);
		virtual ~LightAttenuationConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(LightAttenuationConstant);
	typedef Pointer0<LightAttenuationConstant> LightAttenuationConstantPtr;

}

#endif
