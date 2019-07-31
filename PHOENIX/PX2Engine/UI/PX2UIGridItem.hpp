// PX2UIGridItem.hpp

#ifndef PX2UIGRIDITEM_HPP
#define PX2UIGRIDITEM_HPP

#include "PX2UIItem.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIGridItem : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UIGridItem);

	public:
		UIGridItem();
		virtual ~UIGridItem();
	};

	PX2_REGISTER_STREAM(UIGridItem);
	typedef Pointer0<UIGridItem> UIGridItemPtr;

}

#endif