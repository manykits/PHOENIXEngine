// PX2UIProgressBar.cpp

#include "PX2UIProgressBar.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI_V(PX2, UIFrame, UIProgressBar, 1);
PX2_IMPLEMENT_STREAM(UIProgressBar);
PX2_IMPLEMENT_FACTORY(UIProgressBar);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIProgressBar);

//----------------------------------------------------------------------------
UIProgressBar::UIProgressBar() :
mProgress(1.0f),
mIsNeedAdujst(true),
mProgressNumMax(100)
{
	SetName("UIProgressBar");

	UIFPicBox *backPicBox = new0 UIFPicBox();
	backPicBox->GetUIPicBox()->SetTexture("Data/engine/common.xml", "progress_back");
	backPicBox->GetUIPicBox()->SetPicBoxType(UIPicBox::PBT_NINE);
	backPicBox->GetUIPicBox()->SetName("Back");
	backPicBox->GetUIPicBox()->SetTexCornerSize(4.0f, 4.0f, 4.0f, 4.0f);
	backPicBox->SetAnchorHor(0.0f, 1.0f);
	backPicBox->SetAnchorVer(0.0f, 1.0f);
	SetBackPicBox(backPicBox);

	UIFPicBox *progressPicBox = new0 UIFPicBox();
	progressPicBox->GetUIPicBox()->SetTexture("Data/engine/common.xml", "progress_fill");
	progressPicBox->GetUIPicBox()->SetPicBoxType(UIPicBox::PBT_NINE);
	progressPicBox->GetUIPicBox()->SetName("Back");
	progressPicBox->GetUIPicBox()->SetTexCornerSize(4.0f, 4.0f, 4.0f, 4.0f);
	progressPicBox->SetAnchorHor(0.0f, 0.0f);
	progressPicBox->SetAnchorVer(0.0f, 1.0f);
	progressPicBox->SetPivot(0.0f, 0.5f);
	SetProgressPicBox(progressPicBox);

	//UIPicBoxPtr overPicBox = new0 UIPicBox("Data/engine/common.xml", "progress_marquee");
	//overPicBox->LocalTransform.SetTranslate(APoint(0.0f, -1.0f, 0.0f));
	//overPicBox->SetPicBoxType(UIPicBox::PBT_NINE);
	//overPicBox->SetTexCornerSize(4.0f, 4.0f, 4.0f, 4.0f);
	//overPicBox->SetName("Over");
	//overPicBox->SetPivot(0.0f, 0.5f);
	//SetOverPicBox(overPicBox);

	SetSize(200.0f, 40.0f);

	mPBCtrl = new0 UIProgressBarCtrl();
	AttachController(mPBCtrl);

	SetWidget(true);
}
//----------------------------------------------------------------------------
UIProgressBar::~UIProgressBar()
{
}
//----------------------------------------------------------------------------
UIFText *UIProgressBar::CreateAddProgressText()
{
	if (mProgressText)
	{
		DetachChild(mProgressText);
		mProgressText = 0;
	}

	mProgressText = new0 UIFText();
	AttachChild(mProgressText);
	mProgressText->LocalTransform.SetTranslateY(-4.0f);
	mProgressText->SetAnchorHor(0.0f, 1.0f);
	mProgressText->SetAnchorVer(0.0, 1.0f);
	mProgressText->GetText()->SetFontColor(Float3::WHITE);

	int iPercent = (int)(0.0f * mProgressNumMax);
	mProgressText->GetText()->SetText(StringHelp::IntToString(iPercent) + "/" +
		StringHelp::IntToString(mProgressNumMax));

	return mProgressText;
}
//----------------------------------------------------------------------------
void UIProgressBar::SetProgressTextNumMax(int max)
{
	mProgressNumMax = max;
}
//----------------------------------------------------------------------------
int UIProgressBar::GetProgressTextNumMax() const
{
	return mProgressNumMax;
}
//----------------------------------------------------------------------------
void UIProgressBar::SetBackPicBox(UIFPicBox *picBox)
{
	if (mBackPicBox)
	{
		DetachChild(mBackPicBox);
	}

	mBackPicBox = picBox;

	AttachChild(mBackPicBox);
}
//----------------------------------------------------------------------------
void UIProgressBar::SetProgressPicBox(UIFPicBox *picBox)
{
	if (mProgressPicBox)
	{
		DetachChild(mProgressPicBox);
	}

	mProgressPicBox = picBox;

	AttachChild(mProgressPicBox);

	mProgressPicBox->LocalTransform.SetTranslateY(-1.0f);
}
//----------------------------------------------------------------------------
void UIProgressBar::SetOverPicBox(UIFPicBox *picBox)
{
	if (mOverPicBox)
	{
		DetachChild(mOverPicBox);
	}

	mOverPicBox = picBox;

	AttachChild(mOverPicBox);
}
//----------------------------------------------------------------------------
void UIProgressBar::AutoMakeSizeFixable(float scale)
{
	if (mBackPicBox)
	{
		mBackPicBox->AutoMakeSizeFixable(scale);
		const Sizef &sz = mBackPicBox->GetSize();
		SetSize(sz);
	}
}
//----------------------------------------------------------------------------
void UIProgressBar::OnPivotChanged()
{
	UIFrame::OnPivotChanged();

	mIsNeedAdujst = true;
}
//----------------------------------------------------------------------------
void UIProgressBar::OnSizeChanged()
{
	UIFrame::OnPivotChanged();

	mIsNeedAdujst = true;
}
//----------------------------------------------------------------------------
void UIProgressBar::SetProgress(float progress, bool callLogic)
{
	if (progress > 1.0f)
		progress = 1.0f;

	if (progress < 0.0f)
		progress = 0.0f;

	mProgress = progress;

	const Sizef &size = GetSize();
	Sizef sizeProgross = size;
	sizeProgross.Width = size.Width * progress;

	if (mProgressText)
	{
		int iPercent = (int)(progress * mProgressNumMax);
		mProgressText->GetText()->SetText(StringHelp::IntToString(iPercent) + "/" +
			StringHelp::IntToString(mProgressNumMax));
	}

	if (mProgressPicBox)
	{
		mProgressPicBox->SetSize(sizeProgross);
	}

	if (callLogic)
	{
		_UICallbacksCalls(UICT_PROGRESSCHANGED);
	}
}
//----------------------------------------------------------------------------
void UIProgressBar::SetProgressAnimInit(float progress)
{
	mPBCtrl->SetProgressInit(progress);

	SetProgress(progress);
}
//----------------------------------------------------------------------------
void UIProgressBar::SetProgressAnim(float progress)
{
	mPBCtrl->SetProgress(progress);
}
//----------------------------------------------------------------------------
void UIProgressBar::SetProgressAnimSpeed(float speed)
{
	mPBCtrl->SetProgressSpeed(speed);
}
//----------------------------------------------------------------------------
float UIProgressBar::GetProgressAnimSpeed()
{
	return mPBCtrl->GetProgressSpeed();
}
//----------------------------------------------------------------------------
void UIProgressBar::UpdateWorldData(double applicationTime, 
	double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsNeedAdujst)
	{
		_Adjust();
	}
}
//----------------------------------------------------------------------------
void UIProgressBar::_Adjust()
{
	const Float2 &pvoit = GetPivot();

	float posX = -mSize.Width * pvoit[0];

	if (mBackPicBox)
	{
		mBackPicBox->SetSize(mSize);
		mBackPicBox->LocalTransform.SetTranslateX(posX);
	}

	if (mProgressPicBox)
	{
		mProgressPicBox->SetSize(mSize);

		float posX1 = -mSize.Width * pvoit[0];
		mProgressPicBox->LocalTransform.SetTranslateX(posX1);
	}

	if (mOverPicBox)
	{
		mOverPicBox->SetSize(mSize);
		mOverPicBox->LocalTransform.SetTranslateX(posX);
	}

	SetProgress(mProgress, false);

	mIsNeedAdujst = false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void UIProgressBar::RegistProperties()
{
	UIFrame::RegistProperties();

	AddPropertyClass("UIProgressBar");

	AddProperty("Progress", PT_FLOAT, GetProgress());
}
//----------------------------------------------------------------------------
void UIProgressBar::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);

	if ("Progress" == obj.Name)
	{
		SetProgress(PX2_ANY_AS(obj.Data, float));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIProgressBar::UIProgressBar(LoadConstructor value) :
UIFrame(value),
mProgress(1.0f),
mIsNeedAdujst(true),
mProgressNumMax(100)
{
}
//----------------------------------------------------------------------------
void UIProgressBar::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mBackPicBox);
	source.ReadPointer(mProgressPicBox);
	source.ReadPointer(mOverPicBox);

	source.ReadPointer(mProgressText);

	source.Read(mProgress);

	source.ReadPointer(mPBCtrl);

	PX2_END_DEBUG_STREAM_LOAD(UIProgressBar, source);
}
//----------------------------------------------------------------------------
void UIProgressBar::Link(InStream& source)
{
	UIFrame::Link(source);

	source.ResolveLink(mBackPicBox);
	source.ResolveLink(mProgressPicBox);
	source.ResolveLink(mOverPicBox);

	source.ResolveLink(mProgressText);

	source.ResolveLink(mPBCtrl);
}
//----------------------------------------------------------------------------
void UIProgressBar::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIProgressBar::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		target.Register(mBackPicBox);
		target.Register(mProgressPicBox);
		target.Register(mOverPicBox);

		target.Register(mProgressText);

		target.Register(mPBCtrl);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIProgressBar::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mBackPicBox);
	target.WritePointer(mProgressPicBox);
	target.WritePointer(mOverPicBox);

	target.WritePointer(mProgressText);

	target.Write(mProgress);

	target.WritePointer(mPBCtrl);

	PX2_END_DEBUG_STREAM_SAVE(UIProgressBar, target);
}
//----------------------------------------------------------------------------
int UIProgressBar::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mBackPicBox);
	size += PX2_POINTERSIZE(mProgressPicBox);
	size += PX2_POINTERSIZE(mOverPicBox);

	size += PX2_POINTERSIZE(mProgressText);

	size += sizeof(mProgress);

	size += PX2_POINTERSIZE(mPBCtrl);

	return size;
}
//----------------------------------------------------------------------------