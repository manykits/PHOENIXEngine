// PX2Random.hpp

#ifndef PX2RANDOM_HPP
#define PX2RANDOM_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Random
	{
	public:
		enum Type
		{
			RND_STATE_0 = 8,  /// linear congruential
			RND_STATE_32 = 32,  /// x**7 + x**3 + 1
			RND_STATE_64 = 64,  /// x**15 + x + 1
			RND_STATE_128 = 128,  /// x**31 + x**3 + 1
			RND_STATE_256 = 256   /// x**63 + x + 1
		};

		Random(int stateSize = 256);
		~Random();

		void Seed(uint32_t seed);
		void Seed();

		uint32_t Next();
		uint32_t Next(uint32_t n);
		char NextChar();
		bool NextBool();
		float NextFloat();
		double NextDouble();

	protected:
		void _InitState(uint32_t seed, char* arg_state, int32_t n);
		static uint32_t _GoodRand(int32_t x);

	private:
		enum
		{
			MAX_TYPES = 5,
			NSHUFF = 50
		};

		uint32_t* _fptr;
		uint32_t* _rptr;
		uint32_t* _state;
		int     _randType;
		int     _randDeg;
		int     _randSep;
		uint32_t* _endPtr;
		char*  _pBuffer;
	};

#include "PX2Random.inl"

}

#endif