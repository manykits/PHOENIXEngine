// PX2RenderWindow.cpp

#include "PX2RenderWindow.hpp"
#include "PX2Renderer.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2GraphicsEventData.hpp"
#include "PX2Log.hpp"

#if defined(_WIN32) || defined (WIN32)
#include <windows.h>
#endif

using namespace PX2;

bool RenderWindow::sIsMouseDown[3] = { false, false, false };

PX2_IMPLEMENT_RTTI(PX2, Object, RenderWindow);

//----------------------------------------------------------------------------
int RenderWindow::sNextWindowID = 0;
RenderWindow::CursorType RenderWindow::sCursorType = RenderWindow::CT_CURSOR_ARROW;
RenderWindow::ScreenDragType RenderWindow::sScreenDragType = RenderWindow::SDT_NONE;
Float2 RenderWindow::sMoveDelta;
Float2 RenderWindow::sCurCursorPos;
Float2 RenderWindow::sLastCursorPos;
//----------------------------------------------------------------------------
RenderWindow::RenderWindow() :
mParentRenderWindow(0),
mWindowHandle(0),
mIsMain(false),
mIsFloat(true),
mIsCanvasNeedReCal(true),
mRenderer(0),
mIsMaxSize(false),
mIsShow(true),
mIsCaptureMouse(false),
mIsLeftDown(false)
{
	mScreenSize = Sizef(100.0f, 100.0f);
	ComeInEventWorld();

	SetID(RenderWindow::GetNextWindowID());
}
//----------------------------------------------------------------------------
RenderWindow::~RenderWindow()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void RenderWindow::SetWindowHandle(void *handle)
{
	mWindowHandle = handle;
}
//----------------------------------------------------------------------------
void *RenderWindow::GetWindowHandle()
{
	return mWindowHandle;
}
//----------------------------------------------------------------------------
void RenderWindow::SetMain(bool main)
{
	mIsMain = main;
}
//----------------------------------------------------------------------------
bool RenderWindow::IsMain() const
{
	return mIsMain;
}
//----------------------------------------------------------------------------
void RenderWindow::SetTitle(const std::string &title)
{
	mTitle = title;
}
//----------------------------------------------------------------------------
const std::string &RenderWindow::GetTitle() const
{
	return mTitle;
}
//----------------------------------------------------------------------------
void RenderWindow::SetFloat(bool fl)
{
	mIsFloat = fl;
}
//----------------------------------------------------------------------------
bool RenderWindow::IsFloat() const
{
	return mIsFloat;
}
//----------------------------------------------------------------------------
void RenderWindow::Centre()
{
	if (mParentRenderWindow)
	{
		const Sizef &parentScreenSize = mParentRenderWindow->GetScreenSize();
		float halfX = parentScreenSize.Width * 0.5f;
		float halfZ = parentScreenSize.Height * 0.5f;
		float posX = halfX - GetScreenSize().Width*0.5f;
		float posY = halfZ + GetScreenSize().Height*0.5f;

		SetPosition(APoint(posX, 0.0f, posY));
	}
}
//----------------------------------------------------------------------------
void RenderWindow::SetPosition(const APoint &pos)
{
	mPosition = pos;

	Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowSetPosition);
	ent->SetData<RenderWindow*>(this);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void RenderWindow::SetScreenSize(const Sizef &size)
{
	mScreenSize = size;

	if (mMainCanvas)
	{
		mMainCanvas->SetScreenRect(Rectf(0.0f, 0.0f, mScreenSize.Width, 
			mScreenSize.Height));
		mMainCanvas->SetSize(mScreenSize);

		if (IsMain())
		{
			Event *ent = GraphicsES::CreateEventX(
				GraphicsES::WindowSizeChanged);
			PX2_EW.BroadcastingLocalEvent(ent);
		}
	}

	if (mRenderer)
	{
		mRenderer->ResizeWindow((int)mScreenSize.Width,
			(int)mScreenSize.Height);
	}
}
//----------------------------------------------------------------------------
void RenderWindow::MaxSize(bool maxSize)
{
	mIsMaxSize = maxSize;

	Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowMaxSize);
	ent->SetData<RenderWindow*>(this);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void RenderWindow::Show(bool show)
{
	mIsShow = show;

	Event *ent = 0;
	if (show)
	{
		ent = GraphicsES::CreateEventX(GraphicsES::WindowShow);
	}
	else
	{
		ent = GraphicsES::CreateEventX(GraphicsES::WindowHide);
	}
	ent->SetData<RenderWindow*>(this);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void RenderWindow::ShowCursor(bool showCursor)
{
#if defined(_WIN32) || defined (WIN32)

	bool bIsCursorVisible = IsShowCursor();
	if (showCursor && !bIsCursorVisible)
	{
		// Restore the old mouse position when we show the cursor.
		if (mPreCaptureMousePos.X() >= 0.0f && mPreCaptureMousePos.Z() >= 0.0f)
		{
			::SetCursorPos((int)mPreCaptureMousePos.X(), 
				(int)mPreCaptureMousePos.Z());
		}
		while (::ShowCursor(TRUE) < 0);

		mPreCaptureMousePos.X() = -1;
		mPreCaptureMousePos.Z() = -1;
	}
	else if (!showCursor && bIsCursorVisible)
	{
		while (::ShowCursor(FALSE) >= 0);

		// Remember the current mouse position when we hide the cursor.
		mPreCaptureMousePos.X() = -1;
		mPreCaptureMousePos.Z() = -1;

		POINT pos;
		::GetCursorPos(&pos);
		mPreCaptureMousePos.X() = (float)pos.x;
		mPreCaptureMousePos.Z() = (float)pos.y;
	}

#endif
}
//----------------------------------------------------------------------------
bool RenderWindow::IsShowCursor() const
{
#if defined(_WIN32) || defined (WIN32)

	CURSORINFO CursorInfo;
	CursorInfo.cbSize = sizeof(CURSORINFO);
	bool bIsVisible = (::GetCursorInfo(&CursorInfo) != 0);
	bIsVisible = bIsVisible && bool(CursorInfo.flags & 
		CURSOR_SHOWING) && bool(CursorInfo.hCursor != NULL);
	return bIsVisible;

#endif
    
    return false;
}
//----------------------------------------------------------------------------
void RenderWindow::SetCursorPos(const Float2 &pos)
{
	sCurCursorPos = pos;

#if defined(_WIN32) || defined (WIN32)
	int x = (int)pos[0];
	int y = (int)GetSystemMetrics(SM_CYSCREEN) - (int)pos[1];
	::SetCursorPos(x, y);
#endif
}
//----------------------------------------------------------------------------
Float2 RenderWindow::GetCursorPos()
{
	Float2 pos;

#if defined(_WIN32) || defined (WIN32)
	POINT p;
	::GetCursorPos(&p);

	pos[0] = (float)p.x;
	pos[1] = (float)GetSystemMetrics(SM_CYSCREEN) - (float)p.y;
#endif

	return pos;
}
//----------------------------------------------------------------------------
Sizef RenderWindow::GetDeskScreenSize()
{
	Sizef size;

#if defined(_WIN32) || defined (WIN32)
	size.Width = (float)GetSystemMetrics(SM_CXSCREEN);
	size.Height = (float)GetSystemMetrics(SM_CYSCREEN);
#endif

	return size;
}
//----------------------------------------------------------------------------
void RenderWindow::SetScreenDragType(ScreenDragType type)
{
	sScreenDragType = type;
}
//----------------------------------------------------------------------------
RenderWindow::ScreenDragType RenderWindow::GetScreenDragType()
{
	return sScreenDragType;
}
//----------------------------------------------------------------------------
void RenderWindow::SetMouseDown(int mouseType, bool down)
{
	if (0 <= mouseType && mouseType<3)
		sIsMouseDown[mouseType] = down;
}
//----------------------------------------------------------------------------
bool RenderWindow::IsMouseDown(int mouseType)
{
	if (0 <= mouseType && mouseType<3)
		return sIsMouseDown[mouseType];

	return false;
}
//----------------------------------------------------------------------------
bool RenderWindow::IsMouseDownCall(int mouseType)
{
#if defined(_WIN32) || defined (WIN32)
	SHORT val = 0;

	if (0 == mouseType)
	{
		val = ::GetAsyncKeyState(VK_LBUTTON);
	}
	else if (1 == mouseType)
	{
		val = ::GetAsyncKeyState(VK_RBUTTON);
	}
	else if (2 == mouseType)
	{
		val = ::GetAsyncKeyState(VK_MBUTTON);
	}

	int lastVal = (val & 0x8000);
	return lastVal != 0;
#else
	return false;
#endif
}
//----------------------------------------------------------------------------
bool RenderWindow::IsScreenDrag()
{
	return sScreenDragType != RenderWindow::SDT_NONE;
}
//----------------------------------------------------------------------------
Float2 RenderWindow::GetCurCursorPos()
{
	return sCurCursorPos;
}
//----------------------------------------------------------------------------
Float2 RenderWindow::GetLastCursorPos()
{
	return sLastCursorPos;
}
//----------------------------------------------------------------------------
Float2 RenderWindow::GetMoveDelta()
{
	return sMoveDelta;
}
//----------------------------------------------------------------------------
void RenderWindow::CaptureMouse(bool capture)
{
#if defined(_WIN32) || defined (WIN32)
	HWND captureWindow = ::GetCapture();
	bool bIsMouseCaptured = (captureWindow == (HWND)mWindowHandle);

	mIsCaptureMouse = capture;

	if (capture && !bIsMouseCaptured)
	{
		::SetCapture((HWND)mWindowHandle);
	}
	else if (!capture && bIsMouseCaptured)
	{
		::ReleaseCapture();
	}
#endif
}
//----------------------------------------------------------------------------
void RenderWindow::SetParentRenderWindow(RenderWindow *rw)
{
	mParentRenderWindow = rw;
}
//----------------------------------------------------------------------------
int RenderWindow::GetNextWindowID()
{
	return sNextWindowID++;
}
//----------------------------------------------------------------------------
void RenderWindow::SetMainCanvas(Canvas *mainCanvas)
{
	mMainCanvas = mainCanvas;
	mMainCanvas->SetRenderWindow(this);
}
//----------------------------------------------------------------------------
Canvas *RenderWindow::GetMainCanvas()
{
	return mMainCanvas;
}
//----------------------------------------------------------------------------
bool CanvasLessThan(const Canvas *canvas0, const Canvas *canvas1)
{
	if (canvas0->IsMain() && !canvas1->IsMain())
		return true;

	if (!canvas0->IsMain() && canvas1->IsMain())
		return false;

	if (canvas0->WorldTransform.GetTranslate().Y() == 
		canvas1->WorldTransform.GetTranslate().Y())
	{
		return canvas0 < canvas1;
	}
	else
	{
		if (canvas0->WorldTransform.GetTranslate().Y() <
			canvas1->WorldTransform.GetTranslate().Y())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
//----------------------------------------------------------------------------
void RenderWindow::SetRenderer(Renderer *renderer)
{
	mRenderer = renderer;
}
//----------------------------------------------------------------------------
void RenderWindow::_UpdateDragType()
{
	RenderWindow::ScreenDragType sdt = RenderWindow::GetScreenDragType();
	bool isMouseDownLeft = RenderWindow::IsMouseDownCall(0);
	bool isMouseDownRight = RenderWindow::IsMouseDownCall(1);
	bool isMouseDownMiddle = RenderWindow::IsMouseDownCall(2);

	if (sdt == RenderWindow::SDT_LEFT && !isMouseDownLeft)
	{
		RenderWindow::SetScreenDragType(RenderWindow::SDT_NONE);
	}
	else if (sdt == RenderWindow::SDT_RIGHT && !isMouseDownRight)
	{
		RenderWindow::SetScreenDragType(RenderWindow::SDT_NONE);
	}
	else if (sdt == RenderWindow::SDT_MIDDLE && !isMouseDownMiddle)
	{
		RenderWindow::SetScreenDragType(RenderWindow::SDT_NONE);
	}
}
//----------------------------------------------------------------------------
void RenderWindow::Update(double appSeconds, double elapsedSeconds)
{
	if (mRenderer)
	{
#if defined PX2_USE_OPENGL
		mRenderer->PreDraw();
#endif
	}

	if (IsMain())
	{
		Float2 curCursorPos = GetCursorPos();
		sCurCursorPos = curCursorPos;

		if (IsScreenDrag())
		{
			Sizef screenSize = GetDeskScreenSize();

			bool doReset = false;
			if (curCursorPos[0] <= 0)
			{
				doReset = true;
				curCursorPos[0] = screenSize.Width - 2;
			}
			else if (curCursorPos[0] >= screenSize.Width - 1)
			{
				doReset = true;
				curCursorPos[0] = 1;
			}
			else if (curCursorPos[1] <= 1)
			{
				doReset = true;
				curCursorPos[1] = screenSize.Height - 2;
			}
			else if (curCursorPos[1] >= screenSize.Height - 1)
			{
				doReset = true;
				curCursorPos[1] = 2;
			}

			if (doReset)
			{
				sCurCursorPos = curCursorPos;
				SetCursorPos(curCursorPos);
				sLastCursorPos = curCursorPos;
			}
		}

		sMoveDelta[0] = sCurCursorPos[0] - sLastCursorPos[0];
		sMoveDelta[1] = sCurCursorPos[1] - sLastCursorPos[1];
		sLastCursorPos = curCursorPos;

		_UpdateDragType();
	}

	if (mMainCanvas)
	{
		mMainCanvas->Update(appSeconds, elapsedSeconds);
	}
}
//----------------------------------------------------------------------------
void RenderWindow::ComputeVisibleSet()
{
	if (mMainCanvas)
	{
		mMainCanvas->ComputeVisibleSet();
	}
}
//----------------------------------------------------------------------------
void RenderWindow::Draw()
{
	if (mRenderer && mRenderer->PreDraw())
	{
		if (mMainCanvas && mMainCanvas->IsShow())
		{
			mMainCanvas->Draw(mRenderer);
		}

		mRenderer->PostDraw();
		mRenderer->DisplayColorBuffer();
	}
}
//----------------------------------------------------------------------------
void RenderWindow::SetCursorType(CursorType ct)
{
	sCursorType = ct;

	Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowSetCursorType);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
RenderWindow::CursorType RenderWindow::GetCursorType()
{
	return sCursorType;
}
//----------------------------------------------------------------------------
void RenderWindow::OnEvent(Event *event)
{
	if (InputEventSpace::IsIn(event))
	{
		// get data
		InputEventData data = event->GetData<InputEventData>();

		// viewID
		int viewID = data.ViewID;
		if (viewID != GetID())
			return;

		if (mMainCanvas)
			mMainCanvas->OnFirstInput(data);
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::AddObject))
	{
		AddObjectData data = event->GetData<AddObjectData>();
		Object *obj = data.Obj;
		Canvas *canvas = DynamicCast<Canvas>(obj);
		if (canvas)
		{
			mIsCanvasNeedReCal = true;
		}
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::RemoveObject))
	{
		Object *obj = event->GetData<Object*>();
		Canvas *canvas = DynamicCast<Canvas>(obj);
		if (canvas)
		{
			mIsCanvasNeedReCal = true;
		}
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::CanvasDestory))
	{
		mIsCanvasNeedReCal = true;
	}
}
//----------------------------------------------------------------------------
int RenderWindow::sNextMenuID = 301;
//----------------------------------------------------------------------------
int RenderWindow::AddMenuMain(const std::string &title)
{
	if (mIsMain)
	{
		AddMenuData dt;
		dt.ParentID = 0;
		dt.ID = _GetNextMenuID();
		dt.Title = title;
		dt.Callback = "";

		Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowAddMenu);
		ent->SetData(dt);
		PX2_EW.BroadcastingLocalEvent(ent);
		
		return dt.ID;
	}

	return 0;
}
//----------------------------------------------------------------------------
int RenderWindow::AddMenuSpearater(int parentID)
{
	if (mIsMain)
	{
		AddMenuData dt;
		dt.ParentID = parentID;
		dt.ID = 0;

		Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowAddMenu);
		ent->SetData(dt);
		PX2_EW.BroadcastingLocalEvent(ent);

		return dt.ID;
	}

	return 0;
}
//----------------------------------------------------------------------------
int RenderWindow::AddMenuSub(int parentID, const std::string &title,
	const std::string &callback)
{
	if (mIsMain)
	{
		AddMenuData dt;
		dt.ParentID = parentID;
		dt.ID = _GetNextMenuID();
		dt.Title = title;
		dt.Callback = callback;

		Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowAddMenu);
		ent->SetData(dt);
		PX2_EW.BroadcastingLocalEvent(ent);

		return dt.ID;
	}

	return 0;
}
//----------------------------------------------------------------------------
void RenderWindow::OpenFileDlg(const std::string &ext, const std::string &callBack)
{
	Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowOpenFileDlg);
	OpenSaveFileDlgData dt;
	dt.ExtStr = ext;
	dt.Callback = callBack;
	ent->SetData<OpenSaveFileDlgData>(dt);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void RenderWindow::SaveFileDlg(const std::string &ext, const std::string &callBack)
{
	Event *ent = GraphicsES::CreateEventX(GraphicsES::WindowSaveFileDlg);
	OpenSaveFileDlgData dt;
	dt.ExtStr = ext;
	dt.Callback = callBack;
	ent->SetData<OpenSaveFileDlgData>(dt);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
int RenderWindow::_GetNextMenuID()
{
	return sNextMenuID++;
}
//----------------------------------------------------------------------------