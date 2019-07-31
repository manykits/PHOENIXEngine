// PX2NotFreeObjectPool.inl

//----------------------------------------------------------------------------
template <typename T>
NotFreeObjectPool<T>::NotFreeObjectPool () :
mNumMaxObjects(0)
{
}
//----------------------------------------------------------------------------
template <typename T>
NotFreeObjectPool<T>::~NotFreeObjectPool ()
{
}
//----------------------------------------------------------------------------
template <typename T>
bool NotFreeObjectPool<T>::AllocAllObjects (uint32_t numMaxObjects, 
	const Any &initParam)
{
	mInitParam = initParam;

	while (!mIDsQueue.empty())
	{
		mIDsQueue.pop();
	}
	mObjects.clear();
	mObjectFlags.clear();

	mNumMaxObjects = numMaxObjects;

	for (uint32_t i=0; i<numMaxObjects; ++i)
	{
		mIDsQueue.push(i);

		T *tmp = new0 T();
		if (!tmp)
			return false;

		tmp->OnAllocAll(initParam);

		mObjects.push_back(tmp);
	}

	mObjectFlags.resize(mNumMaxObjects, 0);

	return true;
}
//----------------------------------------------------------------------------
template <typename T>
bool NotFreeObjectPool<T>::ClearAllObjects()
{
	while(!mIDsQueue.empty())
	{
		mIDsQueue.pop();
	}

	mObjects.clear();
	mObjectFlags.clear();

	return true;
}
//----------------------------------------------------------------------------
template <typename T>
uint32_t NotFreeObjectPool<T>::GetNumObjects ()
{
	return (uint32_t)mAllAlloctIDs.size();
}
//----------------------------------------------------------------------------
template <typename T>
uint32_t NotFreeObjectPool<T>::GetNumAllocedObjects ()
{
	return (uint32_t)mObjects.size();
}
//----------------------------------------------------------------------------
template <typename T>
T *NotFreeObjectPool<T>::GetAllocedObjectAt (uint32_t index)
{
	return mObjects[index];
}
//----------------------------------------------------------------------------
template <typename T>
int NotFreeObjectPool<T>::GetNumFree () const
{
	return (int)mIDsQueue.size();
}
//----------------------------------------------------------------------------
template <typename T>
T *NotFreeObjectPool<T>::AllocObject ()
{
	if (mIDsQueue.empty())
		return 0;

	uint32_t id = mIDsQueue.front();

	if (id<(uint32_t)mObjects.size() && 0==mObjectFlags[id])
	{
		mAllAlloctIDs.insert(id);
		mObjects[id]->OnAlloc();
		mObjects[id]->SetObjectID(id);
		mIDsQueue.pop();
		mObjectFlags[id] = 1;
		return mObjects[id];
	}

	return 0;
}
//----------------------------------------------------------------------------
template <typename T>
T *NotFreeObjectPool<T>::AddAllocedObject ()
{
	uint32_t id = (int32_t)mObjects.size();
	mIDsQueue.push(id);
	mObjectFlags.push_back(0);
	
	T *newTemp = new T();
	newTemp->OnAllocAll(mInitParam);
	mObjects.push_back(newTemp);

	mNumMaxObjects++;

	if (id<(int32_t)mObjects.size() && 0==mObjectFlags[id])
	{
		mAllAlloctIDs.insert(id);
		mObjects[id]->OnAlloc();
		mObjects[id]->SetObjectID(id);
		mIDsQueue.pop();
		mObjectFlags[id] = 1;
		return mObjects[id];
	}
	else
	{
		assertion(false, "AddAllocedObject failed.\n");
	}

	return 0;
}
//----------------------------------------------------------------------------
template <typename T>
void NotFreeObjectPool<T>::FreeObject (uint32_t objID)
{
	if (objID<(uint32_t)mObjects.size() && 0!=mObjectFlags[objID])
	{
		mObjects[objID]->OnFree();
		mObjects[objID]->SetObjectID(0);
		mIDsQueue.push(objID);
		mAllAlloctIDs.erase(objID);
		mObjectFlags[objID] = 0;
	}
}
//----------------------------------------------------------------------------
template <typename T>
void NotFreeObjectPool<T>::FreeAllAllocedObjects()
{
	std::set<uint32_t> ids = mAllAlloctIDs;

	std::set<uint32_t>::iterator it = ids.begin();
	for (; it != ids.end(); it++)
	{
		FreeObject(*it);
	}

	mAllAlloctIDs.clear();
}
//----------------------------------------------------------------------------
template <typename T>
const std::set<uint32_t> &NotFreeObjectPool<T>::GetAllObjectIDs()
{
	return mAllAlloctIDs;
}
//----------------------------------------------------------------------------
template <typename T>
T *NotFreeObjectPool<T>::GetObjectByID (uint32_t objID)
{
	if (objID<(uint32_t)mObjects.size() && 0!=mObjectFlags[objID])
		return mObjects[objID];

	return 0;
}
//----------------------------------------------------------------------------