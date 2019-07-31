// PX2InitTerm.hpp

#ifndef PX2INITTERM_HPP
#define PX2INITTERM_HPP

#include "PX2CorePre.hpp"

namespace PX2
{
	
	/// 初始化函数类
	/**	
	* 在应用程序刚启动的时候创建需要预先创建的对象，在应用程序结束的时候释放预
	* 先创建的对象。设置这个类的目的是为了避免在main函数启动支持进行动态的内存
	* 分配，这样可以更加容易地管理和跟踪内存泄漏。
	*/
	class PX2_ENGINE_ITEM InitTerm
	{
	public:
		typedef void (*Initializer)(void);
		static void AddInitializer (Initializer function);
		static void ExecuteInitializers ();

		typedef void (*Terminator)(void);
		static void AddTerminator (Terminator function);
		static void ExecuteTerminators ();

	private:
		/** 
		* 为了支持这个应用程序，这个数值必须足够大。当你的应用程序使用的初始化
		* 行为超出这个值是，一个断言会触发出来。
		*/
		enum
		{
			MAX_ELEMENTS = 1024
		};

		static int msNumInitializers;
		static Initializer msInitializers[MAX_ELEMENTS];

		static int msNumTerminators;
		static Terminator msTerminators[MAX_ELEMENTS];
	};

}

//----------------------------------------------------------------------------
#define PX2_DECLARE_INITIALIZE \
public: \
	static bool RegisterInitialize (); \
	static void Initialize (); \
private: \
	static bool msInitializeRegistered
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_INITIALIZE(classname) \
	bool classname::msInitializeRegistered = false; \
	bool classname::RegisterInitialize () \
	{ \
		if (!msInitializeRegistered) \
		{ \
			InitTerm::AddInitializer(classname::Initialize); \
			msInitializeRegistered = true; \
		} \
		return msInitializeRegistered; \
	}
//----------------------------------------------------------------------------
#define PX2_REGISTER_INITIALIZE(classname) \
	static bool gsInitializeRegistered_##classname = \
	classname::RegisterInitialize ()
//----------------------------------------------------------------------------
#define PX2_DECLARE_TERMINATE \
public: \
	static bool RegisterTerminate (); \
	static void Terminate (); \
private: \
	static bool msTerminateRegistered
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_TERMINATE(classname) \
	bool classname::msTerminateRegistered = false; \
	bool classname::RegisterTerminate () \
	{ \
		if (!msTerminateRegistered) \
		{ \
			InitTerm::AddTerminator(classname::Terminate); \
			msTerminateRegistered = true; \
		} \
		return msTerminateRegistered; \
	}
//----------------------------------------------------------------------------
#define PX2_REGISTER_TERMINATE(classname) \
	static bool gsTerminateRegistered_##classname = \
	classname::RegisterTerminate ()
//----------------------------------------------------------------------------

#endif
