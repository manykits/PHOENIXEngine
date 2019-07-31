// PX2ThreadLocal.cpp

#include "PX2ThreadLocal.hpp"
#include "PX2Thread.hpp"
#include "PX2Memory.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// TLSAbstractSlot
//----------------------------------------------------------------------------
TLSAbstractSlot::TLSAbstractSlot()
{
}
//----------------------------------------------------------------------------
TLSAbstractSlot::~TLSAbstractSlot()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ThreadLocalStorage
//----------------------------------------------------------------------------
ThreadLocalStorage::ThreadLocalStorage()
{
}
//----------------------------------------------------------------------------
ThreadLocalStorage::~ThreadLocalStorage()
{
	for (TLSMap::iterator it = mMap.begin(); it != mMap.end(); ++it)
	{
		delete0(it->second);	
	}
}
//----------------------------------------------------------------------------
TLSAbstractSlot*& ThreadLocalStorage::Get(const void* key)
{
	TLSMap::iterator it = mMap.find(key);
	if (it == mMap.end())
		return mMap.insert(
		TLSMap::value_type(key, reinterpret_cast<TLSAbstractSlot*>(0))
		).first->second;

	return it->second;
}
//----------------------------------------------------------------------------
void ThreadLocalStorage::Clear()
{
}
//----------------------------------------------------------------------------