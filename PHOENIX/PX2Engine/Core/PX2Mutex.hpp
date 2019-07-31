// PX2Mutex.hpp

#ifndef PX2MUTEX_HPP
#define PX2MUTEX_HPP

#include "PX2CorePre.hpp"
#include "PX2MutexType.hpp"

namespace PX2
{

	/// œﬂ≥ÃÀ¯¿‡
	class PX2_ENGINE_ITEM Mutex
	{
	public:
		Mutex (bool isRecursive=true);
		~Mutex ();

		void Enter ();
		void Leave ();

	private:
		bool mIsRecursive;
		MutexType mMutex;
	};

}
#endif
