// PX2ObjectPool.hpp

#ifndef PX2OBJECTPOOL_HPP
#define PX2OBJECTPOOL_HPP

#include "PX2CorePre.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2FixMemoryPool.hpp"

namespace PX2
{

	/**
	* T 需要实现SetObjectID (uint32_t id)
	*/
	template <typename T>
	class ObjectPool
	{
	public:
		ObjectPool ();
		~ObjectPool ();

		T *AllocObject ();
		void FreeObject (uint32_t objID);
		T *GetObjectByID (uint32_t objID);
		const std::set<uint32_t> &GetAllObjectID();

	private:
		typedef std::map<uint32_t, T* > ObjectPoolObjMap;
		typedef ObjectPoolObjMap::iterator ObjectPoolObjMapIter;

		static uint32_t msIDCount;
		FixMemoryPool<T> mObjectAlloctor;
		ObjectPoolObjMap mObjects;
		std::set<uint32_t> mAllAlloctIDs;
		Mutex mMutex;
	};

#include "PX2ObjectPool.inl"

}

#endif