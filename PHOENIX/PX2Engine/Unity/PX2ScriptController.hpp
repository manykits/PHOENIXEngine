// PX2ScriptController.hpp

#ifndef PX2SCRIPTCONTROLLER_HPP
#define PX2SCRIPTCONTROLLER_HPP

#include "PX2UnityPre.hpp"
#include "PX2Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ScriptController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(ScriptController);

	public:
		ScriptController();
		virtual ~ScriptController();

		virtual void SetFileClass(const std::string &filename,
			const std::string &className);
		virtual void SetStringClass(const std::string &str,
			const std::string &className);

		const std::string &GetFilename() const;
		const std::string &GetClassName() const;
		virtual int SelfP() const;

		virtual void SetControlledable(Controlledable* object);

		virtual void Reset();
		virtual void Play();
		virtual void Pause();

		virtual bool Update(double applicationTime, double elapsedTime);
		virtual void FixUpdate();

		virtual void ScriptOnAttached();
		virtual void ScriptOnDetach();
		virtual void ScriptOnInitUpdate();
		virtual void ScriptOnUpdate();
		virtual void ScriptOnFixUpdate();

		virtual void ScriptOnReset();
		virtual void ScriptOnPlay();
		virtual void ScriptOnPause();
		virtual void ScriptOnPlayUpdate();

		void SetNumFixUpdatePerSeconds(int num);
		int GetNumFixUpdatePerSeconds() const;

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);

		bool mIsInitUpdated;
		int mNumFixUpdatePerSeconds;
		float mOneFrameSeconds;
		float mFiexUpdateTiming;

		std::string mFilename;
		std::string mString;
		std::string mClassName;
	};

	PX2_REGISTER_STREAM(ScriptController);
	typedef Pointer0<ScriptController> ScriptControllerPtr;

}

#endif