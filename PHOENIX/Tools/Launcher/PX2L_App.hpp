// PX2L_App.hpp

#ifndef PX2L_APP_HPP
#define PX2L_APP_HPP

#include "PX2L_Pre.hpp"
#include "PX2EventHandler.hpp"

namespace Launcher
{

	class L_Frame;
	class PX2::Event;

	class L_App : public wxApp, public PX2::EventHandler
	{
	public:
		L_App();
		virtual ~L_App();

		static L_App *msL_App;

		virtual bool OnInit();
		virtual int OnExit();
		virtual void OnInitCmdLine(wxCmdLineParser& parser);
		virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

		virtual void OnEvent(PX2::Event *event);

		bool IsNeedToDokUpdater(const std::string &toDirPath) const;
		void _UpdateUpdater(const std::string &wwwAddr, const std::string &toDirPath);
		void _StartUpdater(const std::string &appNamePath, const std::string &toDoStr);

		void _ReStart();

	protected:
		L_Frame *CreateMainFrame(const std::string &name);
		void _LoadPlugin(const std::string &plgName);

	public:
		bool mIsDoStartUpdater;
		bool mIsUpdaterStart;
		bool mIsUpdaterOver;
	};

	DECLARE_APP(L_App)
}

#endif