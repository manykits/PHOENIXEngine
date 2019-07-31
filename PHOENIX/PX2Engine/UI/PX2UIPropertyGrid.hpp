// PX2UIPropertyGrid.hpp

#ifndef PX2UIPROPERTYGRID_HPP
#define PX2UIPROPERTYGRID_HPP

#include "PX2UIFrame.hpp"
#include "PX2UITree.hpp"
#include "PX2NotFreeObjectPool.hpp"
#include "PX2UIPropertyItem.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIPropertyGrid : public UITree
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UIPropertyGrid);

	public:
		UIPropertyGrid();
		virtual ~UIPropertyGrid();

		UIPropertyItem *AddItemType(UIItem *parentItem, 
			const std::string &label, Object::PropertyType pt, 
			const std::string &name, const Any &data, const Any &data1,
			float sliderMin, float sliderMax,
			bool isEnable, Object *obj = 0);

		void RemoveAllProperties();

		UIItem *AddProperty(const std::string &name, const std::string &label,
			PropertyType type, const Any &data, const Any &data1, 
			float sliderMin, float sliderMax, bool enable = true);

		virtual void OnItemChanged(UIItem *item);

	protected:
		void _SpliterDragingCallback(UIFrame *frame, UICallType type);
		void _AddEditObj(UIFrame *obj);
		void _SetItemPos(UIPropertyItem *item);
		void _SetEditObjPos(UIFrame *frame);
		UIPropertyItem::ItemType _GetPropertyItemType(Object::PropertyType pt);
		NotFreeObjectPool<UIPropertyItem> *_GetPool(UIPropertyItem::ItemType itemType);

		UIItemPtr mLastClassItem;
		UISplitterFramePtr mSpliterFrame;
		std::vector<UIFramePtr> mEditObjs;
		std::vector<UIPropertyItemPtr> mAllItems;
		std::map<UIPropertyItem::ItemType, NotFreeObjectPool<UIPropertyItem> *>
			mItemPools;
	};

	PX2_REGISTER_STREAM(UIPropertyGrid);
	typedef Pointer0<UIPropertyGrid> UIPropertyGridPtr;

}

#endif