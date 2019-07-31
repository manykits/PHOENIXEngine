// PX2LightSpecularConstant.hpp

#ifndef PX2LIGHTSPECULARCONSTANT_HPP
#define PX2LIGHTSPECULARCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2LightConstant.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LightSpecularConstant : public LightConstant
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LightSpecularConstant);

	public:
		LightSpecularConstant (Light* light);
		virtual ~LightSpecularConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(LightSpecularConstant);
	typedef Pointer0<LightSpecularConstant> LightSpecularConstantPtr;

}

#endif
