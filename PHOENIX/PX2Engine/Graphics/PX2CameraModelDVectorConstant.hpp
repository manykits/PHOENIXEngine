// PX2CameraModelDVectorConstant.hpp

#ifndef PX2CAMERAMODELDVECTORCONSTANT_HPP
#define PX2CAMERAMODELDVECTORCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CameraModelDVectorConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(CameraModelDVectorConstant);

	public:
		CameraModelDVectorConstant ();
		virtual ~CameraModelDVectorConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(CameraModelDVectorConstant);
	typedef Pointer0<CameraModelDVectorConstant> CameraModelDVectorConstantPtr;

}

#endif
