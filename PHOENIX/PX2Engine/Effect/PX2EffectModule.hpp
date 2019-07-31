// PX2EffectModule.hpp

#ifndef PX2EFFECTMODULE_HPP
#define PX2EFFECTMODULE_HPP

#include "PX2EffectPre.hpp"
#include "PX2Object.hpp"
#include "PX2InterpCurveFloatController.hpp"
#include "PX2InterpCurveFloat3Controller.hpp"

namespace PX2
{

	class Effectable;
	class EffectObject;
	class EffectableController;
	class EffectModule;

	typedef void (*UpdateEffectableCallback) (EffectModule *module, Effectable *able, float ctrlTime);
	typedef void (*UpdateEffectObjectCallback) (EffectModule *module, EffectObject *obj);

	class PX2_ENGINE_ITEM EffectModule : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(EffectModule);

	public:
		enum ModuleType
		{
			MT_FLOAT,
			MT_FLOAT2,
			MT_FLOAT3,
			MT_FLOAT3COLOR,
			MT_MAX_TYPE
		};
		EffectModule (ModuleType type, bool isForEffectable, 
			float initVal=0.0f, const Float3 &initVal1=Float3::ZERO);
		virtual ~EffectModule ();

		void SetTypeName (const std::string &typeName);
		const std::string &GetTypeName () const;

		bool IsForEffectable () const;

		ModuleType GetModuleType ();

		enum RepeatType
		{
			RT_NONE,
			RT_CLAMP,
			RT_WRAP,
			RT_CYCLE,
			RT_MAX_TYPE
		};
		void SetRepeatType (RepeatType type);
		RepeatType GetRepeatType () const;

		void SetRepeatTime (float time);
		float GetRepeatTime () const;

		void SetRange (bool range);
		bool IsRange () const;

		InterpCurveFloat &GetCurveFloatMin();
		InterpCurveFloat &GetCurveFloatMax();
		InterpCurveFloat3 &GetCurveFloat3Min();
		InterpCurveFloat3 &GetCurveFloat3Max();
		int GetNumKeyPoints () const;
		float *GetKeyPointInVal (int i);
		float *GetKeyPointOutValFloatMin (int i);
		float *GetKeyPointOutValFloatMax (int i);
		Float3 *GetKeyPointOutValFloat3Min (int i);
		Float3 *GetKeyPointOutValFloat3Max (int i);
		float *GetKeyPointArriveTangentFloatMin (int i);
		float *GetKeyPointArriveTangentFloatMax (int i);
		float *GetKeyPointLeaveTangentFloatMin (int i);
		float *GetKeyPointLeaveTangentFloatMax (int i);
		Float3 *GetKeyPointArriveTangentFloat3Min (int i);
		Float3 *GetKeyPointArriveTangentFloat3Max (int i);
		Float3 *GetKeyPointLeaveTangentFloat3Min (int i);
		Float3 *GetKeyPointLeaveTangentFloat3Max (int i);

		// Effect
		virtual void UpdateEffectable(Effectable *able, float ctrlTime);
		virtual void UpdateEffectObject (EffectObject *obj);
		float GetModuleTime (float time);

		void SetUpdateEffectableCallback (UpdateEffectableCallback callback);
		UpdateEffectableCallback GetUpdateEffectableCallback () const;

		void SetUpdateEffectObjectCallback (UpdateEffectObjectCallback callback);
		UpdateEffectObjectCallback GetUpdateEffectObjectCallback () const;

		static EffectModule *CreateModule (const std::string &name);

		EffectableController *GetEffectableController () const;

public_internal:
		static bool LessThan (const EffectModule *mod0, const EffectModule *mod1);
		virtual void OnRemove (EffectableController &ctrl);
		void SetEffectableController (EffectableController *ctrl);

	protected:
		EffectModule ();

		UpdateEffectableCallback mEffectableCallback;
		UpdateEffectObjectCallback mEffectObjectCallback;
		EffectableController *mEffectableCtrl;

		std::string mTypeName;
		bool mIsForEffectable;
		bool mIsRange;
		ModuleType mModuleType;
		RepeatType mRepeatType;
		float mRepeatTime;
		InterpCurveFloat mCurveFloatMin;
		InterpCurveFloat mCurveFloatMax;
		InterpCurveFloat3 mCurveFloat3Min;
		InterpCurveFloat3 mCurveFloat3Max;

		static EffectModule *PreCreateModuleFloat_EA (const std::string &name, 
			float initVal, UpdateEffectableCallback callback);
		static EffectModule *PreCreateModuleFloat3_EA (const std::string &name, 
			Float3 initVal, UpdateEffectableCallback callback);
		static EffectModule *PreCreateModuleFloat3Color_EA (const std::string &name, 
			Float3 initVal, UpdateEffectableCallback callback);

		static EffectModule *PreCreateModuleFloat_EO (const std::string &name, 
			float initVal, UpdateEffectObjectCallback callback);
		static EffectModule *PreCreateModuleFloat3_EO (const std::string &name, 
			Float3 initVal, UpdateEffectObjectCallback callback);
		static EffectModule *PreCreateModuleFloat3Color_EO (const std::string &name, 
			Float3 initVal, UpdateEffectObjectCallback callback);

public_internal:
		static bool RegisterInitFinal ();

	protected:
		static void EffectModuleInit ();
		static void EffectModuleFinal ();

		static bool msIsRegisterInitFinal;
		static std::map<std::string, Pointer0<EffectModule> > msPreCreatedModules;
	};

	PX2_REGISTER_STREAM(EffectModule);
	typedef Pointer0<EffectModule> EffectModulePtr;
#include "PX2EffectModule.inl"
	static bool gsEffectModuleInitFinal = EffectModule::RegisterInitFinal();

}

#endif