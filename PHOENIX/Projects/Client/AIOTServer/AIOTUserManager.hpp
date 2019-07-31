// AIOTUserManager.hpp

#ifndef AIOTUSERMANAGER_HPP
#define AIOTUSERMANAGER_HPP

#include "AIOTPre.hpp"
#include "AIOTUser.hpp"

namespace PX2
{

	class AIOTUserManager : public Singleton < AIOTUserManager >
	{
	public:
		AIOTUserManager(int player_pool_size);
		~AIOTUserManager();

		AIOTUser *AllocUser(unsigned int clientid, unsigned int uin);
		void FreeUser(AIOTUser *player);

		AIOTUser *GetPlayerByUIN(unsigned int uin);
		AIOTUser *GetPlayerByClientID(unsigned int clientid);

		AIOTUser *GetPlayerByUKey(const FString &ukey);

		std::map<unsigned int, AIOTUser *> &GetClientMap();
		std::map<unsigned int, AIOTUser *> &GetUINMap();

	private:
		std::map<unsigned int, AIOTUser *> mClientID2Players;
		std::map<unsigned int, AIOTUser *> mUIN2Players;
		std::vector<AIOTUser>mPlayerPool;
		std::vector<AIOTUser *>mFreePlayers;
	};

#define V_PALYERM AIOTUserManager::GetSingleton()

}

#endif