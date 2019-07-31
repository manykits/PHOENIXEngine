// PX2UITabFrame.cpp

#include "PX2UITabFrame.hpp"
#include "PX2UIButton.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIAuiManager.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2Time.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UITabFrame);
PX2_IMPLEMENT_STREAM(UITabFrame);
PX2_IMPLEMENT_FACTORY(UITabFrame);

//----------------------------------------------------------------------------
UITabFrame::UITabFrame() :
mFontSize(16),
mFontScale(1.0f),
mFontColor(Float3::MakeColor(50, 50, 50)),
mLayoutPosType(LPT_TOP),
mTabBarHeight(20.0f),
mTabWidth(100.0f),
mTabHeight(20.0f),
mAuiBlockFrame(0),
mTabLayoutType(TLT_FIX),
mIsTabsNeedReCal(true),
mIsDragingTab(false)
{
	SetAnchorHor(0.0f, 1.0f);
	SetAnchorVer(0.0f, 1.0f);

	SetPivot(0.5f, 0.5f);

	mFrame_TitleBar = new0 UIFrame();
	AttachChild(mFrame_TitleBar);
	mFrame_TitleBar->SetName("TitleBar");
	mFrame_TitleBar->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *picBox = mFrame_TitleBar->CreateAddBackgroundPicBox();
	picBox->SetColor(Float3::MakeColor(60, 60, 60));
	picBox->UseAlphaBlend(true);
	mFrame_TitleBar->SetSize(0.0f, mTabHeight);
	mFrame_TitleBar->SetAnchorHor(Float2(0.0f, 1.0f));
	mFrame_TitleBar->SetAnchorVer(Float2(1.0f, 1.0f));
	mFrame_TitleBar->SetPivot(0.5f, 1.0f);

	mFrame_Content = new0 UIFrame();
	AttachChild(mFrame_Content);
	mFrame_Content->LocalTransform.SetTranslateY(-1.0f);
	mFrame_Content->SetName("Content");
	mFrame_Content->SetAnchorHor(0.0f, 1.0f);
	mFrame_Content->SetAnchorVer(0.0f, 1.0f);
	mFrame_Content->SetAnchorParamVer(0.0f, -mTabBarHeight);

	SetWidget(true);
}
//----------------------------------------------------------------------------
UITabFrame::~UITabFrame()
{
}
//----------------------------------------------------------------------------
void UITabFrame::SetLayoutPos(LayoutPosType posType)
{
	mLayoutPosType = posType;

	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UITabFrame::SetTabBarHeight(float height)
{
	mTabBarHeight = height;
	mFrame_Content->SetAnchorParamVer(0.0f, -mTabBarHeight);
	mFrame_TitleBar->SetHeight(mTabBarHeight);

	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UITabFrame::SetTabWidth(float width)
{
	mTabWidth = width;
	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UITabFrame::SetTabHeight(float height)
{
	mTabHeight = height;
	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UITabFrame::SetFontSize(int size)
{
	mFontSize = size;
}
//----------------------------------------------------------------------------
int UITabFrame::GetFontSize() const
{
	return mFontSize;
}
//----------------------------------------------------------------------------
void UITabFrame::SetFontScale(float scale)
{
	mFontScale = scale;
}
//----------------------------------------------------------------------------
float UITabFrame::GetFontScale() const
{
	return mFontScale;
}
//----------------------------------------------------------------------------
void UITabFrame::SetFontColor(const Float3 &fontColor)
{
	mFontColor = fontColor;
}
//----------------------------------------------------------------------------
const Float3 &UITabFrame::GetFontColor() const
{
	return mFontColor;
}
//----------------------------------------------------------------------------
void UITabFrame::SetTabLayoutType(TabLayoutType tlt)
{
	mTabLayoutType = tlt;
	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
void UITabFrame::OnSizeChanged()
{
	int numTabs = GetNumTabs();
	float allTabLength = numTabs * mTabWidth;

	if (allTabLength <= mSize.Width)
	{
		if (mTabLayoutType != TLT_FIX)
		{
			SetTabLayoutType(TLT_FIX);
		}
	}
	else
	{
		SetTabLayoutType(TLT_SCALE);
	}

	UIFrame::OnSizeChanged();
}
//----------------------------------------------------------------------------
void UITabFrame::OnDragBegin()
{
	_UICallbacksCalls(UICT_TABFRAME_DRAG_BEGIN);

	if (mMemObject && mMemUICallback)
	{
		(mMemObject->*mMemUICallback)(this, UICT_TABFRAME_DRAG_BEGIN);
	}

	std::vector<Visitor *>::iterator it = mVisitors.begin();
	for (; it != mVisitors.end(); it++)
	{
		(*it)->Visit(this, (int)UICT_TABFRAME_DRAG_BEGIN);
	}
}
//----------------------------------------------------------------------------
void UITabFrame::PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas)
{
	if (!IsShow())
		return;

	if (!IsDoPick())
		return;

	if (mIsWidget)
	{
		bool isPosInSizeRange = _IsInRect(inputData.LogicPos);
		if (isPosInSizeRange)
			canvas->_AddInRangePickWidget(this, inputData);
	}

	for (int i = 0; i < GetNumChildren(); i++)
	{
		UIFrame *childFrame = DynamicCast<UIFrame>(GetChild(i));
		if (childFrame)
		{
			if (childFrame == mFrame_Content)
			{
				if (mActiveTabFrame)
					mActiveTabFrame->PreCanvasPick(inputData, canvas);
			}
			else
			{
				childFrame->PreCanvasPick(inputData, canvas);
			}
		}
	}
}
//----------------------------------------------------------------------------
void UITabFrame::OnWidgetPicked(const CanvasInputData &inputData)
{
	UIFrame::OnWidgetPicked(inputData);

	if (UIPT_MOVED == inputData.PickType)
	{
		if (!mIsDragingTab && !mPressingTabName.empty())
		{
			mIsDragingTab = true;
			OnDragBegin();
		}
	}
}
//----------------------------------------------------------------------------
void UITabFrame::OnSizeNodeNotPicked(const CanvasInputData &inputData)
{
	UIFrame::OnSizeNodeNotPicked(inputData);
}
//----------------------------------------------------------------------------
void UITabFrame::_TabButCallback(UIFrame *frame, UICallType type)
{
	const std::string &name = frame->GetName();
	UIButton *but = DynamicCast<UIButton>(frame);
	if (but)
	{
		if (UICT_PRESSED == type)
		{
			SetActiveTab(name);

			mIsDragingTab = false;
			mPressingTabName = name;
		}
		else if (UICT_RELEASED == type)
		{
			mIsDragingTab = false;
			mPressingTabName.clear();
		}
		else if (UICT_RELEASED_NOTPICK == type)
		{
			mIsDragingTab = false;
			mPressingTabName.clear();
		}
	}
}
//----------------------------------------------------------------------------
void UITabFrame::AddTab(const std::string &name, const std::string &title, 
	UIFrame *tabContentFrame)
{
	if (!tabContentFrame) return;

	std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.find(name);
	if (it == mTabContentFrames.end())
	{
		tabContentFrame->Show(false);
		tabContentFrame->SetActivate(false);

		mTabContentFrames[name] = tabContentFrame;
		mFrame_Content->AttachChild(tabContentFrame);

		UIButton *tabBut = new0 UIButton();	
		mFrame_TitleBar->AttachChild(tabBut);
		mTabButs.push_back(tabBut);
		tabBut->LocalTransform.SetTranslateY(-5.0f);
		tabBut->SetName(name);
		tabBut->SetMemUICallback(this, (UIFrame::MemUICallback)(&UITabFrame::_TabButCallback));

		UIFText *text = tabBut->CreateAddText();
		text->GetText()->SetText(title);

		UIPicBox *picBoxNormal = tabBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL);
		if (IsUseSkin())
		{
			tabBut->SetStateColor(UIButtonBase::BS_NORMAL, PX2_UISM.Color_AuiButTab_Normal);
			tabBut->SetStateColor(UIButtonBase::BS_HOVERED, PX2_UISM.Color_AuiButTab_Horvered);
			tabBut->SetStateColor(UIButtonBase::BS_PRESSED, PX2_UISM.Color_AuiButTab_Pressed);
			tabBut->SetActivateColor(PX2_UISM.Color_AuiButTab_Active);

			tabBut->GetText()->SetColor(Float3::WHITE);
			tabBut->GetText()->SetColorSelfCtrled(true);
			tabBut->GetText()->SetFontColor(PX2_UISM.Color_ContentFont);
			tabBut->GetText()->SetBorderShadowAlpha(1.0f);
		}
		else
		{
			tabBut->SetStateColor(UIButtonBase::BS_NORMAL, PX2_UISM.Color_ButTab_Normal);
			tabBut->SetStateColor(UIButtonBase::BS_HOVERED, PX2_UISM.Color_ButTab_Horvered);
			tabBut->SetStateColor(UIButtonBase::BS_PRESSED, PX2_UISM.Color_ButTab_Pressed);

			tabBut->SetActivateColor(PX2_UISM.Color_ButTab_Active);

			tabBut->GetText()->SetFontScale(mFontScale);
			tabBut->GetText()->SetFontWidthHeight(mFontSize, mFontSize);
			tabBut->GetText()->SetFontColor(mFontColor);
			tabBut->GetText()->SetColor(Float3::WHITE);
			tabBut->GetText()->SetColorSelfCtrled(true);
		}

		mIsTabsNeedReCal = true;
	}
}
//----------------------------------------------------------------------------
bool UITabFrame::IsHasTab(const std::string &name)
{
	std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.find(name);

	return (it!=mTabContentFrames.end());
}
//----------------------------------------------------------------------------
UIFrame *UITabFrame::GetTab(const std::string &name)
{
	std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.find(name);
	if (it != mTabContentFrames.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void UITabFrame::RemoveTab(const std::string &name)
{
	// cot
	std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.find(name);
	if (it != mTabContentFrames.end())
	{
		it->second->Show(true);
		it->second->SetActivate(true);
		mFrame_Content->DetachChild(it->second);
		mTabContentFrames.erase(it);
	}

	// tab but
	std::vector<UIButtonPtr>::iterator it1 = mTabButs.begin();
	for (; it1 != mTabButs.end();)
	{
		if ((*it1)->GetName() == name)
		{
			mFrame_TitleBar->DetachChild(*it1);
			it1 = mTabButs.erase(it1);
		}
		else
		{
			it1++;
		}
	}

	mActiveTabBut = 0;
	mActiveTabFrame = 0;
	mIsTabsNeedReCal = true;

	if (mTabContentFrames.size() > 0)
	{
		std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.begin();
		std::string nextName = it->first;
		SetActiveTab(nextName);
	}
}
//----------------------------------------------------------------------------
void UITabFrame::RemoveAllTabs()
{
	SetActiveTab("");

	// cot
	std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.begin();
	for (; it != mTabContentFrames.end(); it++)
	{
		const std::string &name = it->first;

		mFrame_Content->DetachChild(it->second);
	}
	mTabContentFrames.clear();

	// tab but
	std::vector<UIButtonPtr>::iterator it1 = mTabButs.begin();
	for (; it1 != mTabButs.end(); it1++)
	{
		mFrame_TitleBar->DetachChild(*it1);
	}
	mTabButs.clear();

	mIsTabsNeedReCal = true;
}
//----------------------------------------------------------------------------
int UITabFrame::GetNumTabs() const
{
	return (int)mTabButs.size();
}
//----------------------------------------------------------------------------
UIButton *UITabFrame::GetTabButton(const std::string &name)
{
	for (int i = 0; i < (int)mTabButs.size(); i++)
	{
		if (mTabButs[i]->GetName() == name)
			return mTabButs[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
UIFrame *UITabFrame::GetTitleBarFrame()
{
	return mFrame_TitleBar;
}
//----------------------------------------------------------------------------
void UITabFrame::SetActiveTab(const std::string &name)
{
	_SetActiveTab(name);
}
//----------------------------------------------------------------------------
void UITabFrame::_SetActiveTab(const std::string &name)
{
	// new
	UIButton *actBut = GetTabButton(name);
	UIFrame *actTabFrame = GetTab(name);

	// set others activate false
	for (int i = 0; i < (int)mTabButs.size(); i++)
	{
		UIButton *but = mTabButs[i];
		if (but && but != actBut)
		{
			but->SetActivate(false);
		}
	}
	std::map<std::string, UIFramePtr>::iterator it = mTabContentFrames.begin();
	for (; it != mTabContentFrames.end(); it++)
	{
		UIFrame *tabFrame = it->second;
		if (tabFrame && tabFrame != actTabFrame)
		{
			tabFrame->SetActivate(false);
			tabFrame->Show(false);
		}
	}

	// set new
	mActiveTabBut = actBut;
	if (mActiveTabBut)
	{
		mActiveTabBut->SetActivate(true);
	}

	mActiveTabFrame = actTabFrame;
	if (mActiveTabFrame)
	{
		mActiveTabFrame->SetActivate(true);
		mActiveTabFrame->Show(true);
	}

	OnSetActive();
}
//----------------------------------------------------------------------------
void UITabFrame::OnSetActive()
{
	_UICallbacksCalls(UICT_TABFRAME_SETACTIVE);
}
//----------------------------------------------------------------------------
std::string UITabFrame::GetActiveTab() const
{
	if (mActiveTabBut)
	{
		return mActiveTabBut->GetName();
	}

	return "";
}
//----------------------------------------------------------------------------
void UITabFrame::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsTabsNeedReCal)
	{
		_CalTabs();
		mFrame_TitleBar->Update(Time::GetTimeInSeconds());
	}
}
//----------------------------------------------------------------------------
void UITabFrame::_CalTabs()
{
	if (LPT_TOP == mLayoutPosType)
	{
		mFrame_TitleBar->SetAnchorHor(Float2(0.0f, 1.0f));
		mFrame_TitleBar->SetAnchorVer(Float2(1.0f, 1.0f));
		mFrame_TitleBar->SetPivot(0.5f, 1.0f);
		mFrame_TitleBar->SetSize(0.0f, mTabBarHeight);

		mFrame_Content->SetAnchorHor(0.0f, 1.0f);
		mFrame_Content->SetAnchorVer(0.0f, 1.0f);
		mFrame_Content->SetAnchorParamVer(0.0f, -mTabBarHeight);
	}
	else if (LPT_BOTTOM == mLayoutPosType)
	{
		mFrame_TitleBar->SetAnchorHor(Float2(0.0f, 1.0f));
		mFrame_TitleBar->SetAnchorVer(Float2(0.0f, 0.0f));
		mFrame_TitleBar->SetPivot(0.5f, 0.0f);
		mFrame_TitleBar->SetSize(0.0f, mTabBarHeight);

		mFrame_Content->SetAnchorHor(0.0f, 1.0f);
		mFrame_Content->SetAnchorVer(0.0f, 1.0f);
		mFrame_Content->SetAnchorParamVer(mTabBarHeight, 0.0f);
	}

	if (TLT_FIX == mTabLayoutType)
	{
		for (int i = 0; i < (int)mTabButs.size(); i++)
		{
			UIFrame *tabBut = mTabButs[i];
			tabBut->SetSize(mTabWidth, mTabHeight);
			tabBut->SetAnchorHor(0.0f, 0.0f);
			tabBut->SetAnchorParamHor(
				mTabWidth*0.5f + mTabWidth*i + (i*1.0f), 0.0f);
			tabBut->SetAnchorVer(0.0f, 0.0f);
			tabBut->SetAnchorParamVer(mTabHeight / 2.0f, 0.0f);
		}
	}
	else
	{
		float tabWidth = mSize.Width / (float)GetNumTabs() - 1.0f;

		for (int i = 0; i < (int)mTabButs.size(); i++)
		{
			UIFrame *tabBut = mTabButs[i];
			tabBut->SetSize(tabWidth, mTabHeight);

			tabBut->SetAnchorHor(0.0f, 0.0f);
			tabBut->SetAnchorParamHor(
				tabWidth*0.5f + tabWidth*i + (i*1.0f), 0.0f);
			
			tabBut->SetAnchorVer(0.0f, 0.0f);
			tabBut->SetAnchorParamVer(mTabHeight / 2.0f, 0.0f);
		}
	}

	mIsTabsNeedReCal = false;
}
//----------------------------------------------------------------------------
void UITabFrame::SetAuiBlockFrame(UIAuiBlockFrame *auiBlockFrame)
{
	mAuiBlockFrame = auiBlockFrame;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UITabFrame::UITabFrame(LoadConstructor value) :
UIFrame(value),
mLayoutPosType(LPT_TOP),
mTabBarHeight(20.0f),
mTabWidth(100.0f),
mTabHeight(20.0f),
mAuiBlockFrame(0),
mTabLayoutType(TLT_FIX),
mIsTabsNeedReCal(true),
mIsDragingTab(false)
{
}
//----------------------------------------------------------------------------
void UITabFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UITabFrame, source);
}
//----------------------------------------------------------------------------
void UITabFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UITabFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UITabFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UITabFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UITabFrame, target);
}
//----------------------------------------------------------------------------
int UITabFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------