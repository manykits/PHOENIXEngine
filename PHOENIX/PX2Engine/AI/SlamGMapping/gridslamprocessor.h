#ifndef GRIDSLAMPROCESSOR_H
#define GRIDSLAMPROCESSOR_H

#include <climits>
#include <limits>
#include <fstream>
#include <vector>
#include <deque>
#include "particlefilter.h"
#include "point.h"
#include "macro_params.h"
#include "sensorlog.h"
#include "smmap.h"
#include "rangesensor.h"
#include "rangereading.h"
#include "scanmatcher.h"
#include "motionmodel.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <fstream>

#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>


namespace GMapping {


struct TNode;

/**This class defines a particle of the filter. Each particle has a map, a pose, a weight and retains the current node in the trajectory tree*/
/* 这个类定义了滤波去粒子 */
struct Particle
{
    /**constructs a particle, given a map
 @param map: the particle map
     */
    Particle(const ScanMatcherMap& map):
        map(map), pose(0, 0, 0), weight(0), weightSum(0), gweight(0), previousIndex(0)
	{
        node = 0;
    }

    /** @returns the weight of a particle */
    inline operator double() const {return weight;}
    /** @returns the pose of a particle */
    inline operator OrientedPoint() const {return pose;}
    /** sets the weight of a particle
  @param w the weight
     */
    inline void setWeight(double w) 
	{
		weight=w;
	}
    
	/** The map */
    ScanMatcherMap map;
    
	/** The pose of the robot */
    OrientedPoint pose;

    /** The pose of the robot at the previous time frame (used for computing thr odometry displacements) */
    OrientedPoint previousPose;

    /** The weight of the particle */
    double weight;

    /** The cumulative weight of the particle */
	/* 累加粒子的权重 */
    double weightSum;

    double gweight; /* 这个变量好像没有使用 */

    /** The index of the previous particle in the trajectory tree */
    int previousIndex;

    /** Entry to the trajectory tree */
    TNode* node;
};

typedef std::vector<Particle> ParticleVector;


/**This class defines the the node of reversed tree in which the trajectories are stored.
   Each node of a tree has a pointer to its parent and a counter indicating the number of childs of a node.
   The tree is updated in a way consistent with the operation performed on the particles.
 */
struct TNode{
    /**Constructs a node of the trajectory tree.
   @param pose:      the pose of the robot in the trajectory
   @param weight:    the weight of the particle at that point in the trajectory
   @param accWeight: the cumulative weight of the particle
   @param parent:    the parent node in the tree
   @param childs:    the number of childs
     */
    TNode(const OrientedPoint& pose, double weight, TNode* parent=0, unsigned int childs=0);

    /**Destroys a tree node, and consistently updates the tree. If a node whose parent has only one child is deleted,
   also the parent node is deleted. This because the parent will not be reacheable anymore in the trajectory tree.*/
    ~TNode();

    /**The pose of the robot*/
    OrientedPoint pose;

    /**The weight of the particle*/
    double weight;

    /**The sum of all the particle weights in the previous part of the trajectory*/
    /* 轨迹的前半部分，所有粒子的权重之和 */
	double accWeight;

	double gweight; /* 这个变量好像没有使用 */

    /**The parent*/
    TNode* parent;

    /**The range reading to which this node is associated*/
    const RangeReading* reading;

    /**The number of childs*/
    unsigned int childs;

    /**counter in visiting the node (internally used)*/
	/* 访问这个节点的次数 */
    mutable unsigned int visitCounter;

    /**visit flag (internally used)*/
    mutable bool flag;
};

typedef std::vector<TNode*> TNodeVector;
typedef std::deque<TNode*> TNodeDeque;




/**This class defines the basic GridFastSLAM algorithm.  It
     implements a rao blackwellized particle filter. Each particle
     has its own map and robot pose.<br> This implementation works
     as follows: each time a new pair odometry/laser reading is
     received, the particle's robot pose is updated according to the
     motion model.  This pose is subsequently used for initalizing a
     scan matching algorithm.  The scanmatcher performs a local
     optimization for each particle.  It is initialized with the
     pose drawn from the motion model, and the pose is corrected
     according to the each particle map.<br>
     In order to avoid unnecessary computation the filter state is updated
     only when the robot moves more than a given threshold.
 */
class GridSlamProcessor{
public:


    /** Constructs a GridSlamProcessor, initialized with the default parameters */
    GridSlamProcessor();

    /** Constructs a GridSlamProcessor, whose output is routed to a stream.
     @param infoStr: the output stream
     */
    GridSlamProcessor(std::ostream& infoStr);

    /** @returns  a deep copy of the grid slam processor with all the internal structures.
     */
    GridSlamProcessor* clone() const;

    /**Deleted the gridslamprocessor*/
    virtual ~GridSlamProcessor();

