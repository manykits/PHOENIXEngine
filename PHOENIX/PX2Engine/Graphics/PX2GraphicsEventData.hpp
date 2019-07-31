// PX2GraphicsEventData.hpp

#ifndef PX2GRAPHICSEVENTDATA_HPP
#define PX2GRAPHICSEVENTDATA_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2WeakPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM AddObjectData
	{
	public:
		AddObjectData();
		~AddObjectData();

		WeakPointer0<Object> ParentObj;
		Pointer0<Object> Obj;
	};

	class PX2_ENGINE_ITEM AddMenuData
	{
	public:
		AddMenuData();
		~AddMenuData();

		int ParentID;
		int ID;
		std::string Title;
		std::string Callback;
	};

	class PX2_ENGINE_ITEM OpenSaveFileDlgData
	{
	public:
		OpenSaveFileDlgData();
		~OpenSaveFileDlgData();

		std::string ExtStr;
		std::string Callback;
	};

}

#endif