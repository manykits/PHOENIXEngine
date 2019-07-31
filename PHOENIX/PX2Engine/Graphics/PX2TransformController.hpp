// PX2TransformController.hpp

#ifndef PX2TRANSFORMCONTROLLER_HPP
#define PX2TRANSFORMCONTROLLER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Controller.hpp"
#include "PX2Transform.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TransformController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(TransformController);

	public:
		TransformController (const Transform &trans);
		virtual ~TransformController ();

		void SetWorld (bool isWorld);
		bool IsWorld () const;

		// 成员访问
		inline void SetTransform (const Transform &trans);
		inline const Transform& GetTransform () const;

		// 动画更新
		/**
		* 以“毫秒”为单位。更新知识简单的将mLocalTransform拷贝到mObject的
		* LocalTransform中。
		*/
		virtual bool Update (double applicationTime, double elapsedTime);

	protected:
		Transform mTransform;
		bool mIsWorld;
	};

	PX2_REGISTER_STREAM(TransformController);
	typedef Pointer0<TransformController> TransformControllerPtr;
#include "PX2TransformController.inl"

}

#endif
