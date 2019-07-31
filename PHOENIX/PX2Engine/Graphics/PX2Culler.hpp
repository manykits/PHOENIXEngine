// PX2Culler.hpp

#ifndef PX2CULLER_HPP
#define PX2CULLER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Camera.hpp"
#include "PX2Movable.hpp"
#include "PX2VisibleSet.hpp"
#include "PX2Culler.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class Canvas;
	
	/// 场景裁剪器类
	class PX2_ENGINE_ITEM Culler
	{
	public:
		/**
		* 裁剪器需要传递相机。如果camera没有被传递给构造函数，你需要在
		* ComputeVisibleSet函数之前调用SetCamera。
		*/
		Culler ();
		Culler (const Camera* camera);
		virtual ~Culler ();

		// 0 ingore, 1 do, 2 not do
		void SetFlag_CastShadow(int cast);
		int GetFlag_CastShadow() const;

		// 相机和截头体设置
		inline void SetCamera (const Camera* camera);
		inline const Camera* GetCamera () const;
		void SetFrustum (const float* frustum);
		inline const float* GetFrustum () const;
		inline VisibleSet& GetVisibleSet ();
		void Clear();

		/// 插入可见对象到可见列表
		/**
		* 派生可以重载此函数。派生类可以实现一个选择数组，实现最小化的状态改变，
		* 或者实现一个队列支持入口系统。
		*/
		virtual void Insert (Renderable* visible);

		// 你可以插入除了视截头体之外的裁剪面，插入裁剪面要求其在世界坐标系。
		enum { MAX_PLANE_QUANTITY = 32 };
		inline int GetPlaneQuantity () const;
		inline const HPlane* GetPlanes () const;
		inline void SetPlaneState (unsigned int planeState);
		inline unsigned int GetPlaneState () const;
		inline void PushPlane (const HPlane& plane);
		inline void PopPlane ();

		/// 将对象的世界包围体和裁剪面进行比较，作可见判断，只有Movable调用此函数。
		bool IsVisible (const Bound& bound);

		/// 裁剪场景的函数
		/**
		* 这个是你用来裁剪场景的主函数。在这个函数中，遍历整个场景，构建潜在可
		* 见集。
		*/
		void ComputeVisibleSet (Movable* scene);
		Movable *GetCurCullingScene();

	protected:
		// 裁剪器需要的相机
		const Camera* mCamera;

		int mFlag_CastShadow;

		/** 
		* 输入像机的截头体的拷贝，这样容许各式系统在裁剪的时候改变截头体（举例]
		* 来说，向入口系统）不用影响到像机。
		*/
		float mFrustum[Camera::VF_QUANTITY];

		/** 
		* 世界裁剪面是与截头体相关的，我们也可以增加用户自定义的裁剪面。成员
		* mPlaneState是以bit存储作为标志的，表示此面在裁剪系统中是否激活。bit为
		* 1表示平面式激活的，否则平面是不激活的。一个激活的平面会与包围体相比较，
		* 灭有激活的平面不比较。这样可以支持高效的等级裁剪。举例来说，如果一个
		* Node的包围体在像机“左裁剪面”的内部，那么这个“左裁剪面”被设置为不
		* 激活的(inactive)，因为，这个节点的所有的孩子都在“左裁剪面”内，不需
		* 要再与“左裁剪面”比较了。
		*/
		int mPlaneQuantity;
		HPlane mPlane[MAX_PLANE_QUANTITY];
		unsigned int mPlaneState;

		/// 可见集
		VisibleSet mVisibleSet;

		Movable *mCurCullingScene;
	};
	typedef Pointer0<Culler > CullerPtr;
#include "PX2Culler.inl"

}

#endif