// PX2Logic.hpp

#ifndef PX2LOGIC_HPP
#define PX2LOGIC_HPP

#include "PX2CorePre.hpp"
#include "PX2Node.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Logic : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(Logic);
		PX2_DECLARE_STREAM(Logic);

	public:
		Logic();
		virtual ~Logic();

	public:
		void OnEvent(Event *ent);
	};

	PX2_REGISTER_STREAM(Logic);
	typedef Pointer0<Logic> LogicPtr;

}

#endif