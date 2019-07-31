// PX2VoxelSection.cpp

#include "PX2VoxelSection.hpp"
#include "PX2Memory.hpp"
#include "PX2Math.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2VoxelMapData.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, VoxelSection);
PX2_IMPLEMENT_STREAM(VoxelSection);
PX2_IMPLEMENT_FACTORY(VoxelSection);
PX2_IMPLEMENT_DEFAULT_NAMES(Node, VoxelSection);

//----------------------------------------------------------------------------
VoxelSection::VoxelSection() :
mTitleRangeXY(2 * 2 + 1),
mTitleRangeZ(2 * 2 + 1),
mMinSizeX(-5),
mMinSizeY(-5),
mMinSizeZ(-1),
mMaxSizeX(5),
mMaxSizeY(5),
mMaxSizeZ(1),
mIsTitleRangeChanged(true),
CurTitles(0)
{
	SetName("PX2VoxelSection");
}
//----------------------------------------------------------------------------
VoxelSection::VoxelSection(int distXY, int distZ) :
mTitleRangeXY(2 * distXY + 1),
mTitleRangeZ(2 * distZ + 1),
mMinSizeX(-1),
mMinSizeY(-1),
mMinSizeZ(-1),
mMaxSizeX(1),
mMaxSizeY(1),
mMaxSizeZ(1),
mIsTitleRangeChanged(true),
CurTitles(0)
{
	SetName("PX2VoxelSection");
}
//----------------------------------------------------------------------------
VoxelSection::~VoxelSection()
{
	_ClearTitles();
}
//----------------------------------------------------------------------------
void VoxelSection::_ClearTitles()
{
	if (CurTitles)
	{
		for (int iX = 0; iX < mTitleRangeXY; ++iX)
		{
			for (int iY = 0; iY < mTitleRangeXY; ++iY)
			{
				for (int iZ = 0; iZ < mTitleRangeZ; iZ++)
				{
					VoxelTitle *title = CurTitles[iX][iY][iZ];
					if (title)
						DetachChild(title);
					CurTitles[iX][iY][iZ] = 0;
				}
			}
		}

		delete3(CurTitles);
	}
}
//----------------------------------------------------------------------------
void VoxelSection::SetMinSize(int minSizeX, int minSizeY, int minSizeZ)
{
	mMinSizeX = minSizeX;
	mMinSizeY = minSizeY;
	mMinSizeZ = minSizeZ;
}
//----------------------------------------------------------------------------
void VoxelSection::GetMinSize(int &minSizeX, int &minSizeY, int &minSizeZ)const
{
	minSizeX = mMinSizeX;
	minSizeY = mMinSizeY;
	minSizeZ = mMinSizeZ;
}
//----------------------------------------------------------------------------
void VoxelSection::SetMaxSize(int maxSizeX, int maxSizeY, int maxSizeZ)
{
	mMaxSizeX = maxSizeX;
	mMaxSizeY = maxSizeY;
	mMaxSizeZ = maxSizeZ;
}
//----------------------------------------------------------------------------
void VoxelSection::GetMaxSize(int &maxSizeX, int &maxSizeY, int &maxSizeZ)const
{
	maxSizeX = mMaxSizeX;
	maxSizeY = mMaxSizeY;
	maxSizeZ = mMaxSizeZ;
}
//----------------------------------------------------------------------------
void VoxelSection::SetDistXY(int val)
{
	_ClearTitles();

	mTitleRangeXY = 2 * val + 1;
	mIsTitleRangeChanged = true;
}
//----------------------------------------------------------------------------
void VoxelSection::SetDistZ(int val)
{
	_ClearTitles();

	mTitleRangeZ = 2 * val + 1;
	mIsTitleRangeChanged = true;
}
//----------------------------------------------------------------------------
void VoxelSection::SetInitMtlType(int mtlType)
{
	mInitMtlType = mtlType;
}
//----------------------------------------------------------------------------
int VoxelSection::GetTitleRangeXY() const
{
	return mTitleRangeXY;
}
//----------------------------------------------------------------------------
int VoxelSection::GetTitleRangeZ() const
{
	return mTitleRangeZ;
}
//----------------------------------------------------------------------------
void VoxelSection::SetAllBlocks(int mtlType)
{
	for (int iX = 0; iX < mTitleRangeXY; ++iX)
	{
		for (int iY = 0; iY < mTitleRangeXY; ++iY)
		{
			for (int iZ = 0; iZ < mTitleRangeZ; ++iZ)
			{
				if (CurTitles)
				{
					VoxelTitle *title = CurTitles[iX][iY][iZ];
					if (title)
					{
						title->SetAllBlock(mtlType);
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
bool VoxelSection::SetBlock(const APoint &worldPos, int mtlType,
	SetBlockGet *get)
{
	const HMatrix &transInverse = WorldTransform.Inverse();
	APoint localPos = transInverse * worldPos;

	int xIndex = (int)(Math<float>::Floor(localPos.X() / (float)(VOXEL_TITLE_SIZE * V_SIZE)));
	int yIndex = (int)(Math<float>::Floor(localPos.Y() / (float)(VOXEL_TITLE_SIZE * V_SIZE)));
	int zIndex = (int)(Math<float>::Floor(localPos.Z() / (float)(VOXEL_TITLE_SIZE * V_SIZE)));

	if (get)
	{
		get->TitleX = xIndex;
		get->TitleY = yIndex;
		get->TitleZ = zIndex;
		get->MtlType = mtlType;
	}

	APoint titleLocalPos;
	titleLocalPos.X() = localPos.X() - xIndex * VOXEL_TITLE_SIZE * V_SIZE;
	titleLocalPos.Y() = localPos.Y() - yIndex * VOXEL_TITLE_SIZE * V_SIZE;
	titleLocalPos.Z() = localPos.Z() - zIndex * VOXEL_TITLE_SIZE * V_SIZE;

	VoxelTitle *title = GetTitleFromMap(Vector3f((float)xIndex, 
		(float)yIndex, (float)zIndex));
	if (title)
	{
		return title->SetBlock(titleLocalPos, mtlType, get);
	}

	return false;
}
//----------------------------------------------------------------------------
bool VoxelSection::SetBlock(int titleIndexX, int titleIndexY, int titleIndexZ,
	int blockIndexX, int blockIndexY, int blockIndexZ, int mtlType, SetBlockGet *get)
{
	Vector3f key((float)titleIndexX, (float)titleIndexY, (float)titleIndexZ);
	VoxelTitle *title = GetTitleFromMap(key);
	if (title)
	{
		if (get)
		{
			get->TitleX = titleIndexX;
			get->TitleY = titleIndexY;
			get->TitleZ = titleIndexZ;
		}

		return title->SetBlock(blockIndexX, blockIndexY, blockIndexZ, mtlType, get);
	}

	return false;
}
//----------------------------------------------------------------------------
void VoxelSection::_GenTitleRelation()
{
	for (int iX = 0; iX < mTitleRangeXY; ++iX)
	{
		for (int iY = 0; iY < mTitleRangeXY; ++iY)
		{
			for (int iZ = 0; iZ < mTitleRangeZ; ++iZ)
			{
				VoxelTitle *title = CurTitles[iX][iY][iZ];
				if (title)
				{
					// x
					if (iX == 0)
					{
						if (VoxelTitle::IsGenTitleSideBlock())
						{
							if (title->Left)
								title->SetNeedReGenMesh(true);
						}

						title->Left = 0;
					}
					if (iX == mTitleRangeXY - 1)
					{
						if (VoxelTitle::IsGenTitleSideBlock())
						{
							if (title->Right)
								title->SetNeedReGenMesh(true);
						}

						title->Right = 0;
					}

					// y
					if (iY == 0)
					{
						if (VoxelTitle::IsGenTitleSideBlock())
						{
							if (title->Front)
								title->SetNeedReGenMesh(true);
						}

						title->Front = 0;
					}
					if (iY == mTitleRangeXY - 1)
					{
						if (VoxelTitle::IsGenTitleSideBlock())
						{
							if (title->Back)
								title->SetNeedReGenMesh(true);
						}

						title->Back = 0;
					}

					// z
					if (iZ == 0)
					{
						if (VoxelTitle::IsGenTitleSideBlock())
						{
							if (title->Bottom)
								title->SetNeedReGenMesh(true);
						}

						title->Bottom = 0;
					}
					if (iZ == mTitleRangeZ - 1)
					{
						if (VoxelTitle::IsGenTitleSideBlock())
						{
							if (title->Top)
								title->SetNeedReGenMesh(true);
						}

						title->Top = 0;
					}

					if (0 < iX)
					{
						VoxelTitle *newLeft = CurTitles[iX - 1][iY][iZ];
						if (newLeft != title->Left)
						{
							title->Left = newLeft;

							if (VoxelTitle::IsGenTitleSideBlock())
							{
								title->SetNeedReGenMesh(true);
							}
						}
					}
					if (iX < mTitleRangeXY - 1)
					{
						VoxelTitle *newRight = CurTitles[iX + 1][iY][iZ];
						if (newRight != title->Right)
						{
							title->Right = newRight;

							if (VoxelTitle::IsGenTitleSideBlock())
							{
								title->SetNeedReGenMesh(true);
							}
						}
					}
					if (0 < iY)
					{
						VoxelTitle *newFront = CurTitles[iX][iY - 1][iZ];
						if (newFront != title->Front)
						{
							title->Front = newFront;

							if (VoxelTitle::IsGenTitleSideBlock())
							{
								title->SetNeedReGenMesh(true);
							}
						}
					}
					if (iY < mTitleRangeXY - 1)
					{
						VoxelTitle *newBack = CurTitles[iX][iY + 1][iZ];
						if (newBack != title->Back)
						{
							title->Back = newBack;

							if (VoxelTitle::IsGenTitleSideBlock())
							{
								title->SetNeedReGenMesh(true);
							}
						}
					}
					if (0 < iZ)
					{
						VoxelTitle *newBottom = CurTitles[iX][iY][iZ - 1];
						if (newBottom != title->Bottom)
						{
							title->Bottom = newBottom;

							if (VoxelTitle::IsGenTitleSideBlock())
							{
								title->SetNeedReGenMesh(true);
							}
						}
					}
					if (iZ < mTitleRangeZ - 1)
					{
						VoxelTitle *newTop = CurTitles[iX][iY][iZ + 1];
						if (newTop != title->Top)
						{
							title->Top = newTop;

							if (VoxelTitle::IsGenTitleSideBlock())
							{
								title->SetNeedReGenMesh(true);
							}
						}
					}
				}
			}
		}
	}

	if (VoxelTitle::IsGenTitleSideBlock())
	{
		for (int iX = 0; iX < mTitleRangeXY; ++iX)
		{
			for (int iY = 0; iY < mTitleRangeXY; ++iY)
			{
				for (int iZ = 0; iZ < mTitleRangeZ; ++iZ)
				{
					VoxelTitle *title = CurTitles[iX][iY][iZ];
					if (title)
					{
						title->UpdateTitleSideBlock();
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void VoxelSection::GenMesh()
{
	if (CurTitles)
	{
		for (int iX = 0; iX < mTitleRangeXY; ++iX)
		{
			for (int iY = 0; iY < mTitleRangeXY; ++iY)
			{
				for (int iZ = 0; iZ < mTitleRangeZ; iZ++)
				{
					VoxelTitle *title = CurTitles[iX][iY][iZ];
					if (title && title->IsNeedReGenMesh())
					{
						title->GenMesh();
						title->SetNeedReGenMesh(false);
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void VoxelSection::SetActorCenterPos(const APoint &pos)
{
	mCenterPos = pos;

	int xIndex = (int)(Math<float>::Floor(mCenterPos.X() / (float)(VOXEL_TITLE_SIZE * V_SIZE)));
	int yIndex = (int)(Math<float>::Floor(mCenterPos.Y() / (float)(VOXEL_TITLE_SIZE * V_SIZE)));
	int zIndex = (int)(Math<float>::Floor(mCenterPos.Z() / (float)(VOXEL_TITLE_SIZE * V_SIZE)));

	mCenterIndexXYZ.X() = (float)xIndex;
	mCenterIndexXYZ.Y() = (float)yIndex;
	mCenterIndexXYZ.Z() = (float)zIndex;
}
//----------------------------------------------------------------------------
const APoint &VoxelSection::GetActorCenterPos() const
{
	return mCenterPos;
}
//----------------------------------------------------------------------------
VoxelTitle *VoxelSection::GetTitleFromMap(const Vector3f &key)
{
	auto it = mTitleMap.find(key);
	if (it != mTitleMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
VoxelTitle *VoxelSection::GetContainTitle(const APoint &worldPos)
{
	std::map<Vector3f, VoxelTitlePtr>::iterator it = mTitleMap.begin();
	for (; it != mTitleMap.end(); it++)
	{
		VoxelTitle *title = it->second;
		if (title->IsContainPoint(worldPos))
			return title;
	}

	return 0;
}
//----------------------------------------------------------------------------
void VoxelSection::GenTitles(int maxZ, int mtlType)
{
	_ClearTitles();

	CurTitles = new3<VoxelTitlePtr>(mTitleRangeZ, mTitleRangeXY, mTitleRangeXY);
	mTitleMap.clear();

	int startIndexX = mMinSizeX;
	int startIndexY = mMinSizeY;
	int startIndexZ = mMinSizeZ;
	int endIndexX = mMaxSizeX;
	int endIndexY = mMaxSizeY;
	int endIndexZ = maxZ;

	// 加载新的Title
	std::map<Vector3f, VoxelTitlePtr> newMap;
	for (int iX = startIndexX; iX <= endIndexX; iX++)
	{
		for (int iY = startIndexY; iY <= endIndexY; iY++)
		{
			for (int iZ = startIndexZ; iZ <= endIndexZ; iZ++)
			{
				APoint offset((float)iX*VOXEL_TITLE_SIZE * V_SIZE,
					(float)iY*VOXEL_TITLE_SIZE * V_SIZE, (float)iZ*VOXEL_TITLE_SIZE * V_SIZE);

				VoxelTitle *title = new0 VoxelTitle(iX, iY, iZ, mtlType);
				title->LocalTransform.SetTranslate(offset);

				for (int i = 0; i < VOXEL_TITLE_SIZE; i++)
				{
					for (int j = 0; j < VOXEL_TITLE_SIZE; j++)
					{
						for (int k = 0; k < VOXEL_TITLE_SIZE; k++)
						{
							title->Blocks[i][j][k].MtlType = 1;
						}
					}
				}

				newMap[Vector3f((float)iX, (float)iY, (float)iZ)] = title;
			}
		}
	}

	mTitleMap.clear();
	mTitleMap = newMap;

	for (int iX = startIndexX; iX <= endIndexX; iX++)
	{
		for (int iY = startIndexY; iY <= endIndexY; iY++)
		{
			for (int iZ = startIndexZ; iZ <= endIndexZ; iZ++)
			{
				int x = iX - startIndexX;
				int y = iY - startIndexY;
				int z = iZ - startIndexZ;

				VoxelTitle *title = GetTitleFromMap(Vector3f((float)iX, (float)iY, (float)iZ));
				if (title)
				{
					CurTitles[x][y][z] = title;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void VoxelSection::UpdateWorldData(double applicationTime, double elapsedTime)
{
	Node::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsTitleRangeChanged)
	{
		_ClearTitles();

		CurTitles = new3<VoxelTitlePtr>(mTitleRangeZ, mTitleRangeXY, mTitleRangeXY);

		mTitleMap.clear();
		mIsTitleRangeChanged = false;
	}

	int cIndexX = (int)mCenterIndexXYZ.X();
	int cIndexY = (int)mCenterIndexXYZ.Y();
	int cIndexZ = (int)mCenterIndexXYZ.Z();

	int halfRangeXY = (mTitleRangeXY-1) / 2;
	int halfRangeZ = (mTitleRangeZ-1) / 2;

	// 去除超过范围的title
	for (int x = 0; x < mTitleRangeXY; ++x)
	{
		for (int y = 0; y < mTitleRangeXY; ++y)
		{
			for (int z = 0; z < mTitleRangeZ; z++)
			{
				VoxelTitle *title = CurTitles[x][y][z];
				if (title)
				{
					int dstX = Math<int>::FAbs(cIndexX - title->WorldIndexX);
					int dstY = Math<int>::FAbs(cIndexY - title->WorldIndexY);
					int dstZ = Math<int>::FAbs(cIndexZ - title->WorldIndexZ);

					if (dstX > halfRangeXY || dstY > halfRangeXY ||
						dstZ > halfRangeZ)
					{
						DetachChild(title);
						CurTitles[x][y][z] = 0;
					}
				}
			}
		}
	}

	int startIndexX = cIndexX - halfRangeXY;
	int endIndexX = cIndexX + halfRangeXY;
	int startIndexY = cIndexY - halfRangeXY;
	int endIndexY = cIndexY + halfRangeXY;
	int startIndexZ = cIndexZ - halfRangeZ;
	int endIndexZ = cIndexZ + halfRangeZ;

	if (startIndexX < mMinSizeX)
		startIndexX = mMinSizeX;

	if (startIndexY < mMinSizeY)
		startIndexY = mMinSizeY;

	if (startIndexZ < mMinSizeZ)
		startIndexZ = mMinSizeZ;

	if (endIndexX > mMaxSizeX)
		endIndexX = mMaxSizeX;

	if (endIndexY > mMaxSizeY)
		endIndexY = mMaxSizeY;

	if (endIndexZ > mMaxSizeZ)
		endIndexZ = mMaxSizeZ;

	// 加载新的Title
	// 范围内已经存在不需要重新加载
	std::map<Vector3f, VoxelTitlePtr> newMap;
	for (int iX = startIndexX; iX <= endIndexX; iX++)
	{
		for (int iY = startIndexY; iY <= endIndexY; iY++)
		{
			for (int iZ = startIndexZ; iZ <= endIndexZ; iZ++)
			{
				int x = iX - startIndexX;
				int y = iY - startIndexY;
				int z = iZ - startIndexZ;

				VoxelTitle *title = GetTitleFromMap(Vector3f((float)iX, (float)iY, (float)iZ));
				if (title)
				{
					CurTitles[x][y][z] = title;
				}
				else
				{
					title = LoadTitle(iX, iY, iZ);
					if (title)
					{
						AttachChild(title);
					}
					CurTitles[x][y][z] = title;
				}
				newMap[Vector3f((float)iX, (float)iY, (float)iZ)] = title;
			}
		}
	}

	mTitleMap.clear();
	mTitleMap = newMap;	

	// genNew
	_GenTitleRelation();

	GenMesh();
}
//----------------------------------------------------------------------------
VoxelTitle *VoxelSection::LoadTitle(int iX, int iY, int iZ)
{
	int mapID = GetID();

	APoint offset((float)iX*VOXEL_TITLE_SIZE* V_SIZE,
		(float)iY*VOXEL_TITLE_SIZE* V_SIZE, (float)iZ*VOXEL_TITLE_SIZE* V_SIZE);

	V_TitleDataPtr vd = new0 V_TitleData();
	if (V_TitleData::Load(mapID, iX, iY, iZ, vd))
	{
		std::string path = VoxelTitle::GetTitlePath(mapID, iX, iY, iZ);
		VoxelTitle *title = 0;
		title = DynamicCast<VoxelTitle>(PX2_RM.BlockLoad(path));
		if (!title)
			title = new0 VoxelTitle(iX, iY, iZ, 1);
		title->LocalTransform.SetTranslate(offset);

		for (int i = 0; i < vd->NumBlocks; i++)
		{
			V_BlockData &bd = vd->Blocks[i];
			int indexX = bd.IndexX;
			int indexY = bd.IndexY;
			int indexZ = bd.IndexZ;
			title->Blocks[indexX][indexY][indexZ].MtlType = bd.Type;
		}

		return title;
	}
	else
	{
		VoxelTitle *title = new0 VoxelTitle(iX, iY, iZ, mInitMtlType);
		title->LocalTransform.SetTranslate(offset);

		return title;
	}

	return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
VoxelSection::VoxelSection(LoadConstructor value) :
Node(value),
mTitleRangeXY(2 * 2 + 1),
mTitleRangeZ(2 * 2 + 1),
mMinSizeX(-5),
mMinSizeY(-5),
mMinSizeZ(-1),
mMaxSizeX(5),
mMaxSizeY(5),
mMaxSizeZ(1),
mIsTitleRangeChanged(true),
CurTitles(0)
{
}
//----------------------------------------------------------------------------
void VoxelSection::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mMinSizeX);
	source.Read(mMinSizeY);
	source.Read(mMinSizeZ);
	source.Read(mMaxSizeX);
	source.Read(mMaxSizeY);
	source.Read(mMaxSizeZ);

	source.Read(mInitMtlType);

	PX2_END_DEBUG_STREAM_LOAD(VoxelSection, source);
}
//----------------------------------------------------------------------------
void VoxelSection::Link(InStream& source)
{
	Node::Link(source);
}
//----------------------------------------------------------------------------
void VoxelSection::PostLink()
{
	Node::PostLink();
}
//----------------------------------------------------------------------------
bool VoxelSection::Register(OutStream& target) const
{
	if (Node::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void VoxelSection::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mMinSizeX);
	target.Write(mMinSizeY);
	target.Write(mMinSizeZ);
	target.Write(mMaxSizeX);
	target.Write(mMaxSizeY);
	target.Write(mMaxSizeZ);

	target.Write(mInitMtlType);

	PX2_END_DEBUG_STREAM_SAVE(VoxelSection, target);
}
//----------------------------------------------------------------------------
int VoxelSection::GetStreamingSize(Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mMinSizeX);
	size += sizeof(mMinSizeY);
	size += sizeof(mMinSizeZ);
	size += sizeof(mMaxSizeX);
	size += sizeof(mMaxSizeY);
	size += sizeof(mMaxSizeZ);

	size += sizeof(mInitMtlType);

	return size;
}
//----------------------------------------------------------------------------
