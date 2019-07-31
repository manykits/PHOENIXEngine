// PX2UIPropertyItem.hpp

#include "PX2UIPropertyItem.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIPropertyGrid.hpp"
#include "PX2UIComboBox.hpp"
#include "PX2StringHelp.hpp"
#include "PX2UIColorSelect.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIItem, UIPropertyItem);
PX2_IMPLEMENT_STREAM(UIPropertyItem);
PX2_IMPLEMENT_FACTORY(UIPropertyItem);
PX2_IMPLEMENT_DEFAULT_NAMES(UIItem, UIPropertyItem);

//----------------------------------------------------------------------------
UIPropertyItem::UIPropertyItem() :
mObjectID(0)
{
}
//----------------------------------------------------------------------------
UIPropertyItem::~UIPropertyItem()
{
}
//----------------------------------------------------------------------------
void UIPropertyGridClassCBCallback(UIFrame *frame, UICallType type)
{
	UICheckButton *checkBut = DynamicCast<UICheckButton>(frame);
	if (checkBut)
	{
		UIItem *item = (UIItem*)checkBut->GetUserData<Object*>("UIPropertyItem");

		if (UICT_CHECKED == type)
		{
			item->Expand(false);
		}
		else if (UICT_DISCHECKED == type)
		{
			item->Expand(true);
		}
	}
}
//----------------------------------------------------------------------------
void EditBoxCallback(UIFrame *frame, UICallType type)
{
	UIEditBox *eb = DynamicCast<UIEditBox>(frame);
	if (eb && type == UICT_EDITBOX_ENTER)
	{
		const std::string &text = eb->GetText();
		int index = eb->GetUserData<int>("UIPropertyItemIndex");
		Object *itemObj = eb->GetUserData<Object*>("UIPropertyItem");
		UIItem *item = DynamicCast<UIItem>(itemObj);
		if (item)
		{
			const Any &valAny = item->GetValue();

			Object::PropertyType pt = eb->GetUserData<Object::PropertyType>("datatype");
			if (Object::PT_STRING == pt)
			{
				item->SetValue(text);
			}
			else if (Object::PT_STRINGBUTTON == pt)
			{
				item->SetValue(text);
			}
			else if (Object::PT_INT == pt)
			{
				int iVal = StringHelp::StringToInt(text);
				item->SetValue(iVal);
			}
			else if (Object::PT_FLOAT == pt)
			{
				float fVal = (float)atof(text.c_str());
				item->SetValue(fVal);
			}
			else if (Object::PT_FLOAT2 == pt)
			{
				float fVal = (float)atof(text.c_str());

				Float2 val = PX2_ANY_AS(valAny, Float2);
				val[index] = fVal;
				item->SetValue(val);
			}
			else if (Object::PT_SIZE == pt)
			{
				float fVal = (float)atof(text.c_str());

				Sizef val = PX2_ANY_AS(valAny, Sizef);
				
				if (0 == index)
					val.Width = fVal;
				else if (1 == index)
					val.Height = fVal;

				item->SetValue(val);
			}
			else if (Object::PT_FLOAT3 == pt || Object::PT_APOINT3 == pt ||
				Object::PT_AVECTOR3 == pt)
			{
				float fVal = (float)atof(text.c_str());

				if (Object::PT_FLOAT3 == pt)
				{
					Float3 val = PX2_ANY_AS(valAny, Float3);
					val[index] = fVal;
					item->SetValue(val);
				}
				else if (Object::PT_APOINT3 == pt)
				{
					APoint val = PX2_ANY_AS(valAny, APoint);
					val[index] = fVal;
					item->SetValue(val);
				}
				else if (Object::PT_AVECTOR3 == pt)
				{
					AVector val = PX2_ANY_AS(valAny, AVector);
					val[index] = fVal;
					item->SetValue(val);
				}
				else if (Object::PT_COLOR3FLOAT3 == pt)
				{
					Float3 val = PX2_ANY_AS(valAny, Float3);
					val[index] = fVal;
					item->SetValue(val);
				}
			}
			else if (Object::PT_FLOAT4 == pt || Object::PT_APOINT4 == pt ||
				Object::PT_AVECTOR4 == pt)
			{
				float fVal = (float)atof(text.c_str());

				if (Object::PT_FLOAT4 == pt)
				{
					Float4 val = PX2_ANY_AS(valAny, Float4);
					val[index] = fVal;
					item->SetValue(val);
				}
				else if (Object::PT_APOINT4 == pt)
				{
					APoint val = PX2_ANY_AS(valAny, APoint);
					val[index] = fVal;
					item->SetValue(val);
				}
				else if (Object::PT_AVECTOR4 == pt)
				{
					AVector val = PX2_ANY_AS(valAny, AVector);
					val[index] = fVal;
					item->SetValue(val);
				}
			}

			UIPropertyGrid *grid =
				item->GetFirstParentDerivedFromType<UIPropertyGrid>();
			if (grid)
			{
				grid->OnItemChanged(item);
			}
		}
	}
}
//----------------------------------------------------------------------------
void SliderEditBoxCallback(UIFrame *frame, UICallType type)
{
	UIEditBox *eb = DynamicCast<UIEditBox>(frame);
	if (eb && type == UICT_EDITBOX_ENTER)
	{
		const std::string &text = eb->GetText();
		int index = eb->GetUserData<int>("UIPropertyItemIndex");
		Object *itemObj = eb->GetUserData<Object*>("UIPropertyItem");
		UIItem *item = DynamicCast<UIItem>(itemObj);
		if (item)
		{
			const Any &valAny = item->GetValue();

			Object::PropertyType pt = eb->GetUserData<Object::PropertyType>("datatype");
			if (Object::PT_INTSLIDER == pt)
			{
				int iVal = StringHelp::StringToInt(text.c_str());
				item->SetValue(iVal);
			}
			else if (Object::PT_FLOATSLIDER == pt)
			{
				float fVal = (float)atof(text.c_str());
				item->SetValue(fVal);
			}

			UIPropertyGrid *grid =
				item->GetFirstParentDerivedFromType<UIPropertyGrid>();
			if (grid)
			{
				grid->OnItemChanged(item);
			}
		}
	}
}
//----------------------------------------------------------------------------
void SliderBoxCallback(UIFrame *frame, UICallType type)
{
	UISlider *sd = DynamicCast<UISlider>(frame);
	if (UICallType::UICT_SLIDERCHANGED == type)
	{
		bool isInt = sd->GetUserData<bool>("IsInt");
		Object *itemObj = sd->GetUserData<Object*>("UIPropertyItem");
		UIPropertyItem *item = DynamicCast<UIPropertyItem>(itemObj);
		
		if (item)
		{
			const Any &valAny = item->GetValue();			
			float percent = sd->GetPercent();
			float sliderMin = sd->GetUserData<float>("SliderMinVal");
			float sliderMax = sd->GetUserData<float>("SliderMaxVal");
			UIEditBox *editBox = sd->GetUserData<UIEditBox*>("EditBox");

			std::string editBoxStr;
			float val = (sliderMax - sliderMin) * percent;
			if (isInt)
			{
				item->SetValue((int)val);
				editBoxStr = StringHelp::IntToString((int)val);
			}
			else
			{
				item->SetValue(val);
				editBoxStr = StringHelp::FloatToString(val, 2);
			}
			if (editBox)
			{
				editBox->SetText(editBoxStr);
			}

			UIPropertyGrid *grid =
				item->GetFirstParentDerivedFromType<UIPropertyGrid>();
			if (grid)
			{
				grid->OnItemChanged(item);
			}
		}
	}
}
//----------------------------------------------------------------------------
void ComboBoxCallback(UIFrame *frame, UICallType type)
{
	UIComboBox *cb = DynamicCast<UIComboBox>(frame);
	if (cb)
	{
		if (UICallType::UICT_COMBOBOX_OPEN == type)
		{
			cb->LocalTransform.SetTranslateY(-20.0f);
		}
		else if (UICallType::UICT_COMBOBOX_CLOSE == type)
		{
			cb->LocalTransform.SetTranslateY(-5.0f);
		}
		else if (UICallType::UICT_COMBOBOX_CHOOSED == type)
		{
			Object *itemObj = cb->GetUserData<Object*>("UIPropertyItem");
			UIPropertyItem *item = DynamicCast<UIPropertyItem>(itemObj);
			if (item)
			{
				bool isBool = cb->GetUserData<bool>("IsBool");
				int choose = cb->GetChoose();
				if (isBool)
				{
					item->SetValue((bool)(0 == choose));
				}
				else
				{
					item->SetValue(choose);
				}

				UIPropertyGrid *grid =
					item->GetFirstParentDerivedFromType<UIPropertyGrid>();
				if (grid)
				{
					grid->OnItemChanged(item);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void _SliderUICallback (UIFrame *frame, UICallType type)
{
	UIColorSelector *colorSelector = DynamicCast<UIColorSelector>(frame);
	if (colorSelector)
	{
		if (UICT_COLOR_SELECT == type)
		{
			const Float3 &color = colorSelector->GetSelectColor();
			UIItem *item = colorSelector->GetUserData<UIItem*>("UIPropertyItem");
			UIButton *button = colorSelector->GetUserData<UIButton*>("UIButton");
			if (item)
			{
				item->SetValue(color);
				if (button)
				{
					button->SetStateColor(UIButtonBase::BS_NORMAL, color);
					button->SetStateColor(UIButtonBase::BS_HOVERED, color);
					button->SetStateColor(UIButtonBase::BS_PRESSED, color);
				}

				UIPropertyGrid *grid =
					item->GetFirstParentDerivedFromType<UIPropertyGrid>();
				if (grid)
				{
					grid->OnItemChanged(item);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void ColorButCallback(UIFrame *frame, UICallType type)
{
	UIButton *button = DynamicCast<UIButton>(frame);
	if (button)
	{
		if (UICT_RELEASED == type)
		{
			UIFrame *butParent = DynamicCast<UIFrame>(button->GetParent());
			int index = button->GetUserData<int>("UIPropertyItemIndex");
			Object *itemObj = button->GetUserData<Object*>("UIPropertyItem");
			UIItem *item = DynamicCast<UIItem>(itemObj);
			if (item)
			{
				Float3 colorVal = PX2_ANY_AS(item->GetValue(), Float3);

				if (butParent)
				{
					UIColorSelector *colorSelector = DynamicCast<UIColorSelector>(
						butParent->GetObjectByName("UIColorSelector"));
					if (!colorSelector)
					{
						colorSelector = new0 UIColorSelector();
						butParent->AttachChild(colorSelector);
						colorSelector->LocalTransform.SetTranslateY(-20.0f);
						colorSelector->SetAnchorHor(0.0f, 1.0f);
						colorSelector->AddUICallback(_SliderUICallback);
					}
					else
					{
						colorSelector->Show(true);
					}
					colorSelector->SetInitColor(colorVal);
					colorSelector->SetUserData("UIPropertyItem", item);
					colorSelector->SetUserData("UIButton", button);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIPropertyItem::OnAllocAll(const Any &userDat)
{
	UIButton *butBack = GetButBack();
	if (butBack) butBack->LocalTransform.SetTranslateY(-1.0f);

	UIFText *fText = GetFText();
	if (fText) fText->LocalTransform.SetTranslateY(-3.0f);

	ItemType itemType = PX2_ANY_AS(userDat, ItemType);
	if (itemType == IT_NONE)
	{
		/*_*/
	}
	else if (itemType == IT_CLASS)
	{
		UIButton *butBack = GetButBack();
		if (butBack) butBack->LocalTransform.SetTranslateY(-10.0f);

		UICheckButton *cb = CreateButArrow();
		if (cb) cb->LocalTransform.SetTranslateY(-12.0f);
		UIFText *fText = GetFText();
		if (fText) fText->LocalTransform.SetTranslateY(-12.0f);

		cb->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(
			"DataNIRVANAwx/images/icons/tree/tree_expanded.png");
		cb->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture(
			"DataNIRVANAwx/images/icons/tree/tree_collapsed.png");

		cb->AddUICallback(UIPropertyGridClassCBCallback);
		cb->SetUserData("UIPropertyItem", (Object*)this);

		if (butBack)
		{
			butBack->SetStateColor(UIButtonBase::BS_NORMAL,
				PX2_UISM.Color_PropertyClass);
			butBack->SetStateBrightness(UIButtonBase::BS_NORMAL, 1.0f);

			butBack->SetStateColor(UIButtonBase::BS_HOVERED,
				PX2_UISM.Color_PropertyClass);
			butBack->SetStateBrightness(UIButtonBase::BS_HOVERED, 1.2f);

			butBack->SetWidget(false);
		}
	}
	else if (itemType == IT_COMBOX)
	{
		UIComboBox *comboBox = new0 UIComboBox();
		AttachChild(comboBox);
		comboBox->GetChooseList()->SetTextAlignLeftPos(0.0f);
		comboBox->LocalTransform.SetTranslateY(-5.0f);
		comboBox->GetChooseList()->GetBackgroundPicBox()->SetColor(
			PX2_UISM.Color_ViewBackground);
		comboBox->GetSelectButton()->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScaleCombo);
		comboBox->AddUICallback(ComboBoxCallback);
		comboBox->SetUserData("UIPropertyItem", (Object*)this);
		
		mEditCtrl = comboBox;
	}
	else if (itemType == IT_COLORBUTTON)
	{
		SetChildPickOnlyInSizeRange(false);

		UIFrame *frame = new0 UIFrame();
		AttachChild(frame);
		frame->SetAnchorHor(0.0f, 1.0f);
		frame->SetAnchorVer(0.0f, 1.0f);
		frame->SetChildPickOnlyInSizeRange(false);

		UIButton *but = new0 UIButton();
		frame->AttachChild(but);
		but->SetName("ButColor");
		but->SetAnchorHor(0.0f, 1.0f);
		but->SetAnchorParamHor(2.0f, -2.0f);
		but->SetAnchorVer(0.0f, 1.0f);
		but->SetAnchorParamVer(2.0f, -2.0f);
		but->LocalTransform.SetTranslateY(-5.0f);
		but->AddUICallback(ColorButCallback);
		but->SetUserData("UIPropertyItem", (Object*)this);

		mEditCtrl = frame;
	}
	else if (itemType == IT_EDITBOXBUT)
	{
		UIFrame *frame = new0 UIFrame();
		AttachChild(frame);

		float butSize = PX2_UISM.Size_PropertyItem;

		UIEditBox *editBox = new0 UIEditBox();
		frame->AttachChild(editBox);
		editBox->LocalTransform.SetTranslateY(-5.0f);
		UIFPicBox *picBoxBack = editBox->GetBackPicBox();
		if (picBoxBack) picBoxBack->SetColor(PX2_UISM.Color_ContentBackground);
		editBox->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		editBox->AddUICallback(EditBoxCallback);
		editBox->SetUserData("UIPropertyItem", (Object*)this);
		editBox->SetUserData("UIPropertyItemIndex", 0);
		editBox->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
		editBox->SetAnchorHor(0.0f, 1.0f);
		editBox->SetAnchorVer(0.0f, 1.0f);
		editBox->SetAnchorParamHor(0.0f, -butSize);
		editBox->SetName("EditBox");

		UIButton *but = new0 UIButton();
		frame->AttachChild(but);
		but->LocalTransform.SetTranslateY(-2.0f);
		but->SetAnchorHor(1.0f, 1.0f);
		but->SetAnchorVer(0.5f, 0.5f);
		but->SetAnchorParamHor(-butSize / 2.0f, 0.0f);
		but->SetPivot(0.5f, 0.5f);
		but->SetSize(butSize - 2.0f, butSize - 2.0f);
		but->SetUserData("UIPropertyItem", (Object*)this);
		but->SetName("Button");

		mEditCtrl = frame;
	}
	else if (itemType == IT_EDITBOXSLIDER)
	{
		UIFrame *editFrame = new0 UIFrame();
		AttachChild(editFrame);
		editFrame->LocalTransform.SetTranslateY(-1.0f);
		mEditCtrl = editFrame;

		UIEditBox *eb0 = new0 UIEditBox();
		editFrame->AttachChild(eb0);
		eb0->LocalTransform.SetTranslateY(-1.0f);
		eb0->SetAnchorHor(0.0f, 0.2f);
		eb0->SetAnchorParamHor(0.5f, -0.5f);
		eb0->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack0 = eb0->GetBackPicBox();
		if (picBoxBack0) picBoxBack0->SetColor(PX2_UISM.Color_ContentBackground);
		eb0->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb0->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
		eb0->AddUICallback(SliderEditBoxCallback);
		eb0->SetUserData("UIPropertyItem", (Object*)this);

		UISlider *slider = new0 UISlider();
		editFrame->AttachChild(slider);
		slider->LocalTransform.SetTranslateY(-1.0f);
		slider->SetAnchorHor(0.2f, 1.0f);
		slider->SetAnchorParamHor(2.0f, -2.0f);
		slider->SetAnchorVer(0.5f, 0.5f);
		slider->EnableAnchorLayout(true);
		slider->AddUICallback(SliderBoxCallback);
		slider->SetUserData("UIPropertyItem", (Object*)this);
	}
	else if (itemType == IT_EDITBOX)
	{
		UIEditBox *editBox = new0 UIEditBox();
		AttachChild(editBox);
		editBox->LocalTransform.SetTranslateY(-2.0f);
		mEditCtrl = editBox;

		UIFPicBox *picBoxBack = editBox->GetBackPicBox();
		if (picBoxBack) picBoxBack->SetColor(PX2_UISM.Color_ContentBackground);
		editBox->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);

		editBox->AddUICallback(EditBoxCallback);
		editBox->SetUserData("UIPropertyItem", (Object*)this);
		editBox->SetUserData("UIPropertyItemIndex", 0);

		editBox->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
	}
	else if (itemType == IT_EDITBOX2)
	{
		UIFrame *editFrame = new0 UIFrame();
		AttachChild(editFrame);
		editFrame->LocalTransform.SetTranslateY(-1.0f);
		mEditCtrl = editFrame;

		UIEditBox *eb0 = new0 UIEditBox();
		editFrame->AttachChild(eb0);
		eb0->LocalTransform.SetTranslateY(-1.0f);
		eb0->SetAnchorHor(0.0f, 1.0f / 2.0f);
		eb0->SetAnchorParamHor(0.5f, -0.5f);
		eb0->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack0 = eb0->GetBackPicBox();
		if (picBoxBack0) picBoxBack0->SetColor(PX2_UISM.Color_ContentBackground);
		eb0->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb0->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
		eb0->AddUICallback(EditBoxCallback);
		eb0->SetUserData("UIPropertyItem", (Object*)this);
		eb0->SetUserData("UIPropertyItemIndex", 0);

		UIEditBox *eb1 = new0 UIEditBox();
		editFrame->AttachChild(eb1);
		eb1->LocalTransform.SetTranslateY(-1.0f);
		eb1->SetAnchorHor(1.0f / 2.0f, 2.0f / 2.0f);
		eb1->SetAnchorParamHor(0.5f, -0.5f);
		eb1->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack1 = eb1->GetBackPicBox();
		if (picBoxBack1) picBoxBack1->SetColor(PX2_UISM.Color_ContentBackground);
		eb1->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb1->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
		eb1->AddUICallback(EditBoxCallback);
		eb1->SetUserData("UIPropertyItem", (Object*)this);
		eb1->SetUserData("UIPropertyItemIndex", 1);
	}
	else if (itemType == IT_EDITBOX3)
	{
		UIFrame *editFrame = new0 UIFrame();
		AttachChild(editFrame);
		editFrame->LocalTransform.SetTranslateY(-1.0f);
		mEditCtrl = editFrame;

		UIEditBox *eb0 = new0 UIEditBox();
		editFrame->AttachChild(eb0);
		eb0->LocalTransform.SetTranslateY(-1.0f);
		eb0->SetAnchorHor(0.0f, 1.0f / 3.0f);
		eb0->SetAnchorParamHor(0.5f, -0.5f);
		eb0->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack0 = eb0->GetBackPicBox();
		if (picBoxBack0) picBoxBack0->SetColor(PX2_UISM.Color_ContentBackground);
		eb0->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb0->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
		eb0->AddUICallback(EditBoxCallback);
		eb0->SetUserData("UIPropertyItem", (Object*)this);
		eb0->SetUserData("UIPropertyItemIndex", 0);

		UIEditBox *eb1 = new0 UIEditBox();
		editFrame->AttachChild(eb1);
		eb1->LocalTransform.SetTranslateY(-1.0f);
		eb1->SetAnchorHor(1.0f / 3.0f, 2.0f / 3.0f);
		eb1->SetAnchorParamHor(0.5f, -0.5f);
		eb1->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack1 = eb1->GetBackPicBox();
		if (picBoxBack1) picBoxBack1->SetColor(PX2_UISM.Color_ContentBackground);
		eb1->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb1->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
		eb1->AddUICallback(EditBoxCallback);
		eb1->SetUserData("UIPropertyItem", (Object*)this);
		eb1->SetUserData("UIPropertyItemIndex", 1);

		UIEditBox *eb2 = new0 UIEditBox();
		editFrame->AttachChild(eb2);
		eb2->LocalTransform.SetTranslateY(-1.0f);
		eb2->SetAnchorHor(2.0f / 3.0f, 3.0f / 3.0f);
		eb2->SetAnchorParamHor(0.5f, -0.5f);
		eb2->SetAnchorVer(0.0f, 1.0f);
		UIFPicBox *picBoxBack2 = eb2->GetBackPicBox();
		if (picBoxBack2) picBoxBack2->SetColor(PX2_UISM.Color_ContentBackground);
		eb2->GetInputText()->SetFontScale(PX2_UISM.Size_PropertyFontScale);
		eb2->GetInputText()->SetFontColor(PX2_UISM.Color_ContentFont);
		eb2->AddUICallback(EditBoxCallback);
		eb2->SetUserData("UIPropertyItem", (Object*)this);
		eb2->SetUserData("UIPropertyItemIndex", 2);
	}

	ResetItem();
}
//----------------------------------------------------------------------------
UIFrame *UIPropertyItem::GetEditCtrl()
{
	return mEditCtrl;
}
//----------------------------------------------------------------------------
void UIPropertyItem::OnAlloc()
{
}
//----------------------------------------------------------------------------
void UIPropertyItem::OnFree()
{
	ResetItem();
}
//----------------------------------------------------------------------------
void UIPropertyItem::SetObjectID(uint32_t id)
{
	mObjectID = id;
}
//----------------------------------------------------------------------------
uint32_t UIPropertyItem::GetObjectID()
{
	return mObjectID;
}
//----------------------------------------------------------------------------
void UIPropertyItem::SetItemType(ItemType it, Object::PropertyType propType,
	const Any &data, const Any &data1, float sliderMinVal,
	float sliderMaxVal, bool isEnable)
{
	mPropertyType = propType;
	mItemType = it;

	SetValue(data);

	Float3 textColor = PX2_UISM.Color_ContentFont;
	if (!isEnable) textColor = PX2_UISM.Color_ContentFontDisable;

	UIFrame *uiFrame = DynamicCast<UIFrame>(mEditCtrl);
	UIEditBox *editBox = DynamicCast<UIEditBox>(mEditCtrl);
	UIComboBox *comboBox = DynamicCast<UIComboBox>(mEditCtrl);
	
	// PT_FLOAT,
	// PT_BOOL,
	// PT_BOOLCHECK,
	// PT_COLOR3FLOAT3,
	// PT_FLOAT2,
	//	PT_FLOAT3,
	//	PT_FLOAT4,
	//	PT_APOINT3,
	//	PT_APOINT4,
	//	PT_AVECTOR3,
	//	PT_AVECTOR4,
	//	PT_SIZE,
	//	PT_RECT,
	//	PT_STRING,
	//	PT_STRINGBUTTON,
	//	PT_ENUM,
	//	PT_TRANSFORM,
	//	PT_EMFLOAT,
	//	PT_EMFLOATRANGE,
	//	PT_EMFLOAT3,
	//	PT_EMFLOAT3RANGE,
	//	PT_EMFLOAT3COLOR,
	//	PT_EMFLOAT3COLORRANGE,
	//	PT_CLASS,
	if (PT_INT == propType)
	{
		int val = PX2_ANY_AS(data, int);

		std::string strVal = StringHelp::IntToString(val);
		editBox->SetText(strVal);
		editBox->SetUserData("datatype", Object::PT_INT);
	}
	else if (PT_FLOAT == propType)
	{
		float val = PX2_ANY_AS(data, float);

		std::string strVal = StringHelp::FloatToString(val, 2);
		editBox->SetText(strVal);
		editBox->SetUserData("datatype", Object::PT_FLOAT);
	}
	else if (PT_INTSLIDER == propType)
	{
		int val = PX2_ANY_AS(data, int);
		
		UIEditBox *eb0 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(0));
		std::string strVal = StringHelp::IntToString(val);
		eb0->SetText(strVal);
		eb0->SetUserData("datatype", Object::PT_INT);

		UISlider *slider = DynamicCast<UISlider>(mEditCtrl->GetChild(1));
		float percent = ((float)val - sliderMinVal) / (sliderMaxVal - sliderMinVal);
		slider->SetUserData("EditBox", eb0);
		slider->SetUserData("SliderMinVal", sliderMinVal);
		slider->SetUserData("SliderMaxVal", sliderMaxVal);
		slider->SetUserData("IsInt", true);
		slider->SetPercent(percent);
	}
	else if (PT_FLOATSLIDER == propType)
	{
		float val = PX2_ANY_AS(data, float);
		std::string strVal = StringHelp::FloatToString(val, 2);

		UIEditBox *eb0 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(0));
		eb0->SetText(strVal);
		eb0->SetUserData("datatype", Object::PT_FLOAT);

		UISlider *slider = DynamicCast<UISlider>(mEditCtrl->GetChild(1));
		float percent = (val - sliderMinVal) / (sliderMaxVal - sliderMinVal);
		slider->SetUserData("EditBox", eb0);
		slider->SetUserData("SliderMinVal", sliderMinVal);
		slider->SetUserData("SliderMaxVal", sliderMaxVal);
		slider->SetUserData("IsInt", false);
		slider->SetPercent(percent);
	}
	else if (Object::PT_FLOAT2 == propType || Object::PT_SIZE == propType)
	{
		float val0 = 0.0f;
		float val1 = 0.0f;

		if (Object::PT_FLOAT2 == propType)
		{
			Float2 val = PX2_ANY_AS(data, Float2);
			val0 = val[0];
			val1 = val[1];
		}
		else if (Object::PT_SIZE == propType)
		{
			Sizef val = PX2_ANY_AS(data, Sizef);
			val0 = val.Width;
			val1 = val.Height;
		}

		UIEditBox *eb0 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(0));
		eb0->SetText(StringHelp::FloatToString(val0, 2));
		eb0->SetUserData("datatype", propType);
		eb0->SetUserData("UIPropertyItemIndex", 0);
		eb0->Enable(isEnable);
		eb0->GetInputText()->SetFontColor(textColor);

		UIEditBox *eb1 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(1));
		eb1->SetText(StringHelp::FloatToString(val1, 2));
		eb1->SetUserData("datatype", propType);
		eb1->SetUserData("UIPropertyItemIndex", 1);
		eb1->Enable(isEnable);
		eb1->GetInputText()->SetFontColor(textColor);
	}
	else if (Object::PT_COLOR3FLOAT3 == propType)
	{
		Float3 val = PX2_ANY_AS(data, Float3);

		UIFrame *frame = DynamicCast<UIFrame>(mEditCtrl); 
		if (frame)
		{
			UIButton *button = DynamicCast<UIButton>(frame->GetObjectByName("ButColor"));
			if (button)
			{
				button->SetUserData("datatype", propType);
				button->SetStateColor(UIButtonBase::BS_NORMAL, val);
				button->SetStateColor(UIButtonBase::BS_HOVERED, val);
				button->SetStateColor(UIButtonBase::BS_PRESSED, val);
			}
		}
	}
	else if (Object::PT_FLOAT3 == propType || Object::PT_APOINT3 == propType ||
		Object::PT_AVECTOR3 == propType)
	{
		float val0 = 0.0f;
		float val1 = 0.0f;
		float val2 = 0.0f;

		if (Object::PT_FLOAT3 == propType)
		{
			Float3 val = PX2_ANY_AS(data, Float3);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}
		else if (Object::PT_APOINT3 == propType)
		{
			APoint val = PX2_ANY_AS(data, APoint);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}
		else if (Object::PT_AVECTOR3 == propType)
		{
			AVector val = PX2_ANY_AS(data, AVector);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}

		UIEditBox *eb0 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(0));
		eb0->SetText(StringHelp::FloatToString(val0, 2));
		eb0->SetUserData("datatype", propType);
		eb0->SetUserData("UIPropertyItemIndex", 0);
		eb0->Enable(isEnable);
		eb0->GetInputText()->SetFontColor(textColor);

		UIEditBox *eb1 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(1));
		eb1->SetText(StringHelp::FloatToString(val1, 2));
		eb1->SetUserData("datatype", propType);
		eb1->SetUserData("UIPropertyItemIndex", 1);
		eb1->Enable(isEnable);
		eb1->GetInputText()->SetFontColor(textColor);

		UIEditBox *eb2 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(2));
		eb2->SetText(StringHelp::FloatToString(val2, 2));
		eb2->SetUserData("datatype", propType);
		eb2->SetUserData("UIPropertyItemIndex", 2);
		eb2->Enable(isEnable);
		eb2->GetInputText()->SetFontColor(textColor);
	}
	else if (Object::PT_FLOAT4 == propType || Object::PT_APOINT4 == propType ||
		Object::PT_AVECTOR4 == propType)
	{
		float val0 = 0.0f;
		float val1 = 0.0f;
		float val2 = 0.0f;

		if (Object::PT_FLOAT4 == propType)
		{
			Float4 val = PX2_ANY_AS(data, Float4);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}
		else if (Object::PT_APOINT4 == propType)
		{
			APoint val = PX2_ANY_AS(data, APoint);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}
		else if (Object::PT_AVECTOR4 == propType)
		{
			AVector val = PX2_ANY_AS(data, AVector);
			val0 = val[0];
			val1 = val[1];
			val2 = val[2];
		}

		UIEditBox *eb0 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(0));
		eb0->SetText(StringHelp::FloatToString(val0, 2));
		eb0->SetUserData("datatype", propType);
		eb0->SetUserData("UIPropertyItemIndex", 0);
		eb0->Enable(isEnable);
		eb0->GetInputText()->SetFontColor(textColor);

		UIEditBox *eb1 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(1));
		eb1->SetText(StringHelp::FloatToString(val1, 2));
		eb1->SetUserData("datatype", propType);
		eb1->SetUserData("UIPropertyItemIndex", 1);
		eb1->Enable(isEnable);
		eb1->GetInputText()->SetFontColor(textColor);

		UIEditBox *eb2 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(2));
		eb2->SetText(StringHelp::FloatToString(val2, 2));
		eb2->SetUserData("datatype", propType);
		eb2->SetUserData("UIPropertyItemIndex", 2);
		eb2->Enable(isEnable);
		eb2->GetInputText()->SetFontColor(textColor);
	}
	else if (PT_BOOL == propType || PT_BOOLCHECK == propType)
	{
		bool bl = PX2_ANY_AS(data, bool);

		comboBox->RemoveAllChooseStr();
		UIItem *item = comboBox->AddChooseStr("true");
		item->GetFText()->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScaleCombo);
		item = comboBox->AddChooseStr("false");
		item->GetFText()->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScaleCombo);
		comboBox->SetChooseListHeightSameWithChooses();
		comboBox->SetUserData("IsBool", true);
		if (bl) comboBox->Choose(0);
		else comboBox->Choose(1);
	}
	else if (PT_ENUM == propType)
	{
		int selectIndex = PX2_ANY_AS(data, int);
		std::vector<std::string> selectStrs = PX2_ANY_AS(data1, std::vector<std::string>);
		comboBox->RemoveAllChooseStr();
		for (int i = 0; i < (int)selectStrs.size(); i++)
		{
			std::string selectStr = selectStrs[i];
			UIItem *item = comboBox->AddChooseStr(selectStr);
			item->GetFText()->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScaleCombo);
		}
		comboBox->SetChooseListHeightSameWithChooses();
		comboBox->SetUserData("IsBool", false);
		comboBox->Choose(selectIndex);
		comboBox->GetSelectButton()->GetText()->SetFontScale(PX2_UISM.Size_PropertyFontScaleCombo);
		comboBox->SetTextColor(textColor);
	}
	else if (PT_STRING == propType)
	{
		std::string val = PX2_ANY_AS(data, std::string);
		editBox->SetText(val);
		editBox->SetUserData("datatype", Object::PT_STRING);
	}
	else if (PT_STRINGBUTTON == propType)
	{
		std::string val = PX2_ANY_AS(data, std::string);

		UIEditBox *editBoxSub = DynamicCast<UIEditBox>(uiFrame->GetObjectByName("EditBox"));
		UIButton *butSub = DynamicCast<UIButton>(uiFrame->GetObjectByName("Button"));

		if (editBoxSub)
		{
			editBoxSub->SetText(val);
			editBoxSub->SetUserData("datatype", Object::PT_STRINGBUTTON);
		}
	}
	else if (PT_EMFLOAT == propType)
	{
		InterpCurvePointFloat point = PX2_ANY_AS(data, InterpCurvePointFloat);
		std::string text = StringHelp::FloatToString(point.InVal);

		editBox->SetText(text);
		editBox->SetUserData("datatype", Object::PT_STRING);

	}
	else if (PT_EMFLOATRANGE == propType)
	{

	}
	else if (PT_EMFLOAT3 == propType)
	{

	}
	else if (PT_EMFLOAT3RANGE == propType)
	{

	}
	else if (PT_EMFLOAT3COLOR == propType)
	{

	}
	else if (PT_EMFLOAT3COLORRANGE == propType)
	{

	}

	if (editBox)
	{
		editBox->Enable(isEnable);
		editBox->GetInputText()->SetFontColor(textColor);
	}
	if (comboBox)
	{
		comboBox->Enable(isEnable);
		comboBox->SetTextColor(textColor);
	}
}
//----------------------------------------------------------------------------
UIPropertyItem::ItemType UIPropertyItem::GetItemType() const
{
	return mItemType;
}
//----------------------------------------------------------------------------
void UIPropertyItem::ResetItem()
{
	RemoveAllChildItems();

	Select(false);

	mIsNeedRecal = true;
	mIsNumAllChildExpandNeedRecal = true;

	if (IT_CLASS != mItemType)
	{
		SetLevelAdjust(-0.25f);
	}

	SetActivate(false);

	Float3 textColor = PX2_UISM.Color_ContentFont;

	UIEditBox *editBox = DynamicCast<UIEditBox>(mEditCtrl);
	UIComboBox *comboBox = DynamicCast<UIComboBox>(mEditCtrl);
	if (editBox)
	{
		editBox->Enable(true);
		editBox->GetInputText()->SetFontColor(textColor);
	}
	if (comboBox)
	{
		comboBox->Enable(true);
		comboBox->SetTextColor(textColor);
		comboBox->RemoveAllChooseStr();
		comboBox->Choose(0);
	}
	if (Object::PT_COLOR3FLOAT3 == mPropertyType)
	{
		UIFrame *colorFrame = DynamicCast<UIFrame>(mEditCtrl);
		UIButton *colorBut = DynamicCast<UIButton>(colorFrame->GetObjectByName("ButColor"));
		if (colorBut)
		{
			colorBut->Enable(true);
			colorBut->SetStateColor(UIButtonBase::BS_NORMAL, Float3::WHITE);
			colorBut->SetStateColor(UIButtonBase::BS_HOVERED, Float3::WHITE);
			colorBut->SetStateColor(UIButtonBase::BS_PRESSED, Float3::WHITE);
		}
	}
	if (Object::PT_FLOAT3 == mPropertyType || Object::PT_APOINT3 == mPropertyType ||
		Object::PT_AVECTOR3 == mPropertyType)
	{
		UIEditBox *eb0 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(0));
		eb0->Enable(true);

		UIEditBox *eb1 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(1));
		eb1->Enable(true);

		UIEditBox *eb2 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(2));
		eb2->Enable(true);
	}

	if (Object::PT_FLOAT4 == mPropertyType || Object::PT_APOINT4 == mPropertyType ||
		Object::PT_AVECTOR4 == mPropertyType)
	{
		UIEditBox *eb0 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(0));
		eb0->Enable(true);

		UIEditBox *eb1 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(1));
		eb1->Enable(true);

		UIEditBox *eb2 = DynamicCast<UIEditBox>(mEditCtrl->GetChild(2));
		eb2->Enable(true);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIPropertyItem::UIPropertyItem(LoadConstructor value) :
UIItem(value)
{
}
//----------------------------------------------------------------------------
void UIPropertyItem::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIItem::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIPropertyItem, source);
}
//----------------------------------------------------------------------------
void UIPropertyItem::Link(InStream& source)
{
	UIItem::Link(source);
}
//----------------------------------------------------------------------------
void UIPropertyItem::PostLink()
{
	UIItem::PostLink();
}
//----------------------------------------------------------------------------
bool UIPropertyItem::Register(OutStream& target) const
{
	if (UIItem::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIPropertyItem::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIItem::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIItem, target);
}
//----------------------------------------------------------------------------
int UIPropertyItem::GetStreamingSize(Stream &stream) const
{
	int size = UIItem::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------