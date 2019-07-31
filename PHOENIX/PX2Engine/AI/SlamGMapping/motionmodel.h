#ifndef MOTIONMODEL_H
#define MOTIONMODEL_H

#include "point.h"
#include "stat.h"
#include "macro_params.h"

namespace  GMapping { 

/* 定义了运动采样模型 */
struct MotionModel
{
	OrientedPoint drawFromMotion(const OrientedPoint& p, double linearMove, double angularMove) const;
	OrientedPoint drawFromMotion(const OrientedPoint& p, const OrientedPoint& pnew, const OrientedPoint& pold) const;
	Covariance3 gaussianApproximation(const OrientedPoint& pnew, const OrientedPoint& pold) const;
	double srr, str, srt, stt;
};

};

#endif
