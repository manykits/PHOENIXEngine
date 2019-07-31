// PX2ShaderStruct.hpp

#ifndef PX2SHADERSTRUCT_HPP
#define PX2SHADERSTRUCT_HPP

#include "PX2GraphicsPre.hpp"

namespace PX2
{

	class Camera;
	class Renderable;
	class EnvirParamController;

	class PX2_ENGINE_ITEM ShaderStruct
	{
	public:
		ShaderStruct();
		~ShaderStruct();

		Renderable *TheRenderable;
		Camera *TheCamera;
		EnvirParamController *TheEnvirParam;
	};

}

#endif