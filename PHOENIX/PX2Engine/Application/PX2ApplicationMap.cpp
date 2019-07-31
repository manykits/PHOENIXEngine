// PX2ApplicationMap.cpp

#include "PX2Application.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2PluginManager.hpp"
#include "PX2RedoUndo.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2SelectionManager.hpp"
#include "PX2SoundSystem.hpp"
#include "PX2Log.hpp"
#include "PX2Dir.hpp"
#include "PX2MD5.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void Application::NewProject(const std::string &pathname,
	const std::string &projName, int so, int width, int height)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return;

	CloseProject();

	TheProject = new0 Project();
	TheProject->SetName(projName);
	TheProject->SetScreenOrientation((Project::ScreenOrientation)so);
	TheProject->SetSize((float)width, (float)height);
	mProjectFilePath = pathname;
	mProjectName = projName;

	PX2_SC_LUA->SetUserTypePointer("PX2_PROJ", "Project", (Project*)TheProject);

	Event *ent = PX2_CREATEEVENTEX(ProjectES, NewProject);
	PX2_EW.BroadcastingLocalEvent(ent);

	Event *entUI = PX2_CREATEEVENTEX(ProjectES, NewUI);
	entUI->SetData<UI*>(TheProject->GetUI());
	PX2_EW.BroadcastingLocalEvent(entUI);

	Event *entLogic = PX2_CREATEEVENTEX(ProjectES, NewLogic);
	entLogic->SetData<Logic*>(TheProject->GetLogic());
	PX2_EW.BroadcastingLocalEvent(entLogic);
}
//----------------------------------------------------------------------------
void _ReWriteProcessXMLNode(XMLNode node)
{
	std::string writeablePath = PX2_RM.GetWriteablePath();
	const std::string &updateWritePath = PX2_RM.GetDataUpdateWritePath();

	XMLNode childNode = node.IterateChild();
	while (!childNode.IsNull())
	{
		std::string name = childNode.GetName();
		std::string filename = childNode.AttributeToString("filename");
		std::string md5Str = childNode.AttributeToString("md5");
		std::string strData = "Data/";
		std::string subFilename = filename.substr(strData.length(), filename.length() - strData.length());

		if ("directory" == name)
		{
			PX2_RM.CreateFloder(writeablePath + updateWritePath, subFilename);
			_ReWriteProcessXMLNode(childNode);
		}
		else if ("file" == name)
		{
			std::string outPath;
			std::string outFilename;
			StringHelp::SplitFilename(subFilename, outPath, outFilename);

			PX2_RM.CreateFloder(writeablePath + updateWritePath, outPath);

			std::string dstPath = updateWritePath + subFilename;
			PX2_RM.ReWriteFileToWriteablePath(filename, dstPath);
		}

		childNode = node.IterateChild(childNode);
	}
}
//----------------------------------------------------------------------------
void Application::_ProcessReWrite(const std::string &projName)
{
	std::string writeablePath = PX2_RM.GetWriteablePath();
	const std::string &updateWritePath = PX2_RM.GetDataUpdateWritePath();

	std::string writeDataPath = writeablePath + "DataUpdate/";
	std::string projNamePath = projName + "/";

	PX2_RM.CreateFloder(writeDataPath, projNamePath);

	std::string filesPath = "Data/" + projName + "/" + "filelist.xml";
	if (PX2_RM.IsFileFloderExist(filesPath))
	{
		PX2_RM.ReWriteFileToWriteablePath(filesPath,
			updateWritePath + projName + "/" + "filelist.xml");
	}

	int bufSize = 0;
	char *buf = 0;
	if (PX2_RM.LoadBuffer(filesPath, bufSize, buf))
	{
		XMLData data;
		if (data.LoadBuffer(buf, bufSize))
		{
			XMLNode rootNode = data.GetRootNode();
			_ReWriteProcessXMLNode(rootNode);
		}
	}
}
//----------------------------------------------------------------------------
bool Application::LoadProject(const std::string &name, bool doUpdate)
{
	PX2_LOG_INFO("LoadProject %s", name.c_str());

	if (name.empty())
		return false;

	PX2_RM.Clear();

	std::string writeablePath = PX2_RM.GetWriteablePath();
	std::string writeDataPath = writeablePath + "DataUpdate/";
	std::string projWriteDataPath = writeDataPath + name + "/";

	if (mBoostInfo.IsDataReWriteToDataUpdate)
	{
		if (!PX2_RM.IsFileFloderExist(projWriteDataPath))
		{
			PX2_RM.CreateFloder(writeablePath, "DataUpdate/");
			_ProcessReWrite(name);

			mBoostInfoUpdate.AddProject(name);
			_WriteInfos(writeDataPath + "project.list", mBoostInfoUpdate.Projects);
			_WriteInfos(writeDataPath + "plugin.list", mBoostInfoUpdate.Plugins);
		}
	}

	// write data path
	std::string writeDataFilelistPath = projWriteDataPath + "filelist.xml";
	if (PX2_RM.IsFileFloderExist(writeDataFilelistPath))
	{
		if (!PX2_RM.LoadFileTableXML(PX2_RM.GetDataUpdateFileTable(), writeDataFilelistPath))
		{
			PX2_LOG_INFO("LoadFileTableXML %s failed", writeDataFilelistPath.c_str());

			PX2_RM.LoadFileTableXML(PX2_RM.GetDataFiletable(), "Data/" + name + "/filelist.xml");
		}
		else
		{
			PX2_LOG_INFO("LoadFileTableXML %s suc", writeDataFilelistPath.c_str());
		}
	}
	else
	{
		std::string fileListName = "Data/" + name + "/filelist.xml";
#if defined (_WIN32) || defined (WIN32)
		if (PX2_RM.IsFileFloderExist(fileListName))
		{
#endif
			PX2_RM.LoadFileTableXML(PX2_RM.GetDataFiletable(), fileListName);
#if defined (_WIN32) || defined (WIN32)
		}
#endif
	}

	if (doUpdate)
	{
		bool addRet = mBoostInfoUpdate.AddProject(name);
		if (addRet)
		{
			if (!PX2_RM.IsFileFloderExist(projWriteDataPath))
			{
				PX2_RM.CreateFloder(writeablePath, "DataUpdate/");
			}

			_WriteInfos(writeDataPath + "project.list", mBoostInfoUpdate.Projects);

			Event *ent = ProjectES_Internal::CreateEventX(ProjectES_Internal::AddUpdateProject);
			PX2_EW.BroadcastingLocalEvent(ent);
		}

		std::string addrPath = "ftp://127.0.0.1/";
		const std::string &resUpdateAddr = PX2_RM.GetResourceUpdateAddr();
		if (!resUpdateAddr.empty())
			addrPath = resUpdateAddr;

		int typeFTPHTTP = 0;
		auto itFTP = addrPath.find("ftp");
		auto itHTTP = addrPath.find("http");
		if (itFTP != std::string::npos)
		{
			typeFTPHTTP = 1;
		}		
		if (itHTTP != std::string::npos)
		{
			typeFTPHTTP = 2;
		}
		PX2_RM.DoResourceUpdateStuffs(addrPath, name, typeFTPHTTP);
	}

	mProjectName = name;
	std::string path = "Data/" + name + "/" + "project.px2proj";
	return PX2_APP.LoadProjectByPath(path);
}
//----------------------------------------------------------------------------
bool Application::LoadProjectByPath(const std::string &pathname)
{
	PX2_LOG_INFO("LoadProjectByPath %s", pathname.c_str());

	bool canDoChange = (Application::PT_NONE == GetPlayType());
	if (!canDoChange) return false;

	CloseProject();
	
	// load

	bool loadResult = false;
	TheProject = new0 Project();

	if (TheProject->Load(pathname))
	{
		PX2_SC_LUA->SetUserTypePointer("PX2_PROJ", "Project", Project::GetSingletonPtr());

		const std::vector<std::string> &plugins = TheProject->GetPlugins();
		if (!plugins.empty())
		{
			LoadPlugins(plugins);
		}

		Event *ent = PX2_CREATEEVENTEX(ProjectES, LoadedProject);
		PX2_EW.BroadcastingLocalEvent(ent);

		std::string sceneFilename = TheProject->GetSceneFilename();
		if (!sceneFilename.empty())
		{
			LoadScene(sceneFilename);
		}

		std::string uiFilename = TheProject->GetUIFilename();
		if (!uiFilename.empty())
		{
			LoadUI(uiFilename);
		}

		mProjectFilePath = pathname;
		mProjectName = TheProject->GetName();

		loadResult = true;
	}
	else
	{
		Project::Destory();

		loadResult = false;
	}

	std::string outPath;
	std::string outBaseName;
	std::string outExt;
	StringHelp::SplitFullFilename(pathname, outPath, outBaseName, outExt);

	int dataLength = std::string("Data/").length();
	std::string projName = outPath.substr(dataLength, outPath.length()- dataLength-1);
	mProjectName = projName;

#if defined (WIN32) || defined (_WIN32)

	std::string projDllFolder = GetProjDataFolderPath(projName);
	std::string projDllFilename = GetDllFileName(projName);
	std::string dllFullpathFilename = projDllFolder + projDllFilename;

	if (PX2_RM.IsFileFloderExist(dllFullpathFilename))
	{
		PX2_LOG_INFO("Begin load project dll: %s", dllFullpathFilename.c_str());

		PX2_PLUGINMAN.Load(dllFullpathFilename);

		PX2_LOG_INFO("End load project dll: %s", dllFullpathFilename.c_str());
	}

#elif defined (__LINUX__)
	std::string projDllFolder = GetProjDataFolderPath(projName);
	std::string projDllFilename = GetDllFileName(projName);
	std::string dllFullpathFilename = projDllFolder + projDllFilename;

	if (PX2_RM.IsFileFloderExist(dllFullpathFilename))
	{
		PX2_LOG_INFO("Begin load project so: %s", dllFullpathFilename.c_str());

		PX2_PLUGINMAN.Load(dllFullpathFilename);

		PX2_LOG_INFO("End load project so: %s", dllFullpathFilename.c_str());
	}

#elif defined (__ANDROID__)
	//std::string soFilename = "lib" + projName + "SO.so";
	//std::string originSOFilename = "Data/" + soFilename;
	//const std::string writePath = PX2_RM.GetWriteablePath();
	//std::string writePathSOFilename = writePath + soFilename;

	//char *soBuf = 0;
	//int soSize = 0;
	//if (PX2_RM.LoadBuffer(originSOFilename, soSize, soBuf))
	//{
	//	bool ret = FileIO::Save(writePathSOFilename, true, soSize, soBuf);
	//	PX2_LOG_INFO("FileIO::Save ret %s", ret ? "true" : "false");
	//	PX2_LOG_INFO("SoSize:%d", soSize);

	//	PX2_LOG_INFO("Begin load project so: %s", writePathSOFilename.c_str());

	//	PX2_PLUGINMAN.Load(writePathSOFilename);

	//	PX2_LOG_INFO("End load project so: %s", writePathSOFilename.c_str());
	//}
#endif

	std::string callFilenameLua = "Data/" + projName + "/scripts/lua/editor/editor.lua";
	PX2_SC_LUA->CallFileFunction(callFilenameLua.c_str(), "editorstart");

	PX2_LOG_INFO("Loaded Project: %s", projName.c_str());

	StringTokenizer stk(mProjectName, "_");
	if (stk.Count() > 1)
	{
		std::string projName = stk[0];
		std::string idStr = stk[1];
		PX2_RM.SetProjectIDStr(projName, idStr);
	}

	return loadResult;
}
//----------------------------------------------------------------------------
std::string Application::GetProjDataFolderPath(const std::string &projName)
{
	std::string folder;

#if defined(_WIN32) || defined(WIN32) 
	folder = "Data/" + projName + "/";
#elif defined (__LINUX__)
	folder = "Data/" + projName + "/";
#else
	folder = PX2_RM.GetWriteablePath();
#endif

	return folder;
}
//----------------------------------------------------------------------------
std::string Application::GetDllFileName(const std::string &projName)
{
	std::string projDllName;

#if defined(_WIN32) || defined(WIN32) 
	std::string tag = "";

#ifdef PX2_USE_OPENGL
	tag += "OpenGL";
#endif

#if defined(_WIN64) || defined(WIN64)
	tag += "64";
#endif

#ifdef _DEBUG
	tag += "D";
#endif

	projDllName = projName + tag + ".dll";

#elif defined(__ANDROID__)
	projDllName = "lib" + projName + ".so";

#elif defined (__LINUX__)

#ifdef PX2_USE_OPENGLES
	projDllName = "libARM" + projName + ".so";
#else
	projDllName = "lib" + projName + ".so";
#endif

#endif

	return projDllName;
}
//----------------------------------------------------------------------------
bool Application::SaveProject()
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		if (canDoChange)
		{
			if (proj->Save(mProjectFilePath))
			{
				const std::string &projName = proj->GetName();
				const ResourceVersion &resVersion = proj->GetResourceVersion();
				std::string strVersion = resVersion.GetAsString();

				GenerateFileList("Data/", projName, strVersion);

				Event *ent = PX2_CREATEEVENTEX(ProjectES, SavedProject);
				PX2_EW.BroadcastingLocalEvent(ent);

				return true;
			}
		}
		else
		{
			proj->SaveConfig(mProjectFilePath);
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::SaveProjectAs(const std::string &pathname)
{
	bool canDoChange = (Application::PT_NONE == GetPlayType());

	Project *proj = Project::GetSingletonPtr();
	if (proj)
	{
		if (canDoChange)
		{
			if (proj->Save(pathname))
			{
				const std::string &projName = proj->GetName();
				const ResourceVersion &resVersion = proj->GetResourceVersion();
				std::string strVersion = resVersion.GetAsString();

				GenerateFileList("Data/", projName, strVersion);

				Event *ent = PX2_CREATEEVENTEX(ProjectES, SavedProject);
				PX2_EW.BroadcastingLocalEvent(ent);

				return true;
			}
		}
		else
		{
			proj->SaveConfig(pathname);
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void Application::CloseProject()
{
	PX2_LOGICM.Clear();
	PX2_SELECTM.Clear();
	URDoManager::GetSingleton().Clear();
	URStateManager::GetSingleton().Clear();
	PX2_RM.ClearRes(mProjectFilePath);
	PX2_RM.Clear();
	PX2_BLUETOOTH.Clear();

	SoundSystem *ss = SoundSystem::GetSingletonPtr();
	if (ss)
		ss->PlayMusic(0, 0, true, 0.0f);

	Project *oldProj = Project::GetSingletonPtr();
	if (!oldProj) return;

	std::string projName = PX2_PROJ.GetName();
	std::string projDllFolder = GetProjDataFolderPath(projName);
	std::string projDllFilename = GetDllFileName(projName);
	std::string dllFullpathFilename = projDllFolder + projDllFilename;
	std::vector<std::string> plugins = PX2_PROJ.GetPlugins();

	Play(Application::PT_NONE);

	CloseScene();
	CloseUI();

	Event *ent = PX2_CREATEEVENTEX(ProjectES, CloseProject);
	PX2_EW.BroadcastingLocalEvent(ent);

	ClosePlugins(plugins);

	std::string debugTag = "";
#ifdef _DEBUG
	debugTag = "D";
#endif

#if defined (WIN32) || defined (_WIN32) || defined (__LINUX__)
	if (PX2_RM.IsFileFloderExist(dllFullpathFilename))
	{
		PX2_PLUGINMAN.Unload(dllFullpathFilename);
	}
#endif

	Project::Destory();
	mProjectFilePath.clear();
	PX2_RM.SetProjectIDStr("", "");
}
//----------------------------------------------------------------------------
std::string Application::GetProjectVersionByPath(const std::string &pathName)
{
	std::string verStr;
	std::string filename = pathName;

	if (PX2_RM.IsFileFloderExist(filename))
	{
		std::string bufStr;
		if (PX2_RM.LoadBuffer(filename, bufStr))
		{
			XMLData xmlData;
			bool loadedBuffer = xmlData.LoadBuffer(bufStr.c_str(),
				(int)bufStr.length());
			if (loadedBuffer)
			{
				XMLNode rootNode = xmlData.GetRootNode();
				verStr = rootNode.AttributeToString("version");
			}

			PX2_RM.ClearRes(filename);
		}
	}

	return verStr;
}
//----------------------------------------------------------------------------
void Application::NewScene()
{
	if (IsInEditor())
	{
		bool canDoChange = (Application::PT_NONE == GetPlayType());
		if (!canDoChange) return;
	}

	CloseScene();

	Scene *scene = new0 Scene();
	PX2_PROJ.SetScene(scene);
	PX2_PROJ.SetSceneFilename("");
}
//----------------------------------------------------------------------------
bool Application::LoadScene(const std::string &pathname)
{
	if (IsInEditor())
	{
		bool canDoChange = (Application::PT_NONE == GetPlayType());
		if (!canDoChange) return false;
	}

	if (!Project::GetSingletonPtr()) return false;

	CloseScene();

	Scene *newscene = DynamicCast<Scene>(PX2_RM.BlockLoad(pathname));
	if (newscene)
	{
		mSceneFilePath = pathname;

		Project::GetSingleton().SetScene(newscene);
		Project::GetSingleton().SetSceneFilename(pathname);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::SaveScene(const std::string &pathname)
{
	if (IsInEditor())
	{
		bool canDoChange = (Application::PT_NONE == GetPlayType());
		if (!canDoChange) return false;
	}

	std::string toPath = _CalSavePath(pathname);

	if (_SaveSceneInternal(toPath))
	{
		PX2_PROJ.SetSceneFilename(toPath);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::SaveSceneAs(const std::string &pathname)
{
	if (IsInEditor())
	{
		bool canDoChange = (Application::PT_NONE == GetPlayType());
		if (!canDoChange) return false;
	}

	std::string toPath = _CalSavePath(pathname);

	return _SaveSceneInternal(toPath);
}
//----------------------------------------------------------------------------
void Application::CloseScene()
{
	if (IsInEditor())
	{
		bool canDoChange = (Application::PT_NONE == GetPlayType());
		if (!canDoChange) return;
	}

	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	Scene *scene = PX2_PROJ.GetScene();
	if (scene)
	{
		PX2_RM.ClearRes(mSceneFilePath);
		PX2_PROJ.SetScene(0);
		PX2_PROJ.SetSceneFilename("");
		mSceneFilePath.clear();
	}
}
//----------------------------------------------------------------------------
bool Application::LoadUI(const std::string &pathname)
{
	if (IsInEditor())
	{
		bool canDoChange = (Application::PT_NONE == GetPlayType());
		if (!canDoChange) return false;
	}

	ObjectPtr uiObj = PX2_RM.BlockLoad(pathname);
	UI *ui0 = DynamicCast<UI>(uiObj);
	if (ui0)
	{
		mUIFilePath = pathname;
		Project::GetSingleton().SetUI(ui0);
	}
	else
	{
		mUIFilePath = pathname;
		UIPtr ui1 = new0 UI();
		Project::GetSingleton().SetUI(ui1);
	}

	return false;
}
//----------------------------------------------------------------------------
void Application::CloseUI()
{
	if (IsInEditor())
	{
		bool canDoChange = (Application::PT_NONE == GetPlayType());
		if (!canDoChange) return;
	}

	Project *proj = Project::GetSingletonPtr();
	if (!proj) return;

	EngineUICanvas *uiCanvas = EngineUICanvas::GetSingletonPtr();
	if (uiCanvas)
		uiCanvas->Clear();

	PX2_RM.ClearRes(mUIFilePath);
	PX2_PROJ.SetUI(0);
	mUIFilePath.clear();
}
//----------------------------------------------------------------------------
bool Application::LoadPlugins(const std::vector<std::string> &plugins)
{
	for (int i = 0; i < (int)plugins.size(); i++)
	{
		const std::string &pluginName = plugins[i];

#if defined (WIN32) || defined (_WIN32) || defined (__LINUX__)
		if (!pluginName.empty())
		{
			std::string path = "PluginsCommon/" + pluginName + "/" +
				GetDllFileName(pluginName);
			if (PX2_RM.IsFileFloderExist(path))
				PX2_PLUGINMAN.Load(path);
		}
#elif defined (__ANDROID__)
#endif
	}

	return true;
}
//----------------------------------------------------------------------------
void Application::ClosePlugins(const std::vector<std::string> &plugins)
{
	for (int i = 0; i < (int)plugins.size(); i++)
	{
		const std::string &pluginName = plugins[i];

#if defined (WIN32) || defined (_WIN32) || defined (__LINUX__)
		if (!pluginName.empty())
		{
			std::string path = "PluginsCommon/" + pluginName + "/" +
				GetDllFileName(pluginName);
			if (PX2_RM.IsFileFloderExist(path))
				PX2_PLUGINMAN.Unload(path);
		}
#elif defined (__ANDROID__)
#endif
	}
}
//----------------------------------------------------------------------------
std::string Application::_CalSavePath(const std::string &pathname)
{
	std::string pathStand = StringHelp::StandardisePath(pathname);
	pathStand = pathStand.substr(0, pathStand.length() - 1);

	std::string toPath = pathStand;
	size_t begin = toPath.find("Data/");
	if (begin != std::string::npos)
	{
		size_t length = toPath.length() - begin;
		toPath = toPath.substr(begin, length);
	}

	return toPath;
}
//----------------------------------------------------------------------------
std::string _GetMD5(const std::string &strBuffer)
{
	char buffer[17], out[33];
	memset(buffer, 0, sizeof(buffer));
	memset(out, 0, sizeof(out));

	Md5HashBuffer(buffer, strBuffer.c_str(), (int)strBuffer.length());
	Md5HexString(buffer, out);

	return std::string(out);
}
//----------------------------------------------------------------------------
void _RefreshDir(XMLNode node, const std::string &pathName)
{
	std::string eachFilename;
	DirP d;
	if (d.Open(pathName))
	{
		if (!d.HasFiles() && !d.HasSubDirs())
			return;

		int flags = DirP::DIR_DIRS | DirP::DIR_FILES;

		if (d.GetFirst(&eachFilename, "", flags))
		{
			do
			{
				if ((eachFilename != ".") && (eachFilename != ".."))
				{
					_RefreshDir(node, pathName + "/" + eachFilename);
				}

			} while (d.GetNext(&eachFilename));
		}
	}
	else
	{
		if (pathName.find("filelist") == std::string::npos)
		{
			XMLNode childNode = node.NewChild("file");
			childNode.SetAttributeString("filename", pathName);

			char *buffer = 0;
			int bufferSize = 0;
			if (FileIO::Load(pathName, true, bufferSize, buffer))
			{
				std::string strBuf = _GetMD5(std::string(buffer, bufferSize));
				delete1(buffer);
				bufferSize = 0;

				childNode.SetAttributeString("md5", strBuf);
			}
			else
			{
				childNode.SetAttributeString("md5", "");
			}
		}
	}
}
//----------------------------------------------------------------------------
void Application::GenerateFileList(const std::string &parentDir, 
	const std::string &projName, const std::string &versionText)
{
	std::string projDataPath = parentDir + projName;

	XMLData data;
	data.Create();

	XMLNode rootNode = data.NewChild("filelist");
	rootNode.SetAttributeString("version", versionText);
	_RefreshDir(rootNode, projDataPath);

	std::string savePath = projDataPath + "/filelist.xml";
	data.SaveFile(savePath);
}
//----------------------------------------------------------------------------
void _CreateScriptFile(const std::string &pathName, const std::string &subDir,
	const std::string &scFileName)
{
	std::string scriptPath = "Data/" + pathName + subDir + scFileName;
	std::ofstream outputFile;
	outputFile.open(scriptPath.c_str());

	std::string outBaseName;
	std::string outExt;
	StringHelp::SplitBaseFilename(scFileName, outBaseName, outExt);


	std::string noteStr = "--";
	if ("lua" == outExt)
		noteStr = "--";
	else if ("as" == outExt)
		noteStr == "//";

	std::string scriptStart;
	scriptStart += noteStr + scFileName;
	outputFile << scriptStart << std::endl << std::endl;

	std::string scriptContent;
	if ("lua" == outExt)
	{
		if ("play" == outBaseName)
		{
			scriptContent += "function engine_project_preplay() \n";
			scriptContent += "end\n";
			scriptContent += "\n";
			scriptContent += "function engine_project_play() \n";
			scriptContent += "end\n";
			scriptContent += "\n";
			scriptContent += "function engine_project_update(appseconds, elapsedseconds) \n";
			scriptContent += "end\n";
			scriptContent += "\n";
			scriptContent += "function engine_project_cmd(cmd, param0, param1, param2) \n";
			scriptContent += "end\n";
			scriptContent += "\n";
		}
		else if ("stop" == outBaseName)
		{
			scriptContent += "function engine_project_prestop() \n";
			scriptContent += "end\n";
			scriptContent += "\n";
			scriptContent += "function engine_project_stop() \n";
			scriptContent += "end\n";
			scriptContent += "\n";
		}
	}
	outputFile << scriptContent;

	outputFile.close();
}
//----------------------------------------------------------------------------
void Application::MakeAProject(const std::string &projName, 
	Project::ScreenOrientation so, int width, int height)
{
	std::string pathName = projName + "/";
	PX2_RM.CreateFloder("Data/", pathName);
	PX2_RM.CreateFloder("Data/", pathName + "images/");
	PX2_RM.CreateFloder("Data/", pathName + "models/");
	PX2_RM.CreateFloder("Data/", pathName + "scenes/");
	PX2_RM.CreateFloder("Data/", pathName + "scripts/");
	PX2_RM.CreateFloder("Data/", pathName + "scripts/lua/");
	PX2_RM.CreateFloder("Data/", pathName + "scripts/lua/editor/");

	_CreateScriptFile(pathName, "scripts/lua/", "play.lua");
	_CreateScriptFile(pathName, "scripts/lua/", "stop.lua");
	
	_CreateScriptFile(pathName, "scripts/lua/editor/", "editor.lua");

	std::string pathword = "Data/" + pathName + "/project.px2proj";
	std::string pathward = "Data/" + pathName + "/project_ui.px2proj";
	PX2_APP.NewProject(pathword, projName, so, width, height);
	PX2_APP.SaveProject();
	PX2_APP.CloseProject();
}
//----------------------------------------------------------------------------
bool Application::_SaveSceneInternal(const std::string &pathname)
{
	Scene *scene = PX2_PROJ.GetScene();
	if (!scene) return false;

	scene->SetResourcePath(pathname);

	OutStream outStream;

	outStream.Insert(scene);

	if (outStream.Save(pathname))
	{
		Event *ent = PX2_CREATEEVENTEX(ProjectES, SavedScene);
		PX2_EW.BroadcastingLocalEvent(ent);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------