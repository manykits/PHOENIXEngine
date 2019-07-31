// PX2Singleton.hpp

#ifndef PX2SINGLETON_NEEDNEW_HPP
#define PX2SINGLETON_NEEDNEW_HPP

#include "PX2Assert.hpp"

namespace PX2
{

	/// 模板单件类
	template <typename TYPE>
	class Singleton
	{
	public:
		Singleton ()
		{
			assertion(!msSingleton, "msSingleton must be null.");

			int64_t offset = (int64_t)(TYPE*)1 - (int64_t)(Singleton<TYPE>*)(TYPE*)1;
			msSingleton = (TYPE*)((int64_t)this + offset);
		}

		~Singleton ()
		{
			assertion(msSingleton!=0, "msSingleton mustn't be null.");
		}

		static void Set (TYPE *object)
		{
			msSingleton = object;
		}

		static TYPE &GetSingleton ()
		{
			assertion(msSingleton!=0, "msSingleton mustn't be null.");
			return (*msSingleton);
		}

		static TYPE *GetSingletonPtr ()
		{
			return msSingleton;
		}

	private:
		static TYPE *msSingleton;
	};

	template <typename TYPE>
	TYPE *Singleton<TYPE>::msSingleton = 0;

}

#endif