// PX2AppBase.hpp

#ifndef PX2APPBASE_HPP
#define PX2APPBASE_HPP

#include "PX2AppFramePre.hpp"
#include "PX2EventHandler.hpp"

namespace PX2
{

	class AppBase : public EventHandler
	{
	protected:
		AppBase ();
	public:
		virtual ~AppBase ();

		// App
		static AppBase* msApplication;

		static bool IsInitlized ();
		virtual bool Initlize ();
		virtual bool Terminate ();

		virtual void SetTitle(const std::string &title);
		const std::string &GetTitle() const;
		std::string GetTitleProj(const std::string &projName);

		virtual void OnSize (int width, int height);
		virtual void WillEnterForeground (bool isFirstTime);
		virtual void DidEnterBackground ();
		bool IsIsInBackground () const;

		int GetXPosition() const;
		int GetYPosition() const;
		int GetWidth () const;
		int GetHeight() const;

		virtual void OnEvent(Event *ent);
		virtual bool OnIdle ();

		// Enteries
		typedef bool (*AppInitlizeFun)();
		static AppInitlizeFun msAppInitlizeFun;
		typedef bool (*AppTernamateFun)();
		static AppTernamateFun msAppTernamateFun;
		typedef int (*EntryPoint)(int numArguments, char** arguments);
		static EntryPoint msEntry;

		virtual int Main (int numArguments, char** arguments);

	protected:
		static bool msIsInitlized;
	
		// 渲染相关
		// 窗口参数
		std::string mWindowTitle;
		int mXPosition, mYPosition, mWidth, mHeight;
		bool mAllowResize;

		std::string mCmdProjectName;
		int mCMDWidth;
		int mCMDHeight;
		bool mIsUseWindows;

		int mNumArguments;
		char** mArguments;
	};

}

#endif