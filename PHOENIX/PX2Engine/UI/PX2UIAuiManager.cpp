// PX2UIAuiManager.cpp

#include "PX2UIAuiManager.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
int UIAuiManager::msNameID = 0;
//----------------------------------------------------------------------------
UIAuiManager::UIAuiManager()
{
}
//----------------------------------------------------------------------------
UIAuiManager::~UIAuiManager()
{
}
//----------------------------------------------------------------------------
void UIAuiManager::Clear()
{
	mUIAuiFrames.clear();
	mAuiContentFrames.clear();

	RenderWindow *rw = PX2_GR.GetMainWindow();
	Canvas *canvas = rw->GetMainCanvas();
	if (canvas)
	{
		canvas->DetachAllChildren();
	}
}
//----------------------------------------------------------------------------
std::string UIAuiManager::GenName(const std::string &tag)
{
	msNameID++;

	return tag + StringHelp::IntToString(msNameID);
}
//----------------------------------------------------------------------------
void UIAuiManager::AddAuiFrame(RenderWindow *rw, UIAuiFrame *frame)
{
	mUIAuiFrames[rw] = frame;
}
//----------------------------------------------------------------------------
UIAuiFrame *UIAuiManager::GetAuiFrame(RenderWindow *rw)
{
	std::map<RenderWindow *, UIAuiFramePtr>::iterator it = 
		mUIAuiFrames.find(rw);
	if (it != mUIAuiFrames.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void UIAuiManager::SetCaptureBlockFrame(UIAuiBlockFrame *blockFrame)
{
	mCaptureBlockFrame = blockFrame;
}
//----------------------------------------------------------------------------