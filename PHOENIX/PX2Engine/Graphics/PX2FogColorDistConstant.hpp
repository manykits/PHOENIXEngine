// PX2FogColorDistConstant.hpp

#ifndef PX2FOGCOLORDISTCONSTANT_HPP
#define PX2FOGCOLORDISTCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM FogColorDistConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(FogColorDistConstant);

	public:
		FogColorDistConstant ();
		virtual ~FogColorDistConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(FogColorDistConstant);
	typedef Pointer0<FogColorDistConstant> FogColorDistConstantPtr;

}

#endif