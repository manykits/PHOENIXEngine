// PX2WMatrixConstant.hpp

#ifndef PX2DEPTHPROPERTY_HPP
#define PX2DEPTHPROPERTY_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM DepthProperty : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(DepthProperty);

	public:
		DepthProperty ();
		virtual ~DepthProperty ();

		static bool IsEuqal(DepthProperty *propA, DepthProperty *propB);

		enum CompareMode
		{
			CM_NEVER,
			CM_LESS,
			CM_EQUAL,
			CM_LEQUAL,
			CM_GREATER,
			CM_NOTEQUAL,
			CM_GEQUAL,
			CM_ALWAYS,
			CM_QUANTITY
		};

		bool Enabled;         //< default: true
		bool Writable;        //< default: true
		CompareMode Compare;  //< default: CM_LEQUAL
	};

	PX2_REGISTER_STREAM(DepthProperty);
	typedef Pointer0<DepthProperty> DepthPropertyPtr;

}

#endif