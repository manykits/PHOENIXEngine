// PX2CameraWorldDVectorConstant.hpp

#ifndef PX2CAMERAMODELPOSITIONCONSTANT_HPP
#define PX2CAMERAMODELPOSITIONCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CameraModelPositionConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(CameraModelPositionConstant);

	public:
		CameraModelPositionConstant ();
		virtual ~CameraModelPositionConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(CameraModelPositionConstant);
	typedef Pointer0<CameraModelPositionConstant> CameraModelPositionConstantPtr;

}

#endif
