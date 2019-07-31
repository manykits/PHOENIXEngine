// PX2SkinMatrixConstant.hpp

#ifndef PX2SKINMATRIXCONSTANT_HPP
#define PX2SKINMATRIXCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class SkinController;

	class PX2_ENGINE_ITEM SkinMatrixConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(SkinMatrixConstant);

	public:
		SkinMatrixConstant (int numMatrix=36);
		virtual ~SkinMatrixConstant ();

		virtual void Update(const ShaderStruct *struc);

	private:		
		SkinController *mSkinContrl;
	};

	PX2_REGISTER_STREAM(SkinMatrixConstant);
	typedef Pointer0<SkinMatrixConstant> SkinMatrixConstantPtr;

}

#endif