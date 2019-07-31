// PX2ObjectCopyPool.cpp

#include "PX2ObjectCopyPool.hpp"
#include "PX2Log.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ObjectCopyPool::ObjectCopyPool()
{
}
//----------------------------------------------------------------------------
ObjectCopyPool::~ObjectCopyPool()
{
}
//----------------------------------------------------------------------------
void ObjectCopyPool::PreAlloc (Object *prototype, int num)
{
	Pointer0<ObjectPool> pool(new0 ObjectPool);
	pool->ProtoType = prototype;

	for (int i=0; i<num; i++)
	{
		Object *obj = prototype->Copy("");
		pool->FreeObjs.push_back(obj);
	}

	mPools[prototype] = pool;
}
//----------------------------------------------------------------------------
ObjectPtr ObjectCopyPool::Alloc (Object *prototype)
{
	Pointer0<ObjectPool> pool = mPools[prototype];
	assertion(pool->ProtoType==prototype, "should be equal.\n");

	if(pool->FreeObjs.size() == 0)
	{
		ObjectPtr p(prototype->Copy(""));
		return p;
	}

	if(pool->FreeObjs.size() > 0)
	{
		ObjectPtr p = pool->FreeObjs.back();
		pool->FreeObjs.pop_back();
		return p;
	}

	PX2_LOG_ERROR("new pool object: %s", prototype->GetResourcePath().c_str());

	return 0;
}
//----------------------------------------------------------------------------
void ObjectCopyPool::Free (Object *prototype, Object *pobj)
{
	Pointer0<ObjectPool> pool = mPools[prototype];
	assertion(pool->ProtoType==prototype, "should be equal.\n");

	pool->FreeObjs.push_back(pobj);
}
//----------------------------------------------------------------------------