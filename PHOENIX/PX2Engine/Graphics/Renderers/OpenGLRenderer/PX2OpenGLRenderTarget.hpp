// PX2OpenGLRenderTarget.hpp

#ifdef PX2_USE_OPENGL

#ifndef PX2OPENGLRENDERTARGET_H
#define PX2OPENGLRENDERTARGET_H

#include "PX2OpenGLRendererPre.hpp"
#include "PX2RenderTarget.hpp"

namespace PX2
{

	class Renderer;

	class PdrRenderTarget
	{
	public:
		// Construction and destruction.
		PdrRenderTarget(Renderer* renderer, const RenderTarget* renderTarget);
		~PdrRenderTarget();

		// Render target operations.
		void Enable(Renderer* renderer);
		void Disable(Renderer* renderer);
		void ReadColor(int i, Renderer* renderer, Texture2D*& texture);

	private:
		int mNumTargets, mWidth, mHeight;
		Texture::Format mFormat;
		bool mHasMipmaps, mHasDepthStencil;

		GLuint* mColorTextures;
		GLuint mDepthStencilTexture, mFrameBuffer;
		GLenum* mDrawBuffers;
		int mPrevViewport[4];
		double mPrevDepthRange[2];
	};

}

#endif

#endif