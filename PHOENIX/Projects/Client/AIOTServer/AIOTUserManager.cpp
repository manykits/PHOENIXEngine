// AIOTUserManager.cpp

#include "AIOTUserManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
AIOTUserManager::AIOTUserManager(int player_pool_size)
{
	mPlayerPool.resize(player_pool_size);
	mFreePlayers.reserve(player_pool_size);

	for (int i = 0; i < player_pool_size; i++)
	{
		mFreePlayers.push_back(&mPlayerPool[i]);
	}
}
//----------------------------------------------------------------------------
AIOTUserManager::~AIOTUserManager()
{
}
//----------------------------------------------------------------------------
AIOTUser *AIOTUserManager::AllocUser(unsigned int clientid, unsigned int uin)
{
	if (mFreePlayers.empty()) return NULL;

	AIOTUser *p = mFreePlayers.back();
	mFreePlayers.pop_back();
	p->Reset();
	p->IsUsed = true;
	p->ClientID = clientid;
	p->UIN = uin;

	mClientID2Players[clientid] = p;
	mUIN2Players[uin] = p;

	return p;
}
//----------------------------------------------------------------------------
void AIOTUserManager::FreeUser(AIOTUser *player)
{
	std::map<unsigned int, AIOTUser *>::iterator iter =
		mClientID2Players.find(player->ClientID);
	if (iter != mClientID2Players.end())
	{
		mClientID2Players.erase(iter);
	}
	iter = mUIN2Players.find(player->UIN);
	if (iter != mUIN2Players.end())
	{
		mUIN2Players.erase(iter);
	}

	player->IsUsed = false;
	player->AIOTUin = 0;
	player->ClientID = 0;
	player->APPUins.clear();

	mFreePlayers.push_back(player);
}
//----------------------------------------------------------------------------
AIOTUser *AIOTUserManager::GetPlayerByClientID(unsigned int clientid)
{
	auto iter = mClientID2Players.find(clientid);
	if (iter != mClientID2Players.end()) return iter->second;
	else return NULL;
}
//----------------------------------------------------------------------------
AIOTUser *AIOTUserManager::GetPlayerByUIN(unsigned int uin)
{
	auto  iter = mUIN2Players.find(uin);
	if (iter != mUIN2Players.end())
		return iter->second;

	return 0;
}
//----------------------------------------------------------------------------
AIOTUser *AIOTUserManager::GetPlayerByUKey(const FString &ukey)
{
	auto it = mUIN2Players.begin();
	for (; it != mUIN2Players.end(); it++) {
		if (ukey == it->second->UKey)
		{
			return it->second;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
std::map<unsigned int, AIOTUser *> &AIOTUserManager::GetClientMap()
{
	return mClientID2Players;
}
//----------------------------------------------------------------------------
std::map<unsigned int, AIOTUser *> &AIOTUserManager::GetUINMap()
{
	return mUIN2Players;
}
//----------------------------------------------------------------------------