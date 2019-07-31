#include <list>
#include <iostream>
#include <cstring>
#include <limits>

#include "scanmatcher.h"
#include "gridlinetraversal.h"
//#define GENERATE_MAPS

namespace GMapping {

using namespace std;

const double ScanMatcher::nullLikelihood=-.5;

ScanMatcher::ScanMatcher(): m_laserPose(0,0,0)
{
	//m_laserAngles=0;
	m_laserBeams = 0;
	m_optRecursiveIterations = 3;
	m_activeAreaComputed = false;

	// This  are the dafault settings for a grid map of 5 cm
	m_llsamplerange=0.01;
	m_llsamplestep=0.01;
	m_lasamplerange=0.005;
	m_lasamplestep=0.005;
	m_enlargeStep=10.;
	m_fullnessThreshold=0.1;
	m_angularOdometryReliability=0.;
	m_linearOdometryReliability=0.;
	m_freeCellRatio=sqrt(2.);
	m_initialBeamsSkip=0;
	
/*	
	// This  are the dafault settings for a grid map of 10 cm
	m_llsamplerange=0.1;
	m_llsamplestep=0.1;
	m_lasamplerange=0.02;
	m_lasamplestep=0.01;
*/	
	// This  are the dafault settings for a grid map of 20/25 cm
/*
	m_llsamplerange=0.2;
	m_llsamplestep=0.1;
	m_lasamplerange=0.02;
	m_lasamplestep=0.01;
	m_generateMap=false;
*/

	m_linePoints = new IntPoint[20000];
}

ScanMatcher::~ScanMatcher(){
	delete [] m_linePoints;
}

void ScanMatcher::invalidateActiveArea(){
	m_activeAreaComputed=false;
}


/* 计算扫描区域  http://www.docin.com/p-1472709027.html */
/* 用的是 Bresenham 算法 */
/*
功能描述：计算活动区域
ScanMatcherMap& map,    地图数据
const OrientedPoint& p, 机器人的位姿
const double* readings  雷达数据

*/
void ScanMatcher::computeActiveArea(ScanMatcherMap& map, const OrientedPoint& p, const double* readings)
{
	if (m_activeAreaComputed)
		return;

	OrientedPoint lp=p;
	lp.x+=cos(p.theta)*m_laserPose.x-sin(p.theta)*m_laserPose.y;
	lp.y+=sin(p.theta)*m_laserPose.x+cos(p.theta)*m_laserPose.y;
	lp.theta+=m_laserPose.theta;
	IntPoint p0=map.world2map(lp);  /* p0 表示雷达在地图坐标系中的位置坐标 */
	
	Point min(map.map2world(0,0));
	Point max(map.map2world(map.getMapSizeX()-1,map.getMapSizeY()-1));
	       
	if (lp.x<min.x) min.x=lp.x;
	if (lp.y<min.y) min.y=lp.y;
	if (lp.x>max.x) max.x=lp.x;
	if (lp.y>max.y) max.y=lp.y;
	
	/*determine the size of the area*/
	/* 确定区域的大小 */
	const double * angle=m_laserAngles+m_initialBeamsSkip;
	for (const double* r=readings+m_initialBeamsSkip; r<readings+m_laserBeams; r++, angle++)
	{
		if (*r>m_laserMaxRange||*r==0.0)
			continue;

		double d=*r>m_usableRange?m_usableRange:*r;
		
		Point phit=lp;
		phit.x+=d*cos(lp.theta+*angle);
		phit.y+=d*sin(lp.theta+*angle);
		
		if (phit.x<min.x) min.x=phit.x;
		if (phit.y<min.y) min.y=phit.y;
		if (phit.x>max.x) max.x=phit.x;
		if (phit.y>max.y) max.y=phit.y;
	}

	/* 缩放地图的大小 */
	if ( !map.isInside(min)	|| !map.isInside(max))
	{
		Point lmin(map.map2world(0,0));
		Point lmax(map.map2world(map.getMapSizeX()-1,map.getMapSizeY()-1));
		min.x=( min.x >= lmin.x )? lmin.x: min.x-m_enlargeStep;
		max.x=( max.x <= lmax.x )? lmax.x: max.x+m_enlargeStep;
		min.y=( min.y >= lmin.y )? lmin.y: min.y-m_enlargeStep;
		max.y=( max.y <= lmax.y )? lmax.y: max.y+m_enlargeStep;
		map.resize(min.x, min.y, max.x, max.y);
	}
	
	HierarchicalArray2D<PointAccumulator>::PointSet activeArea;  /* 这是一个点集结构体, 只是用于记录位置坐标 */

	/*allocate the active area*/
	angle = m_laserAngles + m_initialBeamsSkip;
	
	for (const double* r=readings+m_initialBeamsSkip; r<readings+m_laserBeams; r++, angle++)
		if (m_generateMap)
		{
			double d=*r;

			if (d>m_laserMaxRange||d==0.0)
				continue;
			
			if (d>m_usableRange)
				d=m_usableRange;
			
			/* 直线经过的所有点被定义为 Active Area  */
			Point phit = lp+Point(d*cos(lp.theta+*angle),d*sin(lp.theta+*angle));
			IntPoint p0 = map.world2map(lp);
			IntPoint p1 = map.world2map(phit);

			GridLineTraversalLine line;
			line.points = m_linePoints;
			GridLineTraversal::gridLine(p0, p1, &line);

			for (int i=0; i<line.num_points-1; i++)
			{
				assert(map.isInside(m_linePoints[i]));
				activeArea.insert(map.storage().patchIndexes(m_linePoints[i]));
				assert(m_linePoints[i].x>=0 && m_linePoints[i].y>=0);
			}
			
			if (d<m_usableRange)
			{
				IntPoint cp=map.storage().patchIndexes(p1);
				assert(cp.x>=0 && cp.y>=0);
				activeArea.insert(cp);
			}
		}
		else 
		{
			if (*r>m_laserMaxRange||*r>m_usableRange||*r==0.0) continue;
			Point phit=lp;
			phit.x+=*r*cos(lp.theta+*angle);
			phit.y+=*r*sin(lp.theta+*angle);
			IntPoint p1=map.world2map(phit);
			assert(p1.x>=0 && p1.y>=0);
			IntPoint cp=map.storage().patchIndexes(p1);
			assert(cp.x>=0 && cp.y>=0);
			activeArea.insert(cp);
		}
		
	map.storage().setActiveArea(activeArea, true);
	m_activeAreaComputed=true;
}



/* 表示的信息熵的变化，变化越小，表示越准确 */
/*

ScanMatcherMap& map,     地图信息
const OrientedPoint& p,  机器人的姿态
const double* readings   激光雷达数据
说明：这里的返回值没用，这里主要是更新了地图中 cell 的概率问题，调用 cell.update 函数

*/
double ScanMatcher::registerScan(ScanMatcherMap& map, const OrientedPoint& p, const double* readings)
{
	if (!m_activeAreaComputed)
		computeActiveArea(map, p, readings);
		
	//this operation replicates the cells that will be changed in the registration operation
	map.storage().allocActiveArea();
	
	/* 雷达在机器人坐标系中的坐标 */
	OrientedPoint lp=p;
	lp.x+=cos(p.theta)*m_laserPose.x-sin(p.theta)*m_laserPose.y;
	lp.y+=sin(p.theta)*m_laserPose.x+cos(p.theta)*m_laserPose.y;
	lp.theta+=m_laserPose.theta;
	IntPoint p0=map.world2map(lp);
	
	const double * angle=m_laserAngles+m_initialBeamsSkip;
	double esum=0; /* 表示的信息熵的变化，变化越小，表示越准确 */
	
	for (const double* r=readings+m_initialBeamsSkip; r<readings+m_laserBeams; r++, angle++)
		if (m_generateMap)
		{
			double d=*r;
			
			if (d>m_laserMaxRange||d==0.0)
				continue;
			
			if (d>m_usableRange)
				d=m_usableRange;
			
			/* 注意，这里 phit 是世界坐标 */
			Point phit=lp+Point(d*cos(lp.theta+*angle),d*sin(lp.theta+*angle));
			IntPoint p1=map.world2map(phit);
			
			GridLineTraversalLine line;
			line.points=m_linePoints;
			
			/* 起点为 p0, 终点为 p1, 产生一条线, 他们都是地图坐标 */
			GridLineTraversal::gridLine(p0, p1, &line);
			
			/* 注意这里减 1 操作，都是非击中点位置, 也就是非障碍物区域 */
			for (int i=0; i<line.num_points-1; i++)
			{
				PointAccumulator& cell=map.cell(line.points[i]);
				double e = -cell.entropy();
				cell.update(false, Point(0,0));
				e+=cell.entropy();  /* 表示信息熵的变化 */
				esum+=e;
			}
			
			/* 障碍物区域 */
			if (d < m_usableRange)
			{
				double e =- map.cell(p1).entropy();
				map.cell(p1).update(true, phit); /* 地图坐标为为 p1 ,但是 phit 是世界坐标 */
				e += map.cell(p1).entropy();
				esum+=e;
			}
		} else {
			if (*r>m_laserMaxRange||*r>m_usableRange||*r==0.0) continue;
			Point phit=lp;
			phit.x+=*r*cos(lp.theta+*angle);
			phit.y+=*r*sin(lp.theta+*angle);
			IntPoint p1=map.world2map(phit);
			assert(p1.x>=0 && p1.y>=0);
			map.cell(p1).update(true,phit);
		}
	//cout  << "informationGain=" << -esum << endl;
	return esum;
}


/* 这个函数暂时可以不看 */
double ScanMatcher::icpOptimize(OrientedPoint& pnew, const ScanMatcherMap& map, const OrientedPoint& init, const double* readings) const
{
	double currentScore;
	double sc=score(map, init, readings);;
	OrientedPoint start=init;
	pnew=init;
	int iterations=0;
	do{
		currentScore=sc;
		sc=icpStep(pnew, map, start, readings);
		//cerr << "pstart=" << start.x << " " <<start.y << " " << start.theta << endl;
		//cerr << "pret=" << pnew.x << " " <<pnew.y << " " << pnew.theta << endl;
		start=pnew;
		iterations++;
	} while (sc>currentScore);
	cerr << "i="<< iterations << endl;
	return currentScore;
}

/* 返回最佳得分以及最佳位置 */
/*
功能：在一定的范围内进行匹配,目的是优化机器人的位姿，采用的是梯度下降扫描匹配方法
参数：
OrientedPoint& pnew,          优化后的机器人位置【需要返回】
const ScanMatcherMap& map,    地图数据
const OrientedPoint& init,    初始的机器人位置
const double* readings        扫描数据
返回：
	评分 best_score
*/
double ScanMatcher::optimize(OrientedPoint& pnew, const ScanMatcherMap& map, const OrientedPoint& init, const double* readings) const
{
	double bestScore=-1;
	
	OrientedPoint currentPose=init;
	
	/* 对当前的位置进行评估 */
	double currentScore = score(map, currentPose, readings);
	
	/* m_optAngularDelta 与 m_optLinearDelta 分别表示优化是的角度步长和线性移动步长 */
	double adelta = m_optAngularDelta, ldelta = m_optLinearDelta;
	
	unsigned int refinement=0;
	
	enum Move{Front, Back, Left, Right, TurnLeft, TurnRight, Done};

	int c_iterations=0; /* 表示迭代的次数 */
	
	do
	{
		if (bestScore >= currentScore) /*  在当前的位置附近，将进行步长减半的搜索最佳匹配 */
		{
			refinement++;   /* 记录迭代次数 */
			adelta *= .5;
			ldelta *= .5;
		}

		bestScore = currentScore;

		OrientedPoint bestLocalPose=currentPose;
		OrientedPoint localPose    =currentPose;

		Move move=Front;
		
		/* 多个方向进行迭代 */
		do 
		{
			localPose = currentPose;
			switch(move)
			{
				case Front:
					localPose.x += ldelta;
					move=Back;
					break;
				case Back:
					localPose.x-=ldelta;
					move=Left;
					break;
				case Left:
					localPose.y-=ldelta;
					move=Right;
					break;
				case Right:
					localPose.y+=ldelta;
					move=TurnLeft;
					break;
				case TurnLeft:
					localPose.theta+=adelta;
					move=TurnRight;
					break;
				case TurnRight:
					localPose.theta-=adelta;
					move=Done;
					break;
				default:;
			}
			
			double odo_gain = 1;
			
			/* 加权信息 m_angularOdometryReliability 缺省值为 0 */
			if (m_angularOdometryReliability>0.)
			{
				double dth= init.theta-localPose.theta; 	
				dth=atan2(sin(dth), cos(dth)); 	
				dth*=dth;
				
				odo_gain*=exp(-m_angularOdometryReliability*dth);
			}
			
			/* 加权信息 m_linearOdometryReliability 缺省值为 0 */
			if (m_linearOdometryReliability>0.)
			{
				double dx=init.x-localPose.x;
				double dy=init.y-localPose.y;
				double drho=dx*dx+dy*dy;
				odo_gain*=exp(-m_linearOdometryReliability*drho);
			}
			
			/* 进行评分处理 */
			double localScore = odo_gain*score(map, localPose, readings);
			
			if (localScore > currentScore)
			{
				currentScore  = localScore;
				bestLocalPose = localPose;
			}

			c_iterations++;
		} 
		while(move!=Done); /* 六种情况的迭代 */
		
		currentPose = bestLocalPose;

	}while (currentScore>bestScore || refinement<m_optRecursiveIterations); /* 外层循环，用迭代次数和最佳分数作为迭代条件 */

	pnew = currentPose;
	return bestScore;
}

struct ScoredMove{
	OrientedPoint pose;
	double score;
	double likelihood;
};

typedef std::list<ScoredMove> ScoredMoveList;

/* 优化匹配，_mean 是平均值， _cov 是协方差矩阵 */
double ScanMatcher::optimize(OrientedPoint& _mean, ScanMatcher::CovarianceMatrix& _cov, const ScanMatcherMap& map, const OrientedPoint& init, const double* readings) const{
	ScoredMoveList moveList;
	double bestScore=-1;
	OrientedPoint currentPose=init;
	ScoredMove sm={currentPose,0,0};
	
	/* sm.score 得分 sm.likelihood 似然， matched 匹配点的个数 */
	unsigned int matched=likelihoodAndScore(sm.score, sm.likelihood, map, currentPose, readings);
	
	double currentScore=sm.score;
	
	moveList.push_back(sm);
	
	double adelta=m_optAngularDelta, ldelta=m_optLinearDelta;
	unsigned int refinement=0;
	int count=0;
	enum Move{Front, Back, Left, Right, TurnLeft, TurnRight, Done};
	
	do{
		if (bestScore>=currentScore){
			refinement++;
			adelta*=.5;
			ldelta*=.5;
		}
		bestScore=currentScore;
//		cout <<"score="<< currentScore << " refinement=" << refinement;
//		cout <<  "pose=" << currentPose.x  << " " << currentPose.y << " " << currentPose.theta << endl;
		OrientedPoint bestLocalPose=currentPose;
		OrientedPoint localPose=currentPose;

		Move move=Front;
		do {
			localPose=currentPose;
			switch(move){
				case Front:
					localPose.x+=ldelta;
					move=Back;
					break;
				case Back:
					localPose.x-=ldelta;
					move=Left;
					break;
				case Left:
					localPose.y-=ldelta;
					move=Right;
					break;
				case Right:
					localPose.y+=ldelta;
					move=TurnLeft;
					break;
				case TurnLeft:
					localPose.theta+=adelta;
					move=TurnRight;
					break;
				case TurnRight:
					localPose.theta-=adelta;
					move=Done;
					break;
				default:;
			}
			double localScore, localLikelihood;
			
			double odo_gain=1;
			if (m_angularOdometryReliability>0.){
				double dth=init.theta-localPose.theta; 	dth=atan2(sin(dth), cos(dth)); 	dth*=dth;
				odo_gain*=exp(-m_angularOdometryReliability*dth);
			}
			if (m_linearOdometryReliability>0.){
				double dx=init.x-localPose.x;
				double dy=init.y-localPose.y;
				double drho=dx*dx+dy*dy;
				odo_gain*=exp(-m_linearOdometryReliability*drho);
			}
			localScore=odo_gain*score(map, localPose, readings);
			//update the score
			count++;
			matched=likelihoodAndScore(localScore, localLikelihood, map, localPose, readings);
			if (localScore>currentScore){
				currentScore=localScore;
				bestLocalPose=localPose;
			}
			sm.score=localScore;
			sm.likelihood=localLikelihood;//+log(odo_gain);
			sm.pose=localPose;
			moveList.push_back(sm); /* 存放到列表中 */
			//update the move list
		} while(move!=Done);
		currentPose=bestLocalPose;
		//cout << __PRETTY_FUNCTION__ << "currentScore=" << currentScore<< endl;
		//here we look for the best move;
	}while (currentScore>bestScore || refinement<m_optRecursiveIterations);
	//cout << __PRETTY_FUNCTION__ << "bestScore=" << bestScore<< endl;
	//cout << __PRETTY_FUNCTION__ << "iterations=" << count<< endl;
	
	//normalize the likelihood
	/* 对似然进行归一化处理 */
	double lmin=1e9;
	double lmax=-1e9;
	
	for (ScoredMoveList::const_iterator it=moveList.begin(); it!=moveList.end(); it++)
	{
		lmin=it->likelihood<lmin?it->likelihood:lmin; /* 计算最大值和最小值 */
		lmax=it->likelihood>lmax?it->likelihood:lmax;
	}
	
	//cout << "lmin=" << lmin << " lmax=" << lmax<< endl;
	/* 计算指数函数 */
	for (ScoredMoveList::iterator it=moveList.begin(); it!=moveList.end(); it++)
	{
		it->likelihood=exp(it->likelihood-lmax);
		//cout << "l=" << it->likelihood << endl;
	}
	
	//compute the mean
	/* 计算平均值 */
	OrientedPoint mean(0,0,0);
	double lacc=0;
	for (ScoredMoveList::const_iterator it=moveList.begin(); it!=moveList.end(); it++)
	{
		mean = mean+it->pose*it->likelihood;
		lacc+=it->likelihood;
	}
	
	mean=mean*(1./lacc);
	//OrientedPoint delta=mean-currentPose;
	//cout << "delta.x=" << delta.x << " delta.y=" << delta.y << " delta.theta=" << delta.theta << endl;
	
	/* 计算协方差矩阵 */
	CovarianceMatrix cov={0.,0.,0.,0.,0.,0.};
	
	for (ScoredMoveList::const_iterator it=moveList.begin(); it!=moveList.end(); it++)
	{
		OrientedPoint delta=it->pose-mean;
		delta.theta=atan2(sin(delta.theta), cos(delta.theta));
		cov.xx+=delta.x*delta.x*it->likelihood;
		cov.yy+=delta.y*delta.y*it->likelihood;
		cov.tt+=delta.theta*delta.theta*it->likelihood;
		cov.xy+=delta.x*delta.y*it->likelihood;
		cov.xt+=delta.x*delta.theta*it->likelihood;
		cov.yt+=delta.y*delta.theta*it->likelihood;
	}
	cov.xx/=lacc, cov.xy/=lacc, cov.xt/=lacc, cov.yy/=lacc, cov.yt/=lacc, cov.tt/=lacc;
	
	_mean=currentPose;
	_cov=cov;
	return bestScore;
}

/* 设置雷达参数 */
void ScanMatcher::setLaserParameters(unsigned int beams, double* angles, const OrientedPoint& lpose)
{
	assert(beams<LASER_MAXBEAMS);
	m_laserPose  = lpose;  /* 雷达相对于机器人的位置坐标, 由雷达数据结构体 RangeSensor 传入 */
	m_laserBeams = beams;
	memcpy(m_laserAngles, angles, sizeof(double)*m_laserBeams);	 /* 角度信息 */
}
	
/* 在一定的范围内计算似然函数, 纯激光 */
double ScanMatcher::likelihood(double& _lmax, OrientedPoint& _mean, CovarianceMatrix& _cov, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings)
{
	ScoredMoveList moveList;
	
	for (double xx=-m_llsamplerange; xx<=m_llsamplerange; xx+=m_llsamplestep)
	for (double yy=-m_llsamplerange; yy<=m_llsamplerange; yy+=m_llsamplestep)
	for (double tt=-m_lasamplerange; tt<=m_lasamplerange; tt+=m_lasamplestep)
	{
		
		OrientedPoint rp=p;
		rp.x+=xx;
		rp.y+=yy;
		rp.theta+=tt;
		
		ScoredMove sm;
		sm.pose=rp;
		
		likelihoodAndScore(sm.score, sm.likelihood, map, rp, readings);
		moveList.push_back(sm);
	}
	
	//OrientedPoint delta=mean-currentPose;
	//cout << "delta.x=" << delta.x << " delta.y=" << delta.y << " delta.theta=" << delta.theta << endl;
	//normalize the likelihood
	double lmax=-1e9;
	double lcum=0;
	for (ScoredMoveList::const_iterator it=moveList.begin(); it!=moveList.end(); it++){
		lmax=it->likelihood>lmax?it->likelihood:lmax;
	}
	for (ScoredMoveList::iterator it=moveList.begin(); it!=moveList.end(); it++){
		//it->likelihood=exp(it->likelihood-lmax);
		lcum+=exp(it->likelihood-lmax);
		it->likelihood=exp(it->likelihood-lmax);
		//cout << "l=" << it->likelihood << endl;
	}
	
	OrientedPoint mean(0,0,0);
	double s=0,c=0;
	for (ScoredMoveList::const_iterator it=moveList.begin(); it!=moveList.end(); it++){
		mean=mean+it->pose*it->likelihood;
		s+=it->likelihood*sin(it->pose.theta);
		c+=it->likelihood*cos(it->pose.theta);
	}
	mean=mean*(1./lcum);
	s/=lcum;
	c/=lcum;
	mean.theta=atan2(s,c);
	
	
	CovarianceMatrix cov={0.,0.,0.,0.,0.,0.};
	for (ScoredMoveList::const_iterator it=moveList.begin(); it!=moveList.end(); it++){
		OrientedPoint delta=it->pose-mean;
		delta.theta=atan2(sin(delta.theta), cos(delta.theta));
		cov.xx+=delta.x*delta.x*it->likelihood;
		cov.yy+=delta.y*delta.y*it->likelihood;
		cov.tt+=delta.theta*delta.theta*it->likelihood;
		cov.xy+=delta.x*delta.y*it->likelihood;
		cov.xt+=delta.x*delta.theta*it->likelihood;
		cov.yt+=delta.y*delta.theta*it->likelihood;
	}
	cov.xx/=lcum, cov.xy/=lcum, cov.xt/=lcum, cov.yy/=lcum, cov.yt/=lcum, cov.tt/=lcum;
	
	_mean=mean;
	_cov=cov;
	_lmax=lmax;
	return log(lcum)+lmax;
}


/* 在一定的范围内计算似然函数, 激光+里程计 */
double ScanMatcher::likelihood(double& _lmax, OrientedPoint& _mean, CovarianceMatrix& _cov, const ScanMatcherMap& map, const OrientedPoint& p,
	Gaussian3& odometry, const double* readings, double gain)
{
	ScoredMoveList moveList;
	
	
	for (double xx=-m_llsamplerange; xx<=m_llsamplerange; xx+=m_llsamplestep)
	for (double yy=-m_llsamplerange; yy<=m_llsamplerange; yy+=m_llsamplestep)
	for (double tt=-m_lasamplerange; tt<=m_lasamplerange; tt+=m_lasamplestep){
		
		OrientedPoint rp=p;
		rp.x+=xx;
		rp.y+=yy;
		rp.theta+=tt;
		
		ScoredMove sm;
		sm.pose=rp;
		
		likelihoodAndScore(sm.score, sm.likelihood, map, rp, readings);
		/* 计算里程计的似然函数 */
		sm.likelihood+=odometry.eval(rp)/gain;
		assert(!isnan(sm.likelihood));
		moveList.push_back(sm);
	}
	
	//OrientedPoint delta=mean-currentPose;
	//cout << "delta.x=" << delta.x << " delta.y=" << delta.y << " delta.theta=" << delta.theta << endl;
	//normalize the likelihood
	double lmax=-std::numeric_limits<double>::max();
	double lcum=0;
	
	for (ScoredMoveList::const_iterator it=moveList.begin(); it!=moveList.end(); it++)
	{
		lmax=it->likelihood>lmax?it->likelihood:lmax;
	}
	
	for (ScoredMoveList::iterator it=moveList.begin(); it!=moveList.end(); it++)
	{
		//it->likelihood=exp(it->likelihood-lmax);
		lcum+=exp(it->likelihood-lmax);
		it->likelihood=exp(it->likelihood-lmax);
		//cout << "l=" << it->likelihood << endl;
	}
	
	OrientedPoint mean(0,0,0);
	double s=0,c=0;
	
	for (ScoredMoveList::const_iterator it=moveList.begin(); it!=moveList.end(); it++)
	{
		mean=mean+it->pose*it->likelihood;
		s+=it->likelihood*sin(it->pose.theta);
		c+=it->likelihood*cos(it->pose.theta);
	}
	
	mean=mean*(1./lcum);
	s/=lcum;
	c/=lcum;
	
	mean.theta=atan2(s,c);
	
	
	CovarianceMatrix cov={0.,0.,0.,0.,0.,0.};
	
	for (ScoredMoveList::const_iterator it=moveList.begin(); it!=moveList.end(); it++)
	{
		OrientedPoint delta=it->pose-mean;
		delta.theta=atan2(sin(delta.theta), cos(delta.theta));
		cov.xx+=delta.x*delta.x*it->likelihood;
		cov.yy+=delta.y*delta.y*it->likelihood;
		cov.tt+=delta.theta*delta.theta*it->likelihood;
		cov.xy+=delta.x*delta.y*it->likelihood;
		cov.xt+=delta.x*delta.theta*it->likelihood;
		cov.yt+=delta.y*delta.theta*it->likelihood;
	}
	cov.xx/=lcum, cov.xy/=lcum, cov.xt/=lcum, cov.yy/=lcum, cov.yt/=lcum, cov.tt/=lcum;
	
	_mean=mean;
	_cov=cov;
	_lmax=lmax;
	double v=log(lcum)+lmax;
	assert(!isnan(v));
	return v;
}

/* 设置 scan match 的参数信息 */
void ScanMatcher::setMatchingParameters	(double urange, double range, double sigma, int kernsize, double lopt, double aopt, int iterations,  double likelihoodSigma, unsigned int likelihoodSkip)
{
	m_usableRange = urange;
	m_laserMaxRange = range;
	m_kernelSize = kernsize;
	m_optLinearDelta = lopt;
	m_optAngularDelta = aopt;
	m_optRecursiveIterations = iterations;
	m_gaussianSigma = sigma;
	m_likelihoodSigma = likelihoodSigma;
	m_likelihoodSkip = likelihoodSkip;
}

};

