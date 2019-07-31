// PX2UISplitterFrame.hpp

#ifndef PX2UISPLITTERFRAME_HPP
#define PX2UISPLITTERFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UIFrame.hpp"

namespace PX2
{

	class UIAuiBlockFrame;

	class PX2_ENGINE_ITEM UISplitterFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UISplitterFrame);
		PX2_NEW(UISplitterFrame);

	public:
		UISplitterFrame(bool hor=true);
		virtual ~UISplitterFrame();

		void SetHor(bool isHor);
		bool IsHor() const;

		enum DragType
		{
			DT_VALUE,
			DT_PERCENT,
			DT_MAX_TYPE
		};
		void SetDragType(DragType dt);
		DragType GetDragType() const;

		void SetDragRange(const Float2 &range);
		const Float2 &GetDragRange() const;

	protected:
		bool mIsHor;
		DragType mDragType;
		Float2 mDragRange;

	public:
		enum PosType
		{
			PT_NONE,
			PT_LEFT,
			PT_RIGHT,
			PT_BOTTOM,
			PT_TOP,
			PT_MAX_TYPE
		};
		void SetPosType(PosType pt);
		PosType GetPosType() const;

	protected:
		PosType mPosType;

	public:
		void SetDragable(bool isDragable);
		bool IsDragable() const;

		void SetDraging(bool isDraged);
		bool IsDraging() const;

		void OnDraging();

	protected:
		bool mIsDragable;
		bool mIsDraging;

	public_internal:
		bool IsCanDraging(float toParam);

		void _AddAuiBlockFrame0(UIAuiBlockFrame *blockframe);
		void _RemoveAuiBlockFrame0(UIAuiBlockFrame *blockframe);
		void _AddAuiBlockFrame1(UIAuiBlockFrame *blockframe);
		void _RemoveAuiBlockFrame1(UIAuiBlockFrame *blockframe);
		void _MarkBlockFramesChange();

	protected:
		std::vector<UIAuiBlockFrame*> mAuiBlockFrames0;
		std::vector<UIAuiBlockFrame*> mAuiBlockFrames1;

	protected:
		virtual void OnWidgetPicked(const CanvasInputData &inputData);
		virtual void OnSizeNodeNotPicked(const CanvasInputData &inputData);
		void _UpdateDraging(const CanvasInputData &inputData);
		void _SetOverMe(bool over);

		bool mIsOverMe;
	};

#include "PX2UISplitterFrame.inl"
	PX2_REGISTER_STREAM(UISplitterFrame);
	typedef Pointer0<UISplitterFrame> UISplitterFramePtr;

}

#endif