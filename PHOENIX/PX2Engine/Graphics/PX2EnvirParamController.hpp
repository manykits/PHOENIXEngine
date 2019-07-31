// PX2EnvirParamController.hpp

#ifndef PX2ENVIRPARAMCONTROLLER_HPP
#define PX2ENVIRPARAMCONTROLLER_HPP

#include "PX2Object.hpp"
#include "PX2Light.hpp"
#include "PX2Projector.hpp"
#include "PX2Texture.hpp"
#include "PX2VisibleSet.hpp"
#include "PX2Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM EnvirParamController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(EnvirParamController);

	public:
		EnvirParamController();
		virtual ~EnvirParamController();

	public:
		// DirLight
		Light *GetLight_Dir();
		Projector *GetLight_Dir_Projector();
		void SetLight_Dir_DepthTexture(Texture *tex);
		Texture *GetLight_Dir_DepthTexture();
		void SetShadowMap_OffsetPropertyScale(float scale);
		float GetShadowMap_OffsetPropertyScale() const;
		void SetShadowMap_OffsetPropertyBias(float bias);
		float GetShadowMap_OffsetPropertyBias() const;

		// PointLight
		void AddLight(Light *light);
		void RemoveLight(Light *light);
		void ClearAllLights();
		int GetNumLights();
		Light *GetLight(int index);

	protected:
		std::vector<LightPtr> mAllLights; // 保存放在场景中的所有灯光

		LightPtr mLight_Dir;
		ProjectorPtr mLight_Dir_Projector;
		TexturePtr mLight_Dir_DepthTexture;

		float mShadowOffsetProperty_Scale;
		float mShadowOffsetProperty_Bias;

		// Fog
	public:
		// Float4(-10.0f, 0.0f, 0.0f, 120.0f);
		// 高度从低到高，远度从近到远
		void SetFogParam(const Float4 &param);
		const Float4 &GetFogParam() const;
		void SetFogColorHeight(const Float4 &fogColor);
		const Float4 &GetFogColorHeight() const;
		void SetFogColorDist(const Float4 &fogColor);
		const Float4 &GetFogColorDist() const;

	protected:
		Float4 mFogParam;
		Float4 mFogColorDistLast;
		Float4 mFogColorHeightLast;

	public_internal :
		void ComputeEnvironment(VisibleSet &vs);

		// shadow bloom
	public:
		bool IsUseShadowMap() const;
		const Sizef &GetShadowRenderTargetSize() const;
		bool IsShadowRenderTargetSizeSameWithCanvas() const;

		bool IsUseBloom() const;

		float GetBloomBrightWeight() const;
		float GetBloomBlurDeviation() const;
		float GetBloomBlurWeight() const;
		float GetBloomWeight() const;
		const Float4 &GetBloomBrightParam() const;
		const Float4 &GetBloomParam() const;

	protected:
		bool mIsUseShadowMap;
		Sizef mShadowRenderTargetSize;
		bool mIsShadowRenderTargetSizeSameWithCanvas;

		bool mIsUseBloom;
		float mBloomBrightWeight;
		Float4 mBloomBrightParam;
		float mBloomBlurDeviation;
		float mBloomBlurWeight;
		float mBloomWeight;
		Float4 mBloomParam;
	};

#include "PX2EnvirParamController.inl"
	PX2_REGISTER_STREAM(EnvirParamController);
	typedef Pointer0<EnvirParamController> EnvirParamControllerPtr;

}

#endif