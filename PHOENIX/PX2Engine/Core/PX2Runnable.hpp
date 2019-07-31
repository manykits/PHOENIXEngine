// PX2Runnable.hpp

#ifndef PX2RUNNABLE_HPP
#define PX2RUNNABLE_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Runnable
	{
	public:
		Runnable ();
		virtual ~Runnable ();

		virtual void Run () = 0;
	};

}

#endif