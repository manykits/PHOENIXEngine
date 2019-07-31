// PX2ObjectPool.inl

//----------------------------------------------------------------------------
template <typename T>
uint32_t ObjectPool<T>::msIDCount = 0;
//----------------------------------------------------------------------------
template <typename T>
ObjectPool<T>::ObjectPool ()
{
	msIDCount = 0;
}
//----------------------------------------------------------------------------
template <typename T>
ObjectPool<T>::~ObjectPool ()
{
	ObjectPoolObjMapIter iter = mObjects.begin();
	for (; iter!=mObjects.end(); ++iter)
	{
		iter->second->~T();
		mObjectAlloctor.FreeBlock(iter->second, sizeof(T));
	}
}
//----------------------------------------------------------------------------
template <typename T>
T *ObjectPool<T>::AllocObject ()
{
	ScopedCS cs(&mMutex);

	T *tmp = new (mObjectAlloctor.AllocBlock(sizeof(T)))T;
	if (tmp)
	{
		tmp->SetObjectID(msIDCount);
		mObjects[msIDCount] = tmp;
		mAllAlloctIDs.insert(msIDCount);
		msIDCount++;
		return tmp;
	}

	return 0;
}
//----------------------------------------------------------------------------
template <typename T>
void ObjectPool<T>::FreeObject (uint32_t objID)
{
	ScopedCS cs(&mMutex);

	ObjectPoolObjMapIter iter = mObjects.find(objID);
	if (iter != mObjects.end())
	{
		iter->second->~T();
		mObjectAlloctor.FreeBlock(iter->second, sizeof(T));
		mObjects.erase(iter);
		mAllAlloctIDs.erase(objID);
	}
}
//----------------------------------------------------------------------------
template <typename T>
T *ObjectPool<T>::GetObjectByID (uint32_t objID)
{
	ScopedCS cs(&mMutex);

	ObjectPoolObjMapIter iter = mObjects.find(objID);
	if (iter == mObjects.end())
		return 0;

	return iter->second;
}
//----------------------------------------------------------------------------
template <typename T>
const std::set<uint32_t> &ObjectPool<T>::GetAllObjectID()
{
	ScopedCS cs(&mMutex);

	return mAllAlloctIDs;
}
//----------------------------------------------------------------------------