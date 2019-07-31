// PX2Application.hpp

#ifndef PX2APPLICATION_HPP
#define PX2APPLICATION_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2Project.hpp"
#include "PX2LanguageManager.hpp"
#include "PX2Bluetooth.hpp"
#include "PX2HardCamera.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2DynLibManager.hpp"
#include "PX2PluginManager.hpp"
#include "PX2FunObject.hpp"
#include "PX2IMEDispatcher.hpp"
#include "PX2InputManager.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2FontManager.hpp"
#include "PX2AddDeleteManager.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2RedoUndo.hpp"
#include "PX2RUStateManager.hpp"
#include "PX2AccoutManager.hpp"
#include "PX2Project.hpp"
#include "PX2UIAuiManager.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2SoundSystem.hpp"
#include "PX2Creater.hpp"
#include "PX2Singleton.hpp"
#include "PX2RendererInput.hpp"
#include "PX2TimerManager.hpp"
#include "PX2LogicManager.hpp"
#include "PX2EngineCanvas.hpp"
#include "PX2UISizeExtendControl.hpp"
#include "PX2AppBoostInfo.hpp"
#include "PX2Wifi.hpp"
#include "PX2Serial.hpp"
#include "PX2GeneralServer.hpp"
#include "PX2GeneralClientConnector.hpp"
#include "PX2EngineServer.hpp"
#include "PX2EngineClientConnector.hpp"
#include "PX2UDPServer.hpp"
#include "PX2ApplicationUDPNetInfo.hpp"
#include "PX2STEAMEdu.hpp"

namespace PX2
{

	class Arduino;
	class VoiceSDK;
	class VoxelManager;
	class PhysicsWorld;
	class AppInitConfig;

	typedef void (*AppCmdCallback)(
		const std::string &cmd,
		const std::string &paramStr,
		const std::string &paramStr1,
		const std::string &paramStr2);

	class PX2_ENGINE_ITEM Application : public Singleton<Application>, public EventHandler
	{
	public:
		Application();
		virtual ~Application();

		enum PlatformType
		{
			PLT_WINDOWS,
			PLT_LINUX,
			PLT_ANDROID,
			PLT_IOS,
			PLT_MAX_TYPE
		};
		PlatformType GetPlatformType() const;

		// pre init
	public:
		void SetPt_Data(void *data);
		void *GetPt_Data();
		void SetPt_Data1(void *data);
		void *GetPt_Data1();
		void SetPt_Data2(void *data);
		void *GetPt_Data2();
		void SetPt_Size(const Sizef &size);
		const Sizef &GetPt_Size() const;

	protected:
		void *mPt_Data;
		void *mPt_Data1;
		void *mPt_Data2;
		Sizef mPt_Size;

		// init term
	public:
		bool Initlize(AppInitConfig *cfg);
		void InitlizeDefaultEngineCanvas(Canvas *parentCanvas);
		Renderer *InitlizeRenderer();
		Renderer *CreateRenderer(const std::string &name, void *winHandle,
			void *ptDataLinuxWindow, void *ptDataLinuxVisual,
			int width, int height, int numMultisamples);
		RendererInput *GetRendererInput(const std::string &name);
		Renderer *GetRenderer(const std::string &name);
		Canvas *GetEngineCanvas();

		void SetInEditor(bool isInEditor);
		bool IsInEditor() const;

		void SetQuit(bool quit);
		bool IsQuit() const;

		const std::string &GetHostName() const;
		int GetLocalAddressSize();
		IPAddress GetLocalAddress(int i);
		std::string GetLocalAddressStr(int i);

		EngineServer *GetEngineServer();
		EngineClientConnector *GetEngineClientConnector();

		UDPServer *CreateEngineUDPServer();
		UDPServer *GetEngineUDPServer();

		UDPServer *CreateEngineUDPServerEditor();
		UDPServer *GetEngineUDPServerEditor();

		GeneralServer *CreateGeneralServer(int port,
			int numMaxConnects, int numMaxMsgHandlers);
		bool ShutdownGeneralServer(GeneralServer *generalServer);
		GeneralClientConnector *CreateGetGeneralClientConnector(const std::string &name);
		GeneralClientConnector *GetGeneralClientConnector(const std::string &name);
		bool ShutdownGeneralClientConnector(const std::string &name);

		void WillEnterForeground(bool isFirstTime);
		void DidEnterBackground();

		bool Terminate();

		void ClearAppCmdCallback();
		bool IsHasAppCmdCallback(AppCmdCallback callback);
		bool AddAppCmdCallback(AppCmdCallback callback);
		bool RemoveAppCmdCallback(AppCmdCallback callback);
		void _CMDCallback(const std::string &cmdStr,
			const std::string &paramStr, const std::string &paramStr1,
			const std::string &paramStr2);

