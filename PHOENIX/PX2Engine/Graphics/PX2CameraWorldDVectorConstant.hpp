// PX2CameraModelPositionConstant.hpp

#ifndef PX2CAMERAWORLDDVECTORCONSTANT_HPP
#define PX2CAMERAWORLDDVECTORCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CameraWorldDVectorConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(CameraWorldDVectorConstant);

	public:
		CameraWorldDVectorConstant ();
		virtual ~CameraWorldDVectorConstant ();

		virtual void Update(const ShaderStruct *struc);
	};

	PX2_REGISTER_STREAM(CameraWorldDVectorConstant);
	typedef Pointer0<CameraWorldDVectorConstant> CameraWorldDVectorConstantPtr;

}

#endif
