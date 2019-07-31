// PX2DynLibManager.hpp

#ifndef PX2DYNLIBMANAGER_HPP
#define PX2DYNLIBMANAGER_HPP

#include "PX2UnityPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2DynLib.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM DynLibManager : public Singleton<DynLibManager>
	{
	public:
		DynLibManager();
		virtual ~DynLibManager();

		DynLib *Load(const std::string &filename);
		void Unload(DynLib *dynlib);

	protected:
		typedef std::map<std::string, DynLib*> PluginList;
		PluginList mPluginList;
	};

}

#endif