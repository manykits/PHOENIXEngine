// PX2MutexType.hpp

#ifndef PX2MUTEXTYPE_HPP
#define PX2MUTEXTYPE_HPP

#include "PX2CorePre.hpp"

//----------------------------------------------------------------------------
#if defined (PX2_USE_PTHREAD)
//----------------------------------------------------------------------------
// 在Linux/Apple中，pthread.h里面仅仅是原始类型数据，包含pthread.h不会暴露
// 额外的接口。
//----------------------------------------------------------------------------
#include <pthread.h>
namespace PX2
{
	struct MutexType
	{
		pthread_mutexattr_t Attribute;
		pthread_mutex_t Mutex;
	};
}
#else

#if defined(_WIN32) || defined(WIN32)
//----------------------------------------------------------------------------
// 在Windows中，Mutex是一个HANDLE，需要包含<windows.h>，这样会暴露大量
// Windows接口。所以我们用void*代替HANDLE。
//----------------------------------------------------------------------------
namespace PX2
{
	typedef void* MutexType;
}

#endif

#endif

#endif
