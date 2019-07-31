// PX2L_Frame.cpp

#include "PX2L_Frame.hpp"
#include "PX2Application.hpp"
using namespace Launcher;
using namespace PX2;

BEGIN_EVENT_TABLE(L_Frame, wxFrame)
EVT_SIZE(L_Frame::OnSize)
END_EVENT_TABLE();

//----------------------------------------------------------------------------
L_Frame::L_Frame(RenderWindow *uiWindow, wxWindow *parent, long style) :
wxFrame((wxFrame*)parent, -1, uiWindow->GetTitle(), wxPoint(0, 0),
wxSize((int)uiWindow->GetScreenSize().Width,
(int)uiWindow->GetScreenSize().Height), style),
mRenderView(0)
{
	std::string name = uiWindow->GetName();
	std::string title = uiWindow->GetTitle();
	int width = (int)uiWindow->GetScreenSize().Width;
	int height = (int)uiWindow->GetScreenSize().Height;

	SetName(name);

	SetFocus();

	mRenderView = new RenderView(uiWindow, this);

	if ("Main" != name)
	{
		Renderer *renderer = PX2_APP.CreateRenderer(name,
			mRenderView->GetHandle(), 0, 0, width, height, 0);
		uiWindow->SetRenderer(renderer);
		uiWindow->SetScreenSize(Sizef((float)width, (float)height));
	}

	wxBoxSizer* bSizer69;
	bSizer69 = new wxBoxSizer(wxVERTICAL);
	bSizer69->Add(mRenderView, 1, wxEXPAND, 5);
	this->SetSizer(bSizer69);
	this->Layout();
}
//----------------------------------------------------------------------------
L_Frame::~L_Frame()
{
}
//----------------------------------------------------------------------------
RenderView *L_Frame::GerRenderView()
{
	return mRenderView;
}
//----------------------------------------------------------------------------
void L_Frame::OnSize(wxSizeEvent& e)
{
	PX2_UNUSED(e);

	mSize = GetClientSize();
	if (mRenderView)
		mRenderView->SetSize(0, 0, mSize.x, mSize.y);
}
//----------------------------------------------------------------------------