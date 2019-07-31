// PX2NotificationQueue.hpp

#ifndef PX2NOTFICATIONQUEUE_HPP
#define PX2NOTFICATIONQUEUE_HPP

#include "PX2NetPre.hpp"
#include "PX2Notification.hpp"
#include "PX2Eventt.hpp"

namespace PX2
{

	/// NotificationQueue对象提供了一种实现异步的方法
	/// 通知。 这对于发送通知特别有用
	/// 从一个线程到另一个线程，例如从后台线程到主（用户界面）线程。
	///
	/// NotificationQueue也可以用于分发工作
	/// 一个控制线程到一个或多个工作线程。 每个工作线程
	/// 重复调用waitDequeueNotification（）并处理返回的Notification。 
	/// 关闭时必须注意关闭和销毁队列的推荐顺序是
	/// 1.为每个工作线程设置终止标志
	/// 2.调用wakeUpAll（）方法
	/// 3.加入每个工作线程
	/// 4.破坏通知队列。
	class PX2_ENGINE_ITEM NotificationQueue
	{
	public:
		NotificationQueue();
		~NotificationQueue();

		void EnqueueNotification(Notification *notification);
		void EnqueueUrgentNotification(Notification *notification);

		NotificationPtr DequeueNotification();
		NotificationPtr WaitDequeueNotification();
		NotificationPtr WaitDequeueNotification(long milliseconds);

		void WakeUpAll();
		bool IsEmpty() const;
		int GetSize() const;
		void Clear();
		bool HasIdleThreads() const;

	protected:
		NotificationPtr DequeueOne();

	private:
		typedef std::deque<NotificationPtr> NfQueue;
		NfQueue mNFQueue;

		struct WaitInfo
		{
			NotificationPtr Notif;
			Eventt EventAvailable;
		};
		typedef std::deque<WaitInfo*> WaitQueue;
		WaitQueue mWaitQueue;

		mutable Mutex mMutex;
	};


}

#endif