// PX2NetMsgQueue.hpp

#ifndef PX2MSGQUEUE_HPP
#define PX2MSGQUEUE_HPP

#include "PX2CorePre.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"

namespace PX2
{

	/// 消息队列
	/* 
	* 适用有1个消费者(从队列中读消息)，多个生产者(往队列里面写消息)
	*/
	template <class MsgPacket>
	class MsgQueue
	{
	public:
		MsgQueue ();
		virtual ~MsgQueue ();

		void Clear();
		size_t GetAllSize();

		void AddMsg (MsgPacket *msg);
		MsgPacket *PopMsg();
		MsgPacket *Front();

		std::queue<MsgPacket*> &GetConsumeQueue();

	private:
		std::queue<MsgPacket*> mQueues[2]; // 一个消费者队列，一个生产者队列
		unsigned int mCurIndex; //当前生产者用的队列索引
		Mutex mMutex;
	};

#include "PX2MsgQueue.inl"

}

#endif