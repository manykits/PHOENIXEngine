// UISkill.cpp

#include "PX2UISkill.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UISkill);
PX2_IMPLEMENT_STREAM(UISkill);
PX2_IMPLEMENT_FACTORY(UISkill);

//----------------------------------------------------------------------------
UISkill::UISkill()
{
	mUIRound = new0 UIRound();
	AttachChild(mUIRound);
	mUIRound->SetAnchorHor(0.0f, 1.0f);
	mUIRound->SetAnchorVer(0.0f, 1.0f);

	mTextSkillName = new0 UIFText();
	AttachChild(mTextSkillName);
	mTextSkillName->LocalTransform.SetTranslateY(-2.0f);
	mTextSkillName->SetAnchorHor(0.0f, 1.0f);
	mTextSkillName->SetAnchorVer(0.0f, 0.0f);
	mTextSkillName->SetPivot(0.5f, 0.0f);
	mTextSkillName->SetSize(0.0f, 20.0f);
	mTextSkillName->GetText()->SetFontColor(Float3::WHITE);

	mTextSkillTime = new0 UIFText();
	AttachChild(mTextSkillTime);
	mTextSkillTime->LocalTransform.SetTranslateY(-4.0f);
	mTextSkillTime->SetAnchorHor(0.0f, 1.0f);
	mTextSkillTime->SetAnchorVer(0.0f, 1.0f);
	mTextSkillTime->GetText()->SetFontColor(Float3::WHITE);
	mTextSkillTime->GetText()->SetText("10");
	mTextSkillTime->Show(false);

	mUIRound->SetMemUICallback(this,
		(UIFrame::MemUICallback)(&UISkill::_UICallback));
}
//----------------------------------------------------------------------------
UISkill::~UISkill()
{
}
//----------------------------------------------------------------------------
void UISkill::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	if (!mSkill)
		return;

	bool isDoCDing = mSkill->IsDoCDing();

	UIButton *but = mUIRound->GetButton();
	but->Enable(!isDoCDing);

	if (isDoCDing)
	{
		int leftCD = mSkill->GetCD() - mSkill->GetCDing();
		float fCD = (float)leftCD * 0.001f;
		int iCD = (int)Mathf::Ceil(fCD);
		std::string showStr;
		if (leftCD <= 1000)
		{
			showStr = StringHelp::FloatToString(fCD, 1);
		}
		else
		{
			showStr = StringHelp::IntToString(iCD);
		}

		mTextSkillTime->Show(true);
		mTextSkillTime->GetText()->SetText(showStr);
	}
	else
	{
		mTextSkillTime->Show(false);
	}
}
//----------------------------------------------------------------------------
void UISkill::_UICallback(UIFrame *frame, UICallType type)
{
	UIRound *round = DynamicCast<UIRound>(frame);
	if (round)
	{
		if (type == UICT_ROUND_PRESSED)
		{
			if (mSkill)
				mSkill->Activate_SkillBut_ClickDown();
		}
		else if (type == UICT_ROUND_RELEASED)
		{
			if (mSkill)
				mSkill->Activate_SkillBut_ClickUp();
		}
		else if (type == UICT_ROUND_RELEASEDNOTPICK)
		{

		}
		else if (type == UICT_ROUND_DRAG)
		{
			if (mSkill)
				mSkill->Activate_SkillBut_Drag();
		}
	}
}
//----------------------------------------------------------------------------
void UISkill::SetSkill(Skill *skill)
{
	mSkill = skill;
}
//----------------------------------------------------------------------------
Skill *UISkill::GetSkill()
{
	return mSkill;
}
//----------------------------------------------------------------------------
UIRound *UISkill::GetRoundBut()
{
	return mUIRound;
}
//----------------------------------------------------------------------------
UIFText *UISkill::GetText()
{
	return mTextSkillName;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UISkill::UISkill(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UISkill::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIFrame, source);
}
//----------------------------------------------------------------------------
void UISkill::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UISkill::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UISkill::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void UISkill::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIFrame, target);
}
//----------------------------------------------------------------------------
int UISkill::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------