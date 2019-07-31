// PX2ScopedCS.hpp

#ifndef PX2SCOPEDCS_HPP
#define PX2SCOPEDCS_HPP

#include "PX2CorePre.hpp"
#include "PX2Mutex.hpp"

namespace PX2
{

	/// 区域范围类：Scoped critial section
	/**
	* 当进入一个范围，这个类的局部临时对象创建一个mutex，当退出区域时，自动释放
	* 这个mutex。
	*/
	class PX2_ENGINE_ITEM ScopedCS
	{
	public:
		ScopedCS (Mutex* mutex);
		~ScopedCS ();

	private:
		ScopedCS();
		ScopedCS(const ScopedCS&);
		ScopedCS& operator = (const ScopedCS&);

		Mutex* mMutex;
	};

}

#endif
