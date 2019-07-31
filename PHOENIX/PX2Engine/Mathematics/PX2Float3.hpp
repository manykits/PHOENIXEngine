// PX2Float3.hpp

#ifndef PX2FLOAT3_HPP
#define PX2FLOAT3_HPP

#include "PX2MathematicsPre.hpp"
#include "PX2Tuple.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Float3 : public Tuple<3, float>
	{
	public:
		Float3 ();
		~Float3 ();  // hides ~Tuple<3,float>
		Float3 (float f0, float f1, float f2);
		Float3 (const Float3& tuple);

		float X() const;
		float& X();
		float Y() const;
		float& Y();
		float Z() const;
		float& Z();

		// ¸³Öµ
		Float3& operator= (const Float3& tuple);

		Float3 operator+ (const Float3& tuple) const;
		Float3 operator- (const Float3& tuple) const;
		Float3 operator* (float scalar) const;
		Float3 operator/ (float scalar) const;
		Float3& operator+= (const Float3& tuple);
		Float3& operator-= (const Float3& tuple);
		Float3& operator*= (float scalar);
		Float3& operator/= (float scalar);
		Float3& operator- ();

		static Float3 MakeColor (unsigned int r, unsigned int g, 
			unsigned int b);

		// hue saturation luminance
		// h [0, 360]
		// s [0, 100]
		// l [0, 100]
		static Float3 RGB2HSL(const Float3 &rgb);
		static Float3 HSL2RGB(const Float3 &hsl);

		static const Float3 ZERO;   //< (0,0,0)
		static const Float3 UNIT;	//< (1,1,1)
		static const Float3 UNIT_X;	//< (1,0,0)
		static const Float3 UNIT_Y;	//< (0,1,0)
		static const Float3 UNIT_Z;	//< (0,0,1)
		static const Float3 BLACK;	//< (0,0,0)
		static const Float3 WHITE;	//< (1,1,1)
		static const Float3 RED;	//< (1,0,0)
		static const Float3 GREEN;  //< (0,1,0)
		static const Float3 BLUE;	//< (0,0,1)
		static const Float3 YELLOW; //< (1,1,0)
	};

}

#endif