// PX2AccoutManager.cpp

#include "PX2AccoutManager.hpp"
#include "PX2XMLData.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
AccoutManager::AccoutManager()
{
}
//----------------------------------------------------------------------------
AccoutManager::~AccoutManager()
{
}
//----------------------------------------------------------------------------
void AccoutManager::Update(float appTime, float elapsedTime)
{
	for (int i = 0; i < (int)mUsers.size(); i++)
	{
		if (mUsers[i])
		{
			mUsers[i]->Update(appTime, elapsedTime);
		}
	}
}
//----------------------------------------------------------------------------
void AccoutManager::AddUser(User *user)
{
	mUsers.push_back(user);
}
//----------------------------------------------------------------------------
void AccoutManager::DeleteUser(User *user)
{
	std::vector<UserPtr>::iterator it = mUsers.begin();

	for (; it != mUsers.end();)
	{
		if (user == *it)
		{
			it = mUsers.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
User *AccoutManager::GetUser(const std::string &name) const
{
	for (int i = 0; i < (int)mUsers.size(); i++)
	{
		if (name == mUsers[i]->GetName())
			return mUsers[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
User *AccoutManager::GetUser(const int i) const
{
	if (i < 0 || (int)mUsers.size() <= 0 || (int)mUsers.size() < i)
	{
		return 0;
	}

	return mUsers[i];

}
//----------------------------------------------------------------------------
void AccoutManager::DeleteAllUsers()
{
	mUsers.clear();
}
//----------------------------------------------------------------------------