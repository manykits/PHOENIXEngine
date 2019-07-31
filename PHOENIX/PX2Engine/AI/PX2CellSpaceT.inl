/*
*
* ÎÄ¼þÃû³Æ	£º	PX2CellSpaceT.inl
*
*/

//-----------------------------------------------------------------------------
// CellT
//-----------------------------------------------------------------------------
template <class T>
CellT<T>::CellT (const APoint &min, const APoint &max)
{
	AlignedBox = AxisAlignedBox3f(min, max);
}
//-----------------------------------------------------------------------------
template <class T>
CellT<T>::~CellT ()
{
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// CellSpaceT
//-----------------------------------------------------------------------------
template <class T>
CellSpaceT<T>::CellSpaceT (float length, float width, int numCellsLength,
	int numCellsWidth, int maxT)
	:
mSpaceLength(length),
mSpaceWidth(width),
mNumCellsLength(numCellsLength),
mNumCellsWidth(numCellsWidth),
mNeighbors(maxT, T())
{
	mCellLength = mSpaceLength/mNumCellsLength;
	mCellWidth = mSpaceWidth/mNumCellsWidth;

	for (int b=0; b<mNumCellsWidth; b++)
	{
		for (int a=0; a<mNumCellsLength; a++)
		{
			float left  = a * mCellLength;
			float right = left + mCellLength;
			float but   = b * mCellWidth;
			float top   = but + mCellWidth;

			APoint minPos(left, but, 0.0f);
			APoint maxPos(right, top, 0.0f);
			mCells.push_back(CellT<T>(minPos, maxPos));
		}
	}
}
//-----------------------------------------------------------------------------
template <class T>
void CellSpaceT<T>::Add (const T &t)
{
	int index = PositionToIndex(t->GetPosition());

	mCells[index].Members.push_back(t);
}
//-----------------------------------------------------------------------------
template <class T>
void CellSpaceT<T>::Remove (const T &t)
{
	int index = PositionToIndex(t->GetPosition());

	mCells[index].Members.remove(t);
}
//-----------------------------------------------------------------------------
template <class T>
void CellSpaceT<T>::Update (const T &t, const APoint &oldPos)
{
	int oldIdx = PositionToIndex(oldPos);
	int newIdx = PositionToIndex(t->GetPosition());

	if (newIdx == oldIdx)
		return;

	mCells[oldIdx].Members.remove(t);
	mCells[newIdx].Members.push_back(t);
}
//-----------------------------------------------------------------------------
template <class T>
void CellSpaceT<T>::CalculateNeighbors (const APoint &targetPos, float queryRadius)
{
	typename std::vector<T>::iterator curNbor = mNeighbors.begin();

	AxisAlignedBox3f queryBox(
		targetPos-AVector(queryRadius, queryRadius, queryRadius),
		targetPos+AVector(queryRadius, queryRadius, queryRadius));

	typename std::vector<CellT<T> >::iterator curCellIt = mCells.begin();
	for (; curCellIt!=mCells.end(); ++curCellIt)
	{
		if (curCellIt->AlignedBox.TestIntersection(queryBox) &&
			!curCellIt->Members.empty())
		{
			typename std::list<T>::iterator it = curCellIt->Members.begin();
			for (; it!=curCellIt->Members.end(); ++it)
			{
				AVector disDir = (*it)->GetPosition() - targetPos;
				float distanceSquared = disDir.SquaredLength();
				if (distanceSquared < queryRadius*queryRadius)
				{
					*curNbor++ = *it;
				}
			}
		}
	}

	*curNbor = 0;
}
//-----------------------------------------------------------------------------
template <class T>
T &CellSpaceT<T>::Begin ()
{
	mCurNeighbor = mNeighbors.begin(); 
	return *mCurNeighbor;
}
//-----------------------------------------------------------------------------
template <class T>
T &CellSpaceT<T>::Next ()
{
	++mCurNeighbor; 
	return *mCurNeighbor;
}
//-----------------------------------------------------------------------------
template <class T>
bool CellSpaceT<T>::End ()
{
	return (mCurNeighbor == mNeighbors.end()) || (*mCurNeighbor == 0);
}
//-----------------------------------------------------------------------------
template <class T>
void CellSpaceT<T>::EmptyCells ()
{
	typename std::vector<CellT<T> >::iterator it = mCells.begin();

	for (; it!=mCells.end(); ++it)
	{
		it->Members.clear();
	}
}
//-----------------------------------------------------------------------------
template <class T>
int CellSpaceT<T>::PositionToIndex (const APoint &pos) const
{
	float posA = pos.X();
	float posB = pos.Y();

	int index = (int)(posA/mCellLength) + 
		((int)(posB/mCellWidth) * mNumCellsLength);

	if (index > (int)mCells.size()-1)
		index = (int)mCells.size()-1;

	if (index < 0)
		index = 0;

	return index;
}
//-----------------------------------------------------------------------------