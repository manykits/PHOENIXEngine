// PX2ParticleEmitter.hpp

#ifndef PX2PARTICLEEMITTER_HPP
#define PX2PARTICLEEMITTER_HPP

#include "PX2EffectPre.hpp"
#include "PX2Effectable.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ParticleEmitter : public Effectable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(ParticleEmitter);

	public:
		ParticleEmitter ();
		virtual ~ParticleEmitter ();

		void SetMaxNumParticles (int num);
		int GetMaxNumParticles () const;

		void SetBlast (bool blast);
		bool IsBlast () const;

		enum DrawType
		{
			DT_NORMAL,
			DT_MODEL,
			DT_MAX_TYPE
		};
		void SetDrawType (DrawType type);
		DrawType GetDrawType () const;

		void SetModelFilename (const std::string &filename);
		const std::string &GetModelFilename () const;

		void SetPivot (const Float2 &point);
		const Float2 &GetPivot () const;

		void SetEmitRate (float rate);
		float GetEmitRate () const;

		void SetEmitDirDynamic (bool dyn);
		bool IsEmitDirDynamic () const;
		void SetEmitDir (const AVector &dir);
		AVector &GetEmitDir ();
		void SetEmitDirLocal (bool isLocal);
		bool IsEmitDirLocal () const;

		enum EmitDirType
		{
			EST_NORMAL,
			EST_TOATTRACTPOINT,
			EST_LEVELATTRACTPOINT,
			EST_MAX_TYPE
		};
		void SetEmitDirType (EmitDirType type);
		EmitDirType GetEmitDirType () const;

		void SetEmitSpeed (float speed);
		float GetEmitSpeed () const;

		void SetEmitAccelerateDir (const AVector &dir);
		const AVector &GetEmitAccelerateDir () const;

		void SetEmitAccelerate (float val);
		float GetEmitAccelerate () const;

		void SetEmitAttractPoint (const APoint &pos);
		const APoint &GetEmitAttactPoint () const;
		void SetEmitAttactStrength (const AVector &strength);
		const AVector &GetEmitAttackStrength () const;

		void SetObstruct (const AVector &obs);
		const AVector &GetObstruct () const;

		enum RotateAxisType
		{
			RAT_D,
			RAT_U,
			RAT_R,
			RAT_FREE,
			RAT_MAX_TYPE
		};
		void SetRotateAxisType (RotateAxisType type);
		RotateAxisType GetRotateAxisType () const;

		void SetEmitRotateAxis (const AVector &dir);
		const AVector &GetEmitRotateAxis() const;

		void SetEmitRotateDegree (float degree);
		float GetEmitRotateDegree () const;

		void SetEmitRotateSpeed (float speed);
		float GetEmitRotateSpeed () const;

		enum CollisionType
		{
			CT_NONE,
			CT_FACE_HEIGHTUP,
			CT_MAX_TYPE
		};
		void SetCollisionType (CollisionType type);
		CollisionType GetCollisionType () const;

		enum CollisionOption
		{
			CO_REFLECT,
			CO_DEAD,
			CO_MAX_TYPE
		};
		void SetCollisionOption (CollisionOption type);
		CollisionOption GetCollisionOption () const;

		void SetCollisionFaceHeight (float height);
		float GetCollisionFaceHeight () const;

		void SetCollisionSpeedPercent (float percent);
		float GetCollisionSpeedPercent () const;

		enum PlacerType
		{
			PT_BOX,
			PT_SPHERE,
			PT_COLUMN,
			PT_MAX_TYPE
		};
		void SetPlacerType (PlacerType type);
		PlacerType GetPlacerType () const;

		void SetPlacerInLength (float val);
		void SetPlacerOutLength (float val);
		void SetPlacerInWidth (float val);
		void SetPlacerOutWidth (float val);
		void SetPlacerInHeight (float val);
		void SetPlacerOutHeight (float val);
		float GetPlacerInLength () const;
		float GetPlacerOutLength () const;
		float GetPlacerInWidth () const;
		float GetPlacerOutWidth () const;
		float GetPlacerInHeight () const;
		float GetPlacerOutHeight () const;
		APoint GetPlacerAPoint () const;

		virtual const std::vector<std::string> &GetAllModuleNames_EA () const;
		virtual const std::vector<std::string> &GetAllModuleNames_EO () const;

		static std::vector<std::string> msModuleNames_EA;
		static std::vector<std::string> msModuleNames_EO;

public_internal:
		virtual void GenBuffers ();

	protected:
		bool mIsBlast;
		DrawType mDrawType;
		std::string mModelFilename;
		Float2 mPivotPoint;
		RotateAxisType mRotateAxisType;
		AVector mRotateAxis;
		float mEmitRotateDegree;
		float mEmitRotateSpeed;

		EmitDirType mEmitDirType;
		bool mIsEmitDirDynamic;
		AVector mEmitDir;
		bool mIsEmitDirLocal;
		float mEmitSpeed;
		AVector mAccelerateDir;
		float mAccelerate;
		APoint mAttactPoint;
		AVector mAttackStrength;
		float mEmitRate;

		PlacerType mPlacerType;
		float mPlacerInLength;
		float mPlacerOutLength;
		float mPlacerInWidth;
		float mPlacerOutWidth;
		float mPlacerInHeight;
		float mPlacerOutHeight;

		AVector mObstruct;
		CollisionType mCollisionType;
		CollisionOption mCollisionOption;
		float mCollisionHeight;
		float mCollisionSpeedPercent;
	};

#include "PX2ParticleEmitter.inl"
	PX2_REGISTER_STREAM(ParticleEmitter);
	typedef Pointer0<ParticleEmitter> ParticleEmitterPtr;

}

#endif