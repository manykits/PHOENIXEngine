// PX2SkillEffectMan.hpp

#ifndef PX2SKILLEFFECTMAN_HPP
#define PX2SKILLEFFECTMAN_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2Movable.hpp"
#include "PX2FString.hpp"

namespace PX2
{

	class SKILLSYS_DLL_ITEM SkillEffectMan : public Singleton<SkillEffectMan>
	{
	public:
		SkillEffectMan();
		virtual ~SkillEffectMan();

		void Clear();

		Movable *AddEffect(FString effectName, const std::string &filename);
		Movable *GetEffect(FString effectName);

	protected:
		std::map<FString, MovablePtr> mEffects;
	};

#define PX2_SEMAN SkillEffectMan::GetSingleton()

}

#endif