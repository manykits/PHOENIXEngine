#ifndef SCANMATCHER_H
#define SCANMATCHER_H

#include "icp.h"
#include "smmap.h"
#include "macro_params.h"
#include "stat.h"
#include <iostream>
#include "gvalues.h"
#include "sse_mathfun.h"


#define LASER_MAXBEAMS 1024


namespace GMapping {

class ScanMatcher{

	public:
		typedef Covariance3 CovarianceMatrix;
		
		ScanMatcher();
		~ScanMatcher();
		double icpOptimize(OrientedPoint& pnew, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		double optimize(OrientedPoint& pnew, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		double optimize(OrientedPoint& mean, CovarianceMatrix& cov, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		
		double   registerScan(ScanMatcherMap& map, const OrientedPoint& p, const double* readings);
		void setLaserParameters
			(unsigned int beams, double* angles, const OrientedPoint& lpose);
		void setMatchingParameters
			(double urange, double range, double sigma, int kernsize, double lopt, double aopt, int iterations, double likelihoodSigma=1, unsigned int likelihoodSkip=0 );
		void invalidateActiveArea();
		void computeActiveArea(ScanMatcherMap& map, const OrientedPoint& p, const double* readings);

        //////remote worker///////
        // bool computeActiveAreaRemote(ScanMatcherMap& map, const OrientedPoint& p, const double* readings,IntPoint& min_, IntPoint& max_);
        //////////////////////////


		inline double icpStep(OrientedPoint & pret, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		inline double score(const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;

		inline double score_org(const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;


		inline unsigned int likelihoodAndScore(double& s, double& l, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const;
		double likelihood(double& lmax, OrientedPoint& mean, CovarianceMatrix& cov, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings);
		double likelihood(double& _lmax, OrientedPoint& _mean, CovarianceMatrix& _cov, const ScanMatcherMap& map, const OrientedPoint& p, Gaussian3& odometry, const double* readings, double gain=180.);
		inline const double* laserAngles() const { return m_laserAngles; }
		inline unsigned int laserBeams() const { return m_laserBeams; }
		
		static const double nullLikelihood;
        double  m_laserAngles[LASER_MAXBEAMS];

        unsigned int m_laserBeams;
	protected:
		//state of the matcher
		bool m_activeAreaComputed;
		
		/**laser parameters*/
        //unsigned int m_laserBeams;
        //double       m_laserAngles[LASER_MAXBEAMS];
		
		//OrientedPoint m_laserPose;
		PARAM_SET_GET(OrientedPoint, laserPose, protected, public, public)
		PARAM_SET_GET(double, laserMaxRange, protected, public, public)
		/**scan_matcher parameters*/
		PARAM_SET_GET(double, usableRange, protected, public, public)
		PARAM_SET_GET(double, gaussianSigma, protected, public, public)
		PARAM_SET_GET(double, likelihoodSigma, protected, public, public)
		PARAM_SET_GET(int,    kernelSize, protected, public, public)
		PARAM_SET_GET(double, optAngularDelta, protected, public, public)
		PARAM_SET_GET(double, optLinearDelta, protected, public, public)
		PARAM_SET_GET(unsigned int, optRecursiveIterations, protected, public, public)
		PARAM_SET_GET(unsigned int, likelihoodSkip, protected, public, public)
		PARAM_SET_GET(double, llsamplerange, protected, public, public)
		PARAM_SET_GET(double, llsamplestep, protected, public, public)
		PARAM_SET_GET(double, lasamplerange, protected, public, public)
		PARAM_SET_GET(double, lasamplestep, protected, public, public)
		PARAM_SET_GET(bool, generateMap, protected, public, public)
		PARAM_SET_GET(double, enlargeStep, protected, public, public)
		PARAM_SET_GET(double, fullnessThreshold, protected, public, public)
		PARAM_SET_GET(double, angularOdometryReliability, protected, public, public)
		PARAM_SET_GET(double, linearOdometryReliability, protected, public, public)
		PARAM_SET_GET(double, freeCellRatio, protected, public, public)
		PARAM_SET_GET(unsigned int, initialBeamsSkip, protected, public, public)

		// allocate this large array only once
		IntPoint* m_linePoints;
};

/* p 代表机器人在世界坐标系中的位置坐标 */
inline double ScanMatcher::icpStep(OrientedPoint & pret, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const
{
	const double * angle=m_laserAngles+m_initialBeamsSkip;
	OrientedPoint lp = p;
	
	/* m_laserPose 表示激光雷达在机器人坐标系中的坐标 */
	lp.x+=cos(p.theta)*m_laserPose.x-sin(p.theta)*m_laserPose.y;
	lp.y+=sin(p.theta)*m_laserPose.x+cos(p.theta)*m_laserPose.y;
	lp.theta+=m_laserPose.theta;
	
	unsigned int skip=0;
	double freeDelta = map.getDelta()*m_freeCellRatio;
	
	std::list<PointPair> pairs;
	
	for (const double* r=readings+m_initialBeamsSkip; r<readings+m_laserBeams; r++, angle++)
	{
		skip++;
		skip=skip>m_likelihoodSkip?0:skip;
		
		if (skip||*r>m_usableRange||*r==0.0) continue;
		
		/* 激光点的位置坐标 */
		Point phit=lp;
		phit.x+=*r*cos(lp.theta+*angle);
		phit.y+=*r*sin(lp.theta+*angle);
		IntPoint iphit=map.world2map(phit);
	
		Point pfree=lp;
		pfree.x+=(*r-map.getDelta()*freeDelta)*cos(lp.theta+*angle);
		pfree.y+=(*r-map.getDelta()*freeDelta)*sin(lp.theta+*angle);
 		pfree=pfree-phit;
		IntPoint ipfree=map.world2map(pfree);
		
		bool found=false;
		Point bestMu(0.,0.);
		Point bestCell(0.,0.);
		
		for (int xx=-m_kernelSize; xx<=m_kernelSize; xx++)
		for (int yy=-m_kernelSize; yy<=m_kernelSize; yy++)
		{
			IntPoint pr=iphit+IntPoint(xx,yy);
			IntPoint pf=pr+ipfree;
			//AccessibilityState s=map.storage().cellState(pr);
			//if (s&Inside && s&Allocated){
				const PointAccumulator& cell=map.cell(pr);
				const PointAccumulator& fcell=map.cell(pf);
				if (((double)cell )> m_fullnessThreshold && ((double)fcell )<m_fullnessThreshold){
					Point mu=phit-cell.mean(); 
					
					if (!found){ /* 第一次 */
						bestMu = mu;
						bestCell = cell.mean();
						found = true;
					}else
						if((mu*mu)<(bestMu*bestMu))
						{
							bestMu = mu;
							bestCell = cell.mean();
						}
				}
			//}
		}
		if (found){
			pairs.push_back(std::make_pair(phit, bestCell));
			//std::cerr << "(" << phit.x-bestCell.x << "," << phit.y-bestCell.y << ") ";
		}
		//std::cerr << std::endl;
	}
	
	OrientedPoint result(0,0,0);
	//double icpError=icpNonlinearStep(result,pairs);
	std::cerr << "result(" << pairs.size() << ")=" << result.x << " " << result.y << " " << result.theta << std::endl;
	pret.x=p.x+result.x;
	pret.y=p.y+result.y;
	pret.theta=p.theta+result.theta;
	pret.theta=atan2(sin(pret.theta), cos(pret.theta));
	return score(map, p, readings);
}


/* 计算得分 */
/*
功能：对机器人的位姿进行评价
const ScanMatcherMap& map,  地图数据
const OrientedPoint& p,     机器人位姿
const double* readings      雷达数据
【返回值】:   得分
*/
inline double ScanMatcher::score_org(const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const
{
	double s=0;
	const double * angle = m_laserAngles + m_initialBeamsSkip;  /* m_initialBeamsSkip 的缺省值为 0 */

	OrientedPoint lp = p;

	/* 激光雷达在世界坐标系中的位置坐标 */
	lp.x+=cos(p.theta)*m_laserPose.x-sin(p.theta)*m_laserPose.y;
	lp.y+=sin(p.theta)*m_laserPose.x+cos(p.theta)*m_laserPose.y;
	lp.theta+=m_laserPose.theta;

	unsigned int skip=0;
	
	/* 解释：m_delta 相当于地图的分辨率，m_freeCellRatio 是 cell 的比例 */
	double freeDelta=map.getDelta()*m_freeCellRatio;

	/* 对每一个激光束找一个最匹配 cell，并计算其分数 */
	for (const double* r=readings+m_initialBeamsSkip; r<readings+m_laserBeams; r++, angle++)
	{
		skip++;
		skip = skip>m_likelihoodSkip ? 0 : skip;         /* m_likelihoodSkip 的缺省值为 0 */

		if (skip||*r>m_usableRange||*r==0.0) continue;   /* 忽略一些异常的激光点 */
		
		Point phit = lp;
		
		phit.x += *r*cos(lp.theta+*angle);     /* 达到障碍物上的激光点在世界坐标系中的位置关系 */
		phit.y += *r*sin(lp.theta+*angle);
		
		IntPoint iphit = map.world2map(phit);  /* 世界坐标转换成地图坐标， phit 为世界坐标     */
		
		Point pfree = lp;
		/* map.getDelta()*freeDelta，这里的乘法是对的，主要是考虑到地图分辨率的问题 */
		pfree.x += (*r-map.getDelta()*freeDelta)*cos(lp.theta+*angle);
		pfree.y += (*r-map.getDelta()*freeDelta)*sin(lp.theta+*angle);
 		
		pfree = pfree-phit;  /* 这里 pfree 是负数 */
		
		IntPoint ipfree=map.world2map(pfree); /* 世界坐标转换成地图坐标， ipfree 为世界坐标     */
		
		bool  found = false;
		Point bestMu(0.,0.);
		
		/* m_kernelSize 这里的缺省值为 1 */
		for (int xx=-m_kernelSize; xx<=m_kernelSize; xx++)
		for (int yy=-m_kernelSize; yy<=m_kernelSize; yy++)
		{
			IntPoint pr = iphit+IntPoint(xx,yy);
			IntPoint pf = pr + ipfree;  /* 沿激光线方向，计算空白区域 free 点 */

			/* 这里要理解 cell 的含义     */
			/* 这里 pr 和 pf 都是地图坐标 */
			const PointAccumulator& cell =map.cell(pr);
			const PointAccumulator& fcell=map.cell(pf);
			
			/* m_fullnessThreshold=0.1; */
			/* double 计算的是 occupy 的概率 */
			if (((double)cell )> m_fullnessThreshold && ((double)fcell ) < m_fullnessThreshold)
			{
				/* 这里 phit 是世界坐标， cell.mean() 计算的也是世界坐标的平均值 */
				Point mu = phit - cell.mean();
				if (!found)
				{
					bestMu = mu;
					found  = true;
				}
				else
					bestMu = (mu*mu) < (bestMu*bestMu) ? mu:bestMu;
			}
		}
		
		/* 理解加法的含义， m_gaussianSigma 的值为 0.05 */
		if (found)
			s+=exp(-1./m_gaussianSigma*bestMu*bestMu);   /* 这里就是高斯函数的计算，只是忽略了常数项 */
	}

	return s;
}


/*
////////////////////////////////////////////////////
// 优化版本
// 作者：沈燕飞
// 2017.1.13
////////////////////////////////////////////////////
功能：对机器人的位姿进行评价
const ScanMatcherMap& map,  地图数据
const OrientedPoint& p,     机器人位姿
const double* readings      雷达数据
【返回值】:   得分
*/
inline double ScanMatcher::score(const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const
{
	double s = 0;
	const double * angle = m_laserAngles + m_initialBeamsSkip;  /* m_initialBeamsSkip 的缺省值为 0 */

	float sin_table[LASER_MAXBEAMS];
	float cos_table[LASER_MAXBEAMS];


	OrientedPoint lp = p;

	/* 激光雷达在世界坐标系中的位置坐标 */
	lp.x += cos(p.theta)*m_laserPose.x - sin(p.theta)*m_laserPose.y;
	lp.y += sin(p.theta)*m_laserPose.x + cos(p.theta)*m_laserPose.y;
	lp.theta += m_laserPose.theta;

	unsigned int skip = 0;

	/* 解释：m_delta 相当于地图的分辨率，m_freeCellRatio 是 cell 的比例 */
	double freeDelta = map.getDelta()*m_freeCellRatio;

	// v4sf cos_ps(v4sf x)
	//for (int i = 0; i < m_laserBeams; i++)
	//{
	//	sin_table[i] = sin(lp.theta + angle[i]);
	//	cos_table[i] = cos(lp.theta + angle[i]);
	//}


	v4sf  in_a;
	v4sf  s_a;
	v4sf  c_a;

	for (int i = 0; i < m_laserBeams; i+=4)
	{
		in_a.m128_f32[0] = lp.theta + angle[i + 0];
		in_a.m128_f32[1] = lp.theta + angle[i + 1];
		in_a.m128_f32[2] = lp.theta + angle[i + 2];
		in_a.m128_f32[3] = lp.theta + angle[i + 3];
		
		sincos_ps(in_a, &s_a, &c_a);
		cos_table[i + 0] = c_a.m128_f32[0];
		cos_table[i + 1] = c_a.m128_f32[1];
		cos_table[i + 2] = c_a.m128_f32[2];
		cos_table[i + 3] = c_a.m128_f32[3];
		sin_table[i + 0] = s_a.m128_f32[0];
		sin_table[i + 1] = s_a.m128_f32[1];
		sin_table[i + 2] = s_a.m128_f32[2];
		sin_table[i + 3] = s_a.m128_f32[3];
	}

	/* 对每一个激光束找一个最匹配 cell，并计算其分数 */
	int i = 0;
	for (const double* r = readings + m_initialBeamsSkip; r<readings + m_laserBeams; r++, angle++,i++)
	{
		skip++;
		skip = skip>m_likelihoodSkip ? 0 : skip;         /* m_likelihoodSkip 的缺省值为 0 */

		if (skip || *r>m_usableRange || *r == 0.0) continue;   /* 忽略一些异常的激光点 */

		Point phit = lp;

		//phit.x += *r*cos(lp.theta + *angle);     /* 达到障碍物上的激光点在世界坐标系中的位置关系 */
		//phit.y += *r*sin(lp.theta + *angle);
		phit.x += *r*cos_table[i];     /* 达到障碍物上的激光点在世界坐标系中的位置关系 */
		phit.y += *r*sin_table[i];

		IntPoint iphit = map.world2map(phit);  /* 世界坐标转换成地图坐标， phit 为世界坐标     */

		Point pfree = lp;
		/* map.getDelta()*freeDelta，这里的乘法是对的，主要是考虑到地图分辨率的问题 */
		//pfree.x += (*r - map.getDelta()*freeDelta)*cos(lp.theta + *angle);
		//pfree.y += (*r - map.getDelta()*freeDelta)*sin(lp.theta + *angle);
		pfree.x += (*r - map.getDelta()*freeDelta)*cos_table[i];
		pfree.y += (*r - map.getDelta()*freeDelta)*sin_table[i];

		pfree = pfree - phit;  /* 这里 pfree 是负数 */

		IntPoint ipfree = map.world2map(pfree); /* 世界坐标转换成地图坐标， ipfree 为世界坐标     */

		bool  found = false;
		Point bestMu(0., 0.);

		/* m_kernelSize 这里的缺省值为 1 */
		for (int xx = -m_kernelSize; xx <= m_kernelSize; xx++)
			for (int yy = -m_kernelSize; yy <= m_kernelSize; yy++)
			{
				IntPoint pr = iphit + IntPoint(xx, yy);
				IntPoint pf = pr + ipfree;  /* 沿激光线方向，计算空白区域 free 点 */

				/* 这里要理解 cell 的含义     */
				/* 这里 pr 和 pf 都是地图坐标 */
				const PointAccumulator& cell  = map.cell(pr);
				const PointAccumulator& fcell = map.cell(pf);

				/* m_fullnessThreshold=0.1; */
				/* double 计算的是 occupy 的概率 */
				// if (((double)cell)> m_fullnessThreshold && ((double)fcell) < m_fullnessThreshold)
				if ((cell.op > 0.1) && (fcell.op < 0.1))
				{
					/* 这里 phit 是世界坐标， cell.mean() 计算的也是世界坐标的平均值 */
					// Point mu = phit - cell.mean();
					Point mu = phit - cell.mean_p;

					if (!found)
					{
						bestMu = mu;
						found = true;
					}
					else
						bestMu = (mu*mu) < (bestMu*bestMu) ? mu : bestMu;
				}
			}

		/* 理解加法的含义， m_gaussianSigma 的值为 0.05 */
		if (found)
			s += exp(-1. / m_gaussianSigma*bestMu*bestMu);   /* 这里就是高斯函数的计算，只是忽略了常数项 */
	}

	return s;
}



/* 计算似然和得分, s 是得分，l 是似然值 */
/*
功能：计算似然和得分
参数：double& s, double& l,     得分和似然值【返回值】
	  const ScanMatcherMap& map, 地图信息
	  const OrientedPoint& p,    当前机器人的位姿
	  const double* readings
返回值：匹配点的个数
*/
inline unsigned int ScanMatcher::likelihoodAndScore(double& s, double& l, const ScanMatcherMap& map, const OrientedPoint& p, const double* readings) const
{
	using namespace std;
	l=0;
	s=0;
	
	const double * angle=m_laserAngles+m_initialBeamsSkip;
	
	OrientedPoint lp=p;
	
	/* 将激光机的坐标转换成世界坐标 */
	lp.x+=cos(p.theta)*m_laserPose.x-sin(p.theta)*m_laserPose.y;
	lp.y+=sin(p.theta)*m_laserPose.x+cos(p.theta)*m_laserPose.y;
	lp.theta+=m_laserPose.theta;
	
	/* const double ScanMatcher::nullLikelihood = -.5 */
	double noHit=nullLikelihood/(m_likelihoodSigma);
	unsigned int skip=0;
	unsigned int c=0;
	double freeDelta = map.getDelta()*m_freeCellRatio;
	
	for (const double* r=readings+m_initialBeamsSkip; r<readings+m_laserBeams; r++, angle++)
	{
		skip++;
		skip=skip>m_likelihoodSkip?0:skip;

		if (*r>m_usableRange) continue;

		if (skip)
			continue;
		
		Point phit=lp;
		
		phit.x+=*r*cos(lp.theta+*angle);
		phit.y+=*r*sin(lp.theta+*angle);
		IntPoint iphit=map.world2map(phit);
		
		Point pfree=lp;
		pfree.x+=(*r-freeDelta)*cos(lp.theta+*angle);
		pfree.y+=(*r-freeDelta)*sin(lp.theta+*angle);
		pfree=pfree-phit;
		
		IntPoint ipfree=map.world2map(pfree);
		bool found=false;
		Point bestMu(0.,0.); /* 记录的是距离 */
		
		for (int xx=-m_kernelSize; xx<=m_kernelSize; xx++)
			for (int yy=-m_kernelSize; yy<=m_kernelSize; yy++)
			{
				IntPoint pr=iphit+IntPoint(xx,yy);
				IntPoint pf=pr+ipfree;
				const PointAccumulator& cell=map.cell(pr);
				const PointAccumulator& fcell=map.cell(pf);
				if (((double)cell )>m_fullnessThreshold && ((double)fcell )<m_fullnessThreshold){
					Point mu=phit-cell.mean();
					if (!found){
						bestMu=mu;
						found=true;
					}else
						bestMu=(mu*mu)<(bestMu*bestMu)?mu:bestMu;
			}
		}
		/* bestMu 距离越大，评分越低，似然越小，但是  */
		if (found){
			s+=exp(-1./m_gaussianSigma*bestMu*bestMu);
			c++;  /* 表示匹配点的个数 */
		}
		if (!skip){
			double f=(-1./m_likelihoodSigma)*(bestMu*bestMu);
			l+=(found)?f:noHit; /* 返回似然值 */
		}
	}
	return c;
}

};

#endif
