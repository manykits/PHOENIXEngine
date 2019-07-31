// PX2VMatrixConstant.hpp

#ifndef PX2VMATRIXCONSTANT_HPP
#define PX2VMATRIXCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM VMatrixConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(VMatrixConstant);

	public:
		VMatrixConstant ();
		virtual ~VMatrixConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(VMatrixConstant);
	typedef Pointer0<VMatrixConstant> VMatrixConstantPtr;

}

#endif