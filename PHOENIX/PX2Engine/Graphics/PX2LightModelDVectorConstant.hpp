// PX2LightModelDVectorConstant.hpp

#ifndef PX2LIGHTMODELDVECTORCONSTANT_HPP
#define PX2LIGHTMODELDVECTORCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2LightConstant.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LightModelDVectorConstant : public LightConstant
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LightModelDVectorConstant);

	public:
		LightModelDVectorConstant (Light* light);
		virtual ~LightModelDVectorConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(LightModelDVectorConstant);
	typedef Pointer0<LightModelDVectorConstant> LightModelDVectorConstantPtr;

}

#endif