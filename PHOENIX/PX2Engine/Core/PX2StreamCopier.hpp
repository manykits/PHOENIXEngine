// PX2StreamCopier.hpp

#ifndef PX2STREAMCOPIER_HPP
#define PX2STREAMCOPIER_HPP

#include "PX2CorePre.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM StreamCopier
	{
	public:
		static std::streamsize CopyStream(std::istream& istr,
			std::ostream& ostr, std::size_t bufferSize = 8192);
		static std::streamsize CopyStreamUnbuffered(std::istream& istr,
			std::ostream& ostr);
		static std::streamsize CopyToString(std::istream& istr,
			std::string& str, std::size_t bufferSize = 8192);
	};


}

#endif