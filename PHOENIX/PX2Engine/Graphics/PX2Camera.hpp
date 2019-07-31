// PX2Camera.hpp

#ifndef PX2CAMERA_HPP
#define PX2CAMERA_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2HMatrix.hpp"
#include "PX2Float4.hpp"
#include "PX2Size.hpp"
#include "PX2APoint.hpp"
#include "PX2Vector2.hpp"
#include "PX2Rect.hpp"

namespace PX2
{
	class CameraNode;
	class Renderer;

	typedef void(*CameraDrawCallback) (Renderer *renderer);

	/// 相机类
	class PX2_ENGINE_ITEM Camera : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Camera);

	public:
		/**
		* 缺省相机，field-of-view为90，aspect ratio为1，near value为1，far		
		* value为10000。
		*/
		Camera (bool isPerspective = true);
		virtual ~Camera ();

		void SetCameraNode(CameraNode *camNode);
		CameraNode *GetCameraNode();

		// 相机总是在世界坐标系
		//   default position  P = (0, 0,  0; 1)
		//   default direction D = (0, 0, -1; 0)
		//   default up        U = (0, 1,  0; 0)
		//   default right     R = (1, 0,  0; 0)
		void SetFrame (const APoint& position, const AVector& dVector,
			const AVector& uVector, const AVector& rVector);

		void SetPosition (const APoint& position);

		void SetAxes (const AVector& dVector, const AVector& uVector,
			const AVector& rVector);

		inline APoint GetPosition () const;
		inline AVector GetDVector () const;
		inline AVector GetUVector () const;
		inline AVector GetRVector () const;

		/// 获得相机矩阵
		/**
		* 如果D = (d0,d1,d2)，U = (u0,u1,u2)，R = (r0,r1,r2)相机矩阵是
		*	+-                     -+
		*	| r0  r1  r2  -Dot(R,P) |
		*	| u0  u1  u2  -Dot(U,P) |
		*	| d0  d1  d2  -Dot(D,P) |
		*	|  0   0   0          1 |
		*	+-                     -+
		* 矩阵左乘向量，viewMat*vector4
		*/
		inline const HMatrix& GetViewMatrix () const;

		/// 截头体匹配信息
		/**
		* 截头体参数是[rmin,rmax], [umin,umax]，和[dmin,dmax]。区间[rmin,rmax]
		* 是在R方向上进行的测量，记录了left和right信息；[umin,umax]是在U方向上
		* 进行的测量,记录了bottom和top信息；[dmin,dmax]是在D方向上进行的测量，
		* 记录了near和far信息。截头体信息被存放在一个数组里，按照下面的信息进行
		* 匹配。
		*/
		enum ViewFrustum
		{
			VF_DMIN     = 0,  //< near
			VF_DMAX     = 1,  //< far
			VF_UMIN     = 2,  //< bottom
			VF_UMAX     = 3,  //< top
			VF_RMIN     = 4,  //< left
			VF_RMAX     = 5,  //< right
			VF_QUANTITY = 6
		};

		void SetPerspective (bool per);
		inline bool IsPerspective () const;

		/// 设置截头体
		void SetFrustum (float dMin, float dMax, float uMin, float uMax,
			float rMin, float rMax);

		/// 设置截头体
		/**
		* 截头体数组的存放顺序如下：dmin, dmax, umin, umax, rmin, rmax。
		*/
		void SetFrustum (const float* frustum);

		/// 设置对称的截头体
		/**
		* 设置一个对称的截头体(umin = -umax，rmin = -rmax)使用一个上下方向的角
		* 度fUpFovDegrees和fAspectRatio(width/height)。fUpFovDegrees的范围必须
		* 在区间(0,180)中。
		*/
		void SetFrustum (float upFovDegrees, float aspectRatio, float dMin,
			float dMax);

		/// 获得相机截头体
		void GetFrustum (float& dMin, float& dMax, float& uMin, float& uMax,
			float& rMin, float& rMax) const;

		/// 获得相机截头体数组
		inline const float* GetFrustum () const;

		/// 获得截头体信息
		/**
		* 函数返回真是，获得的参数才有效。
		*/
		bool GetFrustum (float& upFovDegrees, float& aspectRatio, float& dMin,
			float& dMax) const;

		// 获得单独的相机街头参数
		inline float GetDMin () const;
		inline float GetDMax () const;
		inline float GetUMin () const;
		inline float GetUMax () const;
		inline float GetRMin () const;
		inline float GetRMax () const;

		// 相机透视矩阵类型
		/**
		* 在Direct3D中，深度矩阵的深度被匹配到[0,1]，OpenGL中，深度矩阵的深度被
		* 匹配到[-1,1]。
		*/
		enum DepthType
		{
			PM_DEPTH_ZERO_TO_ONE,       // [0,1]
			PM_DEPTH_MINUS_ONE_TO_ONE,  // [-1,1]
			PM_DEPTH_QUANTITY
		};

		inline DepthType GetDepthType () const;

		/// 获得相机的透视矩阵
		/**
		* 透视矩阵依赖于msDepthType和mIsPerspective。矩阵右乘向量projMat*vector4。
		* 截头体的值为：N (near), F (far), B (bottom), T (top), L (left), 和
		* R (right)。各个透视矩阵的形式如下：
		*	perspective, depth [0,1]
		*	+-                                               -+
		*	| 2*N/(R-L)  0           -(R+L)/(R-L)  0          |
		*	| 0          2*N/(T-B)   -(T+B)/(T-B)  0          |
		*	| 0          0           F/(F-N)       -N*F/(F-N) |
		*	| 0          0           1             0          |
		*	+-                                               -+
		*
		*	perspective, depth [-1,1]
		*	+-                                                 -+
		*	| 2*N/(R-L)  0           -(R+L)/(R-L)  0            |
		*	| 0          2*N/(T-B)   -(T+B)/(T-B)  0            |
		*	| 0          0           (F+N)/(F-N)   -2*F*N/(F-N) |
		*	| 0          0           1             0			|
		*	+-                                                 -+
		*
		*	orthographic, depth [0,1]
		*	+-                                       -+
		*	| 2/(R-L)  0  0              -(R+L)/(R-L) |
		*	| 0        2/(T-B)  0        -(T+B)/(T-B) |
		*	| 0        0        1/(F-N)  -N/(F-N)  0  |
		*	| 0        0        0        1            |
		*	+-                                       -+
		*
		*	orthographic, depth [-1,1]
		*	+-                                       -+
		*	| 2/(R-L)  0        0        -(R+L)/(R-L) |
		*	| 0        2/(T-B)  0        -(T+B)/(T-B) |
		*	| 0        0        2/(F-N)  -(F+N)/(F-N) |
		*	| 0        0        0        1            |
		*	+-                                       -+
		*/
		inline const HMatrix& GetProjectionMatrix () const;

		// 获得projection-view矩阵
		/**
		* 为了避免重复计算，我们将projection-view存储在这个类里。
		*/
		inline const HMatrix& GetProjectionViewMatrix () const;

		/// 设置preview matrix
		/**
		* 这个矩阵在model-to-world之后，view matrix之前，被设置。这个矩阵被用来
		* 做对象反射，缺省值是一个单位矩阵。
		*/
		void SetPreViewMatrix (const HMatrix& preViewMatrix);
		inline const HMatrix& GetPreViewMatrix () const;
		inline bool PreViewIsIdentity () const;

		/// 设置postprojection matrix
		/**
		* 这个矩阵用来做screen-space变换，例如rendered image的变换。缺省值是一
		* 个单位矩阵。
		*/
		void SetPostProjectionMatrix (const HMatrix& postProjMatrix);
		inline const HMatrix& GetPostProjectionMatrix () const;
		inline bool PostProjectionIsIdentity () const;

		bool GetPickRay(float sizePosX, float sizePosZ, const Sizef &size,
			APoint& origin, AVector& direction);

		bool GetPickRay(float posSizePercentWdith, float posSizePercentHeight,
			APoint& origin, AVector& direction);

		Vector2f WorldPos3DTo2D(const Rectf& viewPort, const APoint &screenPos, 
			bool *isInBack=0);
		Vector2f WorldDir3DTo2D(const AVector &worldDir);

		void SetCameraDrawCallback(CameraDrawCallback callback);
		CameraDrawCallback GetCameraDrawCallback();

	protected:
		// 相机改变后，重新计算取景矩阵
		void OnFrameChange ();

		// 在截头体改变之后，计算透视矩阵。
		void OnFrustumChange ();

		// Compute the product postproj-proj-view-preview.
		void UpdatePVMatrix ();

		CameraNode *mCameraNode;

		// The world coordinate frame.
		APoint mPosition;
		AVector mDVector, mUVector, mRVector;

		// 相机的view matrix。
		HMatrix mViewMatrix;

		// 相机截头体掰存储在此数组里 dmin (near), dmax (far), umin (bottom), 
		// umax (top), rmin (left), and rmax (right).
		float mFrustum[VF_QUANTITY];

		// 相机透视矩阵
		HMatrix mProjectionMatrix[PM_DEPTH_QUANTITY];

		// 透视矩阵乘以取景矩阵。
		HMatrix mProjectionViewMatrix[PM_DEPTH_QUANTITY];

		// preview matrix
		HMatrix mPreViewMatrix;
		bool mPreViewIsIdentity;

		// 相机的postprojection matrix
		HMatrix mPostProjectionMatrix;
		bool mPostProjectionIsIdentity;

		/// 为'true'表示一个透视投影相机，为'false'表示一个正交投影相机。
		bool mIsPerspective;

		// 相机投影矩阵类型。这个值在构造函数调用时，被初始化为
		// msDefaultDepthType。这个值不需要持久化，从此类派生的Projector会覆盖
		// 这个值，并且将其持久化。
		DepthType mDepthType;

		CameraDrawCallback mCameraDrawCallback;

		// 缺省投影矩阵类型（在DirectX和OpenGL不同）。这个值在构造函数中用来初始
		// 化mDepthType。
		static DepthType msDefaultDepthType;

public_internal:
		inline static void SetDefaultDepthType (DepthType type);
		inline static DepthType GetDefaultDepthType ();

		// 宏的原定义在PX2GraphicsPre.hpp中
private_internal:
#ifdef PX2_VALIDATE_CAMERA_FRAME_ONCE
		bool mValidateCameraFrame;
#endif

		// Render
	public:
		void SetClearColor(const Float4 &color);
		const Float4 &GetClearColor() const;

		void SetClearDepth(float depth);
		float GetClearDepth() const;

		void SetClearStencil(unsigned int stencil);
		unsigned int GetClearStencil() const;

		void SetClearFlag(bool color, bool depth, bool stencil);
		void GetClearFlag(bool &color, bool &depth, bool &stencil);

	protected:
		Float4 mClearColor;
		float mClearDepth;
		unsigned int mClearStencil;
		bool mClearFlagColor;
		bool mClearFlagDepth;
		bool mClearFlagStencil;
	};

	PX2_REGISTER_STREAM(Camera);
	typedef Pointer0<Camera> CameraPtr;
#include "PX2Camera.inl"

}

#endif
