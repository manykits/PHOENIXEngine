// PX2Integer.hpp

#ifndef PX2INTEGER_HPP
#define PX2INTEGER_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Assert.hpp"

namespace PX2
{

	template <int N> class Rational;

	// N is the number of 32-bit words you want per Integer.
	/// 整数
	template <int N>
	class Integer
	{
	public:
		Integer (int i = 0);
		Integer (const Integer& value);
		~Integer ();

		// 赋值
		Integer& operator= (const Integer& value);

		// 比较
		bool operator== (const Integer& value) const;
		bool operator!= (const Integer& value) const;
		bool operator<  (const Integer& value) const;
		bool operator<= (const Integer& value) const;
		bool operator>  (const Integer& value) const;
		bool operator>= (const Integer& value) const;

		// 计算
		Integer operator- () const;
		Integer operator+ (const Integer& value) const;
		Integer operator- (const Integer& value) const;
		Integer operator* (const Integer& value) const;
		Integer operator/ (const Integer& value) const;
		Integer operator% (const Integer& value) const;

		// 计算更新
		Integer& operator+= (const Integer& value);
		Integer& operator-= (const Integer& value);
		Integer& operator*= (const Integer& value);
		Integer& operator/= (const Integer& value);

		// 位移
		Integer operator<< (int shift) const;
		Integer operator>> (int shift) const;

		// 位移更新
		Integer& operator<<= (int shift);
		Integer& operator>>= (int shift);

	private:
		/// 比较函数
		/**
		* 返回-1，value0 < value1；返回0，value0 == value1；返回+1，
		* value0 > value1。
		*/
		static int Compare (const Integer& value0, const Integer& value1);
		int GetSign () const;

		// Support for division and modulo.
		static bool GetDivMod (const Integer& numer, const Integer& denom,
			Integer& quotient, Integer& remainder);

		static void DivSingle (const Integer& numer, short denom,
			Integer& quotient, Integer& remainder);

		static void DivMultiple (const Integer& numer, const Integer& denom,
			Integer& quotient, Integer& remainder);

		// Miscellaneous utilities.
		int GetLeadingBlock () const;
		int GetTrailingBlock () const;
		int GetLeadingBit (int i) const;  // of mBuffer[i]
		int GetTrailingBit (int i) const;  // of mBuffer[i]
		int GetLeadingBit () const;  // of entire number
		int GetTrailingBit () const;  // of entire number
		void SetBit (int i, bool on);
		bool GetBit (int i) const;
		unsigned int ToUnsignedInt (int i) const;
		void FromUnsignedInt (int i, unsigned int value);
		unsigned int ToUnsignedInt (int lo, int hi) const;
		int ToInt (int i) const;

		enum
		{
			INT_SIZE = 2*N,
			INT_BYTES = INT_SIZE*sizeof(short),
			INT_LAST = INT_SIZE - 1
		};

		short mBuffer[INT_SIZE];

		// Rational needs access to private members of Integer.
		friend class Rational<N>;
	};

	template <int N>
	Integer<N> operator* (int i, const Integer<N>& value);

#include "PX2Integer.inl"

}

#endif