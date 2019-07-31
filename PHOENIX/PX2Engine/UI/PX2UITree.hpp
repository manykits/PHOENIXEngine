// PX2UITree.hpp

#ifndef PX2UITREE_HPP
#define PX2UITREE_HPP

#include "PX2UIItem.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UISlider.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UITree : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UITree);
		PX2_NEW(UITree);

	public:
		UITree();
		virtual ~UITree();

		void SetItemHeight(float height);
		float GetItemHeight() const;

		void SetIconArrowSpace(float space);
		float GetIconArrowSpace() const;

		UIItem *GetRootItem();
		virtual UIItem *AddItem(UIItem *parentItem, const std::string &label, 
			const std::string &name="", Object *obj=0);
		UIItem *GetItemByObject(Object *obj);
		bool RemoveItem(UIItem *item);
		void RemoveAllItemsExceptRoot();
		void ShowRootItem(bool show);
		bool IsShowRootItem() const;

		void AddSelectItem(UIItem *item, bool isDoubleSelect, bool callOnSelected);
		void ClearAllSelectItems(bool callOnSelected);
		UIItem *GetSelectedItem();

		UISlider *GetSlider();

		virtual void OnSelected(UIItem *item, bool isDoubleSelect);
		void SelectCallback(Object *frame, UICallType type);

		virtual void OnSizeChanged();

	protected:
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		virtual void OnSizeNodePicked(const CanvasInputData &inputData);
		void _UpdateContentPos();
		void _SliderCallback(UIFrame *frame, UICallType type);

		bool mIsNeedUpdateContentPos;

		float mSliderSize;
		float mItemHeight;
		float mIconArrowSpace;
		UIItemPtr mRootItem;
		bool mIsShowRootItem;

		UIFramePtr mMaskFrame;

		UIFramePtr mContentFrame;
		std::vector<UIItemPtr> mSelectedItems;
		std::map<std::string, UIItem*> mTagItems;

		UISliderPtr mSlider;
	};

#include "PX2UITree.inl"
	PX2_REGISTER_STREAM(UITree);
	typedef Pointer0<UITree> UITreePtr;

}

#endif