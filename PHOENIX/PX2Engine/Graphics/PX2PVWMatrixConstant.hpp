// PX2PVWMatrixConstant.hpp

#ifndef PX2PVWMATRIXCONSTANT_HPP
#define PX2PVWMATRIXCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM PVWMatrixConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(PVWMatrixConstant);

	public:
		PVWMatrixConstant ();
		virtual ~PVWMatrixConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(PVWMatrixConstant);
	typedef Pointer0<PVWMatrixConstant> PVWMatrixConstantPtr;

}

#endif
