// PX2SingletonHolder.hpp

#ifndef PX2SINGLETONHOLDER_HPP
#define PX2SINGLETONHOLDER_HPP

#include "PX2CorePre.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Mutex.hpp"

namespace PX2
{

	template <class S>
	class SingletonHolder
	{
	public:
		SingletonHolder() :
			_pS(0)
		{
		}

		~SingletonHolder()
		{
			delete _pS;
		}

		S* Get()
		{
			ScopedCS lock(&_m);
			if (!_pS) _pS = new S;
			return _pS;
		}

		void Reset()
		{
			ScopedCS lock(&_m);
			delete _pS;
			_pS = 0;
		}

	private:
		S* _pS;
		Mutex _m;
	};

}

#endif