    //methods for accessing the parameters
    void setSensorMap(const SensorMap& smap);
    void init(unsigned int size, double xmin, double ymin, double xmax, double ymax, double delta,
              OrientedPoint initialPose=OrientedPoint(0,0,0));
    void setMatchingParameters(double urange, double range, double sigma, int kernsize, double lopt, double aopt,
                               int iterations, double likelihoodSigma=1, double likelihoodGain=1, unsigned int likelihoodSkip=0);
    void setMotionModelParameters(double srr, double srt, double str, double stt);
    void setUpdateDistances(double linear, double angular, double resampleThreshold);
    void setUpdatePeriod(double p) {period_=p;}

    //the "core" algorithm
    void processTruePos(const OdometryReading& odometry);
    bool processScan(const RangeReading & reading, int adaptParticles=0);

    /**This method copies the state of the filter in a tree.
     The tree is represented through reversed pointers (each node has a pointer to its parent).
     The leafs are stored in a vector, whose size is the same as the number of particles.
     @returns the leafs of the tree
     */
    TNodeVector getTrajectories() const;
    void integrateScanSequence(TNode* node);

    /**the scanmatcher algorithm*/
    ScanMatcher m_matcher;
    /**the stream used for writing the output of the algorithm*/
    std::ofstream& outputStream();
    /**the stream used for writing the info/debug messages*/
    std::ostream& infoStream();
    /**@returns the particles*/
    inline const ParticleVector& getParticles() const {return m_particles; }

    inline const std::vector<unsigned int>& getIndexes() const{return m_indexes; }
    int getBestParticleIndex() const;
    //callbacks
    virtual void onOdometryUpdate();
    virtual void onResampleUpdate();
    virtual void onScanmatchUpdate();

    //accessor methods
    /**the maxrange of the laser to consider */
    MEMBER_PARAM_SET_GET(m_matcher, double, laserMaxRange, protected, public, public);

    /**the maximum usable range of the laser. A beam is cropped to this value. [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, double, usableRange, protected, public, public);

    /**The sigma used by the greedy endpoint matching. [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher,double, gaussianSigma, protected, public, public);

    /**The sigma  of a beam used for likelihood computation [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher,double, likelihoodSigma, protected, public, public);

    /**The kernel in which to look for a correspondence[scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, int,    kernelSize, protected, public, public);

    /**The optimization step in rotation [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, double, optAngularDelta, protected, public, public);

    /**The optimization step in translation [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, double, optLinearDelta, protected, public, public);

    /**The number of iterations of the scanmatcher [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, unsigned int, optRecursiveIterations, protected, public, public);

    /**the beams to skip for computing the likelihood (consider a beam every likelihoodSkip) [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, unsigned int, likelihoodSkip, protected, public, public);

    /**translational sampling range for the likelihood [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, double, llsamplerange, protected, public, public);

    /**angular sampling range for the likelihood [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, double, lasamplerange, protected, public, public);

    /**translational sampling range for the likelihood [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, double, llsamplestep, protected, public, public);

    /**angular sampling step for the likelihood [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, double, lasamplestep, protected, public, public);

    /**generate an accupancy grid map [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, bool, generateMap, protected, public, public);

    /**enlarge the map when the robot goes out of the boundaries [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, bool, enlargeStep, protected, public, public);

    /**pose of the laser wrt the robot [scanmatcher]*/
    MEMBER_PARAM_SET_GET(m_matcher, OrientedPoint, laserPose, protected, public, public);


    /**odometry error in translation as a function of translation (rho/rho) [motionmodel]*/
    STRUCT_PARAM_SET_GET(m_motionModel, double, srr, protected, public, public);

    /**odometry error in translation as a function of rotation (rho/theta) [motionmodel]*/
    STRUCT_PARAM_SET_GET(m_motionModel, double, srt, protected, public, public);

    /**odometry error in rotation as a function of translation (theta/rho) [motionmodel]*/
    STRUCT_PARAM_SET_GET(m_motionModel, double, str, protected, public, public);

    /**odometry error in  rotation as a function of rotation (theta/theta) [motionmodel]*/
    STRUCT_PARAM_SET_GET(m_motionModel, double, stt, protected, public, public);

    /**minimum score for considering the outcome of the scanmatching good*/
    PARAM_SET_GET(double, minimumScore, protected, public, public);

protected:
    /**Copy constructor*/
    GridSlamProcessor(const GridSlamProcessor& gsp);


    /**the laser beams*/
    unsigned int m_beams;
    double last_update_time_;
    double period_;

    /**the particles*/
    ParticleVector m_particles;

    /**the particle indexes after resampling (internally used)*/
    std::vector<unsigned int> m_indexes;

