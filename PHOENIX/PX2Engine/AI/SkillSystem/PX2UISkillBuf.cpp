// PX2UISkillBuf.cpp

#include "PX2UISkillBuf.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UISkillBuf);
PX2_IMPLEMENT_STREAM(UISkillBuf);
PX2_IMPLEMENT_FACTORY(UISkillBuf);

//----------------------------------------------------------------------------
UISkillBuf::UISkillBuf()
{
	mBut = new0 UIButton();
	AttachChild(mBut);
	mBut->SetAnchorHor(0.0f, 1.0f);
	mBut->SetAnchorVer(0.0f, 1.0f);
	mBut->SetStateColor(UIButtonBase::BS_NORMAL,
		Float3::MakeColor(0, 162, 232));

	mProgrssPic = new0 UIFPicBox();
	AttachChild(mProgrssPic);
	mProgrssPic->GetUIPicBox()->SetTexture("Data/engine/white.png");
	mProgrssPic->LocalTransform.SetTranslateY(-1.0f);
	mProgrssPic->SetAlpha(0.3f);
	mProgrssPic->SetAnchorHor(0.0f, 1.0f);
	mProgrssPic->SetAnchorVer(0.0f, 0.01f);
	mProgrssPic->SetDoPick(false);
}
//----------------------------------------------------------------------------
UISkillBuf::~UISkillBuf()
{
}
//----------------------------------------------------------------------------
void UISkillBuf::SetSkillBuf(SkillBuf *skillBuf)
{
	mSkillBuf = skillBuf;
	DefBuf *defBuf = mSkillBuf->GetDefBuf();

	mBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(
		"Data/Voxel/images/icons/buf/buf.xml", defBuf->Icon);
}
//----------------------------------------------------------------------------
SkillBuf *UISkillBuf::GetSkillBuf()
{
	return mSkillBuf;
}
//----------------------------------------------------------------------------
void UISkillBuf::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	float percent = mSkillBuf->GetPercent();
	if (0.0f == percent)
		percent = 0.01f;
	mProgrssPic->SetAnchorVer(0.0f, percent);

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UISkillBuf::UISkillBuf(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UISkillBuf::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIFrame, source);
}
//----------------------------------------------------------------------------
void UISkillBuf::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UISkillBuf::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UISkillBuf::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void UISkillBuf::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIFrame, target);
}
//----------------------------------------------------------------------------
int UISkillBuf::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------