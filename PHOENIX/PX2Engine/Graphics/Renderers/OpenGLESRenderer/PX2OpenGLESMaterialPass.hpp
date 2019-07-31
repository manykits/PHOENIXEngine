// PX2OpenGLESMaterialPass.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2MATERIALPASS_HPP
#define PX2OPENGLES2MATERIALPASS_HPP

#include "PX2OpenGLESRendererPre.hpp"
#include "PX2PixelShader.hpp"
#include "PX2MaterialPass.hpp"

namespace PX2
{

	class Renderer;

	class PdrMaterialPass
	{
	public:
		PdrMaterialPass (Renderer *renderer, const MaterialPass *pass);
		~PdrMaterialPass ();

		void Enable (Renderer *renderer);
		void Disable (Renderer *renderer);

		GLuint GetProgram () { return mProgramObject; }

	private:
		const MaterialPass *mPass;
		GLuint mProgramObject;

		std::map<std::string, int> mAttribLocations;
	};

}

#endif

#endif