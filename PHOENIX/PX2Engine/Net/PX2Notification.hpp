// PX2Notification.hpp

#ifndef PX2NOTIFICATION_HPP
#define PX2NOTIFICATION_HPP

#include "PX2NetPre.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Notification 
	{
	public:
		Notification();
		virtual ~Notification();

		virtual std::string name() const;
	};

	typedef Pointer0<Notification> NotificationPtr;

}

#endif