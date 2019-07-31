// PX2LightConstant.hpp

#ifndef PX2LIGHTCONSTANT_HPP
#define PX2LIGHTCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2ShaderFloat.hpp"
#include "PX2Light.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LightConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LightConstant);

	public:
		LightConstant(Light* light);
		virtual ~LightConstant();

		void SetDefaultDir(bool isDefaultDir);

		void SetLight(Light *light);
		Light* GetLight();

		virtual void Update(const ShaderStruct *struc);

	protected:
		LightPtr mLight;
		bool mIsDefualtDir;
	};

	PX2_REGISTER_STREAM(LightConstant);
	typedef Pointer0<LightConstant> LightConstantPtr;

}

#endif
