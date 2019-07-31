// PX2UIFrame.hpp

#ifndef PX2UIFRAME_HPP
#define PX2UIFRAME_HPP

#include "PX2UIPre.hpp"
#include "PX2SizeNode.hpp"
#include "PX2Size.hpp"
#include "PX2UIDefine.hpp"
#include "PX2InputPushTransformController.hpp"
#include "PX2UIFrameGridAlignControl.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2InputEventData.hpp"

namespace PX2
{

	class Canvas;

	class PX2_ENGINE_ITEM UIFrame : public SizeNode
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(UIFrame);
		PX2_DECLARE_STREAM(UIFrame);

	public:
		UIFrame();
		virtual ~UIFrame();

		void SetUseSkin(bool isUseSkin);
		bool IsUseSkin() const;
		virtual void ConfigSkinColor();

		virtual void AutoMakeSizeFixable(float scale=1.0f);

		virtual void OnChildAttached(Movable *child);

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

	public:
		virtual void OnSizeChanged();
		virtual void OnPivotChanged();

		UIPicBox *CreateAddBackgroundPicBox(bool setWhite=true, 
			const Float3&color=Float3::WHITE);
		UIPicBox *GetBackgroundPicBox();
		void DestoryBackgroundPicBox();

		void SetActivateColor(const Float3 &color);
		const Float3 &GetActivateColor() const;
		void SetActivateAlpha(float alpha);
		float GetActivateAlpha() const;
		void SetActivateBrightness(float brightness);
		float GetActivateBrightness() const;

	protected:
		bool mIsUseSkin;
		UIPicBoxPtr mBackgroundPicBox;
		Float3 mActivatedColor;
		float mActivatedAlpha;
		float mActivatedBrightness;

		// calls
	public:
		// 圆形按钮使用
		virtual void SetRoundRadius(float radius);
		float GetRoundRadius() const;

		void SetPickRangeMovable(Movable *mov);
		Movable *GetPickRangeMovable();

		void SetPickRangeMovables(std::vector<SizeNode*> nodes);

		virtual void SetRangeAcceptFrame(UIFrame *frame);
		UIFrame *GetRangeAcceptFrame();

		void AddUICallback(UICallback callback);
		const std::vector<UICallback> &GetUICallbacks() const;

		typedef void (UIFrame::*MemUICallback)(UIFrame *frame, UICallType type);

		void SetMemUICallback(UIFrame *object, MemUICallback callback);
		MemUICallback GetMemUICallback() const;

		void SetScriptHandler(const std::string &scriptHandler);
		const std::string &GetScriptHandler() const;

	protected:
		void _UICallbacksCalls(UICallType callType);
		void CallString(const std::string &funStr, const char *format, ...);

		float mRoundRadius;
		MovablePtr mPickRangeMovable;
		std::vector<SizeNode*> mPickRangeSizeNodes;
		std::vector<UICallback> mUICallbacks;
		UIFrame *mMemObject;
		MemUICallback mMemUICallback;
		std::string mUIScriptHandler;

		UIFrame *mRangeAcceptFrame;

	public:
		virtual void PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas);

		// mask
	public:
		UIPicBox *CreateAddMask();
		void SetMaskVal(int maskVal);
		int GetMaskVal() const;
		UIPicBox *GetMask();
		void DestoryMask();
		void SetNeedAdjustChildrenMask(bool adjustMask);		
		bool IsNeedAdjustChildrenMask() const;

	protected:
		void _AdjustChildrenMask();

		bool mIsNeedAdjustChildrenMask;
		int mMaskVal;
		UIPicBoxPtr mMaskPicBox;

		// GridAlign
	public:
		UIFrameGridAlignControl *CreateAddGridAlignCtrl(bool doResetPlay = true);
		void DestoryGridAlignCtrl();
		UIFrameGridAlignControl *GetGridAlignCtrl();

	protected:
		UIFrameGridAlignControlPtr mGridAlignCtrl;
	};

	PX2_REGISTER_STREAM(UIFrame);
	typedef Pointer0<UIFrame> UIFramePtr;
#include "PX2UIFrame.inl"

}

#endif