// PX2TRecyclingArray.inl

//----------------------------------------------------------------------------
template <class T>
TRecyclingArray<T>::TRecyclingArray(int quantity)
{
	mQuantity = quantity;
	mUseds = new1<bool>(mQuantity);
	mElements = new1<T>(mQuantity);

	for (int i=0; i<mQuantity; i++)
		mUseds[i] = false;

	mUsedQuantity = 0;
	mIndexLastNew = 0;
}
//----------------------------------------------------------------------------
template <class T>
TRecyclingArray<T>::~TRecyclingArray()
{
	if (mUseds)
		delete1(mUseds);
	if (mElements)
		delete1(mElements);
}
//----------------------------------------------------------------------------
template <class T>
T *TRecyclingArray<T>::New ()
{
	if (GetFreeQuantity() < 1)
		return 0;

	int index = mIndexLastNew;

	for (int i=0; i<mQuantity; i++)
	{
		if (!mUseds[index])
		{
			break;
		}
		else
		{
			index++;
			if (index >= mQuantity)
				index = 0;
		}
	}

	if (mUseds[index])
		return 0;

	mElements[index] = mNewTemplate;
	mUsedQuantity++;
	mUseds[index] = true;
	mIndexLastNew = index;

	return &mElements[index];
}
//----------------------------------------------------------------------------
template <class T>
int TRecyclingArray<T>::GetLastNewIndex ()
{
	return mIndexLastNew;
}
//----------------------------------------------------------------------------
template <class T>
bool TRecyclingArray<T>::Delete (int index)
{
	if (index<0 || index>=mQuantity || !mUseds[index])
		return false;

	mUseds[index] = false;
	mUsedQuantity--;

	return true;
}
//----------------------------------------------------------------------------
template <class T>
bool TRecyclingArray<T>::Delete (T *element)
{
	if (mUsedQuantity == 0)
		return false;

	int64_t elem = (int64_t)(element);
	elem -= (int64_t)(&mElements[0]);
	int64_t index = elem / sizeof(T);

	if (index<0 || index>=mQuantity || !mUseds[index])
		return false;

	mUseds[index] = false;
	mUsedQuantity--;

	return true;
}
//----------------------------------------------------------------------------
template <class T>
void TRecyclingArray<T>::DeleteAll ()
{
	for (int i=0; i<mQuantity; i++)
		mUseds[i] = false;

	mUsedQuantity = 0;
	mIndexLastNew = 0;
}
//----------------------------------------------------------------------------
template <class T>
int TRecyclingArray<T>::GetFreeQuantity ()
{
	return mQuantity - GetUsedQuantity();
}
//----------------------------------------------------------------------------
template <class T>
int TRecyclingArray<T>::GetUsedQuantity ()
{
	return mUsedQuantity;
}
//----------------------------------------------------------------------------
template <class T>
int TRecyclingArray<T>::GetQuantity ()
{
	return mQuantity;
}
//----------------------------------------------------------------------------
template <class T>
T &TRecyclingArray<T>::GetAt (int index)
{
	return mElements[index];
}
//----------------------------------------------------------------------------
template <class T>
bool TRecyclingArray<T>::IsUsed (int index)
{
	return mUseds[index];
}
//----------------------------------------------------------------------------