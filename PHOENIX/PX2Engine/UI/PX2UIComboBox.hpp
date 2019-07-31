// PX2UIComboBox.hpp

#ifndef PX2UICOMBOBOX_HPP
#define PX2UICOMBOBOX_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"
#include "PX2UIList.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIComboBox : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_NEW(UIComboBox);
		PX2_DECLARE_STREAM(UIComboBox);

	public:
		UIComboBox();
		virtual ~UIComboBox();

		virtual void ConfigSkinColor();

		void SetFontSize(int size);

		UIItem *AddChooseStr(const std::string &choose);
		void RemoveAllChooseStr();
		int GetNumChooseStr() const;
		std::string GetChooseStrByIndex(int i) const;
		const std::vector<std::string> &GetChooseStrs() const;

		void SetChooseListHeightSameWithChooses();

		void Choose(int i, bool isDoCallback=true);
		void ChooseStr(const std::string &str, bool isDoCallback=true);
		int GetChoose() const;
		std::string GetChooseStr() const;
		UIItem *GetChooseItem();
		virtual void OnChoosed();

		UIButton *GetSelectButton();
		UIList *GetChooseList();

		void SetTextColor(const Float3 &textColor);

	protected:
		void _SelectButCallback(UIFrame *frame, UICallType type);
		void _ComboCallback(UIFrame *frame, UICallType type);

		UIButtonPtr mSelectButton;
		UIListPtr mChooseList;
		std::vector<std::string> mChooses;

		int mChoose;
	};

#include "PX2UIComboBox.inl"
	PX2_REGISTER_STREAM(UIComboBox);
	typedef Pointer0<UIComboBox> UIComboBoxPtr;

}

#endif