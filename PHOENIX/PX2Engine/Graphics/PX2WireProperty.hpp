// PX2WMatrixConstant.hpp

#ifndef PX2WIREPROPERTY_HPP
#define PX2WIREPROPERTY_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM WireProperty : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(WireProperty);

	public:
		WireProperty ();
		virtual ~WireProperty ();

		static bool IsEuqal(WireProperty *propA, WireProperty *propB);

		bool Enabled;  //< default: false
	};

	PX2_REGISTER_STREAM(WireProperty);
	typedef Pointer0<WireProperty> WirePropertyPtr;

}

#endif