// PX2AddDeleteManager.cpp

#include "PX2AddDeleteManager.hpp"
#include "PX2Time.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
AddingObj::AddingObj() :
UpdateTime(0.0f),
DelayTime(0.0f),
DoResetPlay(true)
{
}
//----------------------------------------------------------------------------
AddingObj::~AddingObj()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
DeletingObj::DeletingObj() :
UpdateTime(0.0f),
StopDelayTime(0.0f),
DeleteDelayTime(0.0f)
{
}
//----------------------------------------------------------------------------
DeletingObj::~DeletingObj()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
float AddDeleteManager::msFreqElapseTemp = 0.03f;
//----------------------------------------------------------------------------
AddDeleteManager::AddDeleteManager() :
mFreqElapsingTime(msFreqElapseTemp)
{

}
//----------------------------------------------------------------------------
AddDeleteManager::~AddDeleteManager()
{
}
//----------------------------------------------------------------------------
void AddDeleteManager::Clear()
{
	mAddingObjs.clear();

	mStopingObjs.clear();
	mDeletingObjs.clear();
}
//----------------------------------------------------------------------------
void AddDeleteManager::AddResetPlayObj(Movable *mov, float delayTime)
{
	AddingObj *addingObj = new0 AddingObj();
	addingObj->TheMov = mov;
	addingObj->DelayTime = delayTime;
	addingObj->DoResetPlay = true;

	mAddingObjs.push_back(addingObj);
}
//----------------------------------------------------------------------------
void AddDeleteManager::AddAddingObj(Node *parent, Movable *mov, float delayTime,
	bool doResetPlay)
{
	AddingObj *addingObj = new0 AddingObj();
	addingObj->TheNode = parent;
	addingObj->TheMov = mov;
	addingObj->DelayTime = delayTime;
	addingObj->DoResetPlay = doResetPlay;

	mAddingObjs.push_back(addingObj);
}
//----------------------------------------------------------------------------
void AddDeleteManager::AddDeletingObj(Movable *mov, float stopDelayTime,
	float deleteAfterStopDelayTime)
{
	DeletingObj *deletingObj = new0 DeletingObj();
	deletingObj->Mov = mov;
	deletingObj->TheParent = (Node*)mov->GetParent();
	deletingObj->StopDelayTime = stopDelayTime;
	deletingObj->DeleteDelayTime = stopDelayTime + deleteAfterStopDelayTime;

	mStopingObjs.push_back(deletingObj);
}
//----------------------------------------------------------------------------
void AddDeleteManager::Update(float elapsedSeconds)
{
	// 更新频率控制
	mFreqElapsingTime -= elapsedSeconds;
	if (mFreqElapsingTime > 0.0f)
		return;
	else
	{
		elapsedSeconds = msFreqElapseTemp + Mathf::FAbs(mFreqElapsingTime);
		mFreqElapsingTime = msFreqElapseTemp;
	}

	// 加入更新
	{
		std::list<AddingObjPtr>::iterator it = mAddingObjs.begin();
		for (; it != mAddingObjs.end();)
		{
			(*it)->UpdateTime += elapsedSeconds;

			if ((*it)->UpdateTime > (*it)->DelayTime)
			{
				if ((*it)->TheNode)
				{
					if ((*it)->TheMov)
						(*it)->TheNode->AttachChild((*it)->TheMov);

					if ((*it)->DoResetPlay)
					{
						(*it)->TheMov->ResetPlay();
					}
				}

				it = mAddingObjs.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	// 停止更新
	{
		std::list<DeletingObjPtr>::iterator it = mStopingObjs.begin();

		for (; it != mStopingObjs.end();)
		{
			(*it)->UpdateTime += elapsedSeconds;

			if ((*it)->UpdateTime > (*it)->StopDelayTime)
			{
				if ((*it)->Mov)
				{
					(*it)->Mov->Pause();
				}

				mDeletingObjs.push_back(*it);

				it = mStopingObjs.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	// 删除更新
	{
		std::list<DeletingObjPtr>::iterator it1 = mDeletingObjs.begin();
		for (; it1 != mDeletingObjs.end();)
		{
			(*it1)->UpdateTime += elapsedSeconds;

			if ((*it1)->UpdateTime > (*it1)->DeleteDelayTime)
			{
				if ((*it1)->Mov && (*it1)->TheParent)
				{
					(*it1)->TheParent->DetachChild((*it1)->Mov);
				}

				it1 = mDeletingObjs.erase(it1);
			}
			else
			{
				it1++;
			}
		}
	}
}
//----------------------------------------------------------------------------