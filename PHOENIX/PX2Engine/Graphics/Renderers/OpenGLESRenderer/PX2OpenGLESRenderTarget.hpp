// PX2OpenGLESRenderTarget.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2RENDERTARGET_HPP
#define PX2OPENGLES2RENDERTARGET_HPP

#include "PX2OpenGLESRendererPre.hpp"
#include "PX2RenderTarget.hpp"

namespace PX2
{

	class Renderer;

	class PdrRenderTarget
	{
	public:
		PdrRenderTarget (Renderer* renderer, const RenderTarget* renderTarget);
		~PdrRenderTarget ();

		// Render target ²Ù×÷
		void Enable (Renderer* renderer);
		void Disable (Renderer* renderer);
		void ReadColor (int i, Renderer* renderer, Texture2D*& texture);

	private:
        GLint mBeforeFrameBinding;
        GLint mBeforeRenderBufferBinding;
        
		int mNumTargets, mWidth, mHeight;
		Texture::Format mFormat;
		bool mHasMipmaps, mHasDepthStencil;

		GLuint* mColorTextures;
		GLuint mDepthRenderBuffer;

		GLuint mDepthStencilTexture, mFrameBuffer;

		GLenum* mDrawBuffers;
		int mPrevViewport[4];
		float mPrevDepthRange[2];
	};

}

#endif

#endif