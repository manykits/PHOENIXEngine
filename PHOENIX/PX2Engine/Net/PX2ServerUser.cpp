// PX2ServerUser.cpp

#include "PX2ServerUser.hpp"
using namespace PX2;

//-----------------------------------------------------------------------------
ServerUser::ServerUser() :
IsUsed(false)
{
	Reset();
}
//-----------------------------------------------------------------------------
ServerUser::~ServerUser()
{
}
//-----------------------------------------------------------------------------
void ServerUser::Reset()
{
	UIN = 0;
	ClientID = 0;

	SetState(SUT_INIT);
}
//-----------------------------------------------------------------------------
void ServerUser::SetState(UserState state)
{
	mState = state;
}
//-----------------------------------------------------------------------------