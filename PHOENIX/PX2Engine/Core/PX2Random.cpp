// PX2Random.cpp

#include "PX2Random.hpp"
#include "PX2Assert.hpp"
#include "PX2RandomStream.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
#define	TYPE_0		0		/* linear congruential */
#define	BREAK_0		8
#define	DEG_0		0
#define	SEP_0		0

#define	TYPE_1		1		/* x**7 + x**3 + 1 */
#define	BREAK_1		32
#define	DEG_1		7
#define	SEP_1		3

#define	TYPE_2		2		/* x**15 + x + 1 */
#define	BREAK_2		64
#define	DEG_2		15
#define	SEP_2		1

#define	TYPE_3		3		/* x**31 + x**3 + 1 */
#define	BREAK_3		128
#define	DEG_3		31
#define	SEP_3		3

#define	TYPE_4		4		/* x**63 + x + 1 */
#define	BREAK_4		256
#define	DEG_4		63
#define	SEP_4		1
//----------------------------------------------------------------------------
Random::Random(int stateSize)
{
	assertion(BREAK_0 <= stateSize && stateSize <= BREAK_4, "");

	_pBuffer = new char[stateSize];

	_InitState((uint32_t)std::time(NULL), _pBuffer, stateSize);
}
//----------------------------------------------------------------------------
Random::~Random()
{
	delete[] _pBuffer;
}
//----------------------------------------------------------------------------
inline uint32_t Random::_GoodRand(int32_t x)
{
	int32_t hi, lo;

	if (x == 0) x = 123459876;
	hi = x / 127773;
	lo = x % 127773;
	x = 16807 * lo - 2836 * hi;
	if (x < 0) x += 0x7FFFFFFF;

	return x;
}
//----------------------------------------------------------------------------
void Random::Seed(uint32_t x)
{
	int i, lim;

	_state[0] = x;
	if (_randType == TYPE_0)
		lim = NSHUFF;
	else
	{
		for (i = 1; i < _randDeg; i++)
			_state[i] = _GoodRand(_state[i - 1]);
		_fptr = &_state[_randSep];
		_rptr = &_state[0];
		lim = 10 * _randDeg;
	}
	for (i = 0; i < lim; i++)
		Next();
}
//----------------------------------------------------------------------------
void Random::Seed()
{
	std::streamsize len;

	if (_randType == TYPE_0)
		len = sizeof _state[0];
	else
		len = _randDeg * sizeof _state[0];

	RandomInputStream rstr;
	rstr.read((char*)_state, len);
}
//----------------------------------------------------------------------------
void Random::_InitState(uint32_t s, char* argState, int32_t n)
{
	uint32_t* intArgState = (uint32_t*)argState;

	if (n < BREAK_0)
	{
		assertion("not enough state", "");
		return;
	}
	if (n < BREAK_1)
	{
		_randType = TYPE_0;
		_randDeg = DEG_0;
		_randSep = SEP_0;
	}
	else if (n < BREAK_2)
	{
		_randType = TYPE_1;
		_randDeg = DEG_1;
		_randSep = SEP_1;
	}
	else if (n < BREAK_3)
	{
		_randType = TYPE_2;
		_randDeg = DEG_2;
		_randSep = SEP_2;
	}
	else if (n < BREAK_4)
	{
		_randType = TYPE_3;
		_randDeg = DEG_3;
		_randSep = SEP_3;
	}
	else
	{
		_randType = TYPE_4;
		_randDeg = DEG_4;
		_randSep = SEP_4;
	}
	_state = intArgState + 1; /* first location */
	_endPtr = &_state[_randDeg];	/* must set end_ptr before seed */
	Seed(s);
	if (_randType == TYPE_0)
		intArgState[0] = _randType;
	else
		intArgState[0] = MAX_TYPES * (int)(_rptr - _state) + _randType;
}
//----------------------------------------------------------------------------
uint32_t Random::Next()
{
	uint32_t i;
	uint32_t *f, *r;

	if (_randType == TYPE_0)
	{
		i = _state[0];
		_state[0] = i = _GoodRand(i) & 0x7FFFFFFF;
	}
	else
	{
		/*
		* Use local variables rather than static variables for speed.
		*/
		f = _fptr; r = _rptr;
		*f += *r;
		i = (*f >> 1) & 0x7FFFFFFF;	/* chucking least random bit */
		if (++f >= _endPtr) {
			f = _state;
			++r;
		}
		else if (++r >= _endPtr) {
			r = _state;
		}

		_fptr = f; _rptr = r;
	}
	return i;
}
//----------------------------------------------------------------------------