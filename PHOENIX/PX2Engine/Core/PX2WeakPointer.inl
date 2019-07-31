// PX2WeakPointer.inl

//----------------------------------------------------------------------------
// WeakPointer0
//----------------------------------------------------------------------------
template <typename T>
WeakPointer0<T>::WeakPointer0 (T* data)
	:
mData(0)
{
	msMutex.Enter();

	if (data)
	{
		RMIterator iter = msMap.find(data);
		if (iter != msMap.end())
		{
			mData = data;
		}
		else
		{
			mData = data;
			//assertion(false, "data must be in the map\n");
		}
	}

	msMutex.Leave();
}
//----------------------------------------------------------------------------
template <typename T>
WeakPointer0<T>::WeakPointer0 (const Pointer0<T>& pointer)
	:
mData(0)
{
	msMutex.Enter();

	T *data = pointer.mData;
	if (data)
	{
		RMIterator iter = msMap.find(data);
		if (iter != msMap.end())
		{
			mData = pointer.mData;
		}
		else
		{
			assertion(false, "pointer.mData must be in the map\n");
		}
	}

	msMutex.Leave();
}
//----------------------------------------------------------------------------
template <typename T>
WeakPointer0<T>::WeakPointer0 (const WeakPointer0& pointer)
	:
mData(0)
{
	msMutex.Enter();

	T *data = pointer.mData;
	if (data)
	{
		RMIterator iter = msMap.find(data);
		if (iter != msMap.end())
		{
			mData = pointer.mData;
		}
		else
		{
			mData = pointer.mData;
			//assertion(false, "pointer.mData must be in the map\n");
		}
	}

	msMutex.Leave();
}
//----------------------------------------------------------------------------
template <typename T>
WeakPointer0<T>::~WeakPointer0 ()
{
	mData = 0;
}
//----------------------------------------------------------------------------
template <typename T>
inline WeakPointer0<T>::operator T* () const
{
	return mData;
}
//----------------------------------------------------------------------------
template <typename T>
inline T* WeakPointer0<T>::operator-> () const
{
	return mData;
}
//----------------------------------------------------------------------------
template <typename T>
WeakPointer0<T>& WeakPointer0<T>::operator= (T* data)
{
	msMutex.Enter();

	if (mData != data)
	{
		if (data)
		{
			RMIterator iter;
			iter = msMap.find(data);
			if (iter != msMap.end())
			{
				mData = data;
			}
			else
			{
				mData = data;
				//assertion(false, "data must be in the map\n");
			}
		}
	}

	msMutex.Leave();
	return *this;
}
//----------------------------------------------------------------------------
template <typename T>
WeakPointer0<T>& WeakPointer0<T>::operator= (const Pointer0<T>& pointer)
{
	msMutex.Enter();

	if (mData != pointer.mData)
	{
		if (pointer.mData)
		{
			RMIterator iter;
			iter = msMap.find(pointer.mData);
			if (iter != msMap.end())
			{
				mData = pointer.mData;
			}
			else
			{
				assertion(false, "pointer.mData must be in the map\n");
			}
		}
	}

	msMutex.Leave();
	return *this;
}
//----------------------------------------------------------------------------
template <typename T>
WeakPointer0<T>& WeakPointer0<T>::operator= (const WeakPointer0& pointer)
{
	msMutex.Enter();

	if (mData != pointer.mData)
	{
		if (pointer.mData)
		{
			RMIterator iter;
			iter = msMap.find(pointer.mData);
			if (iter != msMap.end())
			{
				mData = pointer.mData;
			}
			else
			{
				mData = pointer.mData;
				//assertion(false, "pointer.mData must be in the map\n");
			}
		}
	}

	msMutex.Leave();
	return *this;
}
//----------------------------------------------------------------------------
template <typename T>
inline bool WeakPointer0<T>::operator== (T* data) const
{
	return mData == data;
}
//----------------------------------------------------------------------------
template <typename T>
inline bool WeakPointer0<T>::operator!= (T* data) const
{
	return mData != data;
}
//----------------------------------------------------------------------------
template <typename T>
inline bool WeakPointer0<T>::operator== (const Pointer0<T>& pointer) const
{
	return mData == pointer.mData;
}
//----------------------------------------------------------------------------
template <typename T>
inline bool WeakPointer0<T>::operator!= (const Pointer0<T>& pointer) const
{
	return mData != pointer.mData;
}
//----------------------------------------------------------------------------
template <typename T>
inline bool WeakPointer0<T>::operator== (const WeakPointer0& pointer) const
{
	return mData == pointer.mData;
}
//----------------------------------------------------------------------------
template <typename T>
inline bool WeakPointer0<T>::operator!= (const WeakPointer0& pointer) const
{
	return mData != pointer.mData;
}
//----------------------------------------------------------------------------
template <typename T>
inline bool WeakPointer0<T>::IsExpired() const
{
	bool isExpired = false;

	msMutex.Enter();

	if (!mData)
	{
		isExpired = true;
	}
	else
	{
		RMIterator iter;
		iter = msMap.find(mData);
		if (iter != msMap.end())
		{
			isExpired = false;
		}
		else
		{
			isExpired = true;
		}
	}

	msMutex.Leave();

	return isExpired;
}
//----------------------------------------------------------------------------