// AIOTUser.cpp

#include "AIOTUser.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
AIOTUser::AIOTUser() :
	IsUsed(false),
	AIOTUin(0)
{
}
//----------------------------------------------------------------------------
AIOTUser::~AIOTUser()
{
}
//----------------------------------------------------------------------------
void AIOTUser::Reset()
{
	UIN = 0;
	ClientID = 0;

	SetState(AUS_INIT);
}
//----------------------------------------------------------------------------
void AIOTUser::SetState(AIOTUserState pst)
{
	CurState = pst;
}
//----------------------------------------------------------------------------
bool AIOTUser::IsHasAppUIN(unsigned int uin)
{
	for (int i = 0; i < (int)APPUins.size(); i++)
	{
		if (uin == APPUins[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool AIOTUser::AddAppUIN(unsigned int uin)
{
	if (IsHasAppUIN(uin))
		return false;

	APPUins.push_back(uin);
}
//----------------------------------------------------------------------------
bool AIOTUser::RemoveAppUIN(unsigned int uin)
{
	auto it = APPUins.begin();
	for (; it != APPUins.end(); ) {
		if (uin == *it) {
			APPUins.erase(it);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void AIOTUser::ClearAppUINs()
{
	APPUins.clear();
}
//----------------------------------------------------------------------------