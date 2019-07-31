// PX2UICheckButton.hpp

#ifndef PX2UICHECKBUTTON_HPP
#define PX2UICHECKBUTTON_HPP

#include "PX2UIButtonBase.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UICheckButton : public UIButtonBase
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(UICheckButton);
		PX2_DECLARE_STREAM(UICheckButton);

	public:
		UICheckButton();
		virtual ~UICheckButton();

		virtual void Enable(bool enable);

		virtual void OnEvent(Event *event);

		void Check(bool check, bool doCall = true);
		bool IsCheck() const;

		void OnCheck();
		void OnDisCheck();

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		virtual void OnWidgetPicked(const CanvasInputData &data);
	};

	PX2_REGISTER_STREAM(UICheckButton);
	typedef Pointer0<UICheckButton> UICheckButtonPtr;

}

#endif