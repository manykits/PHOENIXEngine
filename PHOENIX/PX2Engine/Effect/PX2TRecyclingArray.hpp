// PX2TRecyclingArray.hpp

#ifndef PX2TRECYCLINGARRAY_HPP
#define PX2TRECYCLINGARRAY_HPP

#include "PX2EffectPre.hpp"
#include "PX2Memory.hpp"

namespace PX2
{

	/// 可回收数组
	template <class T>
	class TRecyclingArray
	{
	public:
		TRecyclingArray (int quantity);
		~TRecyclingArray ();

		T* New ();
		int GetLastNewIndex ();

		bool Delete (int index);
		bool Delete (T *element);
		void DeleteAll ();

		int GetUsedQuantity ();
		int GetFreeQuantity ();
		int GetQuantity ();

		T &GetAt (int index);

		bool IsUsed (int index);

	protected:
		T *mElements;
		bool *mUseds;
		int mQuantity;
		int mUsedQuantity;
		int mIndexLastNew;
		T mNewTemplate;
	};

#include "PX2TRecyclingArray.inl"

}

#endif