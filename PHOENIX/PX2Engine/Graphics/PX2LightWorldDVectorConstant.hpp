// PX2LightWorldDVectorConstant.hpp

#ifndef PX2LIGHTWORLDDVECTORCONSTANT_HPP
#define PX2LIGHTWORLDDVECTORCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2LightConstant.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LightWorldDVectorConstant : public LightConstant
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LightWorldDVectorConstant);

	public:
		LightWorldDVectorConstant (Light* light);
		virtual ~LightWorldDVectorConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(LightWorldDVectorConstant);
	typedef Pointer0<LightWorldDVectorConstant> LightWorldDVectorConstantPtr;

}

#endif
