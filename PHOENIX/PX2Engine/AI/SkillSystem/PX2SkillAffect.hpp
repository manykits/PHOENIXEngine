// PX2SkillAffect.hpp

#ifndef PX2SKILLAFFECT_HPP
#define PX2SKILLAFFECT_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2APoint.hpp"
#include "PX2SkillInst.hpp"

namespace PX2
{

	class SKILLSYS_DLL_ITEM GenActorObj
	{
	public:
		GenActorObj();
		~GenActorObj();

		int ID;
		int TypeID;
		int MonsID;
		int GroupID;
		APoint Pos;
		APoint Rot;
	};

	class SKILLSYS_DLL_ITEM SkillAffect
	{
	public:
		SkillAffect();
		~SkillAffect();

		bool IsAffectValue;
		bool IsAffectEffectBlood;

		SkillInst *FromSkillInst;

		enum AffectType
		{
			AT_HP_MINUS,
			AT_HP_ADD,
			AT_MAX_TYPE
		};
		AffectType TheAffectType;

		float Val0;
		float Val1;
		float Val2;
		float Val3;

		std::vector<int> BufIDs;
	};

}

#endif