	private:
		void _UpdateGeneralServerConnectors(float elapseSeconds);
		void _RefreshLocalAddress();
		void _ShutdownClientConnectors();
		void _ShutdownGeneralServers();
	
		std::map<std::string, RendererInput *> mRendererInputMap;
		std::map<std::string, Renderer*> mRenderersMap;
		bool mIsInEditor;
		std::string mHostName;

		ThreadPtr mInputThread;
		DynLibManager *mDynLibMan;
		PluginManager *mPluginMan;
		TimerManager *mTimerMan;
		EventWorld *mEventWorld;
		LanguageManager *mLanguageMan;
		ScriptManager *mScriptMan;
		Bluetooth *mBluetooth;
		Wifi *mWifi;
		HardCamera *mHardCamera;
		GraphicsRoot *mRoot;
		InputManager *mInputMan;
		IMEDispatcher *mIMEDisp;
		ResourceManager *mResMan;
		FontManager *mFontMan;
		SoundSystem *mSoundSys;
		AddDeleteManager *mADMan;
		SelectionManager *mSelectionMan;
		URDoManager *mURDoMan;
		URStateManager *mURStateMan;
		FunObjectManager *mFunObjectManager;
		AccoutManager *mAccoutManager;
		UIAuiManager *mUIAuiManager;
		UISkinManager *mUISkinManager;
		LogicManager *mLogicManager;
		VoxelManager *mVoxelManager;
		Creater *mCreater;
		VoiceSDK *mVoiceSDK;
		STEAMEduManager *mSTEAMEduManager;

		EngineServerPtr mEngineServer;
		EngineClientConnectorPtr mEngineClient;
		UDPServerPtr mEngineUDPServer;
		UDPServerPtr mEngineDUPServerEditor;
		std::vector<GeneralServerPtr> mGeneralServers;
		std::map<std::string, GeneralClientConnectorPtr> mGeneralClientConnectors;

		bool mIsInBackground;
		bool mBeforeInBackgroundMusicEnable;
		bool mBeforeInBackgroundSoundEnable;

		bool mIsQuit;

		std::vector<IPAddress> mLocalAddresses;

		std::vector<AppCmdCallback> mAppCmdCallbacks;

		// Update
	public:
		void Update();
		void Update(float appSeconds, float elapsedSeconds);
		float GetElapsedTime();

	private:
		float _CalElapsedTime();
		double mAppTime;
		double mLastAppTime;
		double mElapsedTime;

		// screen adjust
	public:
		void SetScreenSize(const Sizef &screenSize);
		const Sizef &GetScreenSize() const;

	protected:
		Sizef mScreenSize;

		// boost
	public:
		bool LoadBoost(const std::string &filename);

		AppBoostInfo &GetBoostInfo();
		const Sizef &GetBoostSize() const;
		const std::string &GetBoostProjectName() const;
		AppBoostInfo::PlayLogicMode GetPlayLogicMode() const;
		std::string GetPlayLogicModeStr() const;
		bool IsShowInfo() const;

		void SetBoostProjectName(const std::string &boostProjectName);
		void SetBoostSize(const Sizef &size);
		void SetBoostSize(float width, float height);
		void SetPlayLogicMode(AppBoostInfo::PlayLogicMode mode);
		void SetShowInfo(bool show);

		bool WriteBoost();

		std::set<std::string> GetAllProjects();
		bool IsProjectUpdated(const std::string &name) const;

	protected:
		void _ReadInfosFromCnt(const std::string &cntStr,
			std::vector<std::string> &projList);
		void _WriteInfos(const std::string &path,
			const std::vector<std::string> &list);

		AppBoostInfo::PlayLogicMode _StrToPlayLogicMode(const std::string &str);

		AppBoostInfo mBoostInfo;
		AppBoostInfo mBoostInfoUpdate;

		// project
	public:
		void NewProject(const std::string &pathname,
			const std::string &projName, int so, int width, int height);
		bool LoadProject(const std::string &name, bool doUpdate=false);
		bool LoadProjectByPath(const std::string &pathname);
		bool SaveProject();
		bool SaveProjectAs(const std::string &pathname);
		void CloseProject();
		const std::string &GetProjectName() const;
		const std::string &GetProjectFilePath() const;

		std::string GetProjectVersionByPath(const std::string &projectName);

		void NewScene();
		bool LoadScene(const std::string &pathname);
		bool SaveScene(const std::string &pathname);
		bool SaveSceneAs(const std::string &pathname);
		void CloseScene();

		bool LoadUI(const std::string &pathname);
		void CloseUI();

		bool LoadPlugins(const std::vector<std::string> &plugins);
		void ClosePlugins(const std::vector<std::string> &plugins);

