// PX2VoxelBlockMtl.hpp

#ifndef PX2VOXELBLOCKMTL_HPP
#define PX2VOXELBLOCKMTL_HPP

#include "PX2VoxelPre.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM VoxelBlockMtl
	{
	public:
		VoxelBlockMtl();
		~VoxelBlockMtl();

		int Type;
		std::string Name;

		std::string Left;
		std::string Right;
		std::string Front;
		std::string Back;
		std::string Bottom;
		std::string Top;
	};
	typedef Pointer0<VoxelBlockMtl> VoxelBlockMtlPtr;

}

#endif