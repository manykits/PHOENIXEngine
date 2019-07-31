// PX2SkillDef.hpp

#ifndef PX2SKILLDEF_HPP
#define PX2SKILLDEF_HPP

#include "PX2CorePre.hpp"
#include "PX2SkillDef.hpp"

namespace PX2
{

	class DefBuf
	{
	public:
		DefBuf();
		~DefBuf();

		int ID;
		std::string Icon;
		std::string Name;
		std::string Desc;

		std::string Tag;

		enum TagMode
		{
			TM_OVERLAP,
			TM_REPLACE,
			TM_MAX_TYPE
		};
		TagMode TheTagMode;
		int iTagMode;

		enum Mode
		{
			M_LAST,
			M_LAST_PERSECOND,
			M_MAX_TYPE
		};
		Mode TheMode;
		int iMode;

		int Time;

		int MaxHP;
		int HP;
		int AP;
		int DP;

		int ChangeID;

		bool Stun;

		std::string Effect;
		std::string EffectAnchor;
	};

	class DefChara
	{
	public:
		DefChara();
		~DefChara();

		int ID;
		std::string Name;
		std::string Model;

		int BaseHP;
		int BaseAP;
		int BaseDP;

		std::vector<int> Skills;
	};

	class DefMonster
	{
	public:
		DefMonster();
		~DefMonster();

		int ID;
		int CharacterID;
	};

	class DefSkillBuf
	{
	public:
		DefSkillBuf();
		~DefSkillBuf();

		int TypeID;

		enum GroupType
		{
			GT_SKILL,
			GT_SELF,
			GT_SELF_GROUP
		};

		GroupType TheGroupType;
		int iGroupType;
	};

	class DefSkillAnimEffect
	{
	public:
		DefSkillAnimEffect();
		~DefSkillAnimEffect();

		std::string AffectAnim;
		std::string AffectEffectKey;
		std::string AffectEffect;
		std::string AffectEffectAnchor;
		std::string AffectPosEffect;
		std::string AffectPosEffectAnchor;
	};

	class DefSkill
	{
	public:
		DefSkill();
		~DefSkill();

		int ID;
		std::string Icon;
		std::string Name;
		std::string Desc;

		enum TrigerType
		{
			TT_BUT_DOWN = 0,
			TT_BUT_HOLD,
			TT_BUT_UP,
			TT_BUT_DOWN_PREPARE_UP_ACT,
			TT_BUT_DOWN_PREPARE_TOTIME_ACT,
			TT_BUT_DOWN_PREPARE_TOTIME_HOLDACT,
			TT_BUT_DOWN_CHOOSEDIR,
			TT_BUT_DOWN_CHOOSERANGE,
			TT_BUT_DOWN_CHOOSEDIR_LAUCH,
			TT_EVENT_BATSTART = 10,
			TT_EVENT_BLOOD,
			TT_MAX_TYPE
		};
		TrigerType TheTrigerType;
		int iTrigerType;

		float AimDistance;
		std::string AimEffect;
		std::string AimEffect1;

		int CD;

		int PrepareTime;
		std::string PrepareAnim;
		std::string PrepareEffect;
		std::string PrepareEffectAnchor;
		std::string PreparePosEffect;

		enum Type
		{
			T_NEAR,
			T_FAR_FLY,
			T_FAR_CHAIN,
			T_FAR_CHAIN_LINKS,
			T_MAX_TYPE
		};
		Type TheType;
		int iType;

		enum Mode
		{
			M_SIGNLE,
			M_MULTI,
			M_MULTI_CONTINUE,
			M_MAX_MODE
		};
		Mode TheMode;
		int iMode;

		int ModeNumPerSecond;
		int ModeNumMax;

		bool IsModelInterrupt;

		int AffectTime;
		std::string AffectKey;

		std::vector<DefSkillAnimEffect> AnimEffects;

		float FlySpeed;
		int FlyTime;
		std::string FlyEffect;
		std::string FlyAnchor;

		int NumMaxAffect;

		enum AffectGroupType
		{
			AGT_ENEMY,
			AGT_SELF,
			AGT_SELFGROUP,
			AGT_NONE,
			AGT_MAX_TYPE
		};
		AffectGroupType TheAffectGroupType;
		int iAffectGroup;
		float RangeRadius;
		int RangeDegree;

		std::string HitAnim;
		std::string HitEffect;
		std::string HitEffectAnchor;
		std::string HitPosEffect;

		enum CalAffectType
		{
			CFT_NONE,
			CFT_DEFHIT,
			CFT_DEFRECV,
			CFT_MAX_TYPE
		};
		CalAffectType TheCalAffectType;
		int iCalAffectType;

		int MonsterID;
		enum MonsterPosType
		{
			MPT_ANCHOR,
			MPT_SKILL,
			MPT_MAX_TYPE
		};
		MonsterPosType TheMonsterPosType;
		int iMonsterPosType;
		std::string MonsterPosData;

		std::vector<DefSkillBuf> DefSkillBufs;
	};

	class DefItem
	{
	public:
		DefItem();
		~DefItem();

		int ID;
		std::string Icon;
		std::string Name;
		std::string Desc;

		enum Type
		{
			T_NORMAL,
			T_BLOCK,
			T_MAX_TYPE
		};
		int iType;
		Type TheType;

		std::string Model;
		std::string Mtl;
		std::string Anchor;
		int Skill;
		int Buf;
	};

	// defines
	enum ASM_Posture
	{
		ASM_NONE,
		ASM_DEATH,
		ASM_FIGHTPOSE,
		ASM_HARDSTRAIGHT,
		ASM_RUNNING,
		ASM_STANDING,
		ASM_SKILLING,
		ASM_WALKING,
		ASM_MAX_TYPE
	};

}

#endif
