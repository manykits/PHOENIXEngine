// PX2UIAuiManager.hpp

#ifndef PX2UIAUIMANAGER_HPP
#define PX2UIAUIMANAGER_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIAuiFrame.hpp"
#include "PX2UIButton.hpp"
#include "PX2RenderWindow.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIAuiManager : public Singleton<UIAuiManager>
	{
	public:
		UIAuiManager();
		virtual ~UIAuiManager();

		void Clear();

		std::string GenName(const std::string &tag);

		void AddAuiFrame(RenderWindow *rw, UIAuiFrame *frame);
		UIAuiFrame *GetAuiFrame(RenderWindow *rw);

		// active
		void SetCaptureBlockFrame(UIAuiBlockFrame *blockFrame);
		UIAuiBlockFrame *GetCaptureBlockFrame();

	protected:
		static int msNameID;

		std::map<RenderWindow *, UIAuiFramePtr> mUIAuiFrames;
		std::map<UIButton *, UIFramePtr> mAuiContentFrames;

		UIAuiBlockFramePtr mCaptureBlockFrame;
	};


#define PX2_UIAUIM UIAuiManager::GetSingleton()
#include "PX2UIAuiManager.inl"

}

#endif