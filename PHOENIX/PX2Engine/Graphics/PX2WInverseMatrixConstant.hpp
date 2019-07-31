// PX2WInverseMatrixConstant.hpp

#ifndef PX2WINVERSEMATRIXCONSTANT_HPP
#define PX2WINVERSEMATRIXCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM WInverseMatrixConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(WInverseMatrixConstant);

	public:
		WInverseMatrixConstant();
		virtual ~WInverseMatrixConstant();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(WInverseMatrixConstant);
	typedef Pointer0<WInverseMatrixConstant> WInverseMatrixConstantPtr;

}

#endif
