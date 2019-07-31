// PX2BeamEmitterController.hpp

#ifndef PX2EFFECTABLECONTROLLER_HPP
#define PX2EFFECTABLECONTROLLER_HPP

#include "PX2EffectPre.hpp"
#include "PX2Controller.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2EffectModule.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM EffectableController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(EffectableController);

	public:
		EffectableController ();
		virtual ~EffectableController ();

		virtual void Reset ();

		// Event
		void AddModule (EffectModule *module);
		void RemoveModule (EffectModule *module);
		EffectModule *GetModule (int i);
		EffectModule *GetModuleByRttiName (
			const std::string &moduleRttiName) const;
		EffectModule *GetModuleByTypeName (const std::string &typeName) const;
		int GetNumModules ();
		bool IsHasModule (const std::string &moduleRttiName) const;
		bool IsHasModuleByTypeName (const std::string &typeName) const;

		void ModulesUpdateEffectable (float ctrlTime);
		void ModulesUpdateEffectObject (EffectObject *obj);

		virtual bool Update (double applicationTime, double elapsedTime);

	protected:
		virtual void OnNewAEffectObject (EffectObject *obj);

		typedef std::vector<EffectModulePtr> ModuleList;
		ModuleList mModules;
	};

	PX2_REGISTER_STREAM(EffectableController);
	typedef Pointer0<EffectableController> EffectableControllerPtr;
#include "PX2EffectableController.inl"

}

#endif