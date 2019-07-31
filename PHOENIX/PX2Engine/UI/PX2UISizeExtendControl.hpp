// PX2UISizeExtendControl.hpp

#ifndef PX2UISIZEEXTENDCONTROL_HPP
#define PX2UISIZEEXTENDCONTROL_HPP

#include "PX2Controller.hpp"
#include "PX2Size.hpp"
#include "PX2Float2.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	class UIFrame;

	class PX2_ENGINE_ITEM UISizeExtendControl : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UISizeExtendControl);
		PX2_NEW(UISizeExtendControl);

	public:
		UISizeExtendControl();
		virtual ~UISizeExtendControl();

		void SetWidthOverHeightPercent(float percent);
		float GetWidthOverHeightPercent() const;

		void SetOriginHeight(float height);
		float GetOriginHeight() const;

		enum Mode
		{
			M_SIZEEXTEND,
			M_SIZEEXTEND_ONLY_ON_BIGGER,
			M_SIZEEXTEND_OVER,
			M_MAX_TYPE
		};
		void SetMode(Mode m);
		Mode GetMode() const;

	protected:
		virtual void SetControlledable(Controlledable* object);
		virtual void _Update(double applicationTime, double elapsedTime);
		void _UpdateAlignFrame();

		float mWidthOverHeightPercent;
		float mOriginHeight;
		Mode mMode;
	};

#include "PX2UISizeExtendControl.inl"
	PX2_REGISTER_STREAM(UISizeExtendControl);
	typedef Pointer0<UISizeExtendControl> UISizeExtendControlPtr;

}

#endif