    /**the particle weights (internally used)*/
    std::vector<double> m_weights;

    /**the motion model*/
    MotionModel m_motionModel;

    /**this sets the neff based resampling threshold*/
    PARAM_SET_GET(double, resampleThreshold, protected, public, public);

    //state
    int  m_count, m_readingCount;
    OrientedPoint m_lastPartPose;
    OrientedPoint m_odoPose;
    OrientedPoint m_pose;
    double m_linearDistance, m_angularDistance;
    PARAM_GET(double, neff, protected, public);

    //processing parameters (size of the map)
    PARAM_GET(double, xmin, protected, public);
    PARAM_GET(double, ymin, protected, public);
    PARAM_GET(double, xmax, protected, public);
    PARAM_GET(double, ymax, protected, public);
    //processing parameters (resolution of the map)
    PARAM_GET(double, delta, protected, public);

    //registration score (if a scan score is above this threshold it is registered in the map)
    PARAM_SET_GET(double, regScore, protected, public, public);
    //registration score (if a scan score is below this threshold a scan matching failure is reported)
    PARAM_SET_GET(double, critScore, protected, public, public);
    //registration score maximum move allowed between consecutive scans
    PARAM_SET_GET(double, maxMove, protected, public, public);

    //process a scan each time the robot translates of linearThresholdDistance
    PARAM_SET_GET(double, linearThresholdDistance, protected, public, public);

    //process a scan each time the robot rotates more than angularThresholdDistance
    PARAM_SET_GET(double, angularThresholdDistance, protected, public, public);

    //smoothing factor for the likelihood
    PARAM_SET_GET(double, obsSigmaGain, protected, public, public);

    //stream in which to write the gfs file
    std::ofstream m_outputStream;

    // stream in which to write the messages
    std::ostream& m_infoStream;

private:

	/* 进行扫描匹配 */
   /* 功能描述：对每个粒子进行单独匹配 */
   inline void scanMatch_org(double * plainReading) 
   {
		/* 每个粒子单独扫描匹配 */
#pragma omp for   
		for (int i = 0; i < m_particles.size(); i++) 
		{
            OrientedPoint corrected;

            double score, l, s;
#if 1
			/* 进行匹配评价 */
            score = m_matcher.optimize(corrected, m_particles[i].map, m_particles[i].pose, plainReading);
			

            if (score > m_minimumScore) /* m_minimumScore 这个值的缺省值为 0 */
			{
                m_particles[i].pose = corrected;  /* 纠正后的机器人姿势 */
            }
			else 
			{
                if (m_infoStream) 
				{
                    m_infoStream << "Scan Matching Failed, using odometry. Likelihood=" << l << std::endl;
                    m_infoStream << "lp:" << m_lastPartPose.x << " " << m_lastPartPose.y << " " << m_lastPartPose.theta
                                 << std::endl;
                    m_infoStream << "op:" << m_odoPose.x << " " << m_odoPose.y << " " << m_odoPose.theta << std::endl;
                }
            }

			/* 计算似然， 用作权重计算 */
            m_matcher.likelihoodAndScore(s, l, m_particles[i].map, m_particles[i].pose, plainReading);
			m_particles[i].weight    += l;
            m_particles[i].weightSum += l;
#else
			m_particles[i].weight += 0.5;
			m_particles[i].weightSum += 0.5;

#endif

            //set up the selective copy of the active area
            //by detaching the areas that will be updated
            m_matcher.invalidateActiveArea();  /* 设置活动区域无效 */
            m_matcher.computeActiveArea(m_particles[i].map, m_particles[i].pose, plainReading); /* 计算活动区域 */
        }

    }

   void scanMatch(double * plainReading);

	/* 归一化处理，对权重进行归一化 */
    inline void normalize() 
	{
        //normalize the log m_weights
		/* m_obsSigmaGain 这个值为 1， 因此这个值相当于平均粒子数  */
        double gain = 1. / (m_obsSigmaGain * m_particles.size());
        double lmax = -std::numeric_limits<double>::max();
        
		/*  计算粒子的最大权重 */
		for (ParticleVector::iterator it = m_particles.begin(); it != m_particles.end(); it++) 
		{
            lmax = it->weight > lmax ? it->weight : lmax;
        }

        //cout << "!!!!!!!!!!! maxwaight= "<< lmax << endl;

        m_weights.clear();
		
        double wcum = 0;
        m_neff = 0;
        
		/* 注意权重的计算方法 */
		for (std::vector<Particle>::iterator it = m_particles.begin(); it != m_particles.end(); it++) 
		{
            m_weights.push_back(exp(gain * (it->weight - lmax)));
            wcum += m_weights.back();
            //cout << "l=" << it->weight<< endl;
        }

        m_neff = 0;
        for (std::vector<double>::iterator it = m_weights.begin(); it != m_weights.end(); it++) 
		{
            *it = *it / wcum;
            double w = *it;
            m_neff += w * w;
        }
		
		/* 用作自适应重采样 */
        m_neff = 1. / m_neff;
    }

