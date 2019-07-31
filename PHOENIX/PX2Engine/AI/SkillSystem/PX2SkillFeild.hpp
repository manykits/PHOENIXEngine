// PX2SkillFeild.hpp

#ifndef PX2SKILLFEILD_HPP
#define PX2SKILLFEILD_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2SkillChara.hpp"

namespace PX2
{

	class SKILLSYS_DLL_ITEM SkillFeild : public Singleton<SkillFeild>
	{
	public:
		SkillFeild();
		~SkillFeild();

		void AddSkillChara(SkillChara *chara);
		void RemoveSkillChara(SkillChara *chara);
		bool IsHasSkillChara(SkillChara *chara);
		SkillChara *GetSkillChara(int id);
		int GetNumSkillCharas();

		std::map<int, SkillCharaPtr> &GetSkillCharas();

	private:
		std::map<int, SkillCharaPtr> mSkillCharas;
	};

#include "PX2SkillFeild.inl"

#define PX2_SKILLFEILD SkillFeild::GetSingleton()

}

#endif