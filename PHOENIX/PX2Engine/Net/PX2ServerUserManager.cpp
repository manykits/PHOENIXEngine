// PX2ServerUserManager.cpp

#include "PX2ServerUserManager.hpp"
using namespace PX2;

//-----------------------------------------------------------------------------
ServerUserManager::ServerUserManager(int player_pool_size)
{
	mUserPool.resize(player_pool_size);
	mFreeUsers.reserve(player_pool_size);

	for (int i = 0; i < player_pool_size; i++)
	{
		mFreeUsers.push_back(&mUserPool[i]);
	}
}
//-----------------------------------------------------------------------------
ServerUserManager::~ServerUserManager()
{
}
//-----------------------------------------------------------------------------
ServerUser *ServerUserManager::AllocUser(unsigned int clientid, unsigned int uin)
{
	if (mFreeUsers.empty()) return 0;

	ServerUser *user = mFreeUsers.back();
	mFreeUsers.pop_back();
	
	user->Reset();

	user->IsUsed = true;
	user->ClientID = clientid;
	user->UIN = uin;

	mClientIDToUsers[clientid] = user;
	mUINToUsers[uin] = user;

	return user;
}
//-----------------------------------------------------------------------------
void ServerUserManager::ResetUserClientID(ServerUser *player, unsigned int clientid)
{
	std::map<unsigned int, ServerUser *>::iterator iter = mClientIDToUsers.find(
		player->ClientID);
	if (iter != mClientIDToUsers.end())
	{
		mClientIDToUsers.erase(iter);
	}

	mClientIDToUsers[clientid] = player;
	player->ClientID = clientid;
}
//-----------------------------------------------------------------------------
void ServerUserManager::FreePlayer(ServerUser *player)
{
	std::map<unsigned int, ServerUser *>::iterator iter = mClientIDToUsers.find(
		player->ClientID);
	if (iter != mClientIDToUsers.end())
	{
		mClientIDToUsers.erase(iter);
	}

	iter = mUINToUsers.find(player->UIN);
	if (iter != mUINToUsers.end())
	{
		mUINToUsers.erase(iter);
	}

	player->IsUsed = false;

	mFreeUsers.push_back(player);
}
//-----------------------------------------------------------------------------
ServerUser *ServerUserManager::GetUserByID(unsigned int clientid)
{
	std::map<unsigned int, ServerUser *>::iterator iter =
		mClientIDToUsers.find(clientid);

	if (iter != mClientIDToUsers.end())
		return iter->second;
	
	return 0;
}
//-----------------------------------------------------------------------------
ServerUser *ServerUserManager::GetUserByUIN(unsigned int uin)
{
	std::map<unsigned int, ServerUser *>::iterator iter =
		mUINToUsers.find(uin);

	if (iter != mUINToUsers.end())
		return iter->second;
	
	return 0;
}
//-----------------------------------------------------------------------------