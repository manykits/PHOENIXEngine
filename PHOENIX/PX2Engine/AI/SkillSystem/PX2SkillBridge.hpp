// PX2SkillBridge.hpp

#ifndef PX2SKILLBRIDGE_HPP
#define PX2SKILLBRIDGE_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2SkillAffect.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class SKILLSYS_DLL_ITEM SkillBridge : public Singleton<SkillBridge>
	{
	public:
		SkillBridge();
		virtual ~SkillBridge();

		virtual void C2S_CharacterActivateSkillInstance(int roomID, int mapID, 
			int charaID, int skillID, const APoint &fromPos, const AVector &fromDir,
			int aimTargetID, const APoint &targetPos);

		virtual void S2C_CharacterActivateSkillInstance(int roomID, int mapID, 
			int charaID, int skillID, const APoint & fromPos, const AVector &fromDir, 
			int aimTargetID, const APoint &targetPos);

		virtual void C2S_CharacterAffect(int roomID, int mapID, 
			int fromID, int fromSkillID, int toTargetID, bool calValue);

		virtual void S2C_CharacterAffect(int roomID, int mapID, int toID, SkillAffect sa);

		virtual void C2S_GenMonsters(int roomID, int mapID, int fromID, std::vector<GenActorObj> &monster);

		virtual void S2C_GenedMonsters(int roomID, int mapID, int fromID, std::vector<GenActorObj> &monsters);
	};

#define PX2_SKILLB SkillBridge::GetSingleton()

}

#endif
