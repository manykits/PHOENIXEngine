// PX2Selection.hpp

#ifndef PX2SELECTION_HPP
#define PX2SELECTION_HPP

#include "PX2UnityPre.hpp"
#include "PX2Object.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Selection
	{
	public:
		Selection();
		virtual ~Selection();

		void AddObject(Object *obj);
		void RemoveObject(Object *obj);
		Object *GetObjectAt(int i);
		int GetNumObjects();
		bool IsObjectIn(Object *obj);
		void Clear();

		Object *GetFirstObject() const;

		void Translate(AVector vec);
		void TranslateTo(APoint pos);
		void AddRolate(AVector vec);
		void AddScale(AVector vec);

		const APoint &GetCenter() const;
		float GetRadius() const;

	private:
		void _UpdateSelect();

		std::vector<ObjectPtr> mObjects;
		APoint mCenter;
		float mBoundRadius;
	};

#include "PX2Selection.inl"
	typedef Pointer0<Selection> SelectionPtr;

}

#endif