// PX2BufferAllocator.hpp

#ifndef PX2BUFFERALLOCATOR_HPP
#define PX2BUFFERALLOCATOR_HPP

namespace PX2
{

	template <typename ch>
	class BufferAllocator
	{
	public:
		typedef ch char_type;

		static char_type* allocate(std::streamsize size)
		{
			return new char_type[static_cast<std::size_t>(size)];
		}

		static void deallocate(char_type* ptr, std::streamsize /*size*/) throw()
		{
			delete[] ptr;
		}
	};

}

#endif