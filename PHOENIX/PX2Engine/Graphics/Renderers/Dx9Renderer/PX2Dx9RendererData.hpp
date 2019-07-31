// PX2Dx9RendererData.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9RENDERERDATA_HPP
#define PX2DX9RENDERERDATA_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2Dx9RendererInput.hpp"
#include "PX2AlphaProperty.hpp"
#include "PX2CullProperty.hpp"
#include "PX2DepthProperty.hpp"
#include "PX2OffsetProperty.hpp"
#include "PX2StencilProperty.hpp"
#include "PX2WireProperty.hpp"
#include "PX2Texture.hpp"

namespace PX2
{

	class RendererData
	{
	public:
		RendererData (RendererInput *input, int width, int height,
			Texture::Format colorFormat, Texture::Format depthStencilFormat,
			int numMultisamples);

		~RendererData ();

		void CreateUniqueFont ();
		void DestroyUniqueFont ();
		void SetDefaultProperties (const AlphaProperty* aproperty, 
			const CullProperty* cproperty, const DepthProperty* dproperty,
			const OffsetProperty* oproperty, const StencilProperty* sproperty,
			const WireProperty* wproperty);

		// 保存当前的渲染状态，减少渲染状态的转换
		class RenderState
		{
		public:
			RenderState ();

			void Initialize (IDirect3DDevice9* device, const AlphaProperty* aproperty,
				const CullProperty* cproperty, const DepthProperty* dproperty,
				const OffsetProperty* oproperty, const StencilProperty* sproperty,
				const WireProperty* wproperty);

			// AlphaProperty
			DWORD mAlphaBlendEnable;
			DWORD mAlphaSrcBlend;
			DWORD mAlphaDstBlend;
			DWORD mAlphaTestEnable;
			DWORD mAlphaFunc;
			DWORD mAlphaRef;
			D3DCOLOR mBlendFactor;

			// CullProperty
			DWORD mCullMode;

			// DepthProperty
			DWORD mZEnable;
			DWORD mZFunc;
			DWORD mZWriteEnable;

			// OffsetProperty
			DWORD mSlopeScaleDepthBias;
			DWORD mDepthBias;

			// StencilProperty
			DWORD mStencilEnable;
			DWORD mStencilFunc;
			DWORD mStencilRef;
			DWORD mStencilMask;
			DWORD mStencilWriteMask;
			DWORD mStencilFail;
			DWORD mStencilZFail;
			DWORD mStencilPass;

			// WireProperty
			DWORD mFillMode;
		};

		enum
		{
			MAX_NUM_VSAMPLERS = 4,  //< VSModel 3 支持 4 张纹理, VSModel 2 不支持.
			MAX_NUM_PSAMPLERS = 16  //< PSModel 2 和 PSModel 3 都支持 16 张纹理.
		};
		
		// 保存当前的采样状态，减少状态的转换
		class SamplerState
		{
		public:
			SamplerState ();

			float mAnisotropy;
			float mLodBias;
			DWORD mMagFilter;
			DWORD mMinFilter;
			DWORD mMipFilter;
			D3DCOLOR mBorderColor;
			DWORD mWrap[3];
		};

		// 平台相关的数据
		IDirect3DDevice9* mDevice;
		HWND mWindowHandle;
		D3DPRESENT_PARAMETERS mPresent;
		LPD3DXFONT mFont;
		bool mDeviceLost;

		// 当前渲染数据
		RenderState mCurrentRS;
		SamplerState mCurrentVSState[MAX_NUM_VSAMPLERS];
		SamplerState mCurrentPSState[MAX_NUM_PSAMPLERS];

		// 设备能力（需要运行时确定）
		int mMaxVShaderImages;
		int mMaxPShaderImages;
		int mMaxTCoords;
	};

}

#endif

#endif