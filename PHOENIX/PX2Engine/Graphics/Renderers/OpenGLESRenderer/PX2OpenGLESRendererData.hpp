// PX2OpenGLESRendererData.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2RENDERERDATA_HPP
#define PX2OPENGLES2RENDERERDATA_HPP

#include "PX2Float4.hpp"
#include "PX2OpenGLESRendererPre.hpp"

namespace PX2
{

	class AlphaProperty;
	class CullProperty;
	class DepthProperty;
	class OffsetProperty;
	class StencilProperty;
	class WireProperty;

	class RendererData
	{
	public:

		class RenderState
		{
		public:
			RenderState ();
			void Initialize (const AlphaProperty* astate,
				const CullProperty* cstate, const DepthProperty* dstate,
				const OffsetProperty* ostate, const StencilProperty* sstate,
				const WireProperty* wstate);

			// AlphaProperty
			bool mAlphaBlendEnabled;
			GLenum mAlphaSrcBlend;
			GLenum mAlphaDstBlend;
			bool mAlphaCompareEnabled;
			GLenum mCompareFunction;
			float mAlphaReference;
			Float4 mBlendColor;

			// CullProperty
			bool mCullEnabled;
			bool mCCWOrder;

			// DepthProperty
			bool mDepthEnabled;
			bool mDepthWriteEnabled;
			GLenum mDepthCompareFunction;

			// OffsetProperty
			bool mFillEnabled;
			bool mLineEnabled;
			bool mPointEnabled;
			float mOffsetScale;
			float mOffsetBias;

			// StencilProperty
			bool mStencilEnabled;
			GLenum mStencilCompareFunction;
			GLuint mStencilReference;
			GLuint mStencilMask;
			GLuint mStencilWriteMask;
			GLenum mStencilOnFail;
			GLenum mStencilOnZFail;
			GLenum mStencilOnZPass;

			// WireProperty
			bool mWireEnabled;
		};

		RenderState mCurrentRS;

		enum
		{
			MAX_NUM_VSAMPLERS = 4,  // VSModel 3 has 4, VSModel 2 has 0.
			MAX_NUM_PSAMPLERS = 16  // PSModel 2 and PSModel 3 have 16.
		};

		class SamplerState
		{
		public:
			SamplerState ();

			void GetCurrent (GLenum target);

			GLint mMagFilter;
			GLint mMinFilter;
			GLint mWrap[2];
		};
        
#if defined(_WIN32) || defined(WIN32) || defined(__ANDROID__) || defined (__LINUX__)

		// 平台相关的数据
		EGLNativeWindowType  mWindowHandle;
		EGLNativeDisplayType mDisplayType;
		EGLDisplay mDisplay;
		EGLConfig mConfig;	
		EGLSurface mSurface;
		EGLContext mContext;
#endif

		SamplerState mCurrentSS[MAX_NUM_PSAMPLERS];

		// 设备能力
		int mMaxVShaderImages;
		int mMaxPShaderImages;
		int mMaxCombinedImages;
		int mMaxRenderBufferSize;
	};

}

#endif

#endif