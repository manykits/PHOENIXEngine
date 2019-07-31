// PX2UIItem.hpp

#ifndef PX2UIITEM_HPP
#define PX2UIITEM_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UISplitterFrame.hpp"
#include "PX2UIAuiBlockFrame.hpp"
#include "PX2UIButton.hpp"
#include "PX2UIFText.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2UICheckButton.hpp"
#include "PX2UIEditBox.hpp"

namespace PX2
{

	class UIComboBox;

	class PX2_ENGINE_ITEM UIItem : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(UIItem);

	public:
		UIItem();
		virtual ~UIItem();

		void SetRootItem(bool root);
		bool IsRootItem() const;

		void ShowItem(bool show);
		bool IsShowItem() const;

		UIItem *AddItem(const std::string &label, 
			const std::string &name, Object *obj);
		virtual void OnChildAttached(Movable *child);
		bool RemoveItem(UIItem *item);
		bool RemoveItem(const std::string &name);
		virtual void RemoveAllChildItems();
		virtual void OnChildDetach(Movable *child);
		int GetNumChildItem() const;
		UIItem *GetChildItemByIndex(int index);
		
		void SetItemObject(Object* obj);
		Object *GetItemObject();
		UIItem *GetItemByObject(Object *obj);

		UICheckButton *CreateButArrow();
		UICheckButton *GetButArrow();
		void DestoryArrowBut();

		void Expand(bool expand);
		bool IsExpand() const;
		int GetNumAllChildsExpand() const;

		void SetLabel(const std::string &label);
		const std::string &GetLabel() const;

		enum IconArrowState
		{
			IAS_NONE,
			IAS_ARROW0,
			IAS_ARROW1,
			IAS_MAX_STATE
		};
		void SetIconArrowState(IconArrowState state);
		IconArrowState GetIconArrowState() const;

		UIButton *GetButBack();
		UIFPicBox *GetIcon();
		UIFText *GetFText();

		void Select(bool select);
		bool IsSelected() const;

		virtual void OnSizeChanged();

		void SetLevelAdjust(float adjust);
		float GetLevelAdjust() const;

	public_internal:
		void _SetLevel(float level);
		float _GetLevel() const;

		bool _IsNeedRecal() const;

	protected:
		virtual void PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas);
		virtual void UpdateWorldData(double applicationTime,
			double elapsedTime);
		void _TellParentChildrenExpand();
		void _RecalNumChildExpand();
		void _TellParentChildrenRecal();
		void _Recal();

		bool mIsRootItem;
		bool mIsShowItem;

		bool mIsNeedRecal;

		bool mIsExpand;
		bool mIsNumAllChildExpandNeedRecal;
		int mNumAllChildExpand;

		IconArrowState mIconArrowState;
		UIButtonPtr mButBack;
		UICheckButtonPtr mButArrow;
		UIFPicBoxPtr mIcon;
		UIFTextPtr mFText;
		bool mIsSelected;

		float mLevelAdjust;
		float mLevel;

		Object *mObject;

		std::vector<Pointer0<UIItem> > mChildItems;

	public:
		void SetValue(const Any &val);
		const Any &GetValue() const;

	protected:
		Any mValue;
	};

#include "PX2UIItem.inl"
	PX2_REGISTER_STREAM(UIItem);
	typedef Pointer0<UIItem> UIItemPtr;

}

#endif