	/* 进行重采样处理 */
	/*
	const double* plainReading,       雷达数据
	int adaptSize,                    粒子的数目
	const RangeReading* reading       读取的雷达数据
	*/
    inline bool resample(const double* plainReading, int adaptSize, const RangeReading* reading) 
	{
        bool hasResampled = false;

        TNodeVector oldGeneration;
        
		/* 保存所有粒子的节点 */
		for(unsigned int i = 0; i < m_particles.size(); i++) 
		{
            oldGeneration.push_back(m_particles[i].node);
        }

		/* 必须满足重采样的条件， 在 normalize 函数中完成了计算 */
        if (m_neff < m_resampleThreshold * m_particles.size()) 
		{
            if (m_infoStream)
                m_infoStream << "*************RESAMPLE***************" << std::endl;

			/* 定义采样方法 */
            uniform_resampler<double, double> resampler;
			
			/* m_indexes 重采样后的粒子索引值,采样的规则是权重 */
            m_indexes = resampler.resampleIndexes(m_weights, adaptSize);

            if (m_outputStream.is_open()) 
			{
                m_outputStream << "RESAMPLE " << m_indexes.size() << " ";
                for (std::vector<unsigned int>::const_iterator it = m_indexes.begin(); it != m_indexes.end(); it++) 
				{
                    m_outputStream << *it << " ";
                }
                m_outputStream << std::endl;
            }

            onResampleUpdate(); /* 空函数 */
            //BEGIN: BUILDING TREE
            ParticleVector temp;
            unsigned int j = 0;
            std::vector<unsigned int> deletedParticles; //this is for deleteing the particles which have been resampled away.

            //		cerr << "Existing Nodes:" ;
            for (unsigned int i = 0; i < m_indexes.size(); i++) 
			{
                while (j < m_indexes[i]) 
				{
                    deletedParticles.push_back(j); /* 需要删除的粒子 */
                    j++;
                }
                if (j == m_indexes[i])
                    j++;
                Particle & p = m_particles[m_indexes[i]]; /* 需要保留的粒子 */
                TNode* node = 0;
                TNode* oldNode = oldGeneration[m_indexes[i]];
                node = new TNode(p.pose, 0, oldNode, 0);
                node->reading = reading;

                temp.push_back(p);
                temp.back().node = node;
                temp.back().previousIndex = m_indexes[i];
            }

            while (j < m_indexes.size()) 
			{
                deletedParticles.push_back(j);
                j++;
            }

			/* 删除粒子的节点 */
			for (unsigned int i = 0; i < deletedParticles.size(); i++) 
			{
                std::cerr << " " << deletedParticles[i];
                delete m_particles[deletedParticles[i]].node;
                m_particles[deletedParticles[i]].node = 0;
            }
            std::cerr << " Done" << std::endl;

            //END: BUILDING TREE
            std::cerr << "Deleting old particles...";
            m_particles.clear();
            std::cerr << "Done" << std::endl;
            std::cerr << "Copying Particles and  Registering  scans...";
            
			for (ParticleVector::iterator it = temp.begin(); it != temp.end(); it++)
			{
                it->setWeight(0);
                m_matcher.invalidateActiveArea();
                m_matcher.registerScan(it->map, it->pose, plainReading);
                m_particles.push_back(*it);
            }

            std::cerr << " Done" << std::endl;
            hasResampled = true;
        }
		else /* 不符合重采样条件的情况 */
		{
            int index = 0;
            std::cerr << "Registering Scans:";
            
			TNodeVector::iterator node_it = oldGeneration.begin();
            
			for (ParticleVector::iterator it = m_particles.begin(); it != m_particles.end(); it++) 
			{
                //create a new node in the particle tree and add it to the old tree
                //BEGIN: BUILDING TREE
                TNode* node = 0;
                node = new TNode(it->pose, 0.0, *node_it, 0);

                //node->reading=0;
                node->reading = reading;
                it->node = node;

                //END: BUILDING TREE
                m_matcher.invalidateActiveArea();
                m_matcher.registerScan(it->map, it->pose, plainReading);
                it->previousIndex = index;
                index++;
                node_it++;
            }
            std::cerr << "Done" << std::endl;
        }
        //END: BUILDING TREE

        return hasResampled;
    }

    //tree utilities

    void updateTreeWeights(bool weightsAlreadyNormalized = false);
    void resetTree();
    double propagateWeights();

};

typedef std::multimap<const TNode*, TNode*> TNodeMultimap;


// #include "gridslamprocessor.hxx"

}

#endif
