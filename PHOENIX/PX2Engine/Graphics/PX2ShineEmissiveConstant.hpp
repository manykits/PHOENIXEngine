// PX2ShineEmissiveConstant.hpp

#ifndef PX2MATERIALEMISSIVECONSTANT_HPP
#define PX2MATERIALEMISSIVECONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2Shine.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ShineEmissiveConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ShineEmissiveConstant);

	public:
		ShineEmissiveConstant ();
		virtual ~ShineEmissiveConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(ShineEmissiveConstant);
	typedef Pointer0<ShineEmissiveConstant> ShineEmissiveConstantPtr;

}

#endif
