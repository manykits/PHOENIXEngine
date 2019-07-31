// PX2UIPropertyItem.hpp

#ifndef PX2UIPROPERTYITEM_HPP
#define PX2UIPROPERTYITEM_HPP

#include "PX2UIItem.hpp"
#include "PX2UIEditBox.hpp"
#include "PX2UIComboBox.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIPropertyItem : public UIItem
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UIPropertyItem);

	public:
		UIPropertyItem();
		virtual ~UIPropertyItem();

		enum ItemType
		{
			IT_NONE,
			IT_CLASS,
			IT_COMBOX,
			IT_COLORBUTTON,
			IT_EDITBOX,
			IT_EDITBOXSLIDER,
			IT_EDITBOX2,
			IT_EDITBOX3,
			IT_EDITBOX4,
			IT_TRANSFORM,
			IT_EDITBOXBUT,
			IT_MAX_TYPE
		};

		void OnAllocAll(const Any &userData);
		void OnAlloc();
		void OnFree();
		void SetObjectID(uint32_t id);
		uint32_t GetObjectID();

		void SetItemType(ItemType it, Object::PropertyType propType,
			const Any &data, const Any &data1, float sliderMinVal, 
			float sliderMaxVal, bool isEnable);
		ItemType GetItemType() const;

		UIFrame *GetEditCtrl();

		void ResetItem();

	protected:
		ItemType mItemType;
		PropertyType mPropertyType;
		uint32_t mObjectID;

		UIFramePtr mEditCtrl;
	};
	PX2_REGISTER_STREAM(UIPropertyItem);
	typedef Pointer0<UIPropertyItem> UIPropertyItemPtr;

}

#endif