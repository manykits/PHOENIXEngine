// PX2AISteeringPath.hpp

#ifndef PX2AISTEERINGPATH_HPP
#define PX2AISTEERINGPATH_HPP

#include "PX2CorePre.hpp"
#include "PX2Vector3.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM AISteeringPath
	{
	public:
		AISteeringPath();
		~AISteeringPath();

		Vector3f CurrentWaypoint() const;
		bool Finished();
		inline void SetNextWaypoint();

		void LoopOn() { m_bLooped = true; }
		void LoopOff() { m_bLooped = false; }

		void AddWayPoint(const APoint &new_point);

		void Set(std::list<Vector3f> new_path) { mWayPoints = new_path; curWaypoint = mWayPoints.begin(); }
		void Set(const AISteeringPath& path) { mWayPoints = path.GetPath(); curWaypoint = mWayPoints.begin(); }

		void Clear() { mWayPoints.clear(); }

		std::list<Vector3f> GetPath()const { return mWayPoints; }

		void Update();

	private:
		std::list<Vector3f> mWayPoints;
		std::list<Vector3f>::iterator  curWaypoint;
		bool m_bLooped;

	};
	//----------------------------------------------------------------------------
	inline void AISteeringPath::SetNextWaypoint()
	{
		assert(mWayPoints.size() > 0);

		if (++curWaypoint == mWayPoints.end())
		{
			if (m_bLooped)
			{
				curWaypoint = mWayPoints.begin();
			}
		}
	}
	//----------------------------------------------------------------------------

}

#endif