// PX2UIPicBoxListController.hpp

#ifndef PX2UIPICBOXLISTCONTROLLER_HPP
#define PX2UIPICBOXLISTCONTROLLER_HPP

#include "PX2UIFPicBox.hpp"
#include "PX2Controller.hpp"
#include "PX2UIFPicBox.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIPicBoxListController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_NEW(UIPicBoxListController);
		PX2_DECLARE_STREAM(UIPicBoxListController);

	public:
		UIPicBoxListController();
		virtual ~UIPicBoxListController();

		void SetInterval(float seconds);
		float GetInterval() const;

		UIFPicBox *AddPicBox(const std::string &filename);

	protected:
		virtual bool Update(double applicationTime, double elapsedTime);
		virtual void SetControlledable(Controlledable* object);

		float mInterval;
		std::vector<UIFPicBoxPtr> mPicBoxList;
		int mLastShowIndex;
	};

	PX2_REGISTER_STREAM(UIPicBoxListController);
	typedef Pointer0<UIPicBoxListController> UIPicBoxListControllerPtr;

}

#endif