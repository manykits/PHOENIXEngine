// PX2HTTPBufferAllocator.hpp

#ifndef PX2HTTPBUFFERALLOCATOR_HPP
#define PX2HTTPBUFFERALLOCATOR_HPP

#include "PX2NetPre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM HTTPBufferAllocator
	{
	public:
		static char* Allocate(std::streamsize size);
		static void Deallocate(char* ptr, std::streamsize size);

		enum
		{
			BUFFER_SIZE = 4096
		};

	private:
	};


}

#endif