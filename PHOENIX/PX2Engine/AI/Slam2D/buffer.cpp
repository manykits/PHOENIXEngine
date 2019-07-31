

#include "PX2Thread.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"
#include "buffer.h"
using namespace PX2;

void buffer::put(std::vector<sData> &data)
{
	ScopedCS lock(&mutex);

	if (full == BUF_SIZE)
	{
		p = 0;
		c = 0;
		full = 0;
	}
	msData[p] = data;
	p = (p + 1) % BUF_SIZE;
	full++;
}

std::vector<sData> buffer::getSData()
{
	ScopedCS lock(&mutex);

	if (full == 0)
	{
		return std::vector<sData>();
	}
	std::vector<sData> result = msData[c];
	c = (c + 1) % BUF_SIZE;
	full--;
	return result;
}