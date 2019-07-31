// PX2SkillBloodTextObject.hpp

#ifndef PX2SKILLBLOODTEXTOBJECT_HPP
#define PX2SKILLBLOODTEXTOBJECT_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2Object.hpp"
#include "PX2UIFText.hpp"

namespace PX2
{

	class SKILLSYS_DLL_ITEM SkillBloodTextObj : public Object
	{
	public:
		SkillBloodTextObj();
		virtual ~SkillBloodTextObj();

		static float UpSpeed;

		UIFText *GetTextBlood();

		enum Type
		{
			T_ADD,
			T_MINUS,
			T_MAX_TYPE
		};
		void SetType(Type type);
		Type GetType() const;

		void SetValue(int val);
		int GetValue() const;

		void SetPostion(const APoint &pos);
		const APoint &GetPosition() const;

		void SetLife(int life);
		bool IsOver() const;

		void Update(int elapsedMicroSeconds);

	protected:
		void SetOver(bool over);

		UIFTextPtr mTextBlood;

		Type mTheType;
		int mValue;
		int mLife;
		int mUpdateTime;
		APoint mPosition;
		bool mIsOver;
		float mAlpha;
	};
	typedef Pointer0<SkillBloodTextObj> BloodTextObjPtr;

}

#endif
