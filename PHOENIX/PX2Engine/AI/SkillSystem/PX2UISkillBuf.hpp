// PX2UISkillBuf.hpp

#ifndef PX2UISKILLBUF_HPP
#define PX2UISKILLBUF_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2SkillBuf.hpp"
#include "PX2UIButton.hpp"
#include "PX2UIFPicBox.hpp"

namespace PX2
{ 

	class SKILLSYS_DLL_ITEM UISkillBuf : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_NEW(UISkillBuf);
		PX2_DECLARE_STREAM(UISkillBuf);

	public:
		UISkillBuf();
		virtual ~UISkillBuf();

		void SetSkillBuf(SkillBuf *skillBuf);
		SkillBuf *GetSkillBuf();

	protected:
		virtual void UpdateWorldData(double applicationTime, 
			double elapsedTime);

		SkillBufPtr mSkillBuf;
		UIButtonPtr mBut;
		UIFPicBoxPtr mProgrssPic;
	};

	PX2_REGISTER_STREAM(UISkillBuf);
	typedef Pointer0<UISkillBuf> V_UIBufPtr;

}

#endif