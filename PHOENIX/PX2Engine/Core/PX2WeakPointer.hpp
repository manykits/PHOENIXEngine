// PX2WeakPointer.hpp

#ifndef PX2SWEAKPOINTER_HPP
#define PX2SWEAKPOINTER_HPP

#include "PX2CorePre.hpp"
#include "PX2Memory.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	template <typename T>
	class WeakPointer0 : public PointerBase
	{
	public:
		WeakPointer0 (T* data = 0);
		WeakPointer0 (const Pointer0<T>& pointer);
		WeakPointer0 (const WeakPointer0& pointer);
		~WeakPointer0 ();

		// 隐式转换
		inline operator T* () const;
		inline T* operator-> () const;

		// 赋值
		WeakPointer0& operator= (T* data);
		WeakPointer0& operator= (const Pointer0<T>& pointer);
		WeakPointer0& operator= (const WeakPointer0& pointer);

		// 比较
		inline bool operator== (T* data) const;
		inline bool operator!= (T* data) const;
		inline bool operator== (const Pointer0<T>& pointer) const;
		inline bool operator!= (const Pointer0<T>& pointer) const;
		inline bool operator== (const WeakPointer0& pointer) const;
		inline bool operator!= (const WeakPointer0& pointer) const;

		bool IsExpired() const; 

	protected:
		T *mData;
	};

#include "PX2WeakPointer.inl"

}

#endif