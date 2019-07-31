// PX2UIAuiBlockFrame.hpp

#ifndef PX2UIAUIBLOCKFRAME_HPP
#define PX2UIAUIBLOCKFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2UISplitterFrame.hpp"
#include "PX2UIDefine.hpp"
#include "PX2UITabFrame.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UIAuiBlockFrame : public UIFrame
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_STREAM(UIAuiBlockFrame);

	public:
		UIAuiBlockFrame(UILayoutPosType posType);
		virtual ~UIAuiBlockFrame();

		void Caputure(bool capture);
		bool IsCaputred() const;

		void SetBrotherFrame(UIAuiBlockFrame *frame);
		UIAuiBlockFrame *GetBrotherFrame();

		UILayoutPosType GetLayOutPosType() const;

		void SetAutoExpand(bool expand);
		bool IsAutoExpand() const;

		virtual void OnChildAttached(Movable *child);
		virtual void OnChildDetach(Movable *child);

		UITabFrame *GetUITabFrame();

		virtual void OnSizeChanged();

		void SetMinSize(const Sizef &minSize);
		const Sizef &GetMinSize() const;

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		bool mIsCaptured;
		UIAuiBlockFrame *mBrotherFrame;

		UILayoutPosType mPosType;
		bool mIsAutoExpand;
		UITabFramePtr mUITabFrame;

		Sizef mMinSize;

		// ËÄÌõ±ß
	public:
		// hor:-_
		// ver:||
		void SetSideFrameHor0(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameHor0();

		void SetSideFrameHor1(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameHor1();

		void SetSideFrameVer0(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameVer0();

		void SetSideFrameVer1(UISplitterFrame *frame);
		UISplitterFrame *GetSideFrameVer1();

		void SetSpliterFrame(UISplitterFrame *frame);
		UISplitterFrame *GetSpliterFrame();

	public_internal:
		static void _UpdateLayout(UIAuiBlockFrame *auiBlockFrame,
			bool setToAuiBlockFrame = true,
			Sizef *outSize = 0, APoint *outPos = 0);

		UISplitterFrame *_CalGetNewSideFrameHor0();
		UISplitterFrame *_CalGetNewSideFrameHor1();
		UISplitterFrame *_CalGetNewSideFrameVer0();
		UISplitterFrame *_CalGetNewSideFrameVer1();

	protected:
		UISplitterFrame *mSideFrameHor0;
		UISplitterFrame *mSideFrameHor1;
		UISplitterFrame *mSideFrameVer0;
		UISplitterFrame *mSideFrameVer1;
		UISplitterFramePtr mSpliterFrame;

	protected:
		virtual void UpdateLayout(Movable *parent);

		UISplitterFrame *_CalGetLinkFrame0(UILayoutPosType pos);
		UISplitterFrame *_CalGetLinkFrame1(UILayoutPosType pos);
		UIAuiBlockFrame *_GetSideFrame(UILayoutPosType pos);

	public:
		UIAuiBlockFrame *CreateAddPosFrame(UILayoutPosType pos, const Sizef &size);
		void RemovePosFrame(UILayoutPosType pos);
		UIAuiBlockFrame *GetPosFrame(UILayoutPosType pos);
		int GetNumChildPosFrames();

	protected:
		UIAuiBlockFrame *_GetSameSizeBlockFrame(UIAuiBlockFrame *frame);

		void SetParentAuiBlockFrame(UIAuiBlockFrame *frame);
		UIAuiBlockFrame *GetParentSpBlockFrame();

		std::map<UILayoutPosType, Pointer0<UIAuiBlockFrame> > mSideFrames;
		UIAuiBlockFrame *mParentAuiBlockFrame;
	};

#include "PX2UIAuiBlockFrame.inl"
	PX2_REGISTER_STREAM(UIAuiBlockFrame);
	typedef Pointer0<UIAuiBlockFrame> UIAuiBlockFramePtr;

}

#endif