// PX2FogParamConstant.hpp

#ifndef PX2FOGPARAMCONSTANT_HPP
#define PX2FOGPARAMCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM FogParamConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(FogParamConstant);

	public:
		FogParamConstant ();
		virtual ~FogParamConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(FogParamConstant);
	typedef Pointer0<FogParamConstant> FogParamConstantPtr;

}

#endif
