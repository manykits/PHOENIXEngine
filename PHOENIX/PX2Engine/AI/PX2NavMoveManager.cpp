// PX2NavMoveManager.cpp

#include "PX2NavMoveManager.hpp"
#include "PX2NavGridMap.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2NavMoveUnit.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
MoveManager::MoveManager() :
mFrameTime(0.05f),
mAccumTime(0),
mMap(0)
{
}
//----------------------------------------------------------------------------
MoveManager::~MoveManager()
{
	size_t i;
	for (i = 0; i < mFreeUnits.size(); i++)
	{
		delete mFreeUnits[i];
	}
	for (i = 0; i < mUsedUnits.size(); i++)
	{
		delete mUsedUnits[i];
	}

	if (mMap)
	{
		delete mMap;
		mMap = 0;
	}
}
//----------------------------------------------------------------------------
void MoveManager::Clear()
{
	if (mMap)
	{
		delete mMap;
		mMap = 0;
	}

	mUsedUnits.clear();
	mFreeUnits.clear();
}
//----------------------------------------------------------------------------
void MoveManager::Reset(float xlen, float ylen, int xcells, int ycells)
{
	for (size_t i = 0; i < mUsedUnits.size(); i++)
		mFreeUnits.push_back(mUsedUnits[i]);

	mUsedUnits.resize(0);

	if (mMap)
	{
		delete mMap;
		mMap = 0;
	}

	mMap = new NavLogicMap;
	mMap->CreateGraph(int(xlen*FIXPOINT_SCALE), int(ylen*FIXPOINT_SCALE),
		xcells, ycells);
}
//----------------------------------------------------------------------------
struct MskData
{
	int version;
	int gridsize;
	int ngridx;
	int ngridy;
	unsigned char bits[1];
};
//----------------------------------------------------------------------------
void MoveManager::Reset(float xlen, float ylen, const std::string &mskpath)
{
	for (size_t i = 0; i < mUsedUnits.size(); i++)
		mFreeUnits.push_back(mUsedUnits[i]);
	mUsedUnits.resize(0);

	if (mMap)
	{
		delete mMap;
		mMap = 0;
	}

	mMap = new NavLogicMap;

	char *buffer;
	int bufferlen;
	if (PX2_RM.LoadBuffer(mskpath, bufferlen, buffer))
	{
		MskData *pmskdata = (MskData *)buffer;
		mMap->CreateGraph(int(xlen*FIXPOINT_SCALE), int(ylen*FIXPOINT_SCALE),
			pmskdata->ngridx, pmskdata->ngridy, pmskdata->bits);
	}
	else
	{
		mMap->CreateGraph(int(xlen*FIXPOINT_SCALE), int(ylen*FIXPOINT_SCALE),
			30, int(30 * (ylen / xlen)));
	}
}
//----------------------------------------------------------------------------
MoveUnit *MoveManager::NewUnit()
{
	MoveUnit *unit;
	if (mFreeUnits.empty())
	{
		unit = new MoveUnit;
	}
	else
	{
		unit = mFreeUnits.back();
		mFreeUnits.pop_back();
	}

	mUsedUnits.push_back(unit);

	return unit;
}
//----------------------------------------------------------------------------
void MoveManager::FreeUnit(MoveUnit *unit)
{
	std::vector<MoveUnit *>::iterator iter = mUsedUnits.begin();
	for (; iter != mUsedUnits.end(); iter++)
	{
		if (*iter == unit)
		{
			mUsedUnits.erase(iter);
			unit->LeaveMap();
			mFreeUnits.push_back(unit);
			return;
		}
	}
	assertion(false, "should not go here.");
}
//----------------------------------------------------------------------------
size_t MoveManager::GetNumUnit()
{
	return mUsedUnits.size();
}
//----------------------------------------------------------------------------
MoveUnit *MoveManager::GetIthUnit(size_t i)
{
	return mUsedUnits[i];
}
//----------------------------------------------------------------------------
void MoveManager::Update(float dtime)
{
	if (dtime > 0.5f) 
		dtime = 0.5f;
	
	mAccumTime += dtime;

	for (size_t i = 0; i < mUsedUnits.size(); i++)
	{
		mUsedUnits[i]->Update(dtime);
	}
}
//----------------------------------------------------------------------------
bool MoveManager::CanWalk(float x, float y)
{
	return mMap->CanWalkPixelCoord(int(x*FIXPOINT_SCALE), 
		int(y*FIXPOINT_SCALE));
}
//----------------------------------------------------------------------------
float MoveManager::GetGridSizeX()
{
	return mMap->mlMaskPixelWidth / FIXPOINT_SCALE;
}
//----------------------------------------------------------------------------
float MoveManager::GetGridSizeY()
{
	return mMap->mlMaskPixelHeight / FIXPOINT_SCALE;
}
//----------------------------------------------------------------------------
float MoveManager::GetGridNumX()
{
	return (float)mMap->mlWidthMasks;
}
//----------------------------------------------------------------------------
float MoveManager::GetGridNumY()
{
	return (float)mMap->mlHeightMasks;
}
//----------------------------------------------------------------------------
void MoveManager::LimitPointInRange(float &x, float &y)
{
	if (x < 0) 
		x = 0;
	if (y < 0)
		y = 0;

	float maxx = GetGridSizeX()*GetGridNumX() - 0.01f;
	float maxy = GetGridSizeY()*GetGridNumY() - 0.01f;

	if (x > maxx) 
		x = maxx;
	if (y > maxy) 
		y = maxy;
}
//----------------------------------------------------------------------------