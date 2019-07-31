// PX2VisibleSet.cpp

#include "PX2VisibleSet.hpp"
#include "PX2Renderable.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
VisibleSet::VisibleSet ()
    :
    mNumVisible(0),
		mSortFun(0)
{
}
//----------------------------------------------------------------------------
VisibleSet::~VisibleSet ()
{
}
//----------------------------------------------------------------------------
void VisibleSet::Insert (Renderable* visible)
{
	mVisible.push_back(visible);
    ++mNumVisible;
}
//----------------------------------------------------------------------------
VisibleSet &VisibleSet::Sort ()
{
	if ((int)mVisible.size() > 0)
	{
		if (mSortFun)
		{
			std::sort(mVisible.begin(), mVisible.end(), mSortFun);
		}
		else
		{
			std::sort(mVisible.begin(), mVisible.end(), Renderable::LessThan);
		}
	}
	return *this;
}
//----------------------------------------------------------------------------