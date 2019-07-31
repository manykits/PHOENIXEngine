// PX2EventData.hpp

#ifndef PX2EVENTDATA_HPP
#define PX2EVENTDATA_HPP

#include "PX2CorePre.hpp"
#include "PX2Any.hpp"

namespace PX2
{

	typedef Any EventData;

	template <class T>
	class EventDataTemplate
	{
	public:
		static EventData Set (const T& in)
		{
			return EventData(in);
		}

		static T Get (const EventData& data)
		{
			const T *val = Any_Cast<T>(&data);
			return *val;
		}
	};

	template <class T>
	void EventDataSet (EventData& data, const T& in)
	{
		data = EventDataTemplate<T>::Set(in);
	}

}

#endif