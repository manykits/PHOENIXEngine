// PX2EngineSceneCanvas.hpp

#ifndef PX2ENGINESCENECANVAS_HPP
#define PX2ENGINESCENECANVAS_HPP

#include "PX2Canvas.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2ShadowMapMaterial.hpp"
#include "PX2UIFPicBox.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM EngineSceneCanvas : public Canvas,
		public Singleton<EngineSceneCanvas>
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(EngineSceneCanvas);

	public:
		EngineSceneCanvas();
		virtual ~EngineSceneCanvas();

		virtual void OnEvent(Event *ent);
		virtual void OnSizeChanged();

		virtual void Draw(Renderer *renderer);

		void SetShadowMapConfigChanged(bool changed);
		UIPicBox *GetPicBox_Shadow();

		void SetBloomConfigChanged(bool changed);

	protected:
		virtual void UpdateLayout(Movable *parent);
		void _DoClear(Renderer *renderer, Camera *camera);
		virtual void _Draw(Camera *camera, Renderer *renderer, Culler *culler);
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _UpdateShadowChanged();
		void _UpdateBloomChanged();
		void _UpdateBloomParams();

		CameraPtr mScreenCamera;

		// shadow
		bool mIsShadowMapConfigChanged;
		RenderTargetPtr mRenderTarget_Shadow;
		ShadowMap_MaterialPtr mMaterial_Shadow;
		UIPicBoxPtr mPicBox_Shadow;

		// bloom
		bool mIsBloomConfigChanged;

		RenderTargetPtr mBloomRenderTarget_Normal;
		UIPicBoxPtr mBloomPicBox_Normal;

		RenderTargetPtr mBloomRenderTarget_Bright;
		UIPicBoxPtr mBloomPicBox_Bright;
		ShaderFloatPtr mBloom_BrightParam;

		RenderTargetPtr mBloomRenderTarget_BlurH;
		UIPicBoxPtr mBloomPicBox_BlurH;
		RenderTargetPtr mBloomRenderTarget_BlurV;
		UIPicBoxPtr mBloomPicBox_BlurV;
		Float4 mBloomBlur_UVOffsets_H[16];
		Float4 mBloomBlur_UVOffsets_V[16];

		UIPicBoxPtr mBloomPicBox_Final;
		MaterialInstancePtr mBoom_MtlInstance;
		ShaderFloatPtr mBloom_Param;
	};
	PX2_REGISTER_STREAM(EngineSceneCanvas);
	typedef Pointer0<EngineSceneCanvas> EngineSceneCanvasPtr;

}

#endif