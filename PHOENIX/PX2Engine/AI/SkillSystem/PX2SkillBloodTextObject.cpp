// PX2SkillBloodTextObject.cpp

#include "PX2SkillBloodTextObject.hpp"
#include "PX2StringHelp.hpp"
#include "PX2InterpCurveUniformScaleCtrl.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
float SkillBloodTextObj::UpSpeed = 1.0f;
//----------------------------------------------------------------------------
SkillBloodTextObj::SkillBloodTextObj() :
mTheType(T_MINUS),
mLife(1000),
mUpdateTime(0),
mIsOver(false),
mAlpha(1.0f)
{
	mTextBlood = new0 UIFText();
	mTextBlood->SetAnchorHor(0.0f, 0.0f);
	mTextBlood->SetAnchorVer(0.0f, 0.0f);
	mTextBlood->SetSize(200.0f, 200.0f);
	mTextBlood->GetText()->SetDrawStyle(FD_SHADOW);
	mTextBlood->GetText()->SetBorderShadowAlpha(0.4f);
	mTextBlood->GetText()->SetFontWidthHeight(20, 20);

	InterpCurveUniformScaleController *icusc = 
		new0 InterpCurveUniformScaleController();
	icusc->Clear();
	icusc->AddPoint(0.0f, 1.0f, ICM_CURVE_AUTO);
	icusc->AddPoint(0.2f, 2.0f, ICM_CURVE_AUTO);
	icusc->AddPoint(0.8f, 1.5f, ICM_CURVE_AUTO);
	mTextBlood->AttachController(icusc);
	icusc->ResetPlay();
}
//----------------------------------------------------------------------------
SkillBloodTextObj::~SkillBloodTextObj()
{
}
//----------------------------------------------------------------------------
UIFText *SkillBloodTextObj::GetTextBlood()
{
	return mTextBlood;
}
//----------------------------------------------------------------------------
void SkillBloodTextObj::SetType(Type type)
{
	mTheType = type;
}
//----------------------------------------------------------------------------
SkillBloodTextObj::Type SkillBloodTextObj::GetType() const
{
	return mTheType;
}
//----------------------------------------------------------------------------
void SkillBloodTextObj::SetValue(int val)
{
	mValue = val;
}
//----------------------------------------------------------------------------
int SkillBloodTextObj::GetValue() const
{
	return mValue;
}
//----------------------------------------------------------------------------
void SkillBloodTextObj::SetPostion(const APoint &pos)
{
	mPosition = pos;
}
//----------------------------------------------------------------------------
const APoint &SkillBloodTextObj::GetPosition() const
{
	return mPosition;
}
//----------------------------------------------------------------------------
void SkillBloodTextObj::SetLife(int life)
{
	mLife = life;
}
//----------------------------------------------------------------------------
void SkillBloodTextObj::SetOver(bool over)
{
	mIsOver = over;
}
//----------------------------------------------------------------------------
bool SkillBloodTextObj::IsOver() const
{
	return mIsOver;
}
//----------------------------------------------------------------------------
void SkillBloodTextObj::Update(int elapsedMicroSeconds)
{
	mUpdateTime += elapsedMicroSeconds;
	 
	int leftTime = mLife - mUpdateTime;

	if (leftTime <= 0)
	{
		SetOver(true);
	}

	if (!IsOver())
	{
		mAlpha = (float)leftTime / (float)mLife;	
		mPosition.Z() += UpSpeed * ((float)elapsedMicroSeconds * 0.001f);

		std::string strBlood;
		if (T_ADD == mTheType)
		{
			strBlood = "+" + StringHelp::IntToString(mValue);
			mTextBlood->GetText()->SetFontColor(Float3::MakeColor(0, 255, 0));
		}
		else if (T_MINUS == mTheType)
		{
			strBlood = "-" + StringHelp::IntToString(mValue);
			mTextBlood->GetText()->SetFontColor(Float3::MakeColor(255, 0, 0));
		}
		mTextBlood->GetText()->SetAlpha(mAlpha);
		mTextBlood->GetText()->SetText(strBlood);
	}
	else
	{
		Node *parent = (Node*)mTextBlood->GetParent();
		if (parent)
		{
			parent->DetachChild(mTextBlood);
		}
	}
}
//----------------------------------------------------------------------------