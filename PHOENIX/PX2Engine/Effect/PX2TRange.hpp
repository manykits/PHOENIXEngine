// PX2TRange.hpp

#ifndef PX2TRANGE_HPP
#define PX2TRANGE_HPP

#include "PX2EffectPre.hpp"
#include "PX2Float1.hpp"
#include "PX2Float2.hpp"
#include "PX2Float3.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	/// 范围类
	/**
	* 输入两个值，通过GetRandomInRange获得两值范围内的一个随机值。
	*/
	template <class T>
	class TRange
	{
	public:
		TRange ();
		TRange (T value0, T value1);
		~TRange ();

		T GetRandomInRange();
		T GetRange ();

		T Value0;
		T Value1;
	};

	int RandomNumber (int value0, int value1);
	float RandomNumber (float value0, float value1);
	Float1 RandomNumber (Float1 value0, Float1 value1);
	Float2 RandomNumber (Float2 value0, Float2 value1);
	Float3 RandomNumber (Float3 value0, Float3 value1);
	Float4 RandomNumber (Float4 value0, Float4 value1);

#include "PX2TRange.inl"

}
#endif