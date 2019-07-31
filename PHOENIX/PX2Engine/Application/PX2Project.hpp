// PX2Project.hpp

#ifndef PX2PROJECT_HPP
#define PX2PROJECT_HPP

#include "PX2Singleton_NeedNew.hpp"
#include "PX2Node.hpp"
#include "PX2Scene.hpp"
#include "PX2Size.hpp"
#include "PX2Float4.hpp"
#include "PX2Canvas.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UI.hpp"
#include "PX2Logic.hpp"
#include "PX2ResourceUpdate.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Project : public Object, public Singleton<Project>
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_PROPERTY;

	public:
		Project();
		virtual ~Project();

		virtual void SetName(const std::string &name);

		void SetVersion(const ResourceVersion &version);
		const ResourceVersion &GetResourceVersion() const;

		// 使用该接口删除Project
		static void Destory();

		// ScreenOrientation
	public:
		enum ScreenOrientation
		{
			SO_LANDSCAPE,
			SO_PORTRAIT,
			SO_MAX_TYPE
		};
		void SetScreenOrientation(ScreenOrientation so);
		ScreenOrientation GetScreenOrientation() const;

		void SetSizeSameWithScreen(bool sizeWithScreen);
		bool IsSizeSameWithScreen() const;

	protected:
		ScreenOrientation _FromSOStr(const std::string &str);
		std::string _ToSOStr(ScreenOrientation so);

		ResourceVersion mResourceVersion;
		ScreenOrientation mScreenOrientation;
		bool mIsSizeSameWithScreen;

		// Load Save
	public:
		bool Save(const std::string &filename); //< only used in editor
		bool SaveConfig(const std::string &filename); //< only used in editor	
		bool Load(const std::string &filename);
		static Sizef GetConfigSize(const std::string &filename);
		static int GetProjectID(const std::string &filename);

		// Config
	public:
		void SetSize(float width, float height);
		void SetSize(const Sizef &size);
		const Sizef & GetSize() const;
		float GetWidth() const;
		float GetHeight() const;

		void SetPublish(bool pub);
		bool IsPublish() const;

	protected:
		Sizef mSize;
		bool mIsPublish;

		// Scene
	public:
		void SetScene(Scene *scene);
		Scene *GetScene();
		const std::string &GetSceneFilename() const;

	public_internal:
		void SetSceneFilename(const std::string &scenefilename);

	protected:
		std::string mSceneFilename;
		ScenePtr mScene;

		// UI
	public:
		void SetUI(UI *ui);
		UI *GetUI();
		const std::string &GetUIFilename() const;

	protected:
		std::string mUIFilename;
		UIPtr mUI;

		// Logic
	public:
		void SetLogic(Logic *logic);
		Logic *GetLogic();
		const std::string &GetLogicFilename() const;

	private:
		std::string mLogicFilename;
		LogicPtr mLogic;
		
		// Plugins
	public:
		const std::vector<std::string> &GetPlugins() const;

	protected:
		std::vector<std::string> mPlugins;

		// edit setting
	public:
		void SetEdit_UICameraPercent(float percent);
		float GetEdit_UICameraPercent() const;

	protected:
		float mEdit_UICameraPercent;

		// ObjectPool
	public:
		void PoolClear();
		void PoolSet(const std::string &name, Object *obj);
		Object *PoolGet(const std::string &name);

	protected:
		std::map<const std::string, ObjectPtr> mPoolObjects;

		// config
	public:
		void SetConfig(const std::string &name, const std::string &cfgStr);
		std::string GetConfig(const std::string &name);

	private:
		std::string _GetWritePath(const std::string &projName);

		std::map<std::string, std::string> mCFGs;
	};

#define PX2_PROJ Project::GetSingleton()
#include "PX2Project.inl"

}

#endif