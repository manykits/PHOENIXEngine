// PX2Renderable.hpp

#ifndef PX2RENDERABLE_HPP
#define PX2RENDERABLE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Movable.hpp"
#include "PX2IndexBuffer.hpp"
#include "PX2VertexBuffer.hpp"
#include "PX2VertexFormat.hpp"
#include "PX2MaterialInstance.hpp"
#include "PX2Light.hpp"
#include "PX2DynamicBufferObject.hpp"
#include "PX2Shine.hpp"
#include "PX2Texture2D.hpp"

namespace PX2
{

	class Renderer;

	typedef void(*OnDrawCallback) (Renderer *renderer, Renderable *renderable);

	/// 渲染对象类
	/**
	* 所有渲染对象都从此类派生。
	*/
	class PX2_ENGINE_ITEM Renderable : public Movable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Renderable);

	public:
		enum PrimitiveType
		{
			PT_NONE,  //< default constructor
			PT_POLYPOINT,
			PT_POLYSEGMENTS_DISJOINT,
			PT_POLYSEGMENTS_CONTIGUOUS,
			PT_TRIANGLES,  //< abstract
			PT_TRIMESH,
			PT_TRISTRIP,
			PT_TRIFAN,
			PT_MAX_QUANTITY
		};

	protected:
		/**
		* 抽象基类
		*/
		Renderable (PrimitiveType type = PT_NONE);
		Renderable (PrimitiveType type, VertexFormat* vformat, VertexBuffer* vbuffer,
			IndexBuffer* ibuffer);
	public:
		virtual ~Renderable ();

		inline PrimitiveType GetPrimitiveType () const;

		// vb ib
		inline void SetVertexFormat (VertexFormat* vformat);
		inline const VertexFormat* GetVertexFormat () const;
		inline VertexFormat* GetVertexFormat ();
		inline void SetVertexBuffer (VertexBuffer* vbuffer);
		inline const VertexBuffer* GetVertexBuffer () const;
		inline VertexBuffer* GetVertexBuffer ();
		inline void SetIndexBuffer (IndexBuffer* ibuffer);
		inline const IndexBuffer* GetIndexBuffer () const;
		inline IndexBuffer* GetIndexBuffer ();
		inline void SetIBOffset (int offset);
		inline int GetIBOffset () const;
		inline void SetIBNumElements (int num);
		inline int GetIBNumElements () const;

		// share vb ib
		inline void SetShareDBObject_V (DBObject_V *obj);
		inline const DBObject_V *GetShareDBObject_V () const;
		inline DBObject_V *GetShareDBObject_V ();
		inline void SetShareDBObject_I (DBObject_I *obj);
		inline const DBObject_I *GetShareDBObject_I () const;
		inline DBObject_I *GetShareDBObject_I ();
		void SetUseShareBuffers (bool use);
		bool IsUseShareBuffers () const;

		// bound
		inline const Bound& GetModelBound () const;
		inline Bound& GetModelBound ();
		void SetUseBoundPick (bool isUseBoundPick);
		bool IsUseBoundPick () const;

		/// 渲染层级
		enum RenderLayer
		{
			RL_BACKGROUND = 0,
			RL_SKY,
			RL_TERRAIN,
			RL_SCENE,
			RL_UIMASK,
			RL_UI,
			RL_MAX
		};
		void SetRenderLayer (RenderLayer layer, int sublayer=0);
		RenderLayer GetRenderLayer () const;
		int GetSubLayer () const;

		unsigned int GetSortIndex () const;

		// 绘制几何图形的效果实例
		inline void SetMaterialInstance (MaterialInstance* material);
		inline MaterialInstance* GetMaterialInstance () const;

		void SetFakeTransparent(bool fakeTrasparent);
		bool IsFakeTransparent() const;

		// 灯光
		/*
		* 当前渲染体收到影响的灯光
		*/
		bool AddLight (Light *light);
		bool RemoveLight (Light *light);		
		void ClearLights ();
		int GetNumLights () const;
		Light *GetLight (int i);
		const Light *GetLight(int i) const;

		// Shine
		void SetShine (Shine *shine);
		Shine *GetShine ();
		const Shine *GetShine () const;

		Shine *GetBakeShine ();
		const Shine *GetBakeShine () const;

		void SetLightTexture (Texture2D *tex);
		const std::string &GetLightTexture () const;
		const std::string &GetNormalTexture () const;

		void SetBakeObject (bool bakeObject);
		bool IsBakeObject () const;
		void SetBakeTarget (bool bakeTarget);
		bool IsBakeTarget () const;
		
		// Bake
		enum BakeSizeType
		{
			BST_NORMAL,
			BST_64,
			BST_128,
			BST_256, 
			BST_512,
			BST_1024,
			BST_MAX_TYPE
		};
		void SetBakeSizeType (BakeSizeType type);
		BakeSizeType GetBakeSizeType () const;
		
		void SetUseLightTexture (bool light, Texture2D *lightTex);
		bool IsUseLightTexture () const;

		virtual void SetCastShadow(bool castShadow);
		virtual void SetReceiveShadow(bool reciveShadow);

		void SetFogInfulenceParam_Height (float param);
		float GetFogInfulenceParam_Height () const;

		void SetFogInfulenceParam_Distance (float param);
		float GetFogInfulenceParam_Distance () const;

		// 几何图形跟新类型
		enum UpdateType
		{
			GU_MODEL_BOUND_ONLY = -3,
			GU_NORMALS = -2,
			GU_USE_GEOMETRY = -1,
			GU_USE_TCOORD_CHANNEL = 0
		};

		/// 可视对象模型更新
		/**
		* 如果可视对象的VertexBuffer被修改了，你需要更新（normals,tangents,
		* bitangents）。传入的type决定更新的规则，如下：
		* GU_MODEL_BOUND_ONLY
		*	顶点的空间位置改变了，只更新包围盒。
		* 对于其他更新，包围盒也都会更新。如果可视对象是从Triangles派生的话，
		* 如果有法线信息，会首先更新法线信息。
		* GU_NORMALS
		*	更新完法线信息，不会再跟新纹理坐标空间。
		* GU_USE_GEOMETRY
		* GU_USE_TCOORD_CHANNEL + nonnegative_integer
		*	使用第nonnegative_integer层纹理坐标跟新纹理坐标空间。
		* 为了节省显存，你可以考虑只传入normals，tangents，然后使用像素着色器
		* 计算bitangent。
		*	bitangent = Cross(normal, tangent)			
		*/
		virtual void UpdateModelSpace (UpdateType type);

		virtual void OnForceBind ();

		void SetOnDrawCallback(OnDrawCallback dcb);
		OnDrawCallback GetOnDrawCallback();
		void OnDraw(Renderer *renderer);

		// 物理
		enum PhysicsType
		{
			PHYS_NONE,
			PHYS_BOX,
			PHYS_SPHERE,
			PHYS_CONVEXHULL,
			PHYS_CONVEXTRIMESH,
			PHYS_HEIGHTFIELD,
			PHYS_MAX_TYPE
		};
		void SetPhysicsType (PhysicsType type);
		PhysicsType GetPhysicsType () const;

		void SetPhysicsParam (const Float3 &param);
		const Float3 &GetPhysicsParam () const;

