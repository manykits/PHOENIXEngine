// PX2SkillBuf.hpp

#ifndef PX2SKILLBUF_HPP
#define PX2SKILLBUF_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2Object.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2SkillDef.hpp"

namespace PX2
{

	class SkillChara;

	class SKILLSYS_DLL_ITEM SkillBuf : public Object
	{
		PX2_NEW(SkillBuf);

	public:
		SkillBuf();
		virtual ~SkillBuf();

		void SetTypeID(int typeID);
		int GetTypeID() const;

		DefBuf *GetDefBuf();
		const std::string &GetTag() const;

		void SetChara(SkillChara *chara);
		SkillChara *GetChara();

		void OnAdded();
		void OnRemove();

		float GetPercent() const;
		bool IsOver() const;

		void Update(int elapsedMicroSeconds);

	private:
		int mTypeID;

		DefBuf *mDefBuf;
		SkillChara *mChara;

		int mTimeMicroSeconds;
		bool mIsOver;
	};
	typedef Pointer0<SkillBuf> SkillBufPtr;

#include "PX2SkillBuf.inl"

}

#endif