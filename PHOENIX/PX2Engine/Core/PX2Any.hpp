// PX2Any.hpp

#ifndef PX2ANY_HPP
#define PX2ANY_HPP

#include "PX2CorePre.hpp"
#include <typeinfo>

namespace PX2
{

	class PX2_ENGINE_ITEM Any
	{
	public:
		// 构造，析构
		Any()
			:
		mContent(0)
		{
		}

		template<typename ValueType>
		Any (const ValueType &value)
			: 
		mContent(new Holder<ValueType>(value))
		{
		}

		Any (const Any &other)
			:
		mContent(other.mContent ? other.mContent->Clone() : 0)
		{
		}

		~Any()
		{
			delete mContent;
		}

		// 操作
		Any &Swap(Any & rhs)
		{
			std::swap(mContent, rhs.mContent);
			return *this;
		}

		template<typename ValueType>
		Any &operator=(const ValueType &rhs)
		{
			Any(rhs).Swap(*this);
			return *this;
		}

		Any & operator=(Any rhs)
		{
			rhs.Swap(*this);
			return *this;
		}

		bool IsEmpty() const
		{
			return !mContent;
		}

		const std::type_info &type() const
		{
			return mContent ? mContent->type() : typeid(void);
		}

	public_internal:
		// types
		class PlaceHolder
		{
		public:
			virtual ~PlaceHolder()
			{
			}

		public:
			virtual const std::type_info & type() const = 0;
			virtual PlaceHolder * Clone() const = 0;
		};

		template<typename ValueType>
		class Holder : public PlaceHolder
		{
		public:
			Holder(const ValueType &value)
				: mHeld(value)
			{
			}

			virtual const std::type_info &type() const
			{
				return typeid(ValueType);
			}

			virtual PlaceHolder *Clone() const
			{
				return new Holder(mHeld);
			}

		public:
			ValueType mHeld;

		private:
			// 申明但不实现
			Holder & operator=(const Holder &);
		};

	public_internal:
		PlaceHolder * mContent;
	};

	template<typename ValueType>
	inline ValueType *Any_Cast(Any * operand)
	{
		return operand && operand->type() == typeid(ValueType) ?
			&static_cast<Any::Holder<ValueType> *>(operand->mContent)->mHeld
			: 0;
	}

	template<typename ValueType>
	inline const ValueType *Any_Cast(const Any *operand)
	{
		return Any_Cast<ValueType>(const_cast<Any *>(operand));
	}

#define PX2_ANY_AS(any, T) *Any_Cast<T>(&any)

}

#endif