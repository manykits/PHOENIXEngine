#ifndef _GRIDMAP_H_
#define _GRIDMAP_H_

#include "OccGridMapBase.h"
#include "GridMapLogOdds.h"
#include "GridMapReflectanceCount.h"
#include "GridMapSimpleCount.h"

namespace hectorslam {

typedef OccGridMapBase<LogOddsCell, GridMapLogOddsFunctions> GridMap;
}

#endif
