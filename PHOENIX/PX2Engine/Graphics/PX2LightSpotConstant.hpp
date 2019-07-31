// PX2LightSpotConstant.hpp

#ifndef PX2LIGHTSPOTCONSTANT_HPP
#define PX2LIGHTSPOTCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2LightConstant.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LightSpotConstant : public LightConstant
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LightSpotConstant);

	public:
		LightSpotConstant (Light* light);
		virtual ~LightSpotConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(LightSpotConstant);
	typedef Pointer0<LightSpotConstant> LightSpotConstantPtr;

}

#endif
