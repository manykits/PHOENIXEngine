// PX2VoxelSetBlockGet.hpp

#ifndef PX2VOXELSETBLOCKGET_HPP
#define PX2VOXELSETBLOCKGET_HPP

#include "PX2VoxelPre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM SetBlockGet
	{
	public:
		SetBlockGet();
		~SetBlockGet();

		bool IsValied;

		int TitleX;
		int TitleY;
		int TitleZ;
		int BlockX;
		int BlockY;
		int BlockZ;
		int MtlType;
	};

}

#endif