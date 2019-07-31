// PX2UIProgressBar.hpp

#ifndef PX2UIPROGRESSBAR_HPP
#define PX2UIPROGRESSBAR_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2UIFText.hpp"
#include "PX2Size.hpp"
#include "PX2UIProgressBarCtrl.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIProgressBar : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(UIProgressBar);
		PX2_DECLARE_STREAM(UIProgressBar);

	public:
		UIProgressBar();
		virtual ~UIProgressBar();

		void SetBackPicBox(UIFPicBox *picBox);
		void SetProgressPicBox(UIFPicBox *picBox);
		void SetOverPicBox(UIFPicBox *picBox);
		UIFPicBox *GetBackPicBox();
		UIFPicBox *GetProgressPicBox();
		UIFPicBox *GetOverPicBox();
		UIFText *CreateAddProgressText();
		UIFText *GetProgressText();
		void SetProgressTextNumMax(int max);
		int GetProgressTextNumMax() const;

		virtual void AutoMakeSizeFixable(float scale=1.0f);

		virtual void OnPivotChanged();
		virtual void OnSizeChanged();

		// 0-1.0f
		void SetProgress(float progress, bool callLogic = true);
		float GetProgress() const;

		void SetProgressAnimInit(float progress);
		void SetProgressAnim(float progress);

		void SetProgressAnimSpeed(float speed);
		float GetProgressAnimSpeed();

		UIProgressBarCtrl *GetProgressBarCtrl();

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _Adjust();

		bool mIsNeedAdujst;

		UIFPicBoxPtr mBackPicBox;
		UIFPicBoxPtr mProgressPicBox;
		UIFPicBoxPtr mOverPicBox;
		float mProgress;
		UIProgressBarCtrlPtr mPBCtrl;

		UIFTextPtr mProgressText;
		int mProgressNumMax;
	};

	PX2_REGISTER_STREAM(UIProgressBar);
	typedef Pointer0<UIProgressBar> UIProgressBarPtr;

#include "PX2UIProgressBar.inl"

}

#endif