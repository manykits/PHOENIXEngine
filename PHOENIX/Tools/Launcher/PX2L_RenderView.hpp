// PX2L_RenderView.hpp

#ifndef PX2L_RENDERVIEW_HPP
#define PX2L_RENDERVIEW_HPP

#include "PX2L_Pre.hpp"
#include "PX2EventHandler.hpp"
#include "PX2RenderWindow.hpp"

namespace Launcher
{

	class RenderView : public wxWindow, public PX2::EventHandler
	{
	public:
		RenderView(PX2::RenderWindow *renderWindow, wxWindow *parent);
		virtual ~RenderView();

		void OnTimer(wxTimerEvent& event);
		void OnSize(wxSizeEvent& e);

		void OnEnterWindow(wxMouseEvent& e);
		void OnLeaveWindow(wxMouseEvent& e);
		void OnLeftDown(wxMouseEvent& e);
		void OnLeftUp(wxMouseEvent& e);
		void OnLeftDoubleClick(wxMouseEvent& e);
		void OnMiddleDown(wxMouseEvent& e);
		void OnMiddleUp(wxMouseEvent& e);
		void OnMouseWheel(wxMouseEvent& e);
		void OnRightDown(wxMouseEvent& e);
		void OnRightUp(wxMouseEvent& e);
		void OnMotion(wxMouseEvent& e);
		void OnKeyDown(wxKeyEvent& e);
		void OnKeyUp(wxKeyEvent& e);
		void OnChar(wxKeyEvent& e);

	protected:
		DECLARE_EVENT_TABLE()

		bool mIsInited;

		PX2::RenderWindow *mRenderWindow;

		int mTimerID;
		wxTimer mTimer;
		wxSize mSize;
	};

}

#endif