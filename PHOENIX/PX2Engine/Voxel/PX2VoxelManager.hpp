// PX2VoxelManager.hpp

#ifndef PX2VOXELMANAGER_HPP
#define PX2VOXELMANAGER_HPP

#include "PX2VoxelPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2VoxelBlockMtl.hpp"
#include "PX2TexPackData.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM VoxelManager : public Singleton<VoxelManager>
	{
	public:
		VoxelManager();
		~VoxelManager();

		enum Type
		{
			T_TEX,
			T_COLOR,
			T_MAX_TYPE
		};
		// call it when project play, because PX2_RM may clear some res
		bool Initlize(Type t);
		Type GetType() const;

		bool LoadBlocksConfig(const std::string &filename);

		VoxelBlockMtl *GetVoxelBlockMtlByType(int mtlType);
		VoxelBlockMtl *GetVoxelBlockMtlByName(const std::string &name);

		const TexPackElement &GetTexPackEle(const std::string &faceImageName) const;

	public:
		std::map<int, VoxelBlockMtlPtr> mTypeBlockMtls;
		std::map<std::string, VoxelBlockMtlPtr> mNameBlockMtls;

	protected:
		Type mType;
	};

#define PX2_VOXELM VoxelManager::GetSingleton()

}

#endif