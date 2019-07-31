// PX2ThreadLocal.hpp

#ifndef PX2THREADLOCAL_HPP
#define PX2THREADLOCAL_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TLSAbstractSlot
	{
	public:
		TLSAbstractSlot();
		virtual ~TLSAbstractSlot();
	};

	class PX2_ENGINE_ITEM ThreadLocalStorage
	{
	public:
		ThreadLocalStorage();
		~ThreadLocalStorage();

		TLSAbstractSlot*& Get(const void* key);
		static void Clear();

	private:
		typedef std::map<const void*, TLSAbstractSlot*> TLSMap;
		TLSMap mMap;

		friend class Thread;
	};


}

#endif