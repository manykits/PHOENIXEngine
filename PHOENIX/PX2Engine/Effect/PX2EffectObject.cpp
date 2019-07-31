// PX2EffectObject.cpp

#include "PX2EffectObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
EffectObject::EffectObject ()
	:
SizeXInit(1.0f),
SizeYInit(1.0f),
SizeZInit(1.0f),
SizeX(1.0f),
SizeY(1.0f),
SizeZ(1.0f),
AlphaInit(1.0f),
Alpha(1.0f),
Life(1.0f),
Age(0.0f),
StartRandomIndex(0)
{
	UV0Offset = Float2::ZERO;
	UV0OffsetInit = UV0Offset;
	
	UV0Repeat = Float2::UNIT;
	UV0RepeatInit = UV0Repeat;

	UV0Speed = Float2::ZERO;
	UV0SpeedInit = UV0Speed;

	UV0Begin = UV0Offset;
	UV0End[0] = 1.0f + UV0Offset[0];
	UV0End[1] = 1.0f + UV0Offset[1];

	Color = Float3::WHITE;
	ColorInit = Color;
}
//----------------------------------------------------------------------------
EffectObject::~EffectObject ()
{
}
//----------------------------------------------------------------------------
bool EffectObject::Update (Effectable *able, float elapsedTime)
{
	PX2_UNUSED(able);

	if (elapsedTime < 0.0f)
	{
		elapsedTime = 0.0f;
	}

	UV0Offset[0] += UV0Speed[0] * elapsedTime;
	UV0Offset[1] += UV0Speed[1] * elapsedTime;

	UV0Begin[0] = UV0Offset[0];
	UV0Begin[1] = UV0Offset[1];

	UV0End[0] = UV0Begin[0] + 1.0f * UV0Repeat[0];
	UV0End[1] = UV0Begin[1] + 1.0f * UV0Repeat[1];

	Age += elapsedTime;
	if (Age >= Life)
		return false;

	return true;
}
//----------------------------------------------------------------------------