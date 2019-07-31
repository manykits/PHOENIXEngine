// PX2UIComboBox.cpp

#include "PX2UIComboBox.hpp"
#include "PX2UISkinManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIComboBox);
PX2_IMPLEMENT_STREAM(UIComboBox);
PX2_IMPLEMENT_FACTORY(UIComboBox);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIComboBox);

//----------------------------------------------------------------------------
UIComboBox::UIComboBox() :
mChoose(-1)
{
	SetSize(100.0f, 40.0f);

	mSelectButton = new0 UIButton();
	AttachChild(mSelectButton);
	mSelectButton->LocalTransform.SetTranslateY(-1.0f);
	mSelectButton->SetAnchorHor(0.0f, 1.0f);
	mSelectButton->SetAnchorVer(0.0f, 1.0f);
	mSelectButton->CreateAddText();
	mSelectButton->SetMemUICallback(this, 
		(UIFrame::MemUICallback)(&UIComboBox::_SelectButCallback));

	mChooseList = new0 UIList();
	AttachChild(mChooseList);
	mChooseList->LocalTransform.SetTranslateY(-3.0f);
	mChooseList->SetAnchorHor(0.0f, 1.0f);
	mChooseList->SetAnchorVer(0.0f, 0.0f);
	mChooseList->SetPivot(0.5f, 1.0f);
	mChooseList->SetMemUICallback(this,
		(UIFrame::MemUICallback)(&UIComboBox::_ComboCallback));
	UIPicBox *picBox = mChooseList->CreateAddBackgroundPicBox();
	picBox->SetColor(Float3::BLACK);

	mChooseList->Show(false);

	mChooseList->SetChildPickOnlyInSizeRange(false);
	SetChildPickOnlyInSizeRange(false);
}
//----------------------------------------------------------------------------
UIComboBox::~UIComboBox()
{
}
//----------------------------------------------------------------------------
void UIComboBox::ConfigSkinColor()
{
	if (IsUseSkin())
	{
		if (mSelectButton)
		{
			mSelectButton->SetStateColor(UIButtonBase::BS_NORMAL, PX2_UISM.Color_Combo_But_Normal);
			mSelectButton->SetStateColor(UIButtonBase::BS_HOVERED, PX2_UISM.Color_Combo_But_Horvered);
			mSelectButton->SetStateColor(UIButtonBase::BS_HOVERED, PX2_UISM.Color_Combo_But_Pressed);
		}
	}
}
//----------------------------------------------------------------------------
void UIComboBox::SetFontSize(int size)
{
	mSelectButton->GetFText()->GetText()->SetFontSize(size);
	mChooseList->SetFontSize(size);
}
//----------------------------------------------------------------------------
UIItem *UIComboBox::AddChooseStr(const std::string &choose)
{
	mChooses.push_back(choose);
	UIItem *item = mChooseList->AddItem(choose);
	item->SetUserData("UIComboBoxItemIndex", (int)(mChooses.size() - 1));

	return item;
}
//----------------------------------------------------------------------------
void UIComboBox::RemoveAllChooseStr()
{
	mChooses.clear();
	mChooseList->RemoveAllItems();
	mSelectButton->GetText()->SetText("");
}
//----------------------------------------------------------------------------
int UIComboBox::GetNumChooseStr() const
{
	return (int)mChooses.size();
}
//----------------------------------------------------------------------------
std::string UIComboBox::GetChooseStrByIndex(int i) const
{
	if (0 <= i && i < (int)mChooses.size())
	{
		return mChooses[i];
	}

	return "";
}
//----------------------------------------------------------------------------
void UIComboBox::Choose(int i, bool isDoCallback)
{
	mChoose = i;

	if (mChooseList->IsShow())
	{
		mChooseList->Show(false);
		if (isDoCallback)
		{
			_UICallbacksCalls(UICT_COMBOBOX_CLOSE);
		}
	}

	UIText *text = mSelectButton->GetText();
	text->SetText(GetChooseStrByIndex(mChoose));

	if (isDoCallback)
	{
		OnChoosed();
	}
}
//----------------------------------------------------------------------------
void UIComboBox::ChooseStr(const std::string &str, bool isDoCallback)
{
	int strIndex = mChooseList->GetItemIndex(str);
	if (strIndex >= 0)
	{
		Choose(strIndex, isDoCallback);
	}
}
//----------------------------------------------------------------------------
std::string UIComboBox::GetChooseStr() const
{
	return GetChooseStrByIndex(mChoose);
}
//----------------------------------------------------------------------------
UIItem *UIComboBox::GetChooseItem()
{
	return mChooseList->GetItemByIndex(mChoose);
}
//----------------------------------------------------------------------------
void UIComboBox::OnChoosed()
{
	_UICallbacksCalls(UICT_COMBOBOX_CHOOSED);
}
//----------------------------------------------------------------------------
void UIComboBox::_SelectButCallback(UIFrame *frame, UICallType type)
{
	UIButton *but = DynamicCast<UIButton>(frame);
	if (but == mSelectButton)
	{
		if (UICT_RELEASED == type)
		{
			if (IsEnable())
			{
				if (!mChooseList->IsShow())
				{
					mChooseList->Show(true);
					_UICallbacksCalls(UICT_COMBOBOX_OPEN);
				}
				else
				{
					mChooseList->Show(false);
					_UICallbacksCalls(UICT_COMBOBOX_CLOSE);
				}
			}
		}
		else if (UICT_RELEASED_NOTPICK == type)
		{
			if (IsEnable())
			{
				if (mChooseList->IsShow())
				{
					mChooseList->Show(false);
					_UICallbacksCalls(UICT_COMBOBOX_CLOSE);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIComboBox::_ComboCallback(UIFrame *frame, UICallType type)
{
	UIList *list = DynamicCast<UIList>(frame);
	if (list)
	{
		if (UICT_LIST_SELECTED == type)
		{
			int index = list->GetSelectIndex();
			Choose(index);
		}
	}
}
//----------------------------------------------------------------------------
void UIComboBox::SetTextColor(const Float3 &textColor)
{
	if (mSelectButton)
	{
		mSelectButton->GetText()->SetFontColor(textColor);
	}

	if (mChooseList)
	{
		mChooseList->SetTextColor(textColor);
	}
}
//----------------------------------------------------------------------------
void UIComboBox::SetChooseListHeightSameWithChooses()
{
	mChooseList->SetHeight(mChooseList->GetContentHeight());
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIComboBox::UIComboBox(LoadConstructor value) :
UIFrame(value),
mChoose(-1)
{
}
//----------------------------------------------------------------------------
void UIComboBox::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIComboBox, source);
}
//----------------------------------------------------------------------------
void UIComboBox::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIComboBox::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIComboBox::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIComboBox::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIComboBox, target);
}
//----------------------------------------------------------------------------
int UIComboBox::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------