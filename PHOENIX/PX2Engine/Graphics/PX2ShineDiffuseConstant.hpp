// PX2ShineDiffuseConstant.hpp

#ifndef PX2SHINEDIFFUSECONSTANT_HPP
#define PX2SHINEDIFFUSECONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2Shine.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ShineDiffuseConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ShineDiffuseConstant);

	public:
		ShineDiffuseConstant ();
		virtual ~ShineDiffuseConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(ShineDiffuseConstant);
	typedef Pointer0<ShineDiffuseConstant> ShineDiffuseConstantPtr;

}

#endif
