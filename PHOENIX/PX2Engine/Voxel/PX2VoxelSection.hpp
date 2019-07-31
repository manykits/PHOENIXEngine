// PX2VoxelSection.hpp

#ifndef PX2VOXELSECTION_HPP
#define PX2VOXELSECTION_HPP

#include "PX2VoxelDefine.hpp"
#include "PX2VoxelTitle.hpp"
#include "PX2Node.hpp"
#include "PX2VoxelSetBlockGet.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM VoxelSection : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_NEW(VoxelSection);
		PX2_DECLARE_STREAM(VoxelSection);

	public:
		VoxelSection();
		VoxelSection(int distXY, int distZ);
		virtual ~VoxelSection();

		void SetMinSize(int minSizeX, int minSizeY, int minSizeZ);
		void GetMinSize(int &minSizeX, int &minSizeY, int &minSizeZ) const;		
		void SetMaxSize(int maxSizeX, int maxSizeY, int maxSizeZ);
		void GetMaxSize(int &maxSizeX, int &maxSizeY, int &maxSizeZ) const;
		void GenTitles(int maxZ, int mtlType);

		void SetDistXY(int val);
		void SetDistZ(int val);
		void SetInitMtlType(int mtlType);

		int GetTitleRangeXY() const;
		int GetTitleRangeZ() const;

		void SetAllBlocks(int mtlType);
		bool SetBlock(const APoint &worldPos, int mtlType, SetBlockGet *get = 0);
		bool SetBlock(int titleIndexX, int titleIndexY, int titleIndexZ,
			int blockIndexX, int blockIndexY, int blockIndexZ, int mtlType, SetBlockGet *get = 0);

		void GenMesh();

		VoxelTitlePtr ***CurTitles;

		VoxelTitle *GetTitleFromMap(const Vector3f &key);
		std::map<Vector3f, VoxelTitlePtr> mTitleMap;

		VoxelTitle *GetContainTitle(const APoint &worldPos);

	protected:
		void _ClearTitles();
		void _GenTitleRelation();

		int mMinSizeX;
		int mMinSizeY;
		int mMinSizeZ;
		int mMaxSizeX;
		int mMaxSizeY;
		int mMaxSizeZ;

		int mTitleRangeXY;
		int mTitleRangeZ;
		bool mIsTitleRangeChanged;
		int mInitMtlType;

	public:
		void SetActorCenterPos(const APoint &pos);
		const APoint &GetActorCenterPos() const;
		const AVector &GetCenterIndexXYZ() const;

		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

	protected:
		VoxelTitle *LoadTitle(int wIndexX, int wIndexY, int wIndexZ);

		APoint mCenterPos;
		AVector mCenterIndexXYZ;
	};

#include "PX2VoxelSection.inl"
	PX2_REGISTER_STREAM(VoxelSection);
	typedef Pointer0<VoxelSection> VoxelSectionPtr;

}

#endif