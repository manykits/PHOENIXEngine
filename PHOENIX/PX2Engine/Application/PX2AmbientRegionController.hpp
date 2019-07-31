// PX2AmbientRegionController.hpp

#ifndef PX2AMBIENTREGIONCONTROLLER_HPP
#define PX2AMBIENTREGIONCONTROLLER_HPP

#include "PX2TriggerController.hpp"
#include "PX2Light.hpp"
#include "PX2LightNode.hpp"
#include "PX2Camera.hpp"
#include "PX2CameraNode.hpp"
#include "PX2Float2.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM AmbientRegionController : public TriggerController
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(AmbientRegionController);

	public:
		AmbientRegionController();
		virtual ~AmbientRegionController();

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);

		// Default Dir Light
	public:
		void SetLightCameraLookPostion(const APoint &pos);
		void SetLightCameraLookDistance(float dist);
		void SetLightCameraExtent(float extent);

		void SetHorAngle(float angle);
		float GetHorAngle() const;
		void SetVerAngle(float angle);
		float GetVerAngle() const;

		void SetAmbientColor(const Float3 &color);
		const Float3 &GetAmbientColor() const;

		void SetDiffuseColor(const Float3 &color);
		const Float3 &GetDiffuseColor() const;

		void SetSpecularColor(const Float3 &color);
		const Float3 &GetSpecularColor() const;
		void SetSpecularPow(float pow);
		float GetSpecularPow() const;

		void SetIntensity(float intensity);
		float GetIntensity() const;

		void SetFogColorHeight(const Float3 &color);
		const Float3 &GetFogColorHeight() const;
		void SetFogParamHeight(const Float2 &param);
		const Float2 &GetFogParamHeight() const;

		void SetFogColorDistance(const Float3 &dist);
		const Float3 &GetFogColorDistance() const;
		void SetFogParamDistance(const Float2 &param);
		const Float2 &GetFogParamDistance() const;

	protected:
		virtual void SetControlledable(Controlledable* object);
		void _UpdateDirLightCamera();
		void _UpdateFog();
		bool _IsNeedUpdateLight;
		bool _IsNeedUpdateFog;

		float mHorAngle;
		float mVerAngle;

		Float3 mAmbientColor;
		Float3 mDiffuseColor;
		Float3 mSpecularColor;
		float mSpecularPow;
		float mIntensity;

		APoint mLightCameraLookPosition;
		float mLightCameraLookDistance;
		float mLightCameraExtent;

		Float3 mFogColorHeight;
		Float2 mFogParamHeight;
		Float3 mFogColorDist;
		Float2 mFogParamDist;
	};

#include "PX2AmbientRegionController.inl"
	PX2_REGISTER_STREAM(AmbientRegionController);
	typedef Pointer0<AmbientRegionController> AmbientRegionControllerPtr;

}

#endif