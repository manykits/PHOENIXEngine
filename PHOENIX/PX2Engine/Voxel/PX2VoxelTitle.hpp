// PX2VoxelTitle.hpp

#ifndef PX2VOXELTITLE_HPP
#define PX2VOXELTITLE_HPP

#include "PX2Node.hpp"
#include "PX2VoxelDefine.hpp"
#include "PX2VoxelBlock.hpp"
#include "PX2VoxelTitle.hpp"
#include "PX2VoxelSetBlockGet.hpp"
#include "PX2TriMesh.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM VoxelTitle : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(VoxelTitle);

	public:
		VoxelTitle(int x, int y, int z, int initMtlType);
		virtual ~VoxelTitle();

		void GetBlockIndex(const APoint &localPos, int &indexX, int &indexY, int &indexZ);
		bool SetBlock(const APoint &localPos, int type, SetBlockGet *vbg);
		bool SetBlock(int blockIndexX, int blockIndexY, int blockIndexZ, int mtlType, SetBlockGet *vbg);
		void SetAllBlock(int mtlType);

		bool IsContainPoint(const APoint &pos) const;

	public:
		int WorldIndexX;
		int WorldIndexY;
		int WorldIndexZ;
		int mInitMtlType;

		VoxelTitle *Left;
		VoxelTitle *Right;
		VoxelTitle *Front;
		VoxelTitle *Back;
		VoxelTitle *Bottom;
		VoxelTitle *Top;

		VoxelBlock Blocks[VOXEL_TITLE_SIZE][VOXEL_TITLE_SIZE][VOXEL_TITLE_SIZE];

	public:
		static void SetGenTitleSideBlock(bool sideBlock);
		static bool IsGenTitleSideBlock();
		static std::string GetTitlePath(int mapID, int indexX, int indexY, int indexZ);

		void UpdateTitleSideBlock();

		void SetNeedReGenMesh(bool need);
		bool IsNeedReGenMesh() const;

	public_internal:
		void DetachMesh();
		void AttachMesh();

		void GenMesh();

	protected:
		void _Init();
		void _CalNumFace(int &numFace);

		TriMeshPtr mMesh;
		int mNumFaces;
		bool mIsNeedReGenMesh;

		static bool sIsGenTitleSideBock;

		// Models
	public:
		void AddMovable(Movable *mov, const APoint &worldPos);

		std::vector<MovablePtr> mActorModels;
	};

	PX2_REGISTER_STREAM(VoxelTitle);
	typedef Pointer0<VoxelTitle> VoxelTitlePtr;

}

#endif