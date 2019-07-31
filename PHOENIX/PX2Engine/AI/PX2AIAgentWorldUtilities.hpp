// PX2AIAgentWorldUtilities.hpp

#ifndef PX2AIAGENTWORLDUTILITIES_HPP
#define PX2AIAGENTWORLDUTILITIES_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class AIAgentObject;
	class AIAgentWorld;
	class Node;

	class AIAgentWorldUtilities
	{
	public:
		static AIAgentObject* CreateSandboxObject(
			AIAgentWorld* world, const std::string& meshFileName);
		static AIAgentObject* CreateSandboxObject(
			AIAgentWorld* world, Node* node);

		//static NavigationMesh* CreateNavigationMesh(
		//	AIAgentWorld* const sandbox,
		//	rcConfig config,
		//	const const std::string& navMeshName);
	};

}

#endif