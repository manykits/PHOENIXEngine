#ifndef _BUFFER_
#define _BUFFER_

#include "PX2Thread.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"

const int BUF_SIZE = 5;

typedef struct _sData
{
	_sData()
	{
		signal = 0;
		angle = 0.0;
		distance = 0.0;
	}
	uint8_t signal;
	float   angle;
	float   distance;

} sData;


class buffer
{
public:
    buffer(): p(0), c(0), full(0){}

	void put(std::vector<sData> &data);
	std::vector<sData> getSData();

private:
    PX2::Mutex mutex;
    unsigned int p, c, full;

	std::vector<sData> msData[BUF_SIZE];
};

#endif