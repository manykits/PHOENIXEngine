// PX2SkillChara.hpp

#ifndef PX2SKILLCHARA_HPP
#define PX2SKILLCHARA_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2Object.hpp"
#include "PX2Skill.hpp"
#include "PX2SkillBuf.hpp"
#include "PX2Animation.hpp"
#include "PX2SkillBloodTextObject.hpp"
#include "PX2SkillItem.hpp"

namespace PX2
{

	class Movable;

	class SKILLSYS_DLL_ITEM SkillChara : public Object
	{
		PX2_NEW(SkillChara);

	public:
		SkillChara();
		virtual ~SkillChara();

		void SetRoomID(int roomID);
		int GetRoomID() const;

		void SetMapID(int mapID);
		int GetMapID() const;

		void Update(int elapsedMicroSeconds);

		void SetHeight(float height);
		float GetHeight() const;

		void SetRadius(float radius);
		float GetRadius() const;

		void SetPosition(const APoint &pos);
		const APoint &GetPosition() const;

		void SetDir(const AVector &dir);
		AVector &GetDir();

		// 设置
		void SetOwner(Movable *mov);
		Movable *GetOwner();

		void SetBeMyS(bool isMyS);
		bool IsBeMyS() const;

		void AddBloodTextObj(SkillBloodTextObj *bloodTextObj);
		std::vector<BloodTextObjPtr> &GetBloodTextObjs();

	protected:
		int mRoomID;
		int mMapID;
		Movable *mOwner;
		float mHeight;
		float mRadius;
		APoint mPosition;
		AVector mDir;
		bool mIsMyS;
		std::vector<BloodTextObjPtr> mBloodObjs;

		// props
	public:
		virtual void SetTypeID(int typeID);
		int GetTypeID() const;

		virtual void SetMonsterID(int monsID);
		int GetMonsterID() const;

		void SetGroupID(int groupID);
		int GetGroupID() const;

		void SetLevel(int level);
		int GetLevel() const;

		DefMonster *GetDefMonster();
		DefChara *GetDefChara();

		virtual Animation *GetAnim(const std::string &animName);
		virtual Animation *PlayAnim(const std::string &animName);
		virtual Node *GetAnchor(const std::string &anchorName);

	protected:
		int mTypeID;
		int mMonsterID;
		int mGroupID;
		int mLevel;

		DefMonster *mDefMonster;
		DefChara *mDefChara;

		// skill
	public:
		bool AddSkill(Skill *skill);
		bool HasSkill(Skill *skill);
		int GetNumSkills() const;
		Skill *GetSkillByID(int id);
		Skill *GetSkillByTypeID(int typeID);
		Skill *GetSkillByName(const std::string &name);
		Skill *GetDefSkill();
		Skill *GetSkill(int i);
		bool RemoveSkill(Skill *skill);
		void RemoveAllSkills();

		bool IsAnySkillHasInst();
		void ResetAllSkillsCD();

		void EquipSkill(Skill *skill);
		bool IsSkillEquipped(Skill *skill);
		void UnEquipSkill(Skill *skill);
		void UnEquipAllSkill();
		int GetNumEquippedSkill() const;
		Skill *GetEquippedSkill(int i);
		const Skill *GetEquippedSkill(int i) const;
		void EquipAllSkills();
		bool IsAnySkillActing() const;
		
		// buf
		bool TryAddBuf(SkillBuf *buf);
		bool AddBuf(SkillBuf *buf);
		bool HasBuf(SkillBuf *buf);
		int GetNumBufs() const;
		SkillBuf *GetBuf(int i);
		bool RemoveBuf(SkillBuf *buf);
		void RemoveAllBufs();

		virtual void OnBufChanged();

	protected:
		std::vector<SkillPtr> mSkills;
		SkillPtr mDefSkill;
		std::vector<SkillPtr> mEquippedSkills;
		std::vector<SkillBufPtr> mBufs;

		// ai
	public:
		void SetWatchDistance(float dist);
		float GetWatchDistance() const;

		virtual void SetAimTargetID(int targetID);
		int GetAimTargetID() const;
		SkillChara *GetAimTargetCharacter() const;
		SkillChara *GetAimTargetCharacterAlive() const;

		void SetCanBeAimed(bool can);
		bool IsCanBeAimed() const;
		int GetNumAimingMe() const;

	protected:
		float mWatchDistance;
		bool mIsCanAimed;
		int mAimTargetID;
		int mNumAimingMe;

		// props
	public:
		void SetMaxHP(int maxHP);
		int GetMaxHP() const;

		void SetCurHP(int curHP);
		int GetCurHP() const;
		float GetHPPercent() const;

		void SetCurAP(int curAP);
		int GetCurAP() const;

		void SetCurDP(int curDP);
		int GetCurDP() const;

		virtual void SetDead(bool dead);
		bool IsDead() const;

		virtual void OnSkillActivate(Skill *skill);

		virtual void OnSkillActivateAnimBegin(Skill *skill, Animation *anim);
		virtual void OnSkillActivateAnimOver(Skill *skill, Animation *anim);

		// 当前正在发动的技能动作数量，如果没有才能发动其他技能
		int GetNumActingAnimingSkills() const;

		virtual void OnSkillInstActivate(SkillInst *si);
		virtual void OnSkillInstActivateOver(SkillInst *inst);

		void MarkNeedRecalInfo();
		virtual void OnInfoChanged();

	protected:
		void _RecalInfo();

		int mMaxHP;
		int mCurHP;
		int mCurAP;
		int mCurDP;
		bool mIsDead;

		std::set<Skill*> mActingAnimingSkills;
		SkillPtr mLastActivateSkill;
		SkillInstPtr mLastActivateSkillInst;

		bool mIsNeedRecalInfo;
		
		// item
	public:
		bool AddItem(Item *item);
		bool HasItem(Item *item);
		int GetNumItems() const;
		Item *GetItem(int i);
		Item *GetItemByTypeID(int typeID);
		bool RemoveItem(Item *item);
		void RemoveAllItems();

		void EquipItem(Item *item);
		bool IsItemEquipped(Item *item);
		void UnEquipItem(Item *item);
		void UnEquipAllItems();
		int GetNumEquippedItem() const;
		Item *GetEquippedItem(int i);

	protected:
		std::vector<ItemPtr> mItems;
		std::vector<ItemPtr> mEquippedItems;
	};

#include "PX2SkillChara.inl"
	typedef Pointer0<SkillChara> SkillCharaPtr;

}

#endif