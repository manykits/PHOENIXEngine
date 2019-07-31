// AIOTUser.hpp

#ifndef AIOTUSER_HPP
#define AIOTUSER_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2FString.hpp"

namespace PX2
{

	enum AIOTUserState
	{
		AUS_INIT = 0,  //初始化，没有连接上
		AUS_CONNECTED,
		AUS_PASSED,
		AUS_MAX_STATE
	};

	class AIOTUser
	{
	public:
		AIOTUser();
		~AIOTUser();

		void Reset();
		void SetState(AIOTUserState pst);

		std::string Username_AIOT;
		std::string Key_AIOT;
		FString UKey;

		unsigned int AIOTUin;

		std::vector<unsigned int> APPUins;

		bool IsHasAppUIN(unsigned int uin);
		bool AddAppUIN(unsigned int uin);
		bool RemoveAppUIN(unsigned int uin);
		void ClearAppUINs();

	public:
		bool IsUsed;
		unsigned int UIN;
		unsigned int ClientID;
		AIOTUserState CurState;
	};
}

#endif