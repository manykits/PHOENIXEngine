// PX2TriggerController.hpp

#ifndef PX2TRIGGERCONTROLLER_HPP
#define PX2TRIGGERCONTROLLER_HPP

#include "PX2Controller.hpp"
#include "PX2AVector.hpp"
#include "PX2APoint.hpp"
#include "PX2Movable.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TriggerController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(TriggerController);

	public:
		TriggerController();
		virtual ~TriggerController();

		enum AreaType
		{
			AT_SPHERE,
			AT_BOX,
			AT_MAX_TYPE
		};

		void SetAreaType(AreaType type);
		AreaType GetAreaType() const;

		void SetAreaParam(const AVector &vec);
		const AVector &GetAreaParam() const;

		bool IsPointIn(const PX2::APoint &point) const;

	protected:
		virtual void SetControlledable(Controlledable* object);

		AreaType mAreaType;
		AVector mAreaParam;
		MovablePtr mAreaMovable;

		// Triger
	public:
		enum TrigerType
		{
			TT_ENTER_ONCE,
			TT_ENTER_EVERYENTER,
			TT_MAX_TYPE
		};
		void SetTrigerType(TrigerType type);
		TrigerType GetTrigerType() const;

		virtual void Triger();
		void ResetTriger();
		bool IsTrigered() const;

	private:
		TrigerType mTrigerType;
		bool mIsTrigered;
	};

	PX2_REGISTER_STREAM(TriggerController);
	typedef Pointer0<TriggerController> TriggerControllerPtr;
#include "PX2TriggerController.inl"

}

#endif
