// PX2AISteeringParamLoader.hpp

#ifndef PX2AISTEERINGPARAMLOADER_HPP
#define PX2AISTEERINGPARAMLOADER_HPP

#include "PX2iniFileLoader.hpp"
#include "PX2Math.hpp"

namespace PX2
{

#define Prm (*ParamLoader::Instance())

	class PX2_ENGINE_ITEM ParamLoader : public iniFileLoaderBase
	{
	private:

		ParamLoader() :iniFileLoaderBase("Data/ZERONE/cfg/params.ini")
		{
			NumAgents = GetNextParameterInt();
			NumObstacles = GetNextParameterInt();
			MinObstacleRadius = GetNextParameterFloat();
			MaxObstacleRadius = GetNextParameterFloat();

			NumCellsX = GetNextParameterInt();
			NumCellsY = GetNextParameterInt();

			NumSamplesForSmoothing = GetNextParameterInt();

			SteeringForceTweaker = GetNextParameterFloat();
			MaxSteeringForce = GetNextParameterFloat() * SteeringForceTweaker;
			MaxSpeed = GetNextParameterFloat();
			VehicleMass = GetNextParameterFloat();
			VehicleScale = GetNextParameterFloat();

			SeparationWeight = GetNextParameterFloat() * SteeringForceTweaker;
			AlignmentWeight = GetNextParameterFloat() * SteeringForceTweaker;
			CohesionWeight = GetNextParameterFloat() * SteeringForceTweaker;
			ObstacleAvoidanceWeight = GetNextParameterFloat() * SteeringForceTweaker;
			WallAvoidanceWeight = GetNextParameterFloat() * SteeringForceTweaker;
			WanderWeight = GetNextParameterFloat() * SteeringForceTweaker;
			SeekWeight = GetNextParameterFloat() * SteeringForceTweaker;
			FleeWeight = GetNextParameterFloat() * SteeringForceTweaker;
			ArriveWeight = GetNextParameterFloat() * SteeringForceTweaker;
			PursuitWeight = GetNextParameterFloat() * SteeringForceTweaker;
			OffsetPursuitWeight = GetNextParameterFloat() * SteeringForceTweaker;
			InterposeWeight = GetNextParameterFloat() * SteeringForceTweaker;
			HideWeight = GetNextParameterFloat() * SteeringForceTweaker;
			EvadeWeight = GetNextParameterFloat() * SteeringForceTweaker;
			FollowPathWeight = GetNextParameterFloat() * SteeringForceTweaker;

			ViewDistance = GetNextParameterFloat();
			MinDetectionBoxLength = GetNextParameterFloat();
			WallDetectionFeelerLength = GetNextParameterFloat();

			prWallAvoidance = GetNextParameterFloat();
			prObstacleAvoidance = GetNextParameterFloat();
			prSeparation = GetNextParameterFloat();
			prAlignment = GetNextParameterFloat();
			prCohesion = GetNextParameterFloat();
			prWander = GetNextParameterFloat();
			prSeek = GetNextParameterFloat();
			prFlee = GetNextParameterFloat();
			prEvade = GetNextParameterFloat();
			prHide = GetNextParameterFloat();
			prArrive = GetNextParameterFloat();

			MaxTurnRatePerSecond = Mathf::PI;
		}

	public:

		static ParamLoader* Instance();

		int	NumAgents;
		int	NumObstacles;
		double MinObstacleRadius;
		double MaxObstacleRadius;

		//number of horizontal cells used for spatial partitioning
		int   NumCellsX;
		//number of vertical cells used for spatial partitioning
		int   NumCellsY;

		//how many samples the smoother will use to average a value
		int   NumSamplesForSmoothing;

		//used to tweak the combined steering force (simply altering the MaxSteeringForce
		//will NOT work!This tweaker affects all the steering force multipliers
		//too).
		double SteeringForceTweaker;

		double MaxSteeringForce;
		double MaxSpeed;
		double VehicleMass;

		double VehicleScale;
		double MaxTurnRatePerSecond;

		double SeparationWeight;
		double AlignmentWeight;
		double CohesionWeight;
		double ObstacleAvoidanceWeight;
		double WallAvoidanceWeight;
		double WanderWeight;
		double SeekWeight;
		double FleeWeight;
		double ArriveWeight;
		double PursuitWeight;
		double OffsetPursuitWeight;
		double InterposeWeight;
		double HideWeight;
		double EvadeWeight;
		double FollowPathWeight;

		//how close a neighbour must be before an agent perceives it (considers it
		//to be within its neighborhood)
		double ViewDistance;

		//used in obstacle avoidance
		double MinDetectionBoxLength;

		//used in wall avoidance
		double WallDetectionFeelerLength;

		//these are the probabilities that a steering behavior will be used
		//when the prioritized dither calculate method is used
		double prWallAvoidance;
		double prObstacleAvoidance;
		double prSeparation;
		double prAlignment;
		double prCohesion;
		double prWander;
		double prSeek;
		double prFlee;
		double prEvade;
		double prHide;
		double prArrive;
	};
}

#endif