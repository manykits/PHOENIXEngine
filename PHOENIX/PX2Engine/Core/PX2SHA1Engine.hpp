// PX2SHA1Engine.hpp

#ifndef PX2SHA1ENGINE_HPP
#define PX2SHA1ENGINE_HPP

#include "PX2CorePre.hpp"
#include "PX2DigestEngine.hpp"

namespace PX2 
{

	class PX2_ENGINE_ITEM SHA1Engine : public DigestEngine
	{
	public:
		enum
		{
			BLOCK_SIZE = 64,
			DIGEST_SIZE = 20
		};

		SHA1Engine();
		~SHA1Engine();

		std::size_t digestLength() const;
		void reset();
		const DigestEngine::Digest& digest();

	protected:
		void updateImpl(const void* data, std::size_t length);

	private:
		void transform();
		static void byteReverse(uint32_t* buffer, int byteCount);

		typedef int8_t BYTE;

		struct Context
		{
			uint32_t digest[5]; // Message digest
			uint32_t countLo;   // 64-bit bit count
			uint32_t countHi;
			uint32_t data[16];  // SHA data buffer
			uint32_t slop;      // # of bytes saved in data[]
		};

		Context _context;
		DigestEngine::Digest _digest;

		SHA1Engine(const SHA1Engine&);
		SHA1Engine& operator = (const SHA1Engine&);
	};

}

#endif
