// PX2User.cpp

#include "PX2User.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
User::User()
{
}
//----------------------------------------------------------------------------
User::~User()
{
}
//----------------------------------------------------------------------------
void User::Update(float appTime, float elapsedTime)
{
	for (int i = 0; i < (int)mUserActors.size(); i++)
	{
		if (mUserActors[i])
		{
			mUserActors[i]->Update(appTime, elapsedTime);
		}
	}
}
//----------------------------------------------------------------------------
void User::SetUserName(const std::string &useName)
{
	mUserName = useName;
}
//----------------------------------------------------------------------------
void User::AddUserActor(UserActor *userActor)
{
	mUserActors.push_back(userActor);
	userActor->SetUser(this);
}
//----------------------------------------------------------------------------
void User::DeleteUserActor(UserActor *userActor)
{
	std::vector<UserActorPtr>::iterator it = mUserActors.begin();
	for (; it != mUserActors.end();)
	{
		if (userActor == *it)
		{
			userActor->SetUser(0);
			it = mUserActors.erase(it);
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
UserActor *User::GetUserActor(int index) const
{
	if (0 <= index && index < (int)mUserActors.size())
		return mUserActors[index];

	return 0;
}
//----------------------------------------------------------------------------
UserActor *User::GetUserActorByName(const std::string &useName) const
{
	if (mUserActors.size() <= 0)
	{
		return 0;
	}

	for (int i = 0; i < (int)mUserActors.size(); i++)
	{
		if (mUserActors[i] != 0 && mUserActors[i]->GetName() == useName)
		{
			return mUserActors[i];
		}
	}

	return 0;
}
//----------------------------------------------------------------------------