// PX2VoxelBlock.hpp

#ifndef PX2VOXELBLOCK_HPP
#define PX2VOXELBLOCK_HPP

#include "PX2VoxelPre.hpp"
#include "PX2APoint.hpp"
#include "PX2Float2.hpp"
#include "PX2Float4.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2VertexBufferAccessor.hpp"
#include "PX2IndexBuffer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM VoxelBlock
	{
	public:
		VoxelBlock();
		~VoxelBlock();

		int IndexX;
		int IndexY;
		int IndexZ;

		int MtlType;

		Float4 Color;

		VoxelBlock *Left;
		VoxelBlock *Right;
		VoxelBlock *Front;
		VoxelBlock *Back;
		VoxelBlock *Bottom;
		VoxelBlock *Top;

		void CalNumFace(int &num);
		void GenMesh(VertexBufferAccessor &vba, IndexBuffer *ib, int &startFace);
	};
	typedef Pointer0<VoxelBlock> VoxelBlockPtr;

}

#endif