// PX2SkillItem.hpp

#ifndef PX2SKILLITEM_HPP
#define PX2SKILLITEM_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2Object.hpp"
#include "PX2SkillDef.hpp"

namespace PX2
{

	class SkillChara;

	class SKILLSYS_DLL_ITEM Item : public Object
	{
		PX2_NEW(Item);

	public:
		Item();
		virtual ~Item();

		void SetTypeID(int typeID);
		int GetTypeID() const;

		void SetNum(int num);
		int GetNum() const;

		DefItem *GetDefItem();

		void OnAdded();
		void OnRemove();

		bool IsEquipped() const;
		SkillChara *GetChara();

	public_internal:
		void SetEquipped(bool equipped);
		void SetChara(SkillChara *chara);

	protected:
		int mTypeID;
		int mNum;
		bool mIsEquipped;
		DefItem *mDefItem;
		SkillChara *mChara;
	};

#include "PX2SkillItem.inl"
	typedef Pointer0<Item> ItemPtr;

}

#endif