// PX2UserActor.hpp

#ifndef PX2USERACTOR_HPP
#define PX2USERACTOR_HPP

#include "PX2UnityPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class User;

	class PX2_ENGINE_ITEM UserActor : public Object
	{
	public:
		UserActor();
		virtual ~UserActor();

		virtual void Update(float appTime, float elapsedTime);

		void SetServerID(int id);
		int GetServerID() const;

		virtual void SetNickName(const std::string &name);
		const std::string &GetNickName() const;

		virtual void SetLevel(int level);
		int GetLevel() const;
		virtual void SetMaxLevel(int maxLevel);
		int GetMaxLevel() const;

		virtual void SetCurLevelExp(int exp);
		int GetCurLevelExp() const;
		virtual void SetCurLevelMaxExp(int maxExp);
		int GetCurLevelMaxExp() const;

		virtual void SetCurLevelEnergy(int energy);
		int GetCurLevelEnergy() const;
		virtual void SetCurLevelMaxEnergy(int maxEnergy);
		int GetCurLevelMaxEnergy() const;

		void SetGold(int gold);
		int GetGold() const;

		void SetMoney(int money);
		int GetMoney() const;

		User *GetUser();

	public_internal:
		void SetUser(User *user);

	protected:
		User *mUser;
		int mServerID;
		std::string mNickName;
		int mLevel;
		int mMaxLevel;
		int mCurLevelExp;  // 当前等级角色已有经验
		int mCurLevelMaxExp; // 当前等级经验上限
		int mCurLevelEnergy; // 当前等级已有体力值
		int mCurLevelMaxEnergy; // 当前等级体力值上限
		int mGold;
		int mMoney;
	};

#include "PX2UserActor.inl"
	typedef Pointer0<UserActor> UserActorPtr;

}

#endif