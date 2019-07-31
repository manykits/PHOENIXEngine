// PX2UIEditBox.cpp

#include "PX2UIEditBox.hpp"
#include "PX2InputEvent.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI_V(PX2, UIFrame, UIEditBox, 1);
PX2_IMPLEMENT_STREAM(UIEditBox);
PX2_IMPLEMENT_FACTORY(UIEditBox);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIEditBox);

//-------------------------------------------------------	---------------------
void UIEditBoxInputStaticTextCallback(UIInputText *inputText, 
	UIInputTextCallType type)
{
	UIEditBox *editBox = DynamicCast<UIEditBox>(inputText->GetParent());
	if (!editBox) return;

	const APoint &attachedWorldPos = inputText->GetAttachedWorldPos();
	int lastArrow = inputText->GetLastInsertArrow();

	if (UIITCT_ATTACHWITHIME == type)
	{
		editBox->OnAttachWithIME(attachedWorldPos);
	}
	else if (UIITCT_DETACHWITHWITHIME == type)
	{
		editBox->OnDetachWithIME();
	}
	else if (UIITCT_INSERTTEXT == type)
	{
		editBox->OnTextChanged();
	}
	else if (UIITCT_DELETEBACKWARD == type)
	{
		editBox->OnTextChanged();
	}
	else if (UIITCT_DELETE == type)
	{
		editBox->OnTextChanged();
	}
	else if (UIITCT_ENTER == type)
	{
		editBox->OnEnter();
	}
	else if (UIITCT_ARROW == type)
	{
		editBox->OnInsertArrow(lastArrow);
	}
}
//----------------------------------------------------------------------------
UIEditBox::UIEditBox() :
mFadePicBoxSideWidth(6.0f),
mFixedWidth(-1.0f),
mIsAttachedIME(false),
mTextWidth(0.0f),
mLeftTextWidth(0.0f),
mAttachedOffsetX(0.0f),
mAttachedSelectCharaIndex(0),
mDragingSelectCharaIndex(0),
mIsDragingSelect(false),
mAttachedTimeing(0.0f),
mIsPressed(false)
{
	SetName("UIEditBox");

	mBackPicBox = new0 UIFPicBox();
	AttachChild(mBackPicBox);
	mBackPicBox->LocalTransform.SetTranslateY(-0.5f);
	mBackPicBox->SetName("EditBoxBack");
	mBackPicBox->GetUIPicBox()->SetTexture("Data/engine/white.png");
	mBackPicBox->SetAnchorHor(0.0f, 1.0f);
	mBackPicBox->SetAnchorVer(0.0f, 1.0f);

	mFadePicBox = new0 UIFPicBox();
	AttachChild(mFadePicBox);
	mFadePicBox->GetUIPicBox()->SetColor(Float3::BLACK);
	mFadePicBox->SetAnchorHor(0.0f, 0.0f);
	mFadePicBox->SetAnchorVer(0.0f, 1.0f);
	mFadePicBox->SetAnchorParamHor(mFadePicBoxSideWidth, 0.0f);
	mFadePicBox->SetAnchorParamVer(2.0f, -2.0f);
	mFadePicBox->SetSize(2.0f, 0.0f);
	mFadePicBox->LocalTransform.SetTranslateY(-2.0f);
	mFadePicBox->SetName("EditBoxFadeRect");

	InterpCurveAlphaController *fadeCtrl = new0 InterpCurveAlphaController();
	fadeCtrl->mValues.Reset();
	fadeCtrl->mValues.AddPoint(0.0f, 1.0f, 0.0f, 0.0f, ICM_CONSTANT);
	fadeCtrl->mValues.AddPoint(0.5f, 0.0f, 0.0f, 0.0f, ICM_CONSTANT);
	fadeCtrl->Repeat = Controller::RT_WRAP;
	fadeCtrl->MaxTime = 1.0f;
	fadeCtrl->SetRelatively(false);
	mFadePicBox->AttachController(fadeCtrl);
	fadeCtrl->ResetPlay();
	mFadePicBox->Show(false);

	mInputText = new0 UIInputText();
	mInputText->SetName("EditBoxInputText");
	mInputText->SetRectUseage(UIText::RU_ALIGNS);
	mInputText->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	mInputText->SetFontWidth(20);
	mInputText->SetFontHeight(20);
	mInputText->SetOffset(Float2(mFadePicBoxSideWidth, 0.0f));
	AttachChild(mInputText);
	mInputText->LocalTransform.SetTranslateY(-2.0f);
	mInputText->SetCallback(UIEditBoxInputStaticTextCallback);
	mInputText->SetColorSelfCtrled(true);

	mSelectPicBox = new0 UIFPicBox();
	AttachChild(mSelectPicBox);
	mSelectPicBox->GetUIPicBox()->SetTexture("Data/engine/white.png");
	mSelectPicBox->LocalTransform.SetTranslateY(-1.0f);
	mSelectPicBox->SetColor(Float3::MakeColor(149, 179, 211));
	mSelectPicBox->SetColorSelfCtrled(true);
	mSelectPicBox->SetAnchorHor(0.0f, 0.0f);
	mSelectPicBox->SetAnchorVer(0.0f, 1.0f);
	float horParam = 2.0f;
	float verParam = 2.0f;
	mSelectPicBox->SetAnchorParamHor(horParam, -horParam);
	mSelectPicBox->SetAnchorParamVer(verParam, -verParam);
	mSelectPicBox->SetPivot(0.0f, 0.5f);
	mSelectPicBox->SetSize(100.0f, 0.0f);
	mSelectPicBox->Show(false);

	SetSize(150.0f, 30.0f);
	SetPivot(0.5f, 0.5f);

	SetWidget(true);

	SetText("");

	RegistToScriptSystem();
}
//----------------------------------------------------------------------------
UIEditBox::~UIEditBox()
{
}
//----------------------------------------------------------------------------
void UIEditBox::SetPassword(bool isPassword)
{
	return mInputText->SetPassword(isPassword);
}
//----------------------------------------------------------------------------
bool UIEditBox::IsPassword() const
{
	return mInputText->IsPassword();
}
//----------------------------------------------------------------------------
void UIEditBox::Enable(bool enable)
{
	UIFrame::Enable(enable);

	if (!enable)
	{
		mFadePicBox->Show(false);
		mInputText->DetachWithIME();
	}

	if (mInputText)
		mInputText->Enable(enable);
}
//----------------------------------------------------------------------------
const std::string &UIEditBox::GetText() const
{
	return mInputText->GetText();
}
//----------------------------------------------------------------------------
const std::string &UIEditBox::GetRealText() const
{
	return mInputText->GetRealText();
}
//----------------------------------------------------------------------------
void UIEditBox::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	Rectf rect = GetLocalRect();
	mInputText->SetRect(rect);
}
//----------------------------------------------------------------------------
void UIEditBox::UpdateLeftBottomCornerOffset(Movable *parent)
{
	UIFrame::UpdateLeftBottomCornerOffset(parent);

	Rectf rect = GetLocalRect();
	mInputText->SetRect(rect);
}
//----------------------------------------------------------------------------
void UIEditBox::SetText(const std::string &text)
{
	mInputText->SetRealText(text);

	_AdjustFadePicBoxPos((int)text.length(), false);
}
//----------------------------------------------------------------------------
void UIEditBox::SetFixedWidth(float fixedWidth)
{
	mFixedWidth = fixedWidth;

	if (mInputText)
	{
		mInputText->SetFixedWidth(mFixedWidth);
	}
}
//----------------------------------------------------------------------------
void UIEditBox::OnAttachWithIME(const APoint &screenPos)
{
	PX2_UNUSED(screenPos);

	_UICallbacksCalls(UICT_EDITBOX_ATTACHWITHIME);
}
//----------------------------------------------------------------------------
void UIEditBox::OnDetachWithIME()
{
	_UICallbacksCalls(UICT_EDITBOX_DETACHWITHIME);
}
//----------------------------------------------------------------------------
void UIEditBox::OnTextChanged()
{
	_UICallbacksCalls(UICT_EDITBOX_TEXTCHANGED);

	int charaIndex = mInputText->GetInputTextCharacterIndex();
	_AdjustFadePicBoxPos(charaIndex, false);
	SelectText(charaIndex, charaIndex);

	mFadePicBox->ResetPlay();
}
//----------------------------------------------------------------------------
void UIEditBox::OnEnter()
{
	_UICallbacksCalls(UICT_EDITBOX_ENTER);

	_DetachIME();
}
//----------------------------------------------------------------------------
void UIEditBox::OnInsertArrow(int dirType)
{
	PX2_UNUSED(dirType);

	int charaIndex = mInputText->GetInputTextCharacterIndex();
	_AdjustFadePicBoxPos(charaIndex, false);
}
//----------------------------------------------------------------------------
void UIEditBox::OnEvent(Event *ent)
{
	if (InputEventSpace::IsEqual(ent, InputEventSpace::KeyPressed))
	{

	}
}
//----------------------------------------------------------------------------
void UIEditBox::OnWidgetPicked(const CanvasInputData &inputData)
{
	UIFrame::OnWidgetPicked(inputData);

	if (!IsEnable())
		return;

	if (UIPT_PRESSED == inputData.PickType)
	{
		if (!mIsAttachedIME)
		{
			_AttachIME(inputData.LogicPos);
		}
		else
		{
			mAttachedOffsetX = mInputText->GetOffset()[0];
			mAttachedSelectCharaIndex = _CalAttachCharaIndex(inputData.LogicPos);
			mDragingSelectCharaIndex = mAttachedSelectCharaIndex;
			_AdjustFadePicBoxPos(mAttachedSelectCharaIndex, true);

			SelectText(mAttachedSelectCharaIndex, mDragingSelectCharaIndex);
		}

		mAttachedTimeing = 0.0f;
		mIsPressed = true;
	}
	else if (UIPT_MOVED == inputData.PickType)
	{
		if (mIsPressed && mAttachedTimeing>=0.1f)
		{
			int charaIndex = _CalAttachCharaIndex(inputData.LogicPos);
			mDragingSelectCharaIndex = charaIndex;

			int atttachIndex = mAttachedSelectCharaIndex;
			int dragIndex = mDragingSelectCharaIndex;
			if (atttachIndex > dragIndex)
			{
				atttachIndex = dragIndex;
				dragIndex = mAttachedSelectCharaIndex;
			}

			SelectText(atttachIndex, dragIndex);
			mIsDragingSelect = true;
		}
	}
	else if (UIPT_RELEASED == inputData.PickType)
	{
		mIsPressed = false;
		mAttachedTimeing = 0.0f;
		mIsDragingSelect = false;
	}
}
//----------------------------------------------------------------------------
void UIEditBox::OnSizeNodeNotPicked(const CanvasInputData &data)
{
	if (UIPT_PRESSED == data.PickType || UIPT_RELEASED == data.PickType)
	{
		if (!mIsDragingSelect)
		{
			_DetachIME();
		}
	}

	if (UIPT_RELEASED == data.PickType)
	{
		mIsPressed = false;
		mAttachedTimeing = 0.0f;
		mIsDragingSelect = false;
	}

	UIFrame::OnSizeNodeNotPicked(data);
}
//----------------------------------------------------------------------------
void UIEditBox::_AttachIME(const APoint &attachWorldPos)
{
	if (!mIsAttachedIME)
	{
		mAttachedOffsetX = mInputText->GetOffset()[0];
		const std::string &text = mInputText->GetText();
		int texLength = (int)text.length();
		mAttachedSelectCharaIndex = 0;
		mDragingSelectCharaIndex = texLength;
		_AdjustFadePicBoxPos(texLength, true);

		mFadePicBox->Show(true);
		mInputText->AttachWithIME(attachWorldPos);

		SelectText(mAttachedSelectCharaIndex, mDragingSelectCharaIndex);

		mIsAttachedIME = true;
	}
}
//----------------------------------------------------------------------------
void UIEditBox::_DetachIME()
{
	if (mIsAttachedIME)
	{
		mFadePicBox->Show(false);
		mInputText->DetachWithIME();

		_AdjustFadePicBoxPos(0, false);

		SelectText(0, 0);

		mIsAttachedIME = false;
		mIsDragingSelect = false;
	}
}
//----------------------------------------------------------------------------
int UIEditBox::_CalAttachCharaIndex(const APoint &attachWorldPos)
{
	Transform transInverse = WorldTransform.InverseTransform();
	APoint localPos = transInverse * attachWorldPos;
	float posFromLeftPosX = GetSize().Width * GetPivot()[0] + localPos.X();

	const std::string &text = mInputText->GetText();
	int textLengthNum = (int)text.length();
	float textLength = mInputText->GetTextWidth();
	float offsetX = mInputText->GetOffset()[0];

	int strIndex = 0;
	if (posFromLeftPosX <= mFadePicBoxSideWidth)
	{
		strIndex = 0;
	}
	else if (posFromLeftPosX >= (textLength + mFadePicBoxSideWidth))
	{
		strIndex = textLengthNum;
	}
	else
	{
		float fromLeftPosX = posFromLeftPosX - offsetX;

		std::string textTemp;
		float textTempLength = 0.0f;
		int textTempLegnthIndex = 0;
		int leftIndex = 0;

		if (text.empty())
		{
			strIndex = 0;
		}
		else
		{
			while (textTempLength < fromLeftPosX &&
				textTempLegnthIndex<textLengthNum)
			{
				textTemp += text[textTempLegnthIndex];
				leftIndex = textTempLegnthIndex;
				textTempLegnthIndex++;
	
				textTempLength = mInputText->GetTextWidth(textTemp);
			}

			strIndex = leftIndex;
		}
	}

	return strIndex;
}
//----------------------------------------------------------------------------
void UIEditBox::_AdjustFadePicBoxPos(int characterIndex, bool isAttachInput)
{
	if (mFadePicBox)
	{
		Rectf rect = GetLocalRect();
		float rightMaxPos = rect.Width();

		const std::string &inputText = mInputText->GetText();
		float inputTetLength = mInputText->GetTextWidth();
		PX2_UNUSED(inputTetLength);

		std::string leftStr = inputText;
		if (characterIndex < (int)inputText.length())
			leftStr = inputText.substr(0, characterIndex);

		mTextWidth = mInputText->GetTextWidth();
		mLeftTextWidth = mInputText->GetTextWidth(leftStr);

		float offsetX = 0.0f;
		float fadePBPos = 0.0f;

		float adjust = mFadePicBoxSideWidth + mTextWidth;
		if (adjust < rightMaxPos)
		{
			offsetX = mFadePicBoxSideWidth;
			fadePBPos = mFadePicBoxSideWidth + mLeftTextWidth + 1.0f;
		}
		else
		{
			if (isAttachInput)
			{
				offsetX = mAttachedOffsetX;
				fadePBPos = mLeftTextWidth + mAttachedOffsetX + 1.0f;
			}
			else
			{
				float adjust1 = mInputText->GetOffset()[0] + mLeftTextWidth;
				float adjust2 = mInputText->GetOffset()[0] + mTextWidth;
				if (adjust2 <= rightMaxPos)
				{
					offsetX = rightMaxPos - mTextWidth;
					fadePBPos = mLeftTextWidth + offsetX;
				}
				else 
				{
					if (adjust1 <= rightMaxPos)
					{
						offsetX = mInputText->GetOffset()[0];
						fadePBPos = mLeftTextWidth + offsetX;
					}
					else
					{
						offsetX = rightMaxPos - mLeftTextWidth;
						fadePBPos = rightMaxPos - 1.0f;
					}
				}
			}
		}

		mInputText->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
		mInputText->SetOffset(Float2(offsetX, 0.0f));
		mInputText->SetInputTextCharacterIndex(characterIndex);

		mFadePicBox->SetAnchorParamHor(fadePBPos, 0.0f);
	}
}
//----------------------------------------------------------------------------
void UIEditBox::SelectText(int fromCharaIndex, int toCharaIndex)
{
	mInputText->SetSelectTextIndex(fromCharaIndex, toCharaIndex);

	int numChara = toCharaIndex - fromCharaIndex;
	if (numChara <= 0)
	{
		mSelectPicBox->SetWidth(0.0f);
		mSelectPicBox->Show(false);
	}
	else
	{
		const std::string &text = mInputText->GetText();
		const std::string &leftText = text.substr(0, fromCharaIndex);
		const std::string &rightText = text.substr(0, toCharaIndex);
		float leftWidth = mInputText->GetTextWidth(leftText);
		float rightWidth = mInputText->GetTextWidth(rightText);
		float selectCotWidth = rightWidth - leftWidth;

		float textOffsetX = mInputText->GetOffset()[0];
		float paramHor = textOffsetX + leftWidth;

		mSelectPicBox->Show(true);
		mSelectPicBox->SetWidth(selectCotWidth);
		mSelectPicBox->SetAnchorParamHor(paramHor, paramHor);
	}
}
//----------------------------------------------------------------------------
void UIEditBox::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsPressed)
	{
		mAttachedTimeing += (float)elapsedTime;
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UIEditBox::RegistProperties()
{
	UIFrame::RegistProperties();
	AddPropertyClass("UIEditBox");

	AddProperty("IsPassword", PT_BOOL, IsPassword());
	AddProperty("Size", PT_SIZE, GetSize());
	AddProperty("Text", PT_STRING, GetText());
	AddProperty("RealText", PT_STRING, GetRealText(), false);
	AddProperty("FixedWidth", PT_FLOAT, GetFixedWidth());
}
//----------------------------------------------------------------------------
void UIEditBox::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);

	if ("IsPassword" == obj.Name)
	{
		SetPassword(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("Size" == obj.Name)
	{
		SetSize(PX2_ANY_AS(obj.Data, Sizef));
	}
	else if ("Text" == obj.Name)
	{
		SetText(PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("FixedWidth" == obj.Name)
	{
		SetFixedWidth(PX2_ANY_AS(obj.Data, float));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIEditBox::UIEditBox(LoadConstructor value) :
UIFrame(value),
mFadePicBoxSideWidth(6.0f),
mFixedWidth(-1.0f),
mIsAttachedIME(false),
mTextWidth(0.0f),
mLeftTextWidth(0.0f),
mAttachedOffsetX(0.0f),
mAttachedSelectCharaIndex(0),
mDragingSelectCharaIndex(0),
mAttachedTimeing(0.0f),
mIsDragingSelect(false),
mIsPressed(false)
{
}
//----------------------------------------------------------------------------
void UIEditBox::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mBackPicBox);
	source.ReadPointer(mFadePicBox);
	source.ReadPointer(mInputText);
	source.ReadAggregate(mSize);
	source.Read(mFixedWidth);

	source.ReadPointer(mSelectPicBox);

	PX2_END_DEBUG_STREAM_LOAD(UIEditBox, source);
}
//----------------------------------------------------------------------------
void UIEditBox::Link(InStream& source)
{
	UIFrame::Link(source);

	source.ResolveLink(mBackPicBox);
	source.ResolveLink(mFadePicBox);
	source.ResolveLink(mInputText);
	source.ResolveLink(mSelectPicBox);
}
//----------------------------------------------------------------------------
void UIEditBox::PostLink()
{
	UIFrame::PostLink();

	if (mFadePicBox)
	{
		mFadePicBox->Culling = Movable::CULL_ALWAYS;
		mFadePicBox->ResetPlay();
	}

	if (mInputText)
	{
		mInputText->SetCallback(UIEditBoxInputStaticTextCallback);
	}

	if (mSelectPicBox)
	{
		mSelectPicBox->PostLink();
	}

	RegistToScriptSystem();
}
//----------------------------------------------------------------------------
bool UIEditBox::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		target.Register(mBackPicBox);
		target.Register(mFadePicBox);
		target.Register(mInputText);
		target.Register(mSelectPicBox);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIEditBox::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mBackPicBox);
	target.WritePointer(mFadePicBox);
	target.WritePointer(mInputText);
	target.WriteAggregate(mSize);

	target.Write(mFixedWidth);

	target.WritePointer(mSelectPicBox);

	PX2_END_DEBUG_STREAM_SAVE(UIEditBox, target);
}
//----------------------------------------------------------------------------
int UIEditBox::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mBackPicBox);
	size += PX2_POINTERSIZE(mFadePicBox);
	size += PX2_POINTERSIZE(mInputText);
	size += sizeof(mSize);
	size += sizeof(mFixedWidth);

	size += PX2_POINTERSIZE(mSelectPicBox);

	return size;
}
//----------------------------------------------------------------------------