public_internal:
		float mEyeDistance; // 相机距离
		static bool LessThan (const Renderable *renderable0,
			const Renderable *renderable1);

	protected:
		void AdjustTransparent ();
		void _SetTransparent (bool transparent);

	public_internal:
		bool IsTransparent () const;
		Float4 UpdateFogParam (const Float4 &envFogParam) const;

		// shareDraw
		void EnableShareDraw(bool enableShareDraw);
		bool IsEnableShareDraw() const;
		enum PreDrawRetType
		{
			PBRT_NONE,
			PDRT_VB_FULL,
			PDRT_IB_FULL,
			PDRT_MTL_NOTEQUAL,
			PDRT_MAX_TYPE
		};
		virtual PreDrawRetType OnPreShareDraw(Renderable *shareMesh);
		virtual void OnShareDraw(Renderable *shareMesh);

	protected:
		// 几何图形更新
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		virtual void UpdateWorldBound ();
		virtual void UpdateModelBound ();

		// 场景裁剪
		virtual void OnGetVisibleSet (Culler& culler, bool noCull);

	protected:
		// 渲染作用
		unsigned int mSortIndex; // (layer<<24)|(sublayer<<16)|current

		// 几何图形数据
		PrimitiveType mType;
		VertexFormatPtr mVFormat;
		VertexBufferPtr mVBuffer;
		IndexBufferPtr mIBuffer;		
		Bound mModelBound;
		bool mIsUseBoundPick;

		DBObject_V *mDBObject_V;
		DBObject_I *mDBObject_I;
		bool mIsUseShareBuffers;

		int mIBOffset;
		int mIBNumElements;

		ShinePtr mDefaultShine;
		ShinePtr mBakeShine;

		bool mIsBackObject;
		bool mIsBackTarget;
		BakeSizeType mBakeSizeType;
		bool mIsUseLightTexture;
		std::string mLightTexPath;
		std::string mNormalTexPath;
		MaterialInstancePtr mNormalMaterialInstance;

		// 绘制几何图形的效果
		MaterialInstancePtr mMaterialInstance;

		RenderLayer mLayer;
		int mSubLayer;

		// 当前对象受影响的灯光
		/*
		* 灯光不用持久化，在每次渲染之前计算受影响的灯光。
		*/
		std::vector<LightPtr> mInfulencedLights; 

		float mFogIP_Height;
		float mFogIP_Distance;

		PhysicsType mPhysicsType;
		Float3 mPhysicsParam;

		bool mIsFakeTransparent;

		bool mIsEnableShareDraw;
		OnDrawCallback mOnDrawCallback;
	};

	PX2_REGISTER_STREAM(Renderable);
	typedef Pointer0<Renderable> RenderablePtr;
#include "PX2Renderable.inl"

}

#endif
