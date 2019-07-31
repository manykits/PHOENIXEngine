// PX2Assert.hpp

#ifndef PX2ASSERT_HPP
#define PX2ASSERT_HPP

#include "PX2CorePre.hpp"

#ifdef PX2_USE_ASSERT
//----------------------------------------------------------------------------
// 使用Phoenix2的断言
//----------------------------------------------------------------------------
namespace PX2
{

	class PX2_ENGINE_ITEM Assert
	{
	public:
		Assert (bool condition, const char* file, int line, const char* format,
			...);

		~Assert ();

	private:
		enum { MAX_MESSAGE_BYTES = 1024 };
		static const char *msDebugPrompt;
		static const size_t msDebugPromptLength;
		static const char *msMessagePrefix;

#ifdef PX2_USE_ASSERT_WRITE_TO_MESSAGE_BOX
		static const char *msMessageBoxTitle;
#endif

#ifdef PX2_USE_ASSERT_LOG_TO_FILE
		static const char *msLogFileName;
#endif
	};

}

#define assertion(condition, format, ...) \
	PX2::Assert(condition, __FILE__, __LINE__, format, __VA_ARGS__)
//----------------------------------------------------------------------------
#else
//----------------------------------------------------------------------------
// 使用标准断言
//----------------------------------------------------------------------------
#define assertion(condition, format, ...) assert(condition)
//----------------------------------------------------------------------------
#endif

#endif
