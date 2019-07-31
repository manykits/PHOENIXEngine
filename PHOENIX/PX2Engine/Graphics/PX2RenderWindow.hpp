// PX2RenderWindow.hpp

#ifndef PX2RENDERWINDOW_HPP
#define PX2RENDERWINDOW_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Canvas.hpp"
#include "PX2Object.hpp"
#include "PX2Canvas.hpp"
#include "PX2FString.hpp"
#include "PX2Size.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM RenderWindow : public Object
	{
		PX2_DECLARE_RTTI;

	public:
		RenderWindow();
		virtual ~RenderWindow();

		void SetWindowHandle(void *handle);
		void *GetWindowHandle();

		void SetMain(bool main);
		bool IsMain() const;

		void SetTitle(const std::string &title);
		const std::string &GetTitle() const;

		void SetFloat(bool fl);
		bool IsFloat() const;

		void Centre();
		void SetPosition(const APoint &pos);
		const APoint &GetPosition() const;

		void SetScreenSize(const Sizef &size);
		const Sizef& GetScreenSize() const;

		void MaxSize(bool maxSize);
		bool IsMaxSize() const;

		void Show(bool show);
		bool IsShow() const;

		void ShowCursor(bool showCursor);
		bool IsShowCursor() const;

		void CaptureMouse(bool capture);
		bool IsCaptureMouse() const;

		RenderWindow *GetParentRenderWindow();
	
	public_internal:
		void SetParentRenderWindow(RenderWindow *rw);
		static int GetNextWindowID();

	protected:
		RenderWindow *mParentRenderWindow;

		void *mWindowHandle;
		bool mIsMain;
		bool mIsFloat;
		std::string mTitle;
		APoint mPosition;
		Sizef mScreenSize;

		bool mIsMaxSize;
		bool mIsShow;

		bool mIsCaptureMouse;
		APoint mPreCaptureMousePos;

		static int sNextWindowID;

		// Canvas;
	public:
		Canvas *GetMainCanvas();

	public_internal:
		void SetMainCanvas(Canvas *mainCanvas);

	protected:
		bool mIsCanvasNeedReCal;
		CanvasPtr mMainCanvas;
		
		// Tick
	public:
		void SetRenderer(Renderer *renderer);
		Renderer *GetRenderer();

		void Update(double appSeconds, double elapsedSeconds);
		void ComputeVisibleSet();
		void Draw();

	protected:
		void _UpdateDragType();
		Renderer *mRenderer;

	protected:
		bool mIsLeftDown;

		// DeskScreen
	public:
		enum CursorType
		{
			CT_CURSOR_NONE = 0,
			CT_CURSOR_ARROW,
			CT_CURSOR_RIGHT_ARROW,
			CT_CURSOR_BULLSEYE,
			CT_CURSOR_CHAR,
			CT_CURSOR_CROSS,
			CT_CURSOR_HAND,
			CT_CURSOR_IBEAM,
			CT_CURSOR_LEFT_BUTTON,
			CT_CURSOR_MAGNIFIER,
			CT_CURSOR_MIDDLE_BUTTON,
			CT_CURSOR_NO_ENTRY,
			CT_CURSOR_PAINT_BRUSH,
			CT_CURSOR_PENCIL,
			CT_CURSOR_POINT_LEFT,
			CT_CURSOR_POINT_RIGHT,
			CT_CURSOR_QUESTION_ARROW,
			CT_CURSOR_RIGHT_BUTTON,
			CT_CURSOR_SIZENESW,
			CT_CURSOR_SIZENS,
			CT_CURSOR_SIZENWSE,
			CT_CURSOR_SIZEWE,
			CT_CURSOR_SIZING,
			CT_CURSOR_SPRAYCAN,
			CT_CURSOR_WAIT,
			CT_CURSOR_WATCH,
			CT_CURSOR_BLANK,
			CT_CURSOR_MAX_TYPE
		};
		static void SetCursorType(CursorType ct);
		static CursorType GetCursorType();

		static void SetCursorPos(const Float2 &pos);
		static Float2 GetCursorPos();

		static Sizef GetDeskScreenSize();

		enum ScreenDragType
		{
			SDT_NONE,
			SDT_LEFT,
			SDT_MIDDLE,
			SDT_RIGHT,
			SDT_MAX_TYPE
		};
		static void SetScreenDragType(ScreenDragType type);
		static ScreenDragType GetScreenDragType();
		static bool IsScreenDrag();

		//0left 1right 2middle
		static void SetMouseDown(int mouseType, bool down);
		static bool IsMouseDown(int mouseType);
		static bool IsMouseDownCall(int mouseType); 

		static Float2 GetCurCursorPos();
		static Float2 GetLastCursorPos();
		static Float2 GetMoveDelta();

	protected:
		static CursorType sCursorType;
		static ScreenDragType sScreenDragType;
		static Float2 sMoveDelta;
		static Float2 sCurCursorPos;
		static Float2 sLastCursorPos;
		static bool sIsMouseDown[3];

	public:
		virtual void OnEvent(Event *ent);

		// menu
	public:
		int AddMenuMain(const std::string &title);
		int AddMenuSpearater(int parentID);
		int AddMenuSub(int parentID, const std::string &title, 
			const std::string &callback);
		void OpenFileDlg(const std::string &ext, const std::string &callBack);
		void SaveFileDlg(const std::string &ext, const std::string &callBack);
	
	protected:
		int _GetNextMenuID();
		static int sNextMenuID;
	};

#include "PX2RenderWindow.inl"
	typedef Pointer0<RenderWindow> RenderWindowPtr;

}

#endif