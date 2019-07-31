// PX2L_Frame.hpp

#ifndef PX2L_FRAME_HPP
#define PX2L_FRAME_HPP

#include "PX2L_Pre.hpp"
#include "PX2L_RenderView.hpp"
#include "PX2RenderWindow.hpp"

namespace Launcher
{

	class L_Frame : public wxFrame
	{
	public:
		L_Frame(PX2::RenderWindow *uiWindow, wxWindow *parent, long style);
		virtual ~L_Frame();

		RenderView *GerRenderView();

		void OnSize(wxSizeEvent& e);

	protected:
		DECLARE_EVENT_TABLE()

		RenderView *mRenderView;
		wxSize mSize;
	};

}

#endif