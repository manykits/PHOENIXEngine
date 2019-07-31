// PX2VoxelTitle.cpp

#include "PX2VoxelTitle.hpp"
#include "PX2Creater.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2StringHelp.hpp"
#include "PX2AxisAlignedBox3.hpp"
#include "PX2Vector3.hpp"
#include "PX2VoxelMapData.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2VoxelManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Node, VoxelTitle);
PX2_IMPLEMENT_STREAM(VoxelTitle);
PX2_IMPLEMENT_FACTORY(VoxelTitle);
PX2_IMPLEMENT_DEFAULT_NAMES(Node, VoxelTitle);

//----------------------------------------------------------------------------
bool VoxelTitle::sIsGenTitleSideBock = false;
//----------------------------------------------------------------------------
VoxelTitle::VoxelTitle(int x, int y, int z, int initMtlType)
{
	SetName("PX2VoxelTitle");

	WorldIndexX = x;
	WorldIndexY = y;
	WorldIndexZ = z;

	mInitMtlType = initMtlType;

	_Init();
}
//----------------------------------------------------------------------------
VoxelTitle::~VoxelTitle()
{
}
//----------------------------------------------------------------------------
void VoxelTitle::_Init()
{
	Left = 0;
	Right = 0;
	Top = 0;
	Bottom = 0;
	Front = 0;
	Back = 0;

	SetNeedReGenMesh(true);

	VertexFormat *vf = 0;

	VoxelManager::Type t = PX2_VOXELM.GetType();
	if (VoxelManager::T_TEX == t)
	{
		vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PCNT1);
	}
	else if (VoxelManager::T_COLOR == t)
	{
		vf = PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PC);
	}
	VertexBuffer *vb = new0 VertexBuffer(100, vf->GetStride(), Buffer::BU_DYNAMIC);
	IndexBuffer *ib = new0 IndexBuffer(100, 2, Buffer::BU_STATIC);

	mMesh = new0 TriMesh(vf, vb, ib);
	AttachChild(mMesh);
	mMesh->SetSaveWriteIngore(true);

	MaterialInstance *mi = 0;
	if (VoxelManager::T_TEX == t)
	{
		mi = new0 MaterialInstance(
			"Data/engine_mtls/std/std.px2obj", "std_lightshadow", false);

		Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
			"Data/engine/voxel/blocks.png"));
		if (tex->CanGenMinmaps())
			tex->GenerateMipmaps();
		mi->SetPixelTexture(0, "SampleBase", tex);
	}
	else
	{
		mi = VertexColor4Material::CreateUniqueInstance();
	}
	mMesh->SetMaterialInstance(mi);

	for (int x = 0; x < VOXEL_TITLE_SIZE; x++)
	{
		for (int y = 0; y < VOXEL_TITLE_SIZE; y++)
		{
			for (int z = 0; z < VOXEL_TITLE_SIZE; z++)
			{
				Blocks[x][y][z].IndexX = x;
				Blocks[x][y][z].IndexY = y;
				Blocks[x][y][z].IndexZ = z;
				Blocks[x][y][z].MtlType = mInitMtlType;

				if (0 < x)
				{
					Blocks[x][y][z].Left = &Blocks[x - 1][y][z];
				}
				if (x < VOXEL_TITLE_SIZE - 1)
				{
					Blocks[x][y][z].Right = &Blocks[x + 1][y][z];
				}
				if (0 < y)
				{
					Blocks[x][y][z].Front = &Blocks[x][y - 1][z];
				}
				if (y < VOXEL_TITLE_SIZE - 1)
				{
					Blocks[x][y][z].Back = &Blocks[x][y + 1][z];
				}
				if (0 < z)
				{
					Blocks[x][y][z].Bottom = &Blocks[x][y][z - 1];
				}
				if (z < VOXEL_TITLE_SIZE - 1)
				{
					Blocks[x][y][z].Top = &Blocks[x][y][z + 1];
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void VoxelTitle::GetBlockIndex(const APoint &localPos, int &indexX, int &indexY, int &indexZ)
{
	indexX = (int)Mathf::Floor(localPos.X() / V_SIZE);
	indexY = (int)Mathf::Floor(localPos.Y() / V_SIZE);
	indexZ = (int)Mathf::Floor(localPos.Z() / V_SIZE);
}
//----------------------------------------------------------------------------
bool VoxelTitle::SetBlock(const APoint &localPos, int type, SetBlockGet *vbg)
{
	int x = 0;
	int y = 0;
	int z = 0;
	GetBlockIndex(localPos, x, y, z);

	return SetBlock(x, y, z, type, vbg);
}
//----------------------------------------------------------------------------
void VoxelTitle::SetAllBlock(int mtlType)
{
	VoxelBlock Blocks[VOXEL_TITLE_SIZE][VOXEL_TITLE_SIZE][VOXEL_TITLE_SIZE];

	for (int x = 0; x < VOXEL_TITLE_SIZE; x++)
	{
		for (int y = 0; y < VOXEL_TITLE_SIZE; y++)
		{
			for (int z = 0; z < VOXEL_TITLE_SIZE; z++)
			{
				SetBlock(x, y, z, mtlType, 0);
			}
		}
	}
}
//----------------------------------------------------------------------------
bool VoxelTitle::SetBlock(int x, int y, int z,
	int type, SetBlockGet *vbg)
{
	bool setSuc = false;

	if (0 <= x && x < VOXEL_TITLE_SIZE &&
		0 <= y && y < VOXEL_TITLE_SIZE &&
		0 <= z && z < VOXEL_TITLE_SIZE)
	{
		Blocks[x][y][z].MtlType = type;

		if (vbg)
		{
			vbg->IsValied = true;
			vbg->BlockX = x;
			vbg->BlockY = y;
			vbg->BlockZ = z;
			vbg->MtlType = type;
		}

		setSuc = true;
	}

	if (setSuc)
	{
		SetNeedReGenMesh(true);

		if (IsGenTitleSideBlock())
		{
			if (0 == x)
			{
				if (Left)
					Left->SetNeedReGenMesh(true);
			}
			if (VOXEL_TITLE_SIZE - 1 == x)
			{
				if (Right)
					Right->SetNeedReGenMesh(true);
			}
			if (0 == y)
			{
				if (Front)
				{
					Front->SetNeedReGenMesh(true);
				}
			}
			if (VOXEL_TITLE_SIZE - 1 == y)
			{
				if (Back)
				{
					Back->SetNeedReGenMesh(true);
				}
			}
			if (0 == z)
			{
				if (Bottom)
				{
					Bottom->SetNeedReGenMesh(true);
				}
			}
			if (VOXEL_TITLE_SIZE - 1 == z)
			{
				if (Top)
				{
					Top->SetNeedReGenMesh(true);
				}
			}
		}
	}

	return setSuc;
}
//----------------------------------------------------------------------------
bool VoxelTitle::IsContainPoint(const APoint &pos) const
{
	const APoint &worldPos = WorldTransform.GetTranslate();

	AxisAlignedBox3f box(
		worldPos.X(), worldPos.X() + VOXEL_TITLE_SIZE* V_SIZE,
		worldPos.Y(), worldPos.Y() + VOXEL_TITLE_SIZE* V_SIZE,
		worldPos.Z(), worldPos.Z() + VOXEL_TITLE_SIZE* V_SIZE);

	Vector3f vec(pos.X(), pos.Y(), pos.Z());
	return box.Contain(vec);
}
//----------------------------------------------------------------------------
void VoxelTitle::SetGenTitleSideBlock(bool sideBlock)
{
	sIsGenTitleSideBock = sideBlock;
}
//----------------------------------------------------------------------------
bool VoxelTitle::IsGenTitleSideBlock()
{
	return sIsGenTitleSideBock;
}
//----------------------------------------------------------------------------
std::string VoxelTitle::GetTitlePath(int mapID, int indexX, int indexY, int indexZ)
{
	std::string mapsDir = V_DATA + "maps/";
	std::string mapIDDir = StringHelp::IntToString(mapID) + "/";
	std::string mapIDStr = StringHelp::IntToString(mapID);
	std::string str0 = StringHelp::IntToString(indexX);
	std::string str1 = StringHelp::IntToString(indexY);
	std::string str2 = StringHelp::IntToString(indexZ);
	std::string path = mapsDir + mapIDDir + str0 + "_" + str1 + "_" + str2 + ".vt";

	return path;
}
//----------------------------------------------------------------------------
void VoxelTitle::UpdateTitleSideBlock()
{
	if (Left)
	{
		for (int y = 0; y < VOXEL_TITLE_SIZE; y++)
		{
			for (int z = 0; z < VOXEL_TITLE_SIZE; z++)
			{
				Blocks[0][y][z].Left = &(Left->Blocks[VOXEL_TITLE_SIZE - 1][y][z]);
				Left->Blocks[VOXEL_TITLE_SIZE - 1][y][z].Right = &(Blocks[0][y][z]);
			}
		}
	}
	else
	{
		for (int y = 0; y < VOXEL_TITLE_SIZE; y++)
		{
			for (int z = 0; z < VOXEL_TITLE_SIZE; z++)
			{
				Blocks[0][y][z].Left = 0;
			}
		}
	}

	if (!Right)
	{
		for (int y = 0; y < VOXEL_TITLE_SIZE; y++)
		{
			for (int z = 0; z < VOXEL_TITLE_SIZE; z++)
			{
				Blocks[VOXEL_TITLE_SIZE-1][y][z].Right = 0;
			}
		}
	}

	if (Bottom)
	{
		for (int x = 0; x < VOXEL_TITLE_SIZE; x++)
		{
			for (int y = 0; y < VOXEL_TITLE_SIZE; y++)
			{
				Blocks[x][y][0].Bottom = &(Bottom->Blocks[x][y][VOXEL_TITLE_SIZE - 1]);
				Bottom->Blocks[x][y][VOXEL_TITLE_SIZE - 1].Top = &(Blocks[x][y][0]);
			}
		}
	}
	else
	{
		for (int x = 0; x < VOXEL_TITLE_SIZE; x++)
		{
			for (int y = 0; y < VOXEL_TITLE_SIZE; y++)
			{
					Blocks[x][y][0].Bottom = 0;
			}
		}
	}

	if (!Top)
	{
		for (int x = 0; x < VOXEL_TITLE_SIZE; x++)
		{
			for (int y = 0; y < VOXEL_TITLE_SIZE; y++)
			{
				Blocks[x][y][VOXEL_TITLE_SIZE-1].Top = 0;
			}
		}
	}

	if (Front)
	{
		for (int x = 0; x < VOXEL_TITLE_SIZE; x++)
		{
			for (int z = 0; z < VOXEL_TITLE_SIZE; z++)
			{
				Blocks[x][0][z].Front = &(Front->Blocks[x][VOXEL_TITLE_SIZE - 1][z]);
				Front->Blocks[x][VOXEL_TITLE_SIZE - 1][z].Back = &(Blocks[x][0][z]);
			}
		}
	}
	else
	{
		for (int x = 0; x < VOXEL_TITLE_SIZE; x++)
		{
			for (int z = 0; z < VOXEL_TITLE_SIZE; z++)
			{
				Blocks[x][0][z].Front = 0;
			}
		}
	}

	if (!Back)
	{
		for (int x = 0; x < VOXEL_TITLE_SIZE; x++)
		{
			for (int z = 0; z < VOXEL_TITLE_SIZE; z++)
			{
				Blocks[x][VOXEL_TITLE_SIZE-1][z].Back = 0;
			}
		}
	}
}
//----------------------------------------------------------------------------
void VoxelTitle::SetNeedReGenMesh(bool need)
{
	mIsNeedReGenMesh = need;
}
//----------------------------------------------------------------------------
bool VoxelTitle::IsNeedReGenMesh() const
{
	return mIsNeedReGenMesh;
}
//----------------------------------------------------------------------------
void VoxelTitle::DetachMesh()
{
	DetachChild(mMesh);
}
//----------------------------------------------------------------------------
void VoxelTitle::AttachMesh()
{
	if (mMesh)
	{
		AttachChild(mMesh);
	}
}
//----------------------------------------------------------------------------
void VoxelTitle::GenMesh()
{
	mNumFaces = 0;
	_CalNumFace(mNumFaces);

	if (0 == mNumFaces)
	{
		Show(false);
		return;
	}
	else
	{
		Show(true);
	}

	int numVertex = mNumFaces * 4;
	int numIndex = mNumFaces * 6;

	VertexFormat *vf = mMesh->GetVertexFormat();
	VertexBuffer *vb = new0 VertexBuffer(numVertex, vf->GetStride(),
		Buffer::BU_DYNAMIC);
	IndexBuffer *ib = new0 IndexBuffer(numIndex, 2, Buffer::BU_STATIC);

	VertexBufferAccessor vba(vf, vb);

	int startFace = 0;
	for (int x = 0; x < VOXEL_TITLE_SIZE; x++)
	{
		for (int y = 0; y < VOXEL_TITLE_SIZE; y++)
		{
			for (int z = 0; z < VOXEL_TITLE_SIZE; z++)
			{
				Blocks[x][y][z].GenMesh(vba, ib, startFace);
			}
		}
	}

	mMesh->SetVertexBuffer(vb);
	mMesh->SetIndexBuffer(ib);

	mMesh->UpdateModelSpace(Renderable::GU_USE_TCOORD_CHANNEL);
}
//----------------------------------------------------------------------------
void VoxelTitle::_CalNumFace(int &numFace)
{
	for (int x = 0; x < VOXEL_TITLE_SIZE; x++)
	{
		for (int y = 0; y < VOXEL_TITLE_SIZE; y++)
		{
			for (int z = 0; z < VOXEL_TITLE_SIZE; z++)
			{
				if (0 != Blocks[x][y][z].MtlType)
				{
					Blocks[x][y][z].CalNumFace(numFace);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void VoxelTitle::AddMovable(Movable *mov, const APoint &worldPos)
{
	AVector lp = worldPos - WorldTransform.GetTranslate();

	if (mov)
	{
		AttachChild(mov);
		mov->LocalTransform.SetTranslate(lp.X(), lp.Y(), lp.Z());

		mActorModels.push_back(mov);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
VoxelTitle::VoxelTitle(LoadConstructor value) :
Node(value)
{
}
//----------------------------------------------------------------------------
void VoxelTitle::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Node::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(WorldIndexX);
	source.Read(WorldIndexY);
	source.Read(WorldIndexZ);

	source.Read(mInitMtlType);

	int numActors = 0;
	source.Read(numActors);
	if (numActors > 0)
	{
		mActorModels.resize(numActors);
		for (int i = 0; i < numActors; i++)
		{
			source.ReadPointer(mActorModels[i]);
		}
	}

	PX2_END_DEBUG_STREAM_LOAD(VoxelTitle, source);
}
//----------------------------------------------------------------------------
void VoxelTitle::Link(InStream& source)
{
	Node::Link(source);

	for (int i = 0; i < (int)mActorModels.size(); i++)
	{
		if (mActorModels[i])
			source.ResolveLink(mActorModels[i]);
	}
}
//----------------------------------------------------------------------------
void VoxelTitle::PostLink()
{
	Node::PostLink();

	for (int i = 0; i < (int)mActorModels.size(); i++)
	{
		if (mActorModels[i])
		{
			mActorModels[i]->PostLink();
		}
	}

	_Init();
}
//----------------------------------------------------------------------------
bool VoxelTitle::Register(OutStream& target) const
{
	if (Node::Register(target))
	{
		int numActors = (int)mActorModels.size();
		for (int i = 0; i < numActors; i++)
		{
			if (mActorModels[i])
			{
				target.Register(mActorModels[i]);
			}
		}

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void VoxelTitle::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Node::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(WorldIndexX);
	target.Write(WorldIndexY);
	target.Write(WorldIndexZ);

	target.Write(mInitMtlType);

	int numActors = (int)mActorModels.size();
	target.Write(numActors);
	for (int i = 0; i < numActors; i++)
	{
		target.WritePointer(mActorModels[i]);
	}

	PX2_END_DEBUG_STREAM_SAVE(VoxelTitle, target);
}
//----------------------------------------------------------------------------
int VoxelTitle::GetStreamingSize(Stream &stream) const
{
	int size = Node::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(WorldIndexX);
	size += sizeof(WorldIndexY);
	size += sizeof(WorldIndexZ);

	size += sizeof(mInitMtlType);

	int numActors = (int)mActorModels.size();
	size += sizeof(numActors);
	for (int i = 0; i < numActors; i++)
	{
		size += PX2_POINTERSIZE(mActorModels[i]);
	}

	return size;
}
//----------------------------------------------------------------------------