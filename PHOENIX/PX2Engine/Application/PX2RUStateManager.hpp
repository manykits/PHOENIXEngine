// PX2RUStateManager.hpp

#ifndef PX2RUSTATEMANAGER_HPP
#define PX2RUSTATEMANAGER_HPP

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

	class PX2_ENGINE_ITEM URState
	{
	public:
		URState();
		virtual ~URState();

		virtual void Leave();
		virtual void Enter();
	};

	class PX2_ENGINE_ITEM TransStateObj
	{
	public:
		TransStateObj();
		~TransStateObj();

		Float2 mAnchorHor;
		Float2 mAnchorParamHor;
		Float2 mAnchorVer;
		Float2 mAnchorParamVer;
	};

	class PX2_ENGINE_ITEM TransState : public URState
	{
	public:
		TransState(Object *fromParent, const TransStateObj &fromTrans,
			Object *obj, bool isAnchorLayout);
		virtual ~TransState();

		virtual void Leave();
		virtual void Enter();

	protected:
		bool mIsAnchorLayout;

		ObjectPtr mParentFrom;
		TransStateObj mTransStateFrom;

		ObjectPtr mParent;
		ObjectPtr mObject;
		TransStateObj mTransState;
	};

	typedef PX2::Pointer0<URState> URStatePtr;

	class PX2_ENGINE_ITEM URStateManager : public Singleton<URStateManager>
	{
	public:
		URStateManager();
		~URStateManager();

		void Clear();
		void ClearReDo();

		bool IsHasUnDo() const;
		bool IsHasReDo() const;

		void PushUnDo(URState *state);

		void UnDo();
		void ReDo();

	protected:
		std::deque<URStatePtr> mUnDoStates;
		std::deque<URStatePtr> mReDoStates;
	};

#define PX2_URSTATE_MAN URStateManager::GetSingleton()

}

#endif