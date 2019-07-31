// PX2Eventt.hpp

#ifndef PX2EVENTT_HPP
#define PX2EVENTT_HPP

#include "PX2CorePre.hpp"

#if (defined(_WIN32) || defined(WIN32))
typedef void* EventtType;
#else
#include <pthread.h>
#endif

namespace PX2
{

	/// 信号
	/**
	* 信号是一个线程同步结构，通常一个线程激活这个信号，其他线程等待
	* 这个信号被激活。
	* 
	* 事件对象和互斥对象，一样都属于内核对象，它包含一个使用计数，一个用于标识
	* 该事件是一个自动重置还是一个人工重置的布尔值，和另一个用于指定该事件处于
	* 已通知状态还是未通知状态的布尔值。
	* 事件对象可分为两种，一种是人工重置的，另一种是自动重置的。当人工重置的事
	* 件得到通知时，等待该事件的所有线程均变为可调度线程。而当一个自动事件得到
	* 通知时，等待该事件的所有线程中只有一个线程变为可调度线程。
	* 
	* 有两种状态：发信号，不发信号。Set/Reset分别将EVENT置为这两种状态分别是发
	* 信号与不发信号。Wait()等待，直到参数所指定的信号成为发信号状态时才返回。
	*
	* 举例：
	* 先创建一个全局Eventt对象gEvent:
	* Eventt gEvent;
	* 在程序中可以通过调用Eventt::Set设置事件为有信号状态。
	* 下面是一个线程函数ThreadProc()
	* static void ThreadProc()
	* {
	*	gEvent.Wait();
	*	For(;;)
	*	{
	*		...
	*	}
	*	return 0;
	* }
	* 在这个线程函数中只有设置gEvent为有信号状态时才执行下面的for循环，因为
	* gEvent是全局变量，所以我们可以在别的线程中通过gEvent.Set控制这个线程。
	*/
	class PX2_ENGINE_ITEM Eventt
	{
	public:
		/// 创建一个事件，如果autoReset为true，在Wait()成功后，事件自动Reset，
		/// 变为未激活状态。
		Eventt (bool autoReset=true);
		~Eventt ();

		/// 激活事件，如果autoReset为true，只有一个等待的线程被唤起运行；如果为
		/// false，所有等待的线程都被唤起运行。
		void Set ();

		/// 重置事件到未激活状态
		void Reset ();

		/// 等待事件被激活
		void Wait ();

		/// 在一段时间内等待事件被激活，如果激活了返回true，否则返回false
		// 毫秒（0.001秒）
		bool Wait (long milliseconds);

	protected:
		bool mIsAutoReset;

#if (defined(_WIN32) || defined(WIN32))
		EventtType mEvent;
#else
		volatile bool mState;
		pthread_mutex_t mMutex;
		pthread_cond_t mCond;
#endif

	private:
		Eventt (const Eventt&);
		Eventt& operator = (const Eventt&);
	};

}

#endif