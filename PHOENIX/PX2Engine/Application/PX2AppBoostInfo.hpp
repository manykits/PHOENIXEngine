// PX2AppBoostInfo.hpp

#ifndef PX2APPBOOSTINFO_HPP
#define PX2APPBOOSTINFO_HPP

#include "PX2CorePre.hpp"
#include "PX2Size.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM AppBoostInfo
	{
	public:
		AppBoostInfo();
		~AppBoostInfo();

		enum PlayLogicMode
		{
			PLM_SIMPLE,
			PLM_LOGIC,
			PLM_MAX_MODE
		};

		// general
		std::string ProjectName;

		// app
		Sizef BoostSize;
		PlayLogicMode ThePlayLogicMode;
		bool IsShowInfo;
		bool IsDataReWriteToDataUpdate;

		// projects
		std::vector<std::string> Projects;
		bool IsHasProject(const std::string &projectName) const;
		void ClearProjects();
		bool AddProject(const std::string &projectName);
		bool RemoveProject(const std::string &projectName);

		// plugins
		std::vector<std::string> Plugins;
		bool IsHasPlugin(const std::string &projectName) const;
		void ClearPlugins();
		bool AddPlugin(const std::string &projectName);
		bool RemovePlugin(const std::string &projectName);

	};

}

#endif