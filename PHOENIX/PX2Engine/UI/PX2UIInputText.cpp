// PX2UIInputText.cpp

#include "PX2UIInputText.hpp"
#include "PX2IMEDispatcher.hpp"
#include "PX2Log.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI_V(PX2, UIText, UIInputText, 1);
PX2_IMPLEMENT_STREAM(UIInputText);
PX2_IMPLEMENT_FACTORY(UIInputText);
PX2_IMPLEMENT_DEFAULT_NAMES(UIText, UIInputText);

//----------------------------------------------------------------------------
UIInputText::UIInputText() :
mCallback(0),
mIsPassword(false),
mFixedWidth(-1.0f),
mInputTextCharacterIndex(0)
{
	SetName("NoName");
	SetText("DefaultText");
}
//----------------------------------------------------------------------------
UIInputText::~UIInputText()
{
}
//----------------------------------------------------------------------------
void UIInputText::SetRealText(const std::string &realText)
{
	mRealText = realText;

	SetPassword(mIsPassword);
}
//----------------------------------------------------------------------------
void UIInputText::SetPassword(bool isPassword)
{
	mIsPassword = isPassword;

	std::string realText = GetRealText();

	if (mIsPassword)
	{
		std::string tempText;
		for (int i = 0; i < (int)realText.length(); i++)
		{
			tempText.push_back('*');
		}
		SetText(tempText);
	}
	else
	{
		SetText(realText);
	}
}
//----------------------------------------------------------------------------
void UIInputText::SetFixedWidth(float fixedWidth)
{
	mFixedWidth = fixedWidth;
}
//----------------------------------------------------------------------------
const char *UIInputText::GetContentText() const
{
	return GetText().c_str();
}
//----------------------------------------------------------------------------
void UIInputText::InsertText(const char *text, int length)
{
	_DeleteSelectedCharas();

	std::string sInsert(text, length);

	if (sInsert == "\n")
	{
		if (mCallback)
		{
			mCallback(this, UIITCT_ENTER);
		}

		return;
	}

	// \n 意味着结束
	int nPos = (int)sInsert.find('\n');
	if ((int)sInsert.npos != nPos)
	{
		length = nPos;
		sInsert.erase(nPos);
	}

	if (length > 0)
	{
		std::string leftStr = mInputString.substr(0, mInputTextCharacterIndex);
		std::string rightStr = mInputString.substr(mInputTextCharacterIndex, mInputString.length());
		mInputString = leftStr + sInsert + rightStr;

		mInputTextCharacterIndex += length;
	}

	if (-1 != mFixedWidth)
	{
		float outWidth = 0.0f;
		int outNumBytes = 0;
		mFont->GetTextExtentFitInWidth(mInputString.c_str(), mFixedWidth,
			outWidth, outNumBytes, false, mFontScale);

		mInputString = mInputString.substr(0, outNumBytes);
	}

	SetRealText(mInputString);

	if (mCallback)
	{
		mCallback(this, UIITCT_INSERTTEXT);
	}

	if (mInputString.empty())
	{
		Show(false);
	}
	else
	{
		Show(true);
	}
}
//----------------------------------------------------------------------------
void UIInputText::DeleteBackward()
{
	_DeleteSelectedCharas();

	std::string leftStr = mInputString.substr(0, mInputTextCharacterIndex);
	std::string rightStr = mInputString.substr(mInputTextCharacterIndex, 
		mInputString.length());

	int strLen = (int)leftStr.length();
	if (strLen > 0)
	{
		int deleteLength = 1;
		while (0x80 == (0xC0 & leftStr.at(strLen - deleteLength)))
		{
			++deleteLength;
		}

		if (strLen <= deleteLength)
		{
			leftStr.clear();
		}
		else
		{
			leftStr = leftStr.substr(0, strLen - deleteLength);
		}
	}

	mInputTextCharacterIndex = (int)leftStr.length();
	mInputString = leftStr + rightStr;
	SetRealText(mInputString);

	if (mCallback)
	{
		mCallback(this, UIITCT_DELETEBACKWARD);
	}

	if (mInputString.empty())
	{
		Show(false);
	}
	else
	{
		Show(true);
	}
}
//----------------------------------------------------------------------------
void UIInputText::Delete()
{
	_DeleteSelectedCharas();

	if (mCallback)
	{
		mCallback(this, UIITCT_DELETE);
	}
}
//----------------------------------------------------------------------------
void UIInputText::_DeleteSelectedCharas()
{
	int fromSelectIndex = GetSelectTextIndexFrom();
	int toSelectIndex = GetSelectTextIndexTo();

	int numChara = toSelectIndex - fromSelectIndex;
	if (numChara <= 0)
		return;

	std::string leftStr = mText.substr(0, fromSelectIndex);
	std::string rightStr = mText.substr(toSelectIndex, mText.length() - toSelectIndex);
	std::string str = leftStr + rightStr;
	mInputString = leftStr + rightStr;
	SetRealText(mInputString);
	mInputTextCharacterIndex = fromSelectIndex;

	if (mInputString.empty())
	{
		Show(false);
	}
	else
	{
		Show(true);
	}

	SetSelectTextIndex(fromSelectIndex, fromSelectIndex);
}
//----------------------------------------------------------------------------
void UIInputText::InsertArrow(int dirType)
{
	IMEDelegate::InsertArrow(dirType);

	if (0 == dirType)
	{
		mInputTextCharacterIndex--;
		if (mInputTextCharacterIndex < 0)
			mInputTextCharacterIndex = 0;
	}
	else if (1 == dirType)
	{
		mInputTextCharacterIndex++;
		if (mInputTextCharacterIndex > (int)mText.length())
			mInputTextCharacterIndex = (int)mText.length();
	}

	if (mCallback)
	{
		mCallback(this, UIITCT_ARROW);
	}
}
//----------------------------------------------------------------------------
bool UIInputText::AttachWithIME(const APoint &screenPos)
{
    mInputString = GetRealText();
    IMEDispatcher::GetSingleton().SetOpenStr(mInputString);
    
	if (IMEDelegate::AttachWithIME(screenPos))
	{
		if (mCallback)
		{
			mCallback(this, UIITCT_ATTACHWITHIME);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool UIInputText::DetachWithIME()
{
	if (IMEDelegate::DetachWithIME())
	{
		mInputString.clear();

		if (mCallback)
		{
			mCallback(this, UIITCT_DETACHWITHWITHIME);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIInputText::SetInputTextCharacterIndex(int charaIndex)
{
	mInputTextCharacterIndex = charaIndex;
}
//----------------------------------------------------------------------------
int UIInputText::GetInputTextCharacterIndex() const
{
	return mInputTextCharacterIndex;
}
//----------------------------------------------------------------------------
float UIInputText::GetTextWidth() const
{
	return UIText::GetTextWidth();
}
//---------------------------------------------------------------------------
float UIInputText::GetTextWidth(const std::string &text) const
{
	return UIText::GetTextWidth(text);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIInputText::UIInputText(LoadConstructor value) :
UIText(value),
mCallback(0),
mIsPassword(false),
mFixedWidth(-1.0f),
mInputTextCharacterIndex(0)
{
}
//----------------------------------------------------------------------------
void UIInputText::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIText::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsPassword);
	source.ReadString(mRealText);

	int readedVersion = GetReadedVersion();
	if (1 <= readedVersion)
	{
		source.Read(mFixedWidth);
	}

	PX2_END_DEBUG_STREAM_LOAD(UIInputText, source);
}
//----------------------------------------------------------------------------
void UIInputText::Link(InStream& source)
{
	UIText::Link(source);
}
//----------------------------------------------------------------------------
void UIInputText::PostLink()
{
	UIText::PostLink();
}
//----------------------------------------------------------------------------
bool UIInputText::Register(OutStream& target) const
{
	if (UIText::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIInputText::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIText::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsPassword);
	target.WriteString(mRealText);

	target.Write(mFixedWidth);

	PX2_END_DEBUG_STREAM_SAVE(UIInputText, target);
}
//----------------------------------------------------------------------------
int UIInputText::GetStreamingSize(Stream &stream) const
{
	int size = UIText::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsPassword);
	size += PX2_STRINGSIZE(mRealText);

	if (stream.IsIn())
	{
		int readedVersion = GetReadedVersion();
		if (1 <= readedVersion)
		{
			size += sizeof(mFixedWidth);
		}
	}
	else
	{
		size += sizeof(mFixedWidth);
	}

	return size;
}
//----------------------------------------------------------------------------
