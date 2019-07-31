// PX2LP_Manager.hpp

#ifndef PX2LP_MANAGER_HPP
#define PX2LP_MANAGER_HPP

#include "PX2LP_Pre.hpp"
#include "PX2EventHandler.hpp"
#include "PX2Singleton.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UITabFrame.hpp"
#include "PX2UIList.hpp"
#include "PX2NetEventBuffer.hpp"
#include "PX2HTTPServer.hpp"
#include "PX2LanguageManager.hpp"
#include "PX2LanguagePackage.hpp"
#include "PX2Application.hpp"
#include "PX2LP_ProjectItem.hpp"

namespace PX2
{

	class PX2LP_DLL_ITEM LP_Manager : public EventHandler, public Visitor,public Singleton<LP_Manager>
	{
	public:
		PX2_SINGLETION(LP_Manager);

	public:
		void Initlize();
		void Terminate();	
		void Update(double appSeconds, double elapsedSeconds);
		virtual void OnEvent(Event *event);
		virtual void Visit(Object *obj, int info);

		void ShowUpdating(bool show);

		void _ShowProjectCreate(bool isShow);
		void _ShowSignIn(bool isShow);
		UIEditBox *GetCreateProjectUIEditBox();
		UIEditBox *GetCreateProjectWidthUIEditBox();
		UIEditBox *GetCreateProjectHeightUIEditBox();
		UICheckButton *GetCreateProjectLandscapeButton();

	public:
		UIFrame *AddTabFrame(const std::string &name, const std::string &title);
		UIFrame *CreateManyKitFrame();
		UIFrame *CreateEngineFrame();
		void _RefreshProjects();
		int _GetProjType(const std::string &projName);
		void _SimuApp(const std::string &name);
		void _OpenEditor();
		UIItem *_AddProjectItem(const std::string &name, int id, bool isLocalExist,
			LP_ProjectItem *projItem);
		void _RefreshAddresses();
		void _OnGenProjInfo();
		void _OnSyn();
		void _OnLoad();
		void _OnClose();
		void _OnSend();
		void _CreateNewProjectFrame();
		void _CreateLoginFrame();
		UIFrame *_CreateZeroneRobotFrame();
		UIFrame *CreateCodePlayFrame();
		UIFrame *CreateToy();

		UIFramePtr mUIMainFrame;
		UIFramePtr mUpdaterFrame;

		UIFramePtr mFrameTop;
		UITabFramePtr mTableFrame;
		UIComboBoxPtr mLanguageBox;
		UIButtonPtr mLoginButton;
		UIButtonPtr mRegistButton;
		UIButtonPtr mUserButton;
		UIListPtr mUserButtonListFrame;

		UIFramePtr mLoginInFrame;

		UIFramePtr mManyKitFrame;
		UIFramePtr mEngineFrame;
		UIFramePtr mCreateProjectBackground;
		UIFramePtr mCreateProjectUiFrame;
		UIFramePtr mProjectFrame;
		UIButtonPtr mGitHubBut;
		UIEditBoxPtr mEditBoxVersion;
		UICheckButtonPtr mCheckCMDStart;
		UIListPtr mProjectList;
		UIListPtr mPluginList;		
		UIFramePtr mServerFrame;
		UIEditBoxPtr mDeviceInput;
		UIListPtr mDeviceList;
		HTTPServerPtr mHttpServer;
		BufferEventQueue *mBEventQue;
		UIFramePtr mPromptUIFrame;
		UIEditBoxPtr mProjectName;
		bool mIsNewNewProject;

		UIFramePtr mZeroneRobotFrame;
		UIFramePtr mCodePlayFrame;

		CurlObjPtr mCurlObject;
	
	private:
		int _CurlLogin(const std::string &userName, const std::string &password);
		void _CurlLogout();

		int mSceneOrientation;
		UIEditBox *mCreateProEditbox;
		UIEditBox *mCreateProWidthEditbox;
		UIEditBox *mCreateProHeightEditbox;
		UICheckButton *mCreateProLandscapeCheckButton;
		UIEditBox *mEditBoxUserName;
		UIEditBox *mEditBoxPassword;
		int mProjWidth;
		int mProjHeight;
		bool mIsStartNoWindow;

		std::string mCfgUserName;
		std::string mCfgPassword;

	public_internal:
		Pointer0<Project> TheProject;

	public:
		void ClearProjects();
		bool IsHasProject(int id);
		bool AddProject(LP_ProjectItem *projItem);
		int GetNumProjects() const;
		LP_ProjectItem *GetProjectItem(int index);

	private:
		std::vector<LP_ProjectItemPtr> mProjects;

	public:
		void ClearProjects_Cloud();
		bool IsHasProject_Cloud(int id);
		bool AddProject_Cloud(LP_ProjectItem *projItem);
		int GetNumProjects_Cloud() const;
		LP_ProjectItem *GetProjectItem_Cloud(int index);

	private:
		std::vector<LP_ProjectItemPtr> mProjects_Cloud;
	};

}

#endif