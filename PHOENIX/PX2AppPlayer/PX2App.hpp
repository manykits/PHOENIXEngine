// PX2App.hpp

#ifndef PX2APP_HPP
#define PX2APP_HPP

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include <ShellAPI.h>
#elif defined(__LINUX__)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#endif

#include "PX2AppBase.hpp"

namespace PX2
{

#define PX2_DECLARE_APPLICATION(classname) \
	\
	static bool RegisterInitTerm (); \
	static bool AppInitialize (); \
	static bool AppTerminate ();

#define PX2_REGISTER_APPLICATION(classname) \
	static bool gsAppInitializeRegistered_##classname = \
	classname::RegisterInitTerm ();

#define PX2_IMPLEMENT_APPLICATION(classname) \
	\
	bool classname::RegisterInitTerm () \
	{ \
		AppBase::msAppInitlizeFun = &classname::AppInitialize; \
		AppBase::msAppTernamateFun = &classname::AppTerminate; \
		AppBase::msEntry = &App::Entry; \
		return true; \
	} \
	\
	bool classname::AppInitialize () \
	{ \
		msApplication = new classname(); \
		return true; \
	} \
	\
	bool classname::AppTerminate () \
	{ \
		delete (msApplication); \
		msApplication = 0; \
		return true; \
	}
	//----------------------------------------------------------------------------

	class App : public AppBase
	{
	public:
		App ();
		virtual ~App ();

		static int Entry (int numArguments, char** arguments);
		virtual int Main (int numArguments, char** arguments);
		virtual bool OnIdle ();

		virtual bool Initlize();
		virtual bool Terminate();

		virtual void OnEvent(Event *ent);
		virtual void SetTitle(const std::string &title);
		void MaxSizeWindow(bool maxSize);

#if defined(_WIN32) || defined(WIN32)
		void _CreateMyMenu();
		void _OnMenuCMD(int cmd);
		void _OnOpenFile(std::string fileExt, const std::string &callback);
		void _OnSaveFile(std::string fileExt, const std::string &callback);

		HMENU mhMenu;
		std::map<int, HMENU> mMenus;
		std::map<int, std::string> mMenuCallbacks;
		HWND mhWnd;

#elif defined (__LINUX__)
		PX2::KeyCode _ConvertKeyCode(unsigned char chKey);

		Display *mDisplay;
		Window mWindow;
		Atom mPX2DeleteWindow;
#endif
	};

}

#endif