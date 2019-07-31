// PX2UIFrameGridAlignController.hpp

#ifndef PX2UIFRAMEGRIDALIGNCONTROLLER_HPP
#define PX2UIFRAMEGRIDALIGNCONTROLLER_HPP


#include "PX2Controller.hpp"
#include "PX2Size.hpp"
#include "PX2Float2.hpp"
#include "PX2Float4.hpp"

namespace PX2
{
	
	class UIFrame;

	class PX2_ENGINE_ITEM UIFrameGridAlignControl : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIFrameGridAlignControl);

	public:
		UIFrameGridAlignControl();
		virtual ~UIFrameGridAlignControl();

		void SetCellSize(const Sizef &cellSize);
		const Sizef &GetCellSize() const;

		void SetSpacing(const Float2 &spacing);
		const Float2 &GetSpacing() const;

		void SetBorder(float left, float right, float bottom, float top);
		void GetBorder(float &left, float &right, float &bottom, float &top);

		enum StartAxisType
		{
			SAT_HORIZONTAL,
			SAT_VERTICAL,
			SAT_MAX_TYPE
		};
		void SetStartAxis(StartAxisType sa);
		StartAxisType GetStartAxis() const;

		enum ChildAlignmentType
		{
			CAT_UPPER_LEFT,
			CAT_UPPER_CENTER,
			CAT_UPPER_RIGHT,
			CAT_MIDDLE_LEFT,
			CAT_MIDDLE_CENTER,
			CAT_MIDDLE_RIGHT,
			CAT_LOWER_LEFT,
			CAT_LOWER_CENTER,
			CAT_LOWER_RIGHT,
			CAT_MAX_TYPE
		};
		void SetChildAlignment(ChildAlignmentType ca);
		ChildAlignmentType GetChildAlignment() const;

		enum ConstraintType
		{
			CT_FLEXIBLE,
			CT_FIXED_COLUMN_COUNT,
			CT_FIXED_ROW_COUNT,
			CT_MAX_TYPE
		};
		void SetConstraintType(ConstraintType type);
		ConstraintType GetConstraintType() const;
		void SetConstraintValue(int value);
		int GetConstraintValue() const;
		void SetConstraintExtend(bool extend);
		bool IsConstraintExtend() const;

		void MarkLayoutChanged();
		bool IsLayoutChanged() const;

		void SetAutoAdjustContentSize(bool adjust);
		bool IsAutoAdjustContentSize() const;
		Sizef GetContentSize() const;

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);
		void _UpdateAlignItems();
		void _UpdateItems(UIFrame *parent, const std::vector<UIFrame*> &frames,
			StartAxisType startAxis, ChildAlignmentType childAlignmentType);

		bool mIsLayoutChanged;
		Sizef mCellSize;
		Float2 mSpacing;
		Float4 mBorder;
		StartAxisType mStartAxisType;
		ChildAlignmentType mChildAlignmentType;
		ConstraintType mConstraintType;
		int mConstraintValue;
		bool mIsConstraintExtend;

		bool mIsAutoAdjustContentSize;
		Sizef mContentSize;
	};

#include "PX2UIFrameGridAlignControl.inl"
	PX2_REGISTER_STREAM(UIFrameGridAlignControl);
	typedef Pointer0<UIFrameGridAlignControl> UIFrameGridAlignControlPtr;

}

#endif