// PX2Movable.hpp

#ifndef PX2SPATIAL_HPP
#define PX2SPATIAL_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Controlledable.hpp"
#include "PX2Bound.hpp"
#include "PX2Transform.hpp"
#include "PX2Function.hpp"
#include "PX2InputEventData.hpp"

namespace PX2
{

	class Culler;
	
	/// 空间几何体类
	/**
	* 所有需要空间数据支持的类型都从此类派生，同时支持截头体裁剪功能。空间数据
	* 包括：空间变换，包围盒。
	*/
	class PX2_ENGINE_ITEM Movable : public Controlledable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Movable);

	protected:
		Movable ();
	public:
		virtual ~Movable ();

		/// 本地和世界变换
		/**
		* 在某些情况下，你也许需要直接设置世界变换。如果World被设置了，
		* WorldIsCurrent标识也因该被设置为true。
		*/
		Transform LocalTransform;
		Transform WorldTransform;
		bool WorldTransformIsCurrent;

		Transform &GetLocalTransform();
		Transform &GetWorldTransform();

		bool IsSkinCtrlSetWroldTrans;
		Transform BoundWorldTransform;

		/// 世界包围盒访问
		/**
		* 在某些情况下，你也许需要直接设置世界包围盒。如果WorldBound被直接设置
		* ，WorldBoundIsCurrent应该被设置为true。
		*/
		Bound WorldBound;
		bool WorldBoundIsCurrent;

		/// 裁剪类型
		enum CullingMode
		{
			/// 通过比较包围盒和裁剪面，决定物体的可见性。
			CULL_DYNAMIC,

			/// 强制物体被裁剪。如果是一个Node被裁剪了，该Node下的所有子节点也
			/// 被裁剪。	
			CULL_ALWAYS,

			/// 永远不裁剪物体。如果一个Node不被裁剪，该Node下的所有子节点也不
			/// 被裁剪。
			CULL_NEVER
		};

		CullingMode Culling;

		virtual void Show (bool show);
		bool IsShow () const;

		/// 更新几何状态和控制器
		/**
		* 函数向下更新场景变换，向上更新世界包围盒。
		*/
		void Update (double applicationTime = -Mathd::MAX_REAL, double elapsedTime=0.0f,
			bool initiator = true);

		/// 父亲
		virtual void OnBeAttached();
		virtual void OnBeDetach();
		Movable* GetParent ();
		void DetachFromParent();
		void SetParentTransformIngore (bool trans, bool rotate, bool scale);
		void GetParentTransformIngore (bool &trans, bool &rotate, bool &scale);

		Movable *GetTopestParent();

		template <typename CLASSTYPE>
		CLASSTYPE *GetFirstParentDerivedFromType(int *numLevels = 0);

		// Pick
	public:
		virtual void OnPicked (int pickInfo); // called during Picker::Execute
		virtual void OnNotPicked (int pickInfo);

		void SetDoPick (bool doPick);
		bool IsDoPick () const;

		void SetPickIngoreCullingMode (bool pickIngoreCullingMode);
		bool IsPickIngoreCullingMode () const;

		void SetNotPickedParentChildrenNotPicked (bool use);
		bool IsNotPickedParentChildrenNotPicked () const;

		// Color Alpha Brightness
	public:
		void SetColorSelfCtrled(bool selfCtrled);
		bool IsColorSelfCtrled() const;
		virtual void SetColor(const Float3 &color);
		const Float3 &GetColor() const;

		void SetAlphaSelfCtrled (bool selfCtrled);
		bool IsAlphaSelfCtrled () const;
		virtual void SetAlpha(float alpha);
		float GetAlpha () const;

		void SetBrightnessSelfCtrled (bool selfCtrled);
		bool IsBrightnessSelfCtrled () const;
		virtual void SetBrightness(float brightness);
		float GetBrightness () const;

		const Float3 &GetLastColor() const;
		float GetLastAlpha() const;
		float GetLastBrightness() const;

		virtual void MarkAlphaColorBrightnessChanged();

		virtual void SetCastShadow(bool castShadow);
		bool IsCastShadow() const;
		virtual void SetReceiveShadow(bool reciveShadow);
		bool IsReceiveShadow() const;

	protected:
		bool mIsAlphaColorBrightnessChanged;

		void _AdjustAlphaColorBrightnes();

		// enable active
	public:
		void SetEnableSelfCtrled(bool selfCtrled);
		bool IsEnableSelfCtrled() const;

		void SetActivateSelfCtrled(bool selfCtrled);
		bool IsActivateSelfCtrled() const;

		// Update
	public:
		void SetFixUpdateTime (float time);
		float GetFixUpdateTime () const;
		void ResetFixUpdateTiming();

		void SetOnlyShowUpdate(bool onlyShowUpdate);
		bool IsOnlyShowUpdate() const;

		// 数值越大，更新优先级越高
		void SetUpdatePriority (int updatePriority);
		int GetUpdatePriority () const;

	protected:
		// 几何状态更新
		virtual void UpdateWorldData (double applicationTime, double elapsedTime);
		virtual void UpdateWorldBound () = 0;
		void PropagateBoundToRoot ();

public_internal:
		// 场景裁剪
		void GetVisibleSet (Culler& culler, bool noCull);
		virtual void OnGetVisibleSet (Culler& culler, bool noCull) = 0;
		virtual void OnCulled(Culler& culler);

		// 设置父节点，节点在attach/detach的时候调用此函数。
		void SetParent (Movable* parent);

		void SetSaveWriteIngore(bool doSaveWrite);
		bool IsSaveWriteIngore() const;

	protected:
		Movable* mParent;

	protected:
		bool mIsIngoreParent_Translate;
		bool mIsIngoreParent_Rotate;
		bool mIsIngoreParent_Scale;

		bool mIsAlphaSelfCtrled;
		float mAlpha;
		bool mIsColorSelfCtrled;
		Float3 mColor;
		bool mIsBrightnessSelfCtrled;
		float mBrightness;

		float mLastAlpha;
		Float3 mLastColor;
		float mLastBrightness;

		bool mIsEnableSelfCtrled;
		bool mIsActivateSelfCtrled;

		float mUpdateTime;
		float mUpdateTiming;

		bool mIsOnlyShowUpdate;
		int mUpdatePriority;

		bool mIsDoPick;
		bool mIsPickIngoreCullingMode;
		bool mIsNotPickedParentChildrenNotPicked;

		bool mIsSaveWriteIngore;
		
		bool mIsCastShadow;
		bool mIsReceiveShadow;
	};

	PX2_REGISTER_STREAM(Movable);
	typedef Pointer0<Movable> MovablePtr;
#include "PX2Movable.inl"

}

#endif