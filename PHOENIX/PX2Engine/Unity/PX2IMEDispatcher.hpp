// PX2IMEDispatcher.hpp

#ifndef PX2IMEDISPATCHER_HPP
#define PX2IMEDISPATCHER_HPP

#include "PX2UnityPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2IMEDelegate.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM IMEDispatcher : public Singleton<IMEDispatcher>
	{
	public:
		IMEDispatcher ();
		virtual ~IMEDispatcher ();

        void SetOpenStr(const std::string &openStr);
		void SetKeyboardState (bool open);

		void DispathInsertText (const char *text, int length);
		void DispathDeleteBackward ();
		void DispathDelete();
		void DispathArrow(int dirType); // left 0, right 1, bottom 2, to 3

		const char *GetContentText () const;

		void Update();

		void DispatchKeyboardWillShow (IMEKeyboardNotificationInfo &info);
		void DispatchKeyboardDidShow (IMEKeyboardNotificationInfo &info);
		void DispatchKeyboardWillHide (IMEKeyboardNotificationInfo &info);
		void DispatchKeyboardDidHide (IMEKeyboardNotificationInfo &info);

	protected:
		friend class IMEDelegate;

		void AddDelegate (IMEDelegate *delegate);
		bool AttachDelegateWithIME (IMEDelegate *delegate);
		bool DetachDelegateWithIME (IMEDelegate *delegate);
		void RemoveDelegate (IMEDelegate *delegate);

		typedef std::list <IMEDelegate *> DelegateList;
		typedef std::list <IMEDelegate *>::iterator DelegateIter;

		class Impl
		{
		public:
			Impl ();
			~Impl ();

			DelegateIter FindDelegate (IMEDelegate *delegate);

			DelegateList mDelegateList;
			IMEDelegate *mDelegateWithIME;
		};

		Impl *mImpl;
		std::string mEmptyString;
        std::string mOpenStr;
	};

}

#endif
