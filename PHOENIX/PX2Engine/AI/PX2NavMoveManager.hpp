// PX2NavMoveManager.hpp

#ifndef PX2NAVMOVEMANAGER_HPP
#define PX2NAVMOVEMANAGER_HPP

#include "PX2CorePre.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class MoveUnit;
	class NavLogicMap;

	class MoveManager : public Singleton<MoveManager>
	{
	public:
		MoveManager();
		~MoveManager();

		void Clear();
		void Reset(float xlen, float ylen, int xcells, int ycells);
		void Reset(float xlen, float ylen, const std::string &mskpath);

		MoveUnit *NewUnit();
		void FreeUnit(MoveUnit *unit);

		std::size_t GetNumUnit();
		MoveUnit *GetIthUnit(std::size_t i);

		void Update(float dtime);

		bool CanWalk(float x, float y);
		float GetGridSizeX();
		float GetGridSizeY();
		float GetGridNumX();
		float GetGridNumY();
		void LimitPointInRange(float &x, float &y);
		NavLogicMap *GetMap();

	private:
		std::vector<MoveUnit *> mUsedUnits;
		std::vector<MoveUnit *> mFreeUnits;

		float mFrameTime;
		float mAccumTime;

		NavLogicMap *mMap;
	};

#include "PX2NavMoveManager.inl"

}

#endif