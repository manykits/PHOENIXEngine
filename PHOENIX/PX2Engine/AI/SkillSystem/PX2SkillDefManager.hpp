// PX2SkillDefManager.hpp

#ifndef PX2SKILLDEFMANAGER_HPP
#define PX2SKILLDEFMANAGER_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2SkillDef.hpp"

namespace PX2
{

	class SKILLSYS_DLL_ITEM SkillDefManager : public Singleton <SkillDefManager>
	{
	public:
		SkillDefManager();
		virtual ~SkillDefManager();

		void Clear();
		bool ReLoadAllDefs();

		std::map<int, DefBuf> DefBufs;
		std::map<int, DefChara> DefCharacters;
		std::map<int, DefMonster> DefMonsters;
		std::map<int, DefSkill> DefSkills;
		std::map<int, DefItem> DefItems;

	protected:
		bool LoadAllDefs();

		bool LoadSkills(const std::string &filename);
		bool LoadBufs(const std::string &filename);
		bool LoadCharacters(const std::string &filename);
		bool LoadMonsters(const std::string &filename);
		bool LoadItems(const std::string &filename);
	};

#define SKILL_DEFM SkillDefManager::GetSingleton()

}

#endif