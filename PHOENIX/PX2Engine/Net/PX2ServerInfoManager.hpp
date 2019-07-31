// PX2ServerInfoManager.hpp

#ifndef PX2SERVERINFOMANAGER_HPP
#define PX2SERVERINFOMANAGER_HPP

#include "PX2NetPre.hpp"
#include "PX2Singleton.hpp"
#include "PX2ServerInfo.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ServerInfoManager
	{
		PX2_SINGLETION(ServerInfoManager);

	public:
		bool LoadServerInfo(const std::string &filename);
		void Clear();

		const ServerInfo *GetServerInfoByType(const std::string &type) const;
		const ServerInfo *GetServerInfoByName(const std::string &name) const;
		std::vector<ServerInfoPtr> &GetServerInfos();

	protected:
		std::vector<ServerInfoPtr> mServerInfos;
	};

#define PX2_SVRINFOMAN ServerInfoManager::GetSingleton()

}

#endif