// PX2AILidarData.hpp

#ifndef PX2AILIDARDATA_HPP
#define PX2AILIDARDATA_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class RslidarDataComplete
	{
	public:
		RslidarDataComplete();

		int signal;
		float angle;
		float distance;

		static bool LessThan(const RslidarDataComplete &data0,
			const RslidarDataComplete &data1)
		{
			if (data0.angle == data1.angle)
			{
				return data0.distance < data1.distance;
			}

			return data0.angle < data1.angle;
		}
	};

}

#endif