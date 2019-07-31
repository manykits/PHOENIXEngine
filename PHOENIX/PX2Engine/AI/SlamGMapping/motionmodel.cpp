#include "motionmodel.h"
#include "stat.h"
#include <iostream>

#define MotionModelConditioningLinearCovariance 0.01
#define MotionModelConditioningAngularCovariance 0.001

namespace GMapping 
{

OrientedPoint 
MotionModel::drawFromMotion (const OrientedPoint& p, double linearMove, double angularMove) const
{
	OrientedPoint n(p);
	double lm=linearMove  + fabs( linearMove ) * sampleGaussian( srr ) + fabs( angularMove ) * sampleGaussian( str );
	double am=angularMove + fabs( linearMove ) * sampleGaussian( srt ) + fabs( angularMove ) * sampleGaussian( stt );

	n.x += lm*cos(n.theta+.5*am);
	n.y += lm*sin(n.theta+.5*am);

	n.theta += am;
	n.theta = atan2(sin(n.theta), cos(n.theta));

	return n;
}

/* 
功能：进行运动采样 
const OrientedPoint& p,     现在机器人位置
const OrientedPoint& pnew,  新的机器人位姿
const OrientedPoint& pold， 旧的机器人位姿
*/
OrientedPoint
MotionModel::drawFromMotion(const OrientedPoint& p, const OrientedPoint& pnew, const OrientedPoint& pold) const
{
	double sxy=0.3*srr;
	OrientedPoint delta = absoluteDifference(pnew, pold);
	OrientedPoint noisypoint(delta);

	noisypoint.x+=sampleGaussian(srr*fabs(delta.x)+str*fabs(delta.theta)+sxy*fabs(delta.y));
	noisypoint.y+=sampleGaussian(srr*fabs(delta.y)+str*fabs(delta.theta)+sxy*fabs(delta.x));
	noisypoint.theta+=sampleGaussian(stt*fabs(delta.theta)+srt*sqrt(delta.x*delta.x+delta.y*delta.y));
	noisypoint.theta=fmod(noisypoint.theta, 2*M_PI); /* 取模运算 */

	if (noisypoint.theta>M_PI)
		noisypoint.theta-=2*M_PI; /* 转换到 -PI 到 +PI 之间 */

	return absoluteSum(p,noisypoint);
}

/* 计算协方差 */
Covariance3 MotionModel::gaussianApproximation(const OrientedPoint& pnew, const OrientedPoint& pold) const
{
	OrientedPoint delta=absoluteDifference(pnew,pold);

	double linearMove=sqrt(delta.x*delta.x+delta.y*delta.y);
	double angularMove=fabs(delta.x);

	double s11=srr*srr*linearMove*linearMove;
	double s22=stt*stt*angularMove*angularMove;
	double s12=str*angularMove*srt*linearMove;

	Covariance3 cov;
	double s=sin(pold.theta),c=cos(pold.theta);

	cov.xx=c*c*s11+MotionModelConditioningLinearCovariance;
	cov.yy=s*s*s11+MotionModelConditioningLinearCovariance;
	cov.tt=s22+MotionModelConditioningAngularCovariance;

	cov.xy=s*c*s11;
	cov.xt=c*s12;
	cov.yt=s*s12;

	return cov;
}

};

