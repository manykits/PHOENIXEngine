// PX2CameraWorldPositionConstant.hpp

#ifndef PX2CAMERAWORLDPOSITIONCONSTANT_HPP
#define PX2CAMERAWORLDPOSITIONCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CameraWorldPositionConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(CameraWorldPositionConstant);

	public:
		CameraWorldPositionConstant ();
		virtual ~CameraWorldPositionConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(CameraWorldPositionConstant);
	typedef Pointer0<CameraWorldPositionConstant> CameraWorldPositionConstantPtr;

}

#endif
