// PX2SkillFlyObj.hpp

#ifndef PX2SKILLFLYOBJ_HPP
#define PX2SKILLFLYOBJ_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class SKILLSYS_DLL_ITEM SkillFlyObj : public Object
	{
	public:
		SkillFlyObj();
		virtual ~SkillFlyObj();

		void Update(int elapsedMicroSeconds);

		enum FlyType
		{
			FT_DIR,
			FT_PURSUIT_TARGET,
			FT_PURSUIT_POS,
			FT_MAX_TYPE
		};
		void SetFlyType(FlyType ft);
		FlyType GetFlyType() const;

		void SetOver(bool over);
		bool IsOver() const;

	protected:
		FlyType mFlyType;
		bool mIsOver;
	};

#include "PX2SkillFlyObj.inl"
	typedef Pointer0<SkillFlyObj> SkillFlyObjPtr;

}

#endif