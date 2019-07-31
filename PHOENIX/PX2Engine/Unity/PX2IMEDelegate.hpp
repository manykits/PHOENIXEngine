// PX2IMEDelegate.hpp

#ifndef PX2IMEDELEGATE_HPP
#define PX2IMEDELEGATE_HPP

#include "PX2UnityPre.hpp"
#include "PX2Rect.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2UnityPre.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM IMEKeyboardNotificationInfo
	{
	public:
		IMEKeyboardNotificationInfo ();
		~IMEKeyboardNotificationInfo ();

		Rectf Begin;
		Rectf End;
		float Duration;
	};
	
	class PX2_ENGINE_ITEM IMEDelegate
	{
	public:
		virtual ~IMEDelegate();

		virtual bool AttachWithIME(const APoint &screenPos);
		const APoint &GetAttachedWorldPos() const;
		virtual bool DetachWithIME();

		void SetSelectTextIndex(int fromChraraIndex, int toCharaIndex);
		int GetSelectTextIndexFrom() const;
		int GetSelectTextIndexTo() const;

		int GetLastInsertArrow() const;

	protected:
		friend class IMEDispatcher;

		IMEDelegate ();

		virtual bool CanAttachWithIME ();
		virtual void DidAttachWidhIME ();
		virtual bool CanDetachWidthIME ();
		virtual void DidDetachWidthIME ();

		virtual void InsertText (const char *text, int length);
		virtual void DeleteBackward ();
		virtual void Delete();
		virtual void InsertArrow(int dirType);

		virtual const char *GetContentText () const;

		virtual void KeyboardWillShow (IMEKeyboardNotificationInfo &info);
		virtual void KeyboardDidShow (IMEKeyboardNotificationInfo &info);
		virtual void KeyboardWillHide (IMEKeyboardNotificationInfo &info);
		virtual void KeyboardDidHide (IMEKeyboardNotificationInfo &info);

	private:
		std::string mEmptyStr;
		APoint mAttachedWorldPos;
		int mLastInsertArrow;
		int mDeleteFromCharaIndex;
		int mDeleteToCharaIndex;
	};
	typedef Pointer0<IMEDelegate> IMEDelegatePtr;

}

#endif