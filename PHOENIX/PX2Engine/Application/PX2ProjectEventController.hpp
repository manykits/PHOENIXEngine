// PX2ProjectEventController.hpp

#ifndef PX2PROJECTEVENTCONTROLLER_HPP
#define PX2PROJECTEVENTCONTROLLER_HPP

#include "PX2Controller.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ProjectEventController : public Controller
	{
	public:
		ProjectEventController();
		virtual ~ProjectEventController();

		virtual void OnEvent(Event *event);
	};

}

#endif