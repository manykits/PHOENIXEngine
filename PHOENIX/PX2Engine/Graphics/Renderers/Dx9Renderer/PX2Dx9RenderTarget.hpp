// PX2Dx9RenderTarget.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9RENDERTARGET_HPP
#define PX2DX9RENDERTARGET_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2RenderTarget.hpp"

namespace PX2
{

	class Renderer;

	class PdrRenderTarget
	{
	public:
		PdrRenderTarget (Renderer* renderer, const RenderTarget* renderTarget);
		~PdrRenderTarget ();

		// Render target 操作
		void Enable (Renderer* renderer);
		void Disable (Renderer* renderer);
		void ReadColor (int i, Renderer* renderer, Texture2D*& texture);

	private:
		// 支持多个render target
		int mNumTargets;
		unsigned int mWidth, mHeight;
		Texture::Format mFormat;
		bool mHasDepthStencil;

		// 颜色渲染目标可以有多个，depth-stencil渲染目标只需要一个。深度模板
		// 值只需要计算一次。
		IDirect3DTexture9** mColorTextures;
		IDirect3DSurface9** mColorSurfaces;
		IDirect3DTexture9* mDepthStencilTexture;
		IDirect3DSurface9* mDepthStencilSurface;

		// 保存之前的渲染目标，设置完render target之后需要置回。
		IDirect3DSurface9* mSaveColorSurface;
		IDirect3DSurface9* mSaveDepthStencilSurface;
	};

}

#endif

#endif