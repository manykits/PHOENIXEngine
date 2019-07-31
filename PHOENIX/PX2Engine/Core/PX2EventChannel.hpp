// PX2EventChannel.hpp

#ifndef PX2EVENTCHANNEL_HPP
#define PX2EVENTCHANNEL_HPP

#include "PX2CorePre.hpp"
#include "PX2Assert.hpp"

namespace PX2
{

	/// 通道类型定义
	typedef unsigned int EventChannelType;

	// 32位和64位
	template <class T,int I = (sizeof(T)>>2)>
	class ChannelLength
	{
	public:
		enum
		{
			Left = (I<<3),
			Right = (sizeof(T) - I)<<3,
			Length = Left + Right
		};
	};

	// 16位
	template <class T>
	class ChannelLength <T, 0>
	{
	public:
		enum
		{
			Left = 8,
			Right = (sizeof(T) - 1)<<3,
			Length = Left + Right
		};
	};

	/// 通道长度
	/**
	* 见EventChannel说明
	*/
	enum
	{
		SystemChannelLength = ChannelLength<EventChannelType>::Left,
		UserChannelLength = ChannelLength<EventChannelType>::Right,
		AllChannelLength = ChannelLength<EventChannelType>::Length,
	};

	// 用户频道
	template <int N> 
	class UserChannel;

	template <int N>
	class UserChannel
	{
	public:
		enum {Value = UserChannel<N-1>::Value<<1};
	};

	template <>
	class UserChannel<0>
	{
	public:
		enum {Value = 1<<SystemChannelLength};

	};

	// 系统频道
	// 假设
	template <int N>
	class SystemChannel
	{
	public:
		enum {Value = SystemChannel<N-1>::Value<<1};
	};

	template <>
	class SystemChannel<0>
	{
	public:
		enum {Value = 1};
	};

	/// 事件通道
	/**
	* 对数据EventChannelType进行封装，因为没有虚函数，所以
	* sizeof(MessageChannel) 等于 sizeof(MessageChannel_Type)。
	* 对于32bit的EventChannelType，8bit为系统频道，24bit为用户频道。
	* 对于16bit的EventChannelType，8bit为系统频道，8bit为用户频道。
	* 对于64bit的EventChannelType，16bit为系统频道，48bit为用户频道。
	*/
	class PX2_ENGINE_ITEM EventChannel
	{
	public:
		EventChannel ();
		EventChannel (const EventChannel &channel);
		~EventChannel ();

		// 赋值
		EventChannel &operator= (const EventChannel &channel); 

		// 频道操作

		/**
		* 函数返回引用，可以按照下面方式调用：
		* EventChannel::Create()
		*		.AddUserChannel<2>()
		*		.AddUserChannel<3>()
		*/
		template <int N>
		EventChannel &AddUserChannel ();		

		template <int N>
		EventChannel &RemoveUserChannel ();

		template <int N>
		EventChannel &AddSystemChannel ();

		template <int N>
		EventChannel &RemoveSystemChannel ();

		EventChannel &Fill ();
		EventChannel &Clear ();
		EventChannel &FillUserChannel ();
		EventChannel &ClearUserChannel ();
		EventChannel &FillSystemChannel ();
		EventChannel &ClearSystemChannel ();

		// 合并，移除
		void Merge (const EventChannel &channel);
		void DisMerge (const EventChannel &channel);

		// 其他
		bool IsEmpty () const;
		bool IsListening (const EventChannel &channel) const; //< 两个通道是否有相同的消息在被听

		// 创建
		static EventChannel Create ();

	protected:
		void Initlize (const EventChannel &channel);
		void AddChannel (const EventChannelType type);
		void RemoveChannel (const EventChannelType type);
		
		EventChannelType mEventChannelType;
	};

#include "PX2EventChannel.inl"

}

#endif