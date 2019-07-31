// PX2VoxelManager.cpp

#include "PX2VoxelManager.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2XMLData.hpp"
#include "PX2VoxelBlock.hpp"
using namespace PX2;

VoxelManager::VoxelManager()
{
	mType = T_COLOR;
}
//----------------------------------------------------------------------------
VoxelManager::~VoxelManager()
{
}
//----------------------------------------------------------------------------
bool VoxelManager::Initlize(Type t)
{
	LoadBlocksConfig("Data/engine/voxel/blockconfig.xml");
	PX2_RM.AddTexPack("Data/engine/voxel/blocks.xml");

	mType = t;

	return true;
}
//----------------------------------------------------------------------------
VoxelManager::Type VoxelManager::GetType() const
{
	return mType;
}
//----------------------------------------------------------------------------
const TexPackElement &VoxelManager::GetTexPackEle(
	const std::string &faceImageName) const
{
	return PX2_RM.GetTexPackElement("Data/engine/voxel/blocks.xml", 
		faceImageName);
}
//----------------------------------------------------------------------------
bool VoxelManager::LoadBlocksConfig(const std::string &filename)
{
	int bufSize = 0;
	char *buf = 0;
	if (PX2_RM.LoadBuffer(filename, bufSize, buf))
	{
		XMLData data;
		if (data.LoadBuffer(buf, bufSize))
		{
			XMLNode rootNode = data.GetRootNode();
			XMLNode blockNode = rootNode.IterateChild();
			while (!blockNode.IsNull())
			{
				std::string name = blockNode.AttributeToString("name");
				int mtlType = blockNode.AttributeToInt("type");

				std::vector<std::string> texs;

				VoxelBlockMtlPtr vbm = new0 VoxelBlockMtl();
				vbm->Name = name;

				XMLNode sideNode = blockNode.IterateChild();
				while (!sideNode.IsNull())
				{
					std::string mtlStr = sideNode.AttributeToString("tex");
					texs.push_back(mtlStr);

					sideNode = sideNode.IterateChild(sideNode);
				}

				vbm->Type = mtlType;
				vbm->Left = texs[0];
				vbm->Right = texs[1];
				vbm->Front = texs[2];
				vbm->Back = texs[3];
				vbm->Bottom = texs[4];
				vbm->Top = texs[5];

				mTypeBlockMtls[mtlType] = vbm;
				mNameBlockMtls[name] = vbm;

				blockNode = blockNode.IterateChild(blockNode);
			}
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
VoxelBlockMtl *VoxelManager::GetVoxelBlockMtlByType(int mtlType)
{
	auto it = mTypeBlockMtls.find(mtlType);
	if (it != mTypeBlockMtls.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
VoxelBlockMtl *VoxelManager::GetVoxelBlockMtlByName(
	const std::string &name)
{
	auto it = mNameBlockMtls.find(name);
	if (it != mNameBlockMtls.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------