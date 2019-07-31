// PX2UIAuiFrame.hpp

#ifndef PX2UIAUIFRAME_HPP
#define PX2UIAUIFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UISplitterFrame.hpp"
#include "PX2UIAuiBlockFrame.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIAuiFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIAuiFrame);
		PX2_NEW(UIAuiFrame);

	public:
		UIAuiFrame();
		virtual ~UIAuiFrame();

		void SetSplitterSize(float size);

		UISplitterFramePtr SpLeft;
		UISplitterFramePtr SpRight;
		UISplitterFramePtr SpBottom;
		UISplitterFramePtr SpTop;
		UIAuiBlockFramePtr AuiBlockDefault;
	};

	PX2_REGISTER_STREAM(UIAuiFrame);
	typedef Pointer0<UIAuiFrame> UIAuiFramePtr;

}

#endif