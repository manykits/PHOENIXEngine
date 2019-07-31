// PX2ShineSpecularConstant.hpp

#ifndef PX2MATERIALSPECULARCONSTANT_HPP
#define PX2MATERIALSPECULARCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2Shine.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ShineSpecularConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ShineSpecularConstant);

	public:
		ShineSpecularConstant ();
		virtual ~ShineSpecularConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(ShineSpecularConstant);
	typedef Pointer0<ShineSpecularConstant> ShineSpecularConstantPtr;

}

#endif
