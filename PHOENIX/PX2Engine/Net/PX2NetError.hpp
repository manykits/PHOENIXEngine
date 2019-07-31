// PX2NetError.hpp

#ifndef PX2NETERROR_HPP
#define PX2NETERROR_HPP

#include "PX2NetPre.hpp"
#include "PX2NetDefine.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM NetError
	{
	public:

		static int LastError ();
		static void Error ();
		static void Error (const std::string& arg);
		static void Error (int code);
		static void Error (int code, const std::string& arg);
	};
	
}

#endif