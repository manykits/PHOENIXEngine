// PX2LightAmbientConstant.hpp

#ifndef PX2LIGHTAMBIENTCONSTANT_HPP
#define PX2LIGHTAMBIENTCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2LightConstant.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LightAmbientConstant : public LightConstant
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LightAmbientConstant);

	public:
		LightAmbientConstant(Light* light);
		virtual ~LightAmbientConstant();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(LightAmbientConstant);
	typedef Pointer0<LightAmbientConstant> LightAmbientConstantPtr;

}

#endif
