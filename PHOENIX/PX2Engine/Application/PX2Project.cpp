// PX2Project.cpp

#include "PX2Project.hpp"
#include "PX2XMLData.hpp"
#include "PX2StringHelp.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2Renderer.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2Application.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2Application.hpp"
#if defined __ANDROID__
#include "AppPlayJNI.hpp"
#endif
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, Project);

//----------------------------------------------------------------------------
Project::Project() :
mEdit_UICameraPercent(1.0f),
mScreenOrientation(SO_LANDSCAPE),
mIsPublish(false),
mIsSizeSameWithScreen(false)
{
	mUI = new0 UI();

	ComeInEventWorld();
}
//----------------------------------------------------------------------------
Project::~Project()
{
	GoOutEventWorld();
}
//----------------------------------------------------------------------------
void Project::SetName(const std::string &name)
{
	Object::SetName(name);
}
//----------------------------------------------------------------------------
void Project::SetVersion(const ResourceVersion &version)
{
	mResourceVersion = version;
}
//----------------------------------------------------------------------------
const ResourceVersion &Project::GetResourceVersion() const
{
	return mResourceVersion;
}
//----------------------------------------------------------------------------
void Project::Destory()
{
	if (PX2_APP.TheProject)
	{
		PX2_APP.TheProject = 0;
		Project::Set(0);
	}
}
//----------------------------------------------------------------------------
void Project::SetScreenOrientation(ScreenOrientation so)
{
	mScreenOrientation = so;

	if (SO_LANDSCAPE == mScreenOrientation)
	{
#if defined __ANDROID__
		SetScreenOrientate(1);
#endif
	}
	else
	{
#if defined __ANDROID__
		SetScreenOrientate(0);
#endif
	}
}
//----------------------------------------------------------------------------
void Project::SetSizeSameWithScreen(bool sizeWithScreen)
{
	mIsSizeSameWithScreen = sizeWithScreen;

	Event *ent = ProjectES_Internal::CreateEventX(
		ProjectES_Internal::ProjectSizeSameWithScreenChanged);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
bool Project::IsSizeSameWithScreen() const
{
	return mIsSizeSameWithScreen;
}
//----------------------------------------------------------------------------
Project::ScreenOrientation Project::_FromSOStr(const std::string &str)
{
	if ("landscape" == str) return SO_LANDSCAPE;
	else if ("portrait" == str) return SO_PORTRAIT;

	return SO_PORTRAIT;
}
//----------------------------------------------------------------------------
std::string Project::_ToSOStr(Project::ScreenOrientation so)
{
	if (SO_LANDSCAPE == so) return "landscape";
	else if (SO_PORTRAIT == so) return "portrait";

	return "portrait";
}
//----------------------------------------------------------------------------
bool Project::Save(const std::string &filename)
{
	if (!SaveConfig(filename))
		return false;

	std::string outPath;
	std::string outBaseName;
	std::string outExt;
	StringHelp::SplitFullFilename(filename, outPath, outBaseName, outExt);

	if (mUI)
	{
		std::string outName = outPath + outBaseName + "_ui.px2obj";

		OutStream output;
		output.Insert(mUI);
		output.Save(outName);
	}

	return true;
}
//----------------------------------------------------------------------------
bool Project::SaveConfig(const std::string &filename)
{
	// xml
	XMLData data;
	data.Create();

	XMLNode projNode = data.NewChild("project");

	// general
	XMLNode generalNode = projNode.NewChild("general");
	generalNode.SetAttributeString("name", GetName().c_str());
	generalNode.SetAttributeString("screenorientation", _ToSOStr(mScreenOrientation));
	generalNode.SetAttributeInt("width", (int)mSize.Width);
	generalNode.SetAttributeInt("height", (int)mSize.Height);

	// scene
	XMLNode sceneNode = projNode.NewChild("scene");
	sceneNode.SetAttributeString("filename", mSceneFilename.c_str());

	// render setting
	XMLNode renderNode = projNode.NewChild("render_setting");

	// language
	XMLNode languageNode = projNode.NewChild("language");

	// publish
	XMLNode publish = projNode.NewChild("publish");
	publish.SetAttributeBool("ispublish", IsPublish());

	// setting
	XMLNode settingNode = projNode.NewChild("edit_setting");
	settingNode.SetAttributeFloat("uicamerapercent", mEdit_UICameraPercent);

	// plugins
	XMLNode nodePlugins = projNode.NewChild("plugins");
	for (int i = 0; i < (int)mPlugins.size(); i++)
	{
		const std::string &pluginName = mPlugins[i];
		XMLNode nodePlugin = nodePlugins.NewChild("var");
		nodePlugin.SetAttributeString("name", pluginName);
	}

	if (data.SaveFile(filename))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Project::Load(const std::string &filename)
{
#if defined (_WIN32) || defined(WIN32)
	if (!PX2_RM.IsFileFloderExist(filename))
		return false;
#endif

	std::string name;
	int height = 0;
	std::string sceneFilename;
	int width = 0;
	std::string uiFilename;
	std::string languageFilename;

	int id = 0;
	char *buffer = 0;
	int bufferSize = 0;
	if (PX2_RM.LoadBuffer(filename, bufferSize, buffer))
	{
		XMLData data;
		if (data.LoadBuffer(buffer, bufferSize))
		{
			XMLNode rootNode = data.GetRootNode();

			if (rootNode.HasAttribute("id"))
			{
				id = rootNode.AttributeToInt("id");
				SetID(id);
			}

			// general
			XMLNode generalNode = rootNode.GetChild("general");
			if (!generalNode.IsNull())
			{
				name = generalNode.AttributeToString("name");

				SetScreenOrientation(_FromSOStr(generalNode.AttributeToString("screenorientation")));

				width = generalNode.AttributeToInt("width");
				height = generalNode.AttributeToInt("height");

				Sizef size = Sizef((float)width, (float)height);
				SetName(name);
				SetSize(size);
			}

			// scene
			XMLNode sceneNode = rootNode.GetChild("scene");
			if (!sceneNode.IsNull())
			{
				sceneFilename = sceneNode.AttributeToString("filename");
				SetSceneFilename(sceneFilename);
			}

			XMLNode renderNode = rootNode.GetChild("render_setting");
			if (!renderNode.IsNull())
			{
			}

			// language
			XMLNode languageNode = rootNode.GetChild("language");

			// publish
			XMLNode publishNode = rootNode.GetChild("publish");
			if (!publishNode.IsNull())
			{
				if (publishNode.HasAttribute("ispublish"))
				{
					mIsPublish = publishNode.AttributeToBool("ispublish");
				}
			}

			// setting
			XMLNode settingNode = rootNode.GetChild("edit_setting");
			if (!settingNode.IsNull())
			{
				if (settingNode.HasAttribute("uicamerapercent"))
					mEdit_UICameraPercent = settingNode.AttributeToFloat("uicamerapercent");
			}

			// split file names
			std::string outPath;
			std::string outBaseName;
			std::string outExt;
			StringHelp::SplitFullFilename(filename, outPath, outBaseName, outExt);

			// ui
			mUIFilename = outPath + outBaseName + "_ui.px2obj";

			// logic
			mLogicFilename = outPath + outBaseName + "_logic.px2obj";

			// plugins
			XMLNode nodePlugins = rootNode.GetChild("plugins");
			if (!nodePlugins.IsNull())
			{
				XMLNode nodePlugin = nodePlugins.IterateChild();
				while (!nodePlugin.IsNull())
				{
					std::string name = nodePlugin.AttributeToString("name");
					mPlugins.push_back(name);

					nodePlugin = nodePlugins.IterateChild(nodePlugin);
				}
			}

			// version
			std::string versionFilename = outPath + "filelist.xml";
			std::string resVersionStr =
				PX2_APP.GetProjectVersionByPath(versionFilename);
			mResourceVersion = ResourceVersion(resVersionStr);

			const std::string &name = GetName();
			Application::_LoadConfigs(mCFGs, name);
		}
	}
	else
	{
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
Sizef Project::GetConfigSize(const std::string &filename)
{
	Sizef sz;

#if defined (_WIN32) || defined(WIN32)
	if (!PX2_RM.IsFileFloderExist(filename))
		return sz;
#endif

	std::string name;
	int height = 0;
	std::string sceneFilename;
	int width = 0;
	std::string uiFilename;
	std::string languageFilename;

	char *buffer = 0;
	int bufferSize = 0;
	if (PX2_RM.LoadBuffer(filename, bufferSize, buffer))
	{
		XMLData data;
		if (data.LoadBuffer(buffer, bufferSize))
		{
			XMLNode rootNode = data.GetRootNode();

			// general
			XMLNode generalNode = rootNode.GetChild("general");
			if (!generalNode.IsNull())
			{
				name = generalNode.AttributeToString("name");
				width = generalNode.AttributeToInt("width");
				height = generalNode.AttributeToInt("height");

				sz = Sizef((float)width, (float)height);
			}
		}
	}

	return sz;
}
//----------------------------------------------------------------------------
int Project::GetProjectID(const std::string &filename)
{
	int id = 0;

#if defined (_WIN32) || defined(WIN32)
	if (!PX2_RM.IsFileFloderExist(filename))
		return 0;
#endif

	std::string name;
	int height = 0;
	std::string sceneFilename;
	int width = 0;
	std::string uiFilename;
	std::string languageFilename;

	char *buffer = 0;
	int bufferSize = 0;
	if (PX2_RM.LoadBuffer(filename, bufferSize, buffer))
	{
		XMLData data;
		if (data.LoadBuffer(buffer, bufferSize))
		{
			XMLNode rootNode = data.GetRootNode();

			if (rootNode.HasAttribute("id"))
			{
				id = rootNode.AttributeToInt("id");
			}
		}
	}

	return id;
}
//----------------------------------------------------------------------------
void Project::SetScene(Scene *scene)
{
	if (mScene)
	{
		Scene *sceneOrigin = mScene;

		mScene->GoOutEventWorld();
		mScene = 0;

		Event *ent = PX2_CREATEEVENTEX(ProjectES, CloseScene);
		ent->SetData<Scene*>(sceneOrigin);
		PX2_EW.BroadcastingLocalEvent(ent);
	}

	mScene = scene;

	if (mScene)
	{
		mScene->WorldTransformIsCurrent = true;

		Event *ent = PX2_CREATEEVENTEX(ProjectES, NewScene);
		ent->SetData<Scene*>((Scene*)mScene);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void Project::SetSceneFilename(const std::string &scenefilename)
{
	mSceneFilename = scenefilename;
}
//----------------------------------------------------------------------------
void Project::SetUI(UI *ui)
{
	UIPtr ui1 = ui;

	if (mUI)
	{
		UI *uiBefore = mUI;

		mUI = 0;
		Event *ent = PX2_CREATEEVENTEX(ProjectES, CloseUI);
		ent->SetData<UI*>(uiBefore);
		PX2_EW.BroadcastingLocalEvent(ent);
	}

	mUI = ui1;

	if (mUI)
	{
		mUI->SetSize(mSize);

		Event *ent = PX2_CREATEEVENTEX(ProjectES, NewUI);
		ent->SetData<UI*>((UI*)mUI);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
UI *Project::GetUI()
{
	return mUI;
}
//----------------------------------------------------------------------------
void Project::SetLogic(Logic *logic)
{
	LogicPtr logic1 = logic;

	if (mLogic)
	{
		Logic *logicBefore = mLogic;

		mLogic = 0;
		Event *ent = PX2_CREATEEVENTEX(ProjectES, CloseLogic);
		ent->SetData<Logic*>(logicBefore);
		PX2_EW.BroadcastingLocalEvent(ent);
	}

	mLogic = logic1;

	if (mLogic)
	{
		Event *ent = PX2_CREATEEVENTEX(ProjectES, NewLogic);
		ent->SetData<Logic*>((Logic*)mLogic);
		PX2_EW.BroadcastingLocalEvent(ent);
	}
}
//----------------------------------------------------------------------------
void Project::SetSize(float width, float height)
{
	SetSize(Sizef(width, height));
}
//----------------------------------------------------------------------------
void Project::SetSize(const Sizef &size)
{
	mSize = size;

	if (mUI)
	{
		mUI->SetSize(mSize);
	}
}
//----------------------------------------------------------------------------
void Project::SetPublish(bool pub)
{
	mIsPublish = pub;
}
//----------------------------------------------------------------------------
bool Project::IsPublish() const
{
	return mIsPublish;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Project::RegistProperties()
{
	Object::RegistProperties();

	AddPropertyClass("Project");

	std::vector<std::string> screenOrientations;
	screenOrientations.push_back("SO_LANDSCAPE");
	screenOrientations.push_back("SO_PORTRAIT");

	AddPropertyEnum("ScreenOrientation", (int)mScreenOrientation,
		screenOrientations);

	AddProperty("Size", PT_SIZE, mSize);
}
//----------------------------------------------------------------------------
void Project::OnPropertyChanged(const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("ScreenOrientation" == obj.Name)
	{
		SetScreenOrientation((ScreenOrientation)PX2_ANY_AS(obj.Data, int));
	}
	else if ("Size" == obj.Name)
	{
		SetSize(PX2_ANY_AS(obj.Data, Sizef));
	}
}
//----------------------------------------------------------------------------
void Project::PoolClear()
{
	mPoolObjects.clear();
}
//----------------------------------------------------------------------------
void Project::PoolSet(const std::string &name, Object *obj)
{
	mPoolObjects[name] = obj;
}
//----------------------------------------------------------------------------
Object *Project::PoolGet(const std::string &name)
{
	auto it = mPoolObjects.find(name);
	if (it != mPoolObjects.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void Project::SetConfig(const std::string &name, const std::string &cfgStr)
{
	const std::string &projName = GetName();

	mCFGs[name] = cfgStr;
	Application::_WriteConfigs(mCFGs, projName);
}
//----------------------------------------------------------------------------
std::string Project::GetConfig(const std::string &name)
{
	auto it = mCFGs.find(name);
	if (it != mCFGs.end())
		return it->second;

	return "";
}
//----------------------------------------------------------------------------
std::string Project::_GetWritePath(const std::string &projName)
{
	return "Write_" + projName;
}
//----------------------------------------------------------------------------