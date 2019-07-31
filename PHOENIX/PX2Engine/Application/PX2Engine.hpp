// PX2Engine.hpp

#ifndef PX2ENGINE_HPP
#define PX2ENGINE_HPP

#include "PX2CorePre.hpp"
#include "PX2Actor.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Engine
	{
	public:
		Engine();
		~Engine();

		static Actor *CreateActor();
		static Actor *CreateActorBox();
	};

}

#endif