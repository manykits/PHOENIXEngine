// PX2LP_ProjectItem.hpp

#ifndef PX2LP_PROJECTITEM_HPP
#define PX2LP_PROJECTITEM_HPP

#include "PX2LP_Pre.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{
	
	class UIItem;
	class UIFPicBox;
	class UICheckButton;
	class UIButton;
	
	class PX2LP_DLL_ITEM LP_ProjectItem
	{
	public:
		LP_ProjectItem();
		~LP_ProjectItem();

		int ID;
		std::string Name;
		
		bool IsLocalExist;
		bool IsCloud;

		UIItem *TheItem;
		UICheckButton *TheCheckButton;
		UIButton *TheCloudButton;
	};
	typedef Pointer0<LP_ProjectItem> LP_ProjectItemPtr;

}

#endif