// PX2BtPre.hpp

#ifndef PX2BTPRE_HPP
#define PX2BTPRE_HPP

// 在cpp中包含，不要再头文件中包含，减少依赖
#pragma warning(push) 
#pragma warning(disable : 4127)
#pragma warning(disable : 4200)
#pragma warning(disable : 619)
#pragma warning(disable : 4100)
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#pragma warning(pop)

#endif