// PX2Canvas.hpp

#ifndef PX2CANVAS_HPP
#define PX2CANVAS_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2SizeNode.hpp"
#include "PX2Rect.hpp"
#include "PX2Culler.hpp"
#include "PX2Node.hpp"
#include "PX2Vector2.hpp"
#include "PX2Polysegment.hpp"
#include "PX2RenderTarget.hpp"
#include "PX2WireProperty.hpp"
#include "PX2CanvasRenderBind.hpp"
#include "PX2InputEventData.hpp"
#include "PX2CameraNode.hpp"
#include "PX2TriMesh.hpp"

namespace PX2
{

	class Renderer;
	class RenderWindow;

	class PX2_ENGINE_ITEM Canvas : public SizeNode
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Canvas);
		PX2_NEW(Canvas);

	public:
		Canvas();
		virtual ~Canvas();

		void SetMain(bool main);
		bool IsMain() const;

		// 当一个节点挂在多个Canvas下，无法使用AttachChild
		// 可以使用这个方法，加入裁减序列
		virtual void SetRenderNode(Node *movable);
		Node *GetRenderNode();
		void SetRenderNodeUpdate(bool update);
		bool IsRenderNodeUpdate() const;	

		virtual void SetScreenRect(const Rectf &rect);

	public_internal:
		void SetRenderWindow(RenderWindow *rw);
		RenderWindow *GetRenderWindow();

	protected:
		virtual void OnSizeChanged();
		virtual void UpdateWorldData(double applicationTime, 
			double elapsedTime);
		virtual void UpdateWorldBound();
		virtual void UpdateLayout(Movable *parent);
		virtual void UpdateLeftBottomCornerOffset(Movable *parent);
		void _UpdateCamera();

		RenderWindow *mRenderWindow;
		
		bool mIsMain;
		bool mIsAsNode;
		Node *mRenderNode;
		bool mIsRenderNodeUpdate;

		// Renderer Camera Culler
	public:
		// 一个观看整个场景的相机
		void SetOverCamera(Camera *overCamera);
		Camera *GetOverCamera();

		void AddCamera(Camera *camera);
		void RemoveCamera(Camera *camera);
		void RemoveAllCameras();

		CameraNode *CreateUICameraNode();
		CameraNode *GetUICameraNode();
		void EnableUICameraAutoAdjust(bool autoAdjust);
		bool IsUICameraAutoAdjust() const;

		static float msUICameraY;

	public_internal:
		virtual void OnGetVisibleSet(Culler& culler, bool noCull);
		virtual void OnCulled(Culler& culler);

	protected:
		std::vector<Camera *> mCameras;
		std::map<Camera*, CullerPtr> mCullers;

		CameraNodePtr mUICameraNode;
		bool mIsUICameraAutoAdjust;
		TriMeshPtr mUIShareMesh;

		CameraPtr mOverCamera;
		CullerPtr mOverCameraCuller;

		CanvasRenderBindPtr mCanvasRenderBind;

		WirePropertyPtr mOverrideWireProperty;

		// Draw
	public:
		void SetClearColor(const Float4 &color);
		const Float4 &GetClearColor() const;

		void SetClearDepth(float depth);
		float GetClearDepth() const;

		void SetClearStencil(unsigned int stencil);
		unsigned int GetClearStencil() const;

		void SetClearFlag(bool color, bool depth, bool stencil);
		void GetClearFlag(bool &color, bool &depth, bool &stencil);

		void SetOverWireframe(bool wireframe);
		bool IsOverWireframe() const;

		virtual void ClearVisibleSet();
		virtual void ComputeVisibleSet();
		virtual void Draw(Renderer *renderer);

	protected:
		void _Clear(Renderer *renderer, bool bColor, const Float4 &color,
			bool bDepth, float depth, bool bStencil, int stencil);
		virtual void _Draw(Camera *camera, Renderer *renderer, Culler *culler);
		void _FlushShareDraw(Renderer *renderer);

		Float4 mClearColor;
		float mClearDepth;
		int mClearStencil;
		bool mClearFlagColor;
		bool mClearFlagDepth;
		bool mClearFlagStencil;
		bool mIsOverWireframe;
		WirePropertyPtr mOverWireProperty;

		// pick
	public:
		// 子项目，重新计算坐标
		void SetPickPosRecal(bool recal);
		bool IsPickPosRecal() const;

		void OnFirstInput(const InputEventData &data);
		CanvasInputData ConvertInputEventDataToCanvasInputData(const APoint &screenPos,
			const APoint &logicPos, int entType, int butID, float wheel);

		bool IsEntered() const;
		virtual void OnEnter();
		virtual void OnLeave();
		virtual void OnLeftDown(const PickInputData &data);
		virtual void OnLeftUp(const PickInputData &data);
		virtual void OnLeftDClick(const PickInputData &data);
		virtual void OnMiddleDown(const PickInputData &data);
		virtual void OnMiddleUp(const PickInputData &data);
		virtual void OnMouseWheel(const PickInputData &data);
		virtual void OnRightDown(const PickInputData &data);
		virtual void OnRightUp(const PickInputData &data);
		virtual void OnMotion(const PickInputData &data);

		virtual void PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas);
		virtual void OnWidgetPicked(const CanvasInputData &inputData);
		virtual void OnSizeNodePicked(const CanvasInputData &inputData);
		virtual void OnSizeNodeNotPicked(const CanvasInputData &inputData);

		SizeNode *GetPickedWidget();

		bool IsMoved() const;
		bool IsLeftPressed() const;
		bool IsRightPressed() const;
		bool IsMiddlePressed() const;
		const APoint &GetCurPickPos() const;
		const AVector &GetMoveDelta() const;

	public_internal:
		void _AddInRangePickWidget(SizeNode *widget, const CanvasInputData &inputData);
		void _ClearInRangePickWidget();
		void _SortInRangePickWidget();

	protected:
		virtual void _DoPick(const CanvasInputData &inputData);
		APoint _PreScreenPosToLogicPos(const APoint &pos);
		APoint _PreLogicPosToCameraPos(const APoint &pos, float worldRectLeft, 
			float worldRectBottom, float worldRectWidth, float worldRectHeight);

		bool mIsPickPosRecal;

		float mMoveAdjugeParam;
		float mMoveAdjugeParamSquare;

		bool mIsPressed;
		APoint mCurPickPos;
		APoint mLastPickPos;
		APoint mPressedPos;
		APoint mReleasedPos;
		bool mIsMoved;
		AVector mMoveDelta;

		bool mIsEntered;
		bool mIsLeftPressed;
		APoint mLeftPressedPos;
		APoint mLeftReleasedPos;

		bool mIsRightPressed;
		APoint mRightPressedPos;
		APoint mRightReleasedPos;

		bool mIsMiddlePressed;
		APoint mMiddlePressedPos;
		APoint mMiddleReleasedPos;

		std::list<SizeNode*> mInRangeWidgets;
		SizeNode *mPickedWidget;

		// Help
	public:
		virtual std::pair<float, float> CalPixelToWorld(Camera *camera);

	protected:
		std::pair<float, float> mPixelToWorld;

		// Debug
	public:
		Polysegment *GetDebugLine();
		void ClearDebugLine();
		void AddDebugLine(const APoint &fromPos, const APoint &toPos,
			const Float4 &color);

	protected:
		VertexBufferPtr mDebugPolyVB;
		Pointer0<Polysegment> mDebugPoly;
	};

	PX2_REGISTER_STREAM(Canvas);
#include "PX2Canvas.inl"
	typedef Pointer0<Canvas> CanvasPtr;

}

#endif