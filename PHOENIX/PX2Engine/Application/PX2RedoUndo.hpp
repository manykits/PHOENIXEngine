// PX2RedoUndo.hpp

#ifndef PX2REDOUNDO_HPP
#define PX2REDOUNDO_HPP

#include "PX2CorePre.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2WeakPointer.hpp"
#include "PX2APoint.hpp"
#include "PX2Float2.hpp"
#include "PX2HMatrix.hpp"
#include "PX2Object.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2Movable.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM URDo
	{
	public:
		URDo();
		virtual ~URDo();

		virtual void OnUnDo();
		virtual void OnReDo();
	};

	typedef PX2::Pointer0<URDo> URDoPtr;

	class PX2_ENGINE_ITEM MovablesTransURDo : public URDo
	{
	public:
		MovablesTransURDo(bool isAnchorLayout=false);
		virtual ~MovablesTransURDo();

		virtual void OnUnDo();
		virtual void OnReDo();

	protected:
		bool mIsAnchorLayout;

		std::vector<PX2::ObjectPtr> mObjects;

		std::vector<PX2::APoint> mPositions;
		std::vector<PX2::APoint> mRotations;
		std::vector<PX2::HMatrix> mRotationMats;
		std::vector<PX2::APoint> mScales;
		std::vector<PX2::Float2> mAnchorHors;
		std::vector<PX2::Float2> mAnchorParamHors;
		std::vector<PX2::Float2> mAnchorVers;
		std::vector<PX2::Float2> mAnchorParamVers;

		std::vector<PX2::APoint> mPositionsReDo;
		std::vector<PX2::APoint> mRotationsReDo;
		std::vector<PX2::HMatrix> mRotationMatsReDo;
		std::vector<PX2::APoint> mScalesReDo;

		std::vector<PX2::Float2> mAnchorHorsReDo;
		std::vector<PX2::Float2> mAnchorParamHorsReDo;
		std::vector<PX2::Float2> mAnchorVersReDo;
		std::vector<PX2::Float2> mAnchorParamVersReDo;
	};

	class PX2_ENGINE_ITEM MovableTransURDo : public URDo
	{
	public:
		MovableTransURDo(Movable *mov, bool isAnchorLayout = false);
		virtual ~MovableTransURDo();

		virtual void OnUnDo();
		virtual void OnReDo();

	protected:
		bool mIsAnchorLayout;

		ObjectPtr mObject;
		APoint mPosition;
		APoint mRotation;
		HMatrix mRotationMat;
		APoint mScale;
		Float2 mAnchorHor;
		Float2 mAnchorParamHor;
		Float2 mAnchorVer;
		Float2 mAnchorParamVer;

		APoint mPositionReDo;
		APoint mRotationReDo;
		HMatrix mRotationMatReDo;
		APoint mScaleReDo;

		Float2 mAnchorHorReDo;
		Float2 mAnchorParamHorReDo;
		Float2 mAnchorVerReDo;
		Float2 mAnchorParamVerReDo;
	};

	class PX2_ENGINE_ITEM ObjectAddDeleteURDo : public URDo
	{
	public:
		ObjectAddDeleteURDo(bool isAdd, PX2::Object *object, Object *repalceParent=0);
		virtual ~ObjectAddDeleteURDo();

		Object *GetObject() { return mObject; }
		bool IsAdd() const { return mIsAdd; }

		virtual void OnUnDo();
		virtual void OnReDo();

		Float2 mAnchorHor;
		Float2 mAnchorParamHor;
		Float2 mAnchorVer;
		Float2 mAnchorParamVer;

		Float2 mAnchorHor_InReplaceParent;
		Float2 mAnchorParamHor_InReplaceParent;
		Float2 mAnchorVer_InReplaceParent;
		Float2 mAnchorParamVer_InReplaceParent;

	protected:
		ObjectAddDeleteURDo();

		bool mIsAdd;
		ObjectPtr mObject;
		ObjectPtr mParent;
		ObjectPtr mReplaceParent;
	};

	class PX2_ENGINE_ITEM URDoManager : public Singleton<URDoManager>
	{
	public:
		URDoManager();
		~URDoManager();

		void Clear();
		bool IsHasUnDo() const;
		bool IsHasReDo() const;

		void PushUnDo(URDo *command);

		void UnDo();
		void ReDo();

	protected:
		std::deque<URDoPtr> mUnDoCommands;
		std::deque<URDoPtr> mReDoCommands;
	};

#define PX2_URDOM URDoManager::GetSingleton()

}

#endif