// PX2IMEDispatcher.hpp

#include "PX2IMEDispatcher.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

#if defined(__ANDROID__)
#include "AppPlayJNI.hpp"
#elif defined (__IOS__)
#include "AppPlayIOS.hpp"
#endif

//----------------------------------------------------------------------------
// IMEDispatcher::Impl
//----------------------------------------------------------------------------
IMEDispatcher::Impl::Impl () :
mDelegateWithIME(0)
{
}
//----------------------------------------------------------------------------
IMEDispatcher::Impl::~Impl ()
{
}
//----------------------------------------------------------------------------
IMEDispatcher::DelegateIter IMEDispatcher::Impl::FindDelegate (
	IMEDelegate *delegate)
{
	DelegateIter it = mDelegateList.begin();
	for (; it!=mDelegateList.end(); ++it)
	{
		if (delegate == *it)
			return it;
	}

	return mDelegateList.end();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// IMEDispatcher
//----------------------------------------------------------------------------
IMEDispatcher::IMEDispatcher ()
{
	mImpl = new0 IMEDispatcher::Impl();
}
//----------------------------------------------------------------------------
IMEDispatcher::~IMEDispatcher ()
{
	delete0(mImpl);
}

void IMEDispatcher::Update()
{
}
//----------------------------------------------------------------------------
void IMEDispatcher::SetOpenStr(const std::string &openStr)
{
    mOpenStr = openStr;
}
//----------------------------------------------------------------------------
void IMEDispatcher::SetKeyboardState (bool open)
{
#if defined(__ANDROID__)
	SetKeyboardStateJNI(open? 1 : 0);
#elif defined (__IOS__)
    AppPlay::SetKeyboardStateIOS(open? 1:0, mOpenStr.c_str());
#endif
}
//----------------------------------------------------------------------------
void IMEDispatcher::DispathInsertText (const char *text, int length)
{
	if (!mImpl || !text || length<=0)
		return;

	if (!mImpl->mDelegateWithIME)
		return;

	mImpl->mDelegateWithIME->InsertText(text, length);
}
//----------------------------------------------------------------------------
void IMEDispatcher::DispathDeleteBackward ()
{
	if (!mImpl)
		return;

	if (!mImpl->mDelegateWithIME)
		return;

	mImpl->mDelegateWithIME->DeleteBackward();
}
//----------------------------------------------------------------------------
void IMEDispatcher::DispathDelete()
{
	if (!mImpl)
		return;

	if (!mImpl->mDelegateWithIME)
		return;

	mImpl->mDelegateWithIME->Delete();
}
//----------------------------------------------------------------------------
void IMEDispatcher::DispathArrow(int dirType)
{
	if (!mImpl)
		return;

	if (!mImpl->mDelegateWithIME)
		return;

	mImpl->mDelegateWithIME->InsertArrow(dirType);
}
//----------------------------------------------------------------------------
const char *IMEDispatcher::GetContentText () const
{
	if (mImpl && mImpl->mDelegateWithIME)
	{
		return mImpl->mDelegateWithIME->GetContentText();
	}

	return mEmptyString.c_str();
}
//----------------------------------------------------------------------------
void IMEDispatcher::DispatchKeyboardWillShow (IMEKeyboardNotificationInfo &info)
{
	if (!mImpl)
		return;

	DelegateIter it = mImpl->mDelegateList.begin();
	for (; it!=mImpl->mDelegateList.end(); it++)
	{
		if (*it)
		{
			(*it)->KeyboardWillShow(info);
		}
	}
}
//----------------------------------------------------------------------------
void IMEDispatcher::DispatchKeyboardDidShow (IMEKeyboardNotificationInfo &info)
{
	if (!mImpl)
		return;

	DelegateIter it = mImpl->mDelegateList.begin();
	for (; it!=mImpl->mDelegateList.end(); it++)
	{
		if (*it)
		{
			(*it)->KeyboardDidShow(info);
		}
	}
}
//----------------------------------------------------------------------------
void IMEDispatcher::DispatchKeyboardWillHide (IMEKeyboardNotificationInfo &info)
{
	if (!mImpl)
		return;

	DelegateIter it = mImpl->mDelegateList.begin();
	for (; it!=mImpl->mDelegateList.end(); it++)
	{
		if (*it)
		{
			(*it)->KeyboardWillHide(info);
		}
	}
}
//----------------------------------------------------------------------------
void IMEDispatcher::DispatchKeyboardDidHide (IMEKeyboardNotificationInfo &info)
{
	if (!mImpl)
		return;

	DelegateIter it = mImpl->mDelegateList.begin();
	for (; it!=mImpl->mDelegateList.end(); it++)
	{
		if (*it)
		{
			(*it)->KeyboardDidHide(info);
		}
	}
}
//----------------------------------------------------------------------------
void IMEDispatcher::AddDelegate (IMEDelegate *delegate)
{
	if (!delegate || !mImpl)
		return;

	if (mImpl->mDelegateList.end() != mImpl->FindDelegate(delegate))
		return;

	mImpl->mDelegateList.push_front(delegate);
}
//----------------------------------------------------------------------------
bool IMEDispatcher::AttachDelegateWithIME (IMEDelegate *delegate)
{
	if (!mImpl || !delegate)
		return false;

	DelegateIter end = mImpl->mDelegateList.end();
	DelegateIter iter = mImpl->FindDelegate(delegate);

	if (end == iter)
		return false;

	if (mImpl->mDelegateWithIME)
	{
		if (!mImpl->mDelegateWithIME->CanAttachWithIME() ||
			!mImpl->mDelegateWithIME->CanDetachWidthIME())
			return false;

		mImpl->mDelegateWithIME->DidDetachWidthIME();
		mImpl->mDelegateWithIME = 0;
	}

	if (delegate->CanAttachWithIME())
	{
		mImpl->mDelegateWithIME = *iter;
		delegate->DidAttachWidhIME();

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool IMEDispatcher::DetachDelegateWithIME (IMEDelegate *delegate)
{
	if (!mImpl || !delegate)
		return false;

	if (mImpl->mDelegateWithIME != delegate)
		return false;

	if (!delegate->CanDetachWidthIME())
		return false;
	
	delegate->DidDetachWidthIME();
	mImpl->mDelegateWithIME = 0;

	return true;
}
//----------------------------------------------------------------------------
void IMEDispatcher::RemoveDelegate (IMEDelegate *delegate)
{
	if (!delegate || !mImpl)
		return;

	DelegateIter it = mImpl->FindDelegate(delegate);
	DelegateIter end = mImpl->mDelegateList.end();

	if (it == end)
		return;

	if (mImpl->mDelegateWithIME)
	{
		if (*it == mImpl->mDelegateWithIME)
		{
			mImpl->mDelegateWithIME = 0;
		}

		mImpl->mDelegateList.erase(it);
	}
}
//----------------------------------------------------------------------------
