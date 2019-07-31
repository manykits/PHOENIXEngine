// PX2SkillDefManager.hpp

#include "PX2SkillDefManager.hpp"
#include "PX2CSVParser.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

const std::string V_DATA = "Data/Voxel/";

//----------------------------------------------------------------------------
SkillDefManager::SkillDefManager()
{
}
//----------------------------------------------------------------------------
SkillDefManager::~SkillDefManager()
{
}
//----------------------------------------------------------------------------
void SkillDefManager::Clear()
{
}
//----------------------------------------------------------------------------
bool SkillDefManager::LoadAllDefs()
{
	bool ret = false;
	ret = LoadCharacters(V_DATA + "defs/character.csv");
	assertion(ret, "");
	ret = LoadMonsters(V_DATA + "defs/monster.csv");
	assertion(ret, "");
	ret = LoadSkills(V_DATA + "defs/skill.csv");
	assertion(ret, "");
	ret = LoadBufs(V_DATA + "defs/buf.csv");
	assertion(ret, "");
	ret = LoadItems(V_DATA + "defs/item.csv");
	assertion(ret, "");

	return true;
}
//----------------------------------------------------------------------------
bool SkillDefManager::ReLoadAllDefs()
{
	Clear();
	return LoadAllDefs();
}
//----------------------------------------------------------------------------
bool SkillDefManager::LoadSkills(const std::string &filename)
{
	CSVParser parser;

	if (!parser.Load(filename))
		return false;

	parser.SetTitleLine(1);

	int numLines = (int)parser.GetNumLines();
	for (int i = 2; i < numLines; ++i)
	{
		DefSkill def;
		def.ID = parser[i]["ID"].Int();
		if (0 == def.ID) continue;

		def.Icon = parser[i]["Icon"].String();
		def.Name = parser[i]["Name"].String();
		def.Desc = parser[i]["Desc"].String();

		def.iTrigerType = parser[i]["TrigerType"].Int();
		def.TheTrigerType = (DefSkill::TrigerType)def.iTrigerType;

		def.CD = parser[i]["CD"].Int();

		def.AimDistance = parser[i]["AimDistance"].Float();
		def.AimEffect = parser[i]["AimEffect"].String();
		def.AimEffect1 = parser[i]["AimEffect1"].String();

		// activate
		def.PrepareTime = parser[i]["PrepareTime"].Int();

		def.PrepareAnim = parser[i]["PrepareAnim"].String();
		def.PrepareEffect = parser[i]["PrepareEffect"].String();
		def.PrepareEffectAnchor = parser[i]["PrepareEffectAnchor"].String();
		def.PreparePosEffect = parser[i]["PreparePosEffect"].String();

		// progress
		def.iType = parser[i]["Type"].Int();
		def.TheType = (DefSkill::Type)def.iType;

		def.iMode = parser[i]["Mode"].Int();
		def.TheMode = (DefSkill::Mode)def.iMode;

		def.ModeNumPerSecond = parser[i]["ModeNumPerSecond"].Int();
		def.ModeNumMax = parser[i]["ModeNumMax"].Int();
		def.IsModelInterrupt = parser[i]["ModeInterrupt"].GetBool();
		def.AffectTime = parser[i]["AffectTime"].Int();
		def.AffectKey = parser[i]["AffectKey"].String();

		DefSkillAnimEffect effect0;
		effect0.AffectAnim = parser[i]["AffectAnim"].String();
		effect0.AffectEffectKey = parser[i]["AffectEffectKey"].String();
		effect0.AffectEffect = parser[i]["AffectEffect"].String();
		effect0.AffectEffectAnchor = parser[i]["AffectEffectAnchor"].String();;
		effect0.AffectPosEffect = parser[i]["AffectPosEffect"].String();;
		effect0.AffectPosEffectAnchor = parser[i]["AffectPosEffectAnchor"].String();
		def.AnimEffects.push_back(effect0);

		DefSkillAnimEffect effect1;
		effect1.AffectAnim = parser[i]["AffectAnim1"].String();
		effect1.AffectEffectKey = parser[i]["AffectEffectKey1"].String();
		effect1.AffectEffect = parser[i]["AffectEffect1"].String();
		effect1.AffectEffectAnchor = parser[i]["AffectEffectAnchor1"].String();;
		effect1.AffectPosEffect = parser[i]["AffectPosEffect1"].String();;
		effect1.AffectPosEffectAnchor = parser[i]["AffectPosEffectAnchor1"].String();
		def.AnimEffects.push_back(effect1);

		def.FlySpeed = parser[i]["FlySpeed"].Float();
		def.FlyTime = parser[i]["FlyTime"].Int();
		def.FlyEffect = parser[i]["FlyEffect"].String();
		def.FlyAnchor = parser[i]["FlyAnchor"].String();

		// result
		def.NumMaxAffect = parser[i]["NumMaxAffect"].Int();
		def.iAffectGroup = parser[i]["AffectGroup"].Int();
		def.TheAffectGroupType = (DefSkill::AffectGroupType)def.iAffectGroup;
		def.RangeRadius = parser[i]["RangeRadius"].Float();
		def.RangeDegree = parser[i]["RangeDegree"].Int();
		def.HitAnim = parser[i]["HitAnim"].String();
		def.HitEffect = parser[i]["HitEffect"].String();
		def.HitEffectAnchor = parser[i]["HitEffectAnchor"].String();
		def.HitPosEffect = parser[i]["HitPosEffect"].String();

		def.iCalAffectType = parser[i]["CalAffectType"].Int();
		def.TheCalAffectType = (DefSkill::CalAffectType)def.iCalAffectType;

		def.MonsterID = parser[i]["MonsterID"].Int();
		def.iMonsterPosType = parser[i]["MonsterPosType"].Int();
		def.TheMonsterPosType = (DefSkill::MonsterPosType)def.iMonsterPosType;
		def.MonsterPosData = parser[i]["MonsterPosData"].String();

		for (int j = 0; j < 4; j++)
		{
			std::string jStr = StringHelp::IntToString(j);
			std::string buffStr = "Buff" + jStr;
			std::string buffGroupStr = buffStr + "Group";

			int id = parser[i][buffStr.c_str()].Int();
			if (0 != id)
			{
				DefSkillBuf defSkillBuf;
				defSkillBuf.TypeID = id;
				defSkillBuf.iGroupType = parser[i][buffGroupStr.c_str()].Int();
				defSkillBuf.TheGroupType = (DefSkillBuf::GroupType)(defSkillBuf.iGroupType);
				def.DefSkillBufs.push_back(defSkillBuf);
			}
		}


		DefSkills[def.ID] = def;
	}

	return true;
}
//----------------------------------------------------------------------------
bool SkillDefManager::LoadBufs(const std::string &filename)
{
	CSVParser parser;

	if (!parser.Load(filename))
		return false;

	parser.SetTitleLine(1);

	int numLines = (int)parser.GetNumLines();
	for (int i = 2; i < numLines; ++i)
	{
		DefBuf def;
		def.ID = parser[i]["ID"].Int();
		if (0 == def.ID) continue;

		def.Icon = parser[i]["Icon"].String();
		def.Name = parser[i]["Name"].String();
		def.Desc = parser[i]["Desc"].String();

		def.Tag = parser[i]["Tag"].String();
		def.iTagMode = parser[i]["TagMode"].Int();
		def.TheTagMode = (DefBuf::TagMode)def.iTagMode;

		def.iMode = parser[i]["Mode"].Int();
		def.TheMode = (DefBuf::Mode)def.iMode;

		def.Time = parser[i]["Time"].Int();

		def.MaxHP = parser[i]["MaxHP"].Int();
		def.HP = parser[i]["HP"].Int();
		def.AP = parser[i]["AP"].Int();
		def.DP = parser[i]["DP"].Int();

		def.ChangeID = parser[i]["ChangeID"].Int();

		def.Stun = parser[i]["Stun"].GetBool();

		def.Effect = parser[i]["Effect"].String();
		def.EffectAnchor = parser[i]["EffectAnchor"].String();

		DefBufs[def.ID] = def;
	}

	return true;
}
//----------------------------------------------------------------------------
bool SkillDefManager::LoadCharacters(const std::string &filename)
{
	CSVParser parser;

	if (!parser.Load(filename))
		return false;

	parser.SetTitleLine(1);

	int numLines = (int)parser.GetNumLines();
	for (int i = 2; i < numLines; ++i)
	{
		DefChara def;
		def.ID = parser[i]["ID"].Int();
		if (0 == def.ID) continue;

		def.Name = parser[i]["Name"].String();

		def.Model = parser[i]["Model"].String();

		int skillID0 = parser[i]["Skill0"].Int();
		if (0 != skillID0)
			def.Skills.push_back(skillID0);

		def.BaseHP = parser[i]["BaseHP"].Int();
		def.BaseAP = parser[i]["BaseAP"].Int();
		def.BaseDP = parser[i]["BaseDP"].Int();

		int skillID1 = parser[i]["Skill1"].Int();
		if (0 != skillID1)
			def.Skills.push_back(skillID1);

		int skillID2 = parser[i]["Skill2"].Int();
		if (0 != skillID2)
			def.Skills.push_back(skillID2);

		int skillID3 = parser[i]["Skill3"].Int();
		if (0 != skillID3)
			def.Skills.push_back(skillID3);

		DefCharacters[def.ID] = def;
	}

	return true;
}
//----------------------------------------------------------------------------
bool SkillDefManager::LoadMonsters(const std::string &filename)
{
	CSVParser parser;

	if (!parser.Load(filename))
		return false;

	parser.SetTitleLine(1);

	int numLines = (int)parser.GetNumLines();
	for (int i = 2; i < numLines; ++i)
	{
		DefMonster def;
		def.ID = parser[i]["ID"].Int();
		if (0 == def.ID) continue;

		def.ID = parser[i]["ID"].Int();
		def.CharacterID = parser[i]["CharaID"].Int();

		DefMonsters[def.ID] = def;
	}

	return true;
}
//----------------------------------------------------------------------------
bool SkillDefManager::LoadItems(const std::string &filename)
{
	CSVParser parser;

	if (!parser.Load(filename))
		return false;

	parser.SetTitleLine(1);

	int numLines = (int)parser.GetNumLines();
	for (int i = 2; i < numLines; ++i)
	{
		DefItem def;
		def.ID = parser[i]["ID"].Int();
		if (0 == def.ID) continue;

		def.ID = parser[i]["ID"].Int();
		def.Icon = parser[i]["Icon"].String();
		def.Name = parser[i]["Name"].String();
		def.Desc = parser[i]["Desc"].String();
		def.iType = parser[i]["Type"].Int();
		def.TheType = (DefItem::Type)def.iType;
		def.Model = parser[i]["Model"].String();
		def.Anchor = parser[i]["Anchor"].String();
		def.Skill = parser[i]["Skill"].Int();
		def.Buf = parser[i]["Buf"].Int();
		def.Mtl = parser[i]["Mtl"].String();

		DefItems[def.ID] = def;
	}

	return true;
}
//----------------------------------------------------------------------------