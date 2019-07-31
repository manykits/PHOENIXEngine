// PX2PushTransformController.hpp

#ifndef PX2PUSHTRANSFORMCONTROLLER_HPP
#define PX2PUSHTRANSFORMCONTROLLER_HPP

#include "PX2Controller.hpp"
#include "PX2AVector.hpp"

namespace PX2
{

	// 给一个力，然后控制器自己处理
	class PX2_ENGINE_ITEM PushTransformController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(PushTransformController);

	public:
		PushTransformController ();
		virtual ~PushTransformController ();

		enum TransType
		{
			TT_TRANSLATE,
			TT_ROTATE,
			TT_SCALE,
			TT_MAX_TYPE
		};
		void SetTransType (TransType transType);
		TransType GetTransType ();

		void SetXY (bool isXY);
		bool IsXY () const;

		void SetMaxVelocity (float forceMax);
		float GetMaxVelocity ();

		// 力相当与附加的速度
		void AddVelocity (const AVector &addedVec);
		virtual void SetVelocity (const AVector &vec);
		const AVector &GetVelocity () const;

		// 每秒降低的速率
		void SetFriction (float fricton);
		float GetFriction ();

		virtual bool Update(double applicationTime, double elapsedTime);

		// 派生类重载此函数，实现具体的移动
		virtual void OnMoving (const AVector &dis);

	protected:
		TransType mTransType;
		float mMaxVelocity;
		float mFriction;
		AVector mVelocity;

		bool mIsXY;
	};

	PX2_REGISTER_STREAM(PushTransformController);
	typedef Pointer0<PushTransformController> PushTransformControllerPtr;
#include "PX2PushTransformController.inl"

}

#endif