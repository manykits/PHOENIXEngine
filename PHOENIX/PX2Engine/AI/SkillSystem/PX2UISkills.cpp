// PX2UISkills.cpp

#include "PX2UISkills.hpp"
#include "PX2SkillEvent.hpp"
#include "PX2SkillChara.hpp"
#include "PX2SkillEvent.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UISkills);
PX2_IMPLEMENT_STREAM(UISkills);
PX2_IMPLEMENT_FACTORY(UISkills);

//----------------------------------------------------------------------------
UISkills::UISkills()
{
	ComeInEventWorld();

	Sizef size(300.0f, 300.0f);
	SetSize(size);

	SetAnchorHor(1.0f, 1.0f);
	SetAnchorVer(0.0f, 0.0f);
	SetPivot(1.0f, 0.0f);

	SetChildPickOnlyInSizeRange(false);
}
//----------------------------------------------------------------------------
UISkills::~UISkills()
{
}
//----------------------------------------------------------------------------
void UISkills::OnEvent(Event *ent)
{
	if (SkillES::IsEqual(ent, SkillES::SetMainChara))
	{
		SkillChara *chara = ent->GetData<SkillChara*>();
		if (chara)
		{
			ClearSkills();
			RefreshSkills(chara);
		}
	}
}
//----------------------------------------------------------------------------
void UISkills::ClearSkills()
{
	for (int i = 0; i < (int)mUISkills.size(); i++)
	{
		UISkill *uiSkill = mUISkills[i];
		DetachChild(uiSkill);
	}
}
//----------------------------------------------------------------------------
void UISkills::RefreshSkills(SkillChara *skillChara)
{
	APoint pos0(-80.0f, 0.0f, 80.0f);

	for (int i = 0; i <skillChara->GetNumEquippedSkill(); i++)
	{
		Skill *skill = skillChara->GetEquippedSkill(i);
		DefSkill *defSkill = skill->GetDefSkill();

		UISkill *uiSkill = new0 UISkill();
		mUISkills.push_back(uiSkill);
		AttachChild(uiSkill);
		uiSkill->SetChildPickOnlyInSizeRange(false);
		uiSkill->GetText()->GetText()->SetText(defSkill->Name);
		uiSkill->SetSkill(skill);

		float sz = 100.0f;
		float roundRadius = sz * 0.5f;
		float butSize = sz * 0.5f;
		bool dragable = false;
		bool showBack = false;

		APoint butPos;
		if (0 == i)
		{
			sz = sz * 1.4f;
			butPos = pos0;
		}
		else
		{
			float degree = (i - 1) * 45.0f * Mathf::DEG_TO_RAD;
			float moveLength = 180.0f;
			float xMoveLength = -moveLength * Mathf::Cos(degree);
			float yMoveLength = moveLength * Mathf::Sin(degree);

			butPos = pos0 + AVector(xMoveLength, 0.0f, yMoveLength);
		}

		DefSkill::TrigerType tt = defSkill->TheTrigerType;
		if (DefSkill::TT_BUT_DOWN_CHOOSEDIR == tt ||
			DefSkill::TT_BUT_DOWN_CHOOSERANGE == tt)
		{
			dragable = true;
			showBack = true;
		}
		else
		{
			dragable = false;
			butSize = sz;
			showBack = false;
		}

		uiSkill->GetRoundBut()->GetBack()->Show(showBack);
		uiSkill->GetRoundBut()->SetDragable(dragable);
		uiSkill->GetRoundBut()->GetButton()->SetSize(butSize, butSize);
		uiSkill->GetRoundBut()->SetRoundRadius(roundRadius);

		uiSkill->SetSize(sz, sz);
		uiSkill->SetAnchorHor(1.0f, 1.0f);
		uiSkill->SetAnchorVer(0.0f, 0.0f);
		uiSkill->SetAnchorParamHor(butPos.X(), butPos.Z());
		uiSkill->SetAnchorParamVer(butPos.Z(), butPos.Z());
	}
}
//----------------------------------------------------------------------------
void UISkills::CreateSampleSkills()
{
	APoint pos0(-80.0f, 0.0f, 80.0f);

	for (int i = 0; i < 4; i++)
	{
		Skill *skill = new0 Skill();
		DefSkill *defSkill = skill->GetDefSkill();

		UISkill *uiSkill = new0 UISkill();
		mUISkills.push_back(uiSkill);
		AttachChild(uiSkill);
		uiSkill->SetChildPickOnlyInSizeRange(false);
		if (defSkill)
			uiSkill->GetText()->GetText()->SetText(defSkill->Name);
		else
			uiSkill->GetText()->GetText()->SetText("None");
		uiSkill->SetSkill(skill);

		float sz = 100.0f;
		float roundRadius = sz * 0.5f;
		float butSize = sz * 0.5f;
		bool dragable = false;
		bool showBack = false;

		APoint butPos;
		if (0 == i)
		{
			sz = sz * 1.4f;
			butPos = pos0;
		}
		else
		{
			float degree = (i - 1) * 45.0f * Mathf::DEG_TO_RAD;
			float moveLength = 180.0f;
			float xMoveLength = -moveLength * Mathf::Cos(degree);
			float yMoveLength = moveLength * Mathf::Sin(degree);

			butPos = pos0 + AVector(xMoveLength, 0.0f, yMoveLength);
		}

		DefSkill::TrigerType tt = DefSkill::TT_BUT_DOWN;
		if (defSkill)
			tt = defSkill->TheTrigerType;
		if (DefSkill::TT_BUT_DOWN_CHOOSEDIR == tt ||
			DefSkill::TT_BUT_DOWN_CHOOSERANGE == tt)
		{
			dragable = true;
			showBack = true;
		}
		else
		{
			dragable = false;
			butSize = sz;
			showBack = false;
		}

		uiSkill->GetRoundBut()->GetBack()->Show(showBack);
		uiSkill->GetRoundBut()->SetDragable(dragable);
		uiSkill->GetRoundBut()->GetButton()->SetSize(butSize, butSize);
		uiSkill->GetRoundBut()->SetRoundRadius(roundRadius);

		uiSkill->SetSize(sz, sz);
		uiSkill->SetAnchorHor(1.0f, 1.0f);
		uiSkill->SetAnchorVer(0.0f, 0.0f);
		uiSkill->SetAnchorParamHor(butPos.X(), butPos.Z());
		uiSkill->SetAnchorParamVer(butPos.Z(), butPos.Z());
	}
}
//----------------------------------------------------------------------------
void UISkills::_UICallback(UIFrame *frame, UICallType type)
{
	PX2_UNUSED(frame);

	if (type == UICT_RELEASED)
	{

	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UISkills::UISkills(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UISkills::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIFrame, source);
}
//----------------------------------------------------------------------------
void UISkills::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UISkills::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UISkills::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void UISkills::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIFrame, target);
}
//----------------------------------------------------------------------------
int UISkills::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------