		void GenerateFileList(const std::string &parentDir, const std::string &dir,
			const std::string &versionText);

		void MakeAProject(const std::string &projName, Project::ScreenOrientation so,
			int width, int height);

	protected:
		std::string _CalSavePath(const std::string &pathname);
		bool _SaveSceneInternal(const std::string &pathname);
		std::string GetProjDataFolderPath(const std::string &projName);
		std::string GetDllFileName(const std::string &projName);
		void _ProcessReWrite(const std::string &projName);
		void _ProcessXMLNode(XMLNode node);

		std::string mProjectName;
		std::string mProjectFilePath;
		std::string mSceneFilePath;
		std::string mUIFilePath;
		std::string mBPFilePath;

		EngineCanvasPtr mEngineCanvas;

	public_internal:
		Pointer0<Project> TheProject;

		// play
	public:
		enum PlayType
		{
			PT_NONE,
			PT_SIMULATE,
			PT_PLAY,
			PT_MAX_TYPE
		};
		void Play(PlayType type);
		PlayType GetPlayType() const;

		void RunFile(const std::string &pathname);

	protected:
		PlayType mPlayType;

		// project tree menus
	public:
		void Menu_Clear();
		void Menu_AddSubItemCatalogue(
			const std::string &parentAllName,
			const std::string &name,
			const std::string &title);
		void Menu_AddItem(
			const std::string &parentAllName,
			const std::string &name,
			const std::string &title,
			const std::string &script);

		class MenuItem
		{
		public:
			MenuItem();
			~MenuItem();

			void Clear();

			enum Type
			{
				T_SUB,
				T_ITEM,
				T_MAX_TYPE
			};
			Type TheType;

			std::string AllName;

			std::string Name;
			std::string Title;
			std::string Script;

			MenuItem *GetMenuItem(const std::string &parentAllName);
			std::vector<Pointer0<MenuItem> > Items;
		};

		MenuItem *GetMenuItem();

	protected:
		Pointer0<MenuItem> mItem;

		// general editmenus
	public:
		void Menu_Main_AddMainItem(const std::string &name, const std::string &title);
		void Menu_Main_AddSubItem(const std::string &parentName, const std::string &name,
			const std::string &title);
		void Menu_Main_AddItem(const std::string &parentName, const std::string &name,
			const std::string &title, const std::string &script, const std::string &scriptParam="",
			const std::string &tag = "");
		void Menu_Main_AddItemSeparater(const std::string &parentName);

		void Menu_Edit_Begin(const std::string &whe, const std::string &name);
		void Menu_Edit_AddSubItem(const std::string &whe,
			const std::string &parentName, const std::string &name,
			const std::string &title);
		void Menu_Edit_AddItem(const std::string &whe,
			const std::string &parentName, const std::string &name,
			const std::string &title,
			const std::string &script,
			const std::string &scriptParam="",
			const std::string &tag="");
		void Menu_Edit_AddItemSeparater(const std::string &whe,
			const std::string &parentName);
		void Menu_Edit_EndPopUp(const std::string &whe,
			const APoint &pos); // x z

	public:
		RenderWindow *CreateUIWindow(RenderWindow *parent, const std::string &name,
			const std::string &title, const APoint &pos, const Sizef &size, bool isFloat);

		// Event
	public:
		virtual void OnEvent(Event *ent);

		// NetInfo
	public:
		UDPNetInfo *GetUDPNetInfo(const std::string &ip);
		bool AddUDPNetInfo(const std::string &ip, const std::string &name,
			UDPNetInfo::Type type = UDPNetInfo::T_DEVICE);
		int GetNumUDPNetInfo() const;
		UDPNetInfo *GetUDPNetInfo(int i);
		void ClearUDPNetInfo();

	protected:
		void _UpdateUDPNetInfos(float elapsedTime);

		std::vector<UDPNetInfoPtr> mUDPNetInfos;
		std::vector<UDPNetInfoPtr> mUDPNetInfosCamera;

		// configs
	public:
		void SetConfigName(const std::string &cfgName);
		const std::string &GetConfigName();

		void SetConfig(const std::string &name, const std::string &cfgStr);
		std::string GetConfig(const std::string &name);

	public_internal:
		static void _WriteConfigs(std::map<std::string, std::string> &cfgs,
			const std::string &cfgName);
		static std::string _GetWritePath(const std::string &cfgName);
		static void _CreateSaveConfigXML(std::map<std::string, std::string> &cfgs,
			const std::string &cfgName);
		static void _LoadConfigs(std::map<std::string, std::string> &cfgs,
			const std::string &cfgName);

		std::map<std::string, std::string> mCFGs;
		std::string mConfigName;
	};
#include "PX2Application.inl"

#define  PX2_APP Application::GetSingleton()

}

#endif
