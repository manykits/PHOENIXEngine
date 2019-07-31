// PX2IMEDelegate.cpp

#include "PX2IMEDelegate.hpp"
#include "PX2IMEDispatcher.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
// IMEKeyboardNotificationInfo
//----------------------------------------------------------------------------
IMEKeyboardNotificationInfo::IMEKeyboardNotificationInfo ()
	:
Duration(0.0f)
{
}
//----------------------------------------------------------------------------
IMEKeyboardNotificationInfo::~IMEKeyboardNotificationInfo ()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// IMEDelegate
//----------------------------------------------------------------------------
IMEDelegate::IMEDelegate () :
mLastInsertArrow(0),
mDeleteFromCharaIndex(0),
mDeleteToCharaIndex(0)
{
	if (IMEDispatcher::GetSingletonPtr())
		IMEDispatcher::GetSingletonPtr()->AddDelegate(this);
}
//----------------------------------------------------------------------------
IMEDelegate::~IMEDelegate()
{
	if (IMEDispatcher::GetSingletonPtr())
		IMEDispatcher::GetSingletonPtr()->RemoveDelegate(this);
}
//----------------------------------------------------------------------------
bool IMEDelegate::AttachWithIME(const APoint &screenPos)
{
	bool rc = IMEDispatcher::GetSingleton().AttachDelegateWithIME(this);

	if (rc)
	{
		IMEDispatcher::GetSingleton().SetKeyboardState(1);
	}

	mAttachedWorldPos = screenPos;

	return rc;
}
//----------------------------------------------------------------------------
const APoint &IMEDelegate::GetAttachedWorldPos() const
{
	return mAttachedWorldPos;
}
//----------------------------------------------------------------------------
bool IMEDelegate::DetachWithIME()
{
	bool rc = IMEDispatcher::GetSingleton().DetachDelegateWithIME(this);

	if (rc)
	{
		IMEDispatcher::GetSingleton().SetKeyboardState(0);
	}

	return rc;
}
//----------------------------------------------------------------------------
void IMEDelegate::SetSelectTextIndex(int fromChraraIndex, int toCharaIndex)
{
	mDeleteFromCharaIndex = fromChraraIndex;
	mDeleteToCharaIndex = toCharaIndex;
}
//----------------------------------------------------------------------------
int IMEDelegate::GetSelectTextIndexFrom() const
{
	return mDeleteFromCharaIndex;
}
//----------------------------------------------------------------------------
int IMEDelegate::GetSelectTextIndexTo() const
{
	return mDeleteToCharaIndex;
}
//----------------------------------------------------------------------------
bool IMEDelegate::CanAttachWithIME ()
{
	return true;
}
//----------------------------------------------------------------------------
void IMEDelegate::DidAttachWidhIME ()
{
}
//----------------------------------------------------------------------------
bool IMEDelegate::CanDetachWidthIME ()
{
	return true;
}
//----------------------------------------------------------------------------
void IMEDelegate::DidDetachWidthIME ()
{
}
//----------------------------------------------------------------------------
void IMEDelegate::InsertText (const char *text, int length)
{
	PX2_UNUSED(text);
	PX2_UNUSED(length);
}
//----------------------------------------------------------------------------
void IMEDelegate::DeleteBackward ()
{
}
//----------------------------------------------------------------------------
void IMEDelegate::Delete()
{
}
//----------------------------------------------------------------------------
void IMEDelegate::InsertArrow(int dirType)
{
	mLastInsertArrow = dirType;
}
//----------------------------------------------------------------------------
int IMEDelegate::GetLastInsertArrow() const
{
	return mLastInsertArrow;
}
//----------------------------------------------------------------------------
const char *IMEDelegate::GetContentText () const
{
	return mEmptyStr.c_str();
}
//----------------------------------------------------------------------------
void IMEDelegate::KeyboardWillShow (IMEKeyboardNotificationInfo &info)
{
	PX2_UNUSED(info);
}
//----------------------------------------------------------------------------
void IMEDelegate::KeyboardDidShow (IMEKeyboardNotificationInfo &info)
{
	PX2_UNUSED(info);
}
//----------------------------------------------------------------------------
void IMEDelegate::KeyboardWillHide (IMEKeyboardNotificationInfo &info)
{
	PX2_UNUSED(info);
}
//----------------------------------------------------------------------------
void IMEDelegate::KeyboardDidHide (IMEKeyboardNotificationInfo &info)
{
	PX2_UNUSED(info);
}
//----------------------------------------------------------------------------