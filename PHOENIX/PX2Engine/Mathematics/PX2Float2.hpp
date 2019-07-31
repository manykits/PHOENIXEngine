// PX2Float2.hpp

#ifndef PX2FLOAT2_HPP
#define PX2FLOAT2_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Tuple.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Float2 : public Tuple<2, float>
	{
	public:
		Float2 ();
		~Float2 ();  // hides ~Tuple<2,float>
		Float2 (float f0, float f1);
		Float2 (const Float2& tuple);

		float X() const;
		float& X();
		float Y() const;
		float& Y();

		// ¸³Öµ
		Float2& operator= (const Float2& tuple);

		static const Float2 ZERO;   //< (0,0)
		static const Float2 UNIT;	//< (1,1)
		static const Float2 HALFUNIT; //< (0.5,0.5)
	};

}

#endif