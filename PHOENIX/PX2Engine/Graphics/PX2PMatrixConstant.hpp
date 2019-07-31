// PX2PMatrixConstant.hpp

#ifndef PX2PMATRIXCONSTANT_HPP
#define PX2PMATRIXCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM PMatrixConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(PMatrixConstant);

	public:
		PMatrixConstant ();
		virtual ~PMatrixConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(PMatrixConstant);
	typedef Pointer0<PMatrixConstant> PMatrixConstantPtr;

}

#endif
