// PX2PVMatrixConstant.hpp

#ifndef PX2PVMATRIXCONSTANT_HPP
#define PX2PVMATRIXCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM PVMatrixConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(PVMatrixConstant);

	public:
		PVMatrixConstant ();
		virtual ~PVMatrixConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(PVMatrixConstant);
	typedef Pointer0<PVMatrixConstant> PVMatrixConstantPtr;

}

#endif
