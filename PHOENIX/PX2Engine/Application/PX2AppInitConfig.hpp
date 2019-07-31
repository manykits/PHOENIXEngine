// PX2ApplicationInitConfig.hpp

#ifndef PX2APPLICATIONINITCONFIG_HPP
#define PX2APPLICATIONINITCONFIG_HPP

#include "PX2CorePre.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM AppInitConfig
	{
	public:
		AppInitConfig();
		~AppInitConfig();

		bool IsInitVoice;
		bool IsInitSound;
	};

}

#endif