// PX2ServerUserManager.hpp

#ifndef PX2SERVERUSERMANAGER_HPP
#define PX2SERVERUSERMANAGER_HPP

#include "PX2CorePre.hpp"
#include "PX2ServerUser.hpp"

namespace PX2
{

	const int MaxNickNameLength = 16;

	class PX2_ENGINE_ITEM ServerUserManager
	{
	public:
		ServerUserManager(int player_pool_size);
		~ServerUserManager();

		ServerUser *GetUserByUIN(unsigned int uin);
		ServerUser *AllocUser(unsigned int clientid, unsigned int uin);
		void FreePlayer(ServerUser *player);
		ServerUser *GetUserByID(unsigned int clientid);
		void ResetUserClientID(ServerUser *player, unsigned int clientid);

	private:
		std::map<unsigned int, ServerUser *> mClientIDToUsers;
		std::map<unsigned int, ServerUser *> mUINToUsers;
		std::vector<ServerUser> mUserPool;
		std::vector<ServerUser*> mFreeUsers;
	};
}

#endif