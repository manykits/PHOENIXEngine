// PX2UIList.hpp

#ifndef PX2UILIST_HPP
#define PX2UILIST_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIItem.hpp"
#include "PX2UISlider.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIList : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_NEW(UIList);
		PX2_DECLARE_STREAM(UIList);

	public:
		UIList();
		virtual ~UIList();

		void SetSliderSize(float size);
		float GetSliderSize() const;

		UISlider *GetSlider();

		void SetItemHeight(float height);
		float GetItemHeight() const;

		void SetItemBackPicBox(const std::string &backPicBox);

		void SetTextAlignLeftPos(float isAlignLeft);
		float GetTextAlignLeftPos() const;

		UIItem *AddItem(const std::string &text);
		UIItem *GetItem(const std::string &text);
		UIItem *GetItemByIndex(int index);
		void RemoveItem(UIItem *item);
		UIItem *GetItemByUserDataString(const std::string &userDataName,
			const std::string &userDataString);

		int GetNumItems() const;
		int GetItemIndex(const std::string &text) const;
		void RemoveAllItems();
		float GetContentHeight() const;

		void SetNumMaxItems(int numMax);
		int GetNumMaxItems() const;

		void SetReleasedDoSelect(bool releasedDoSelect);
		bool IsReleasedDoSelect() const;

		void SelectItem(int index);
		void AddSelectItem(UIItem *item);
		void ClearAllSelectItems();
		UIItem *GetSelectedItem();
		std::string GetSelectedItemText();

		virtual void OnSelected(UIItem *item);
		int GetSelectIndex() const;

		void SetTextColor(const Float3 &textColor);
		const Float3 &GetTextColor() const;

		void SetFont(const std::string &font);
		void SetFontSize(int size);
		int GetFontSize() const;

		void SetItemBackColor(const Float3 &backColor);
		const Float3 &GetItemBackColor() const;

		virtual void OnSizeChanged();

	protected:
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _Recal();
		void _SliderCallback(UIFrame *frame, UICallType type);
		void _UpdateContentPos();
		void _UpdateItemVisible();
		void _SelectButCallback(UIFrame *frame, UICallType type);
		void _OnSelectProcess(UIItem *uiItem);

		bool mIsNeedRecal;
		bool mIsUpdateSliderVisible;
		bool mIsUpdateContentPos;

		std::string mItemBackPicBox;
		float mSliderSize;
		float mItemHeight;
		float mTextAlignLeftPos;
		UIFramePtr mMaskFrame;
		UIFramePtr mContentFrame;
		std::vector<UIItemPtr> mItems;
		UISliderPtr mSlider;
		Float3 mTextColor;
		std::string mFont;
		int mFontSize;
		Float3 mItemBackColor;

		bool mIsReleasedDoSelect;
		std::vector<UIItemPtr> mSelectedItems;
		int mSelectedIndex;

		int mNumMaxItems;
	};

#include "PX2UIList.inl"
	PX2_REGISTER_STREAM(UIList);
	typedef Pointer0<UIList> UIListPtr;

}

#endif