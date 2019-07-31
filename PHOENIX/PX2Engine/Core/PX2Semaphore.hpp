// PX2Semaphore.hpp

#ifndef PX2SEMAPHORE_HPP
#define PX2SEMAPHORE_HPP

#include "PX2CorePre.hpp"

#if defined(__LINUX__) || defined(__APPLE__) || defined(__ANDROID__)
#include<semaphore.h>
#endif

namespace PX2
{

	/// 信号量
	/**
	* 信号量是在多线程环境下使用的一种设施，它负责协调各个线程，以保证它们能够
	* 正确、合理的使用公共资源。
	*
	* 信号量的特性如下：信号量是一个非负整数，所有通过它的线程都会将该整数减
	* 一，当该整数值为零时，所有试图通过它的线程都将处于等待状态。在信号量上我
	* 们定义两种操作： Wait（等待） 和 Set（释放）。 当一个线程调用Wait（等待）
	* 操作时，它要么通过然后将信号量减一，要么一直等下去，直到信号量大于一或超
	* 时。（释放）实际上是在信号量上执行加操作，线程释放了由信号量守护的资源。
	*/
	class PX2_ENGINE_ITEM Semaphore
	{
	public:
		Semaphore(int initNum);
		Semaphore(int initNum, int max);
		~Semaphore ();

		void Set();
		void Wait();

	private:
		Semaphore();
		Semaphore(const Semaphore&);
		Semaphore& operator = (const Semaphore&);

		int mInitNum;
		int mMax;

#if defined(_WIN32) || defined(WIN32)
		void *mHandle;
#elif defined(__LINUX__) || defined(__APPLE__) || defined(__ANDROID__)
		sem_t mSem;
#endif
	};

}

#endif