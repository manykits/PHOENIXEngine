// PX2EffectObject.hpp

#ifndef PX2EFFECTOBJECT_HPP
#define PX2EFFECTOBJECT_HPP

#include "PX2EffectPre.hpp"
#include "PX2Float3.hpp"
#include "PX2Float2.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class Effectable;

	class PX2_ENGINE_ITEM EffectObject
	{
	public:
		EffectObject ();
		virtual ~EffectObject ();
		
		virtual bool Update (Effectable *able, float elapsedTime);

		float		SizeXInit;
		float		SizeYInit;
		float		SizeZInit;
		float		SizeX;
		float		SizeY;
		float		SizeZ;

		Float2		UV0Begin;
		Float2		UV0End;
		Float2		UV0OffsetInit;
		Float2		UV0Offset;
		Float2		UV0SpeedInit;
		Float2		UV0Speed;
		Float2		UV0RepeatInit;
		Float2		UV0Repeat;

		Float3		ColorInit;
		Float3		Color;
		float		AlphaInit;
		float		Alpha;

		float		Life;
		float		Age;

		// –Ú¡–÷° π”√
		int			StartRandomIndex;
	};
	typedef Pointer0<EffectObject> EffectObjectPtr;

}

#endif