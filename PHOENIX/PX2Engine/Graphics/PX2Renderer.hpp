// PX2Renderer.hpp

#ifndef PX2RENDERER_HPP
#define PX2RENDERER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2AlphaProperty.hpp"
#include "PX2Camera.hpp"
#include "PX2CullProperty.hpp"
#include "PX2DepthProperty.hpp"
#include "PX2Float4.hpp"
#include "PX2Vector2.hpp"
#include "PX2Rect.hpp"
#include "PX2HMatrix.hpp"
#include "PX2IndexBuffer.hpp"
#include "PX2Mutex.hpp"
#include "PX2OffsetProperty.hpp"
#include "PX2PixelShader.hpp"
#include "PX2RenderTarget.hpp"
#include "PX2StencilProperty.hpp"
#include "PX2Texture1D.hpp"
#include "PX2Texture2D.hpp"
#include "PX2Texture3D.hpp"
#include "PX2TextureCube.hpp"
#include "PX2VertexBuffer.hpp"
#include "PX2VertexFormat.hpp"
#include "PX2VertexShader.hpp"
#include "PX2VisibleSet.hpp"
#include "PX2Renderable.hpp"
#include "PX2WireProperty.hpp"
#include "PX2Node.hpp"

namespace PX2
{

	class RendererInput;
	class RendererData;
	class GlobalMaterial;

	// 处理与平台相关资源的类platform-dependent resource (pdr)
	class PdrIndexBuffer;
	class PdrPixelShader;
	class PdrRenderTarget;
	class PdrTexture1D;
	class PdrTexture2D;
	class PdrTexture3D;
	class PdrTextureCube;
	class PdrVertexBuffer;
	class PdrVertexFormat;
	class PdrVertexShader;
	class PdrMaterialPass;

	/// 渲染器类
	/**
	* Renderer类是一个抽象接口。通过不同的宏，编译成为各个平台相关的渲染器。这
	* 些平台包括（Direct3D，OpenGL，等等）。Renderer的接口被分为两部分，第一部
	* 分是平台无关的函数和数据，第二部分是平台相关的函数和数据。
	*/
	class PX2_ENGINE_ITEM Renderer
	{
		//============================================================================
		// 与平台无关的接口
	public:
		/// 渲染器构造函数内部调用该函数，进行初始化。
		void Initialize (int width, int height, Texture::Format colorFormat,
			Texture::Format depthStencilFormat, int numMultisamples);

		/// 渲染器析构函数内部调用该函数，进行释放。
		void Terminate ();

		// 通常来说，游戏中只有一个渲染器，调用此接口设置下，方便暴露渲染器接口出来
		static void SetDefaultRenderer (Renderer *renderer);
		static Renderer *GetDefaultRenderer ();

		// 获得构造函数的输入参数
		void SetName(const std::string &name);
		const std::string &GetName() const;
		inline int GetWidth () const;
		inline int GetHeight () const;
		inline Texture::Format GetColorFormat () const;
		inline Texture::Format GetDepthStencilFormat () const;
		inline int GetNumMultisamples () const;

		// === 资源管理
		/*
		* 这些资源是：vertex formats，vertex buffers，index buffers，textures（
		* 1D，2D，3D，cube），render targets，vertex shader，和pixel shader。这
		* 些资源拥有下面的操作

		* Bind：根据输入的对象，创建与之相关的显存资源。渲染器负责mapping对象和
		*	显存资源。如果Enable或者Lock函数在Bind之前被调用，渲染器内部会首先
		*	创建该显存资源，不会产生异常。
		*
		* BindAll：为所有渲染器，创建与输入对象相关联的显存资源。
		*
		* Unbind：释放与输入对象相关联的显存资源。渲染器内部会移除对象和显存资
		*	源的mipping。
		*
		* UnbindAll：释放所有渲染器重，与输入对象相关联的显存资源。
		*
		* Enable：激活资源。
		*
		* Disable：取消激活资源。
		*
		* Lock：获得与输入对象相关的显存资源的指针。（被Lock住的地址在AGP内存中
		*	或是在普通内存中，取决于图形API以及资源的创建属性）使用这个函数更新
		*	显存资源。你可以以只读的方式Lock住，用来读取。尽量不要Lock资源，会
		*	降低速度，越少越好。
		*
		* UnLock：释放Lock住的指针。
		*
		* Update：内部Lock住显存资源，将内存数据拷贝到显示内存中，然后Unlock显存
		*	。这是更新显存资源的最好方式（跟新完对象的内存然后调用Update）。
		*
		* UpdateAll：为所有渲染器，更新与对象相关联的显存资源。
		*
		* ReadColor：这个函数只为RenderTarget设计。在调用ReadColor之前，
		* 	RenderTarget必须先被disabled。这个函数返回RenderTarget颜色显存的一
		*	份Texture2D拷贝。调用者负责管理删除这个返回的Texture2D对象。
		*/

		// 顶点格式管理
		/*
		* vformat必须已经被应用程序分配。
		*/
		void Bind (const VertexFormat* vformat);
		static void BindAll (const VertexFormat* vformat);
		void Unbind (const VertexFormat* vformat);
		static void UnbindAll (const VertexFormat* vformat);
		void Enable (const VertexFormat* vformat);
		void Disable (const VertexFormat* vformat);

		// 顶点缓冲区管理
		/*
		* vbuffer必须已经被应用程序分配。
		*/
		void Bind (const VertexBuffer* vbuffer);
		bool IsBind (const VertexBuffer* vbuffer);
		static bool IsOneBind (const VertexBuffer* vbuffer);
		static void BindAll (const VertexBuffer* vbuffer);
		void Unbind (const VertexBuffer* vbuffer);
		static void UnbindAll (const VertexBuffer* vbuffer);
		void Enable (const VertexBuffer* vbuffer,
			unsigned int streamIndex = 0, unsigned int offset = 0);
		void Disable (const VertexBuffer* vbuffer,
			unsigned int streamIndex = 0);
		void* Lock (const VertexBuffer* vbuffer,
			Buffer::Locking mode);
		void Unlock (const VertexBuffer* vbuffer);
		void Update (const VertexBuffer* vbuffer);
		static void UpdateAll (const VertexBuffer* vbuffer);

		// 顶点索引缓冲区管理
		/*
		* ibuffer必须已经被应用程序分配。
		*/
		void Bind (const IndexBuffer* ibuffer);
		bool IsBind (const IndexBuffer* ibuffer);
		static bool IsOneBind (const IndexBuffer* ibuffer);
		static void BindAll (const IndexBuffer* ibuffer);
		void Unbind (const IndexBuffer* ibuffer);
		static void UnbindAll (const IndexBuffer* ibuffer);
		void Enable (const IndexBuffer* ibuffer);
		void Disable (const IndexBuffer* ibuffer);
		void* Lock (const IndexBuffer* ibuffer, Buffer::Locking mode);
		void Unlock (const IndexBuffer* ibuffer);
		void Update (const IndexBuffer* ibuffer);
		static void UpdateAll (const IndexBuffer* ibuffer);

		// 1D纹理管理
		/*
		* texture必须已经被应用程序分配。
		*/
		void Bind (const Texture1D* texture);
		static void BindAll (const Texture1D* texture);
		void Unbind (const Texture1D* texture);
		static void UnbindAll (const Texture1D* texture);
		void Enable (const Texture1D* texture, int textureUnit);
		void Disable (const Texture1D* texture, int textureUnit);
		void* Lock (const Texture1D* texture, int level,
			Buffer::Locking mode);
		void Unlock (const Texture1D* texture, int level);
		void Update (const Texture1D* texture, int level);
		static void UpdateAll (const Texture1D* texture, int level);

		// 2D纹理管理
		/*
		* texture必须已经被应用程序分配。
		*/
		void Bind (const Texture2D* texture);
		static void BindAll (const Texture2D* texture);
		void Unbind (const Texture2D* texture);
		static void UnbindAll (const Texture2D* texture);
		void Enable (const Texture2D* texture, int textureUnit);
		void Enable (const Texture2D* texture, int textureUnit, int loc); // opengles2
		void Disable (const Texture2D* texture, int textureUnit);
		void* Lock (const Texture2D* texture, int level,
			Buffer::Locking mode);
		void Unlock (const Texture2D* texture, int level);
		void Update (const Texture2D* texture, int level);
		static void UpdateAll (const Texture2D* texture, int level);

		// 3D纹理管理
		/*
		* texture必须已经被应用程序分配。
		*/
		void Bind (const Texture3D* texture);
		static void BindAll (const Texture3D* texture);
		void Unbind (const Texture3D* texture);
		static void UnbindAll (const Texture3D* texture);
		void Enable (const Texture3D* texture, int textureUnit);
		void Disable (const Texture3D* texture, int textureUnit);
		void* Lock (const Texture3D* texture, int level,
			Buffer::Locking mode);
		void Unlock (const Texture3D* texture, int level);
		void Update (const Texture3D* texture, int level);
		static void UpdateAll (const Texture3D* texture, int level);

		// Cube纹理管理
		/*
		* texture必须已经被应用程序分配。
		*/
		void Bind (const TextureCube* texture);
		static void BindAll (const TextureCube* texture);
		void Unbind (const TextureCube* texture);
		static void UnbindAll (const TextureCube* texture);
		void Enable (const TextureCube* texture, int textureUnit);
		void Disable (const TextureCube* texture, int textureUnit);
		void* Lock (const TextureCube* texture, int face, int level,
			Buffer::Locking mode);
		void Unlock (const TextureCube* texture, int face, int level);
		void Update (const TextureCube* texture, int face, int level);
		static void UpdateAll (const TextureCube* texture, int face, int level);

		// Render target管理
		/*
		* renderTarget必须已经被应用程序分配。
		* ReadColor函数中，如果传递的texture指针为NULL，ReadColor会为你创建相应
		* 的纹理；否则你需要自己创建正确顶点格式，正确大小的纹理。
		*/
		void Bind (const RenderTarget* renderTarget);
		static void BindAll (const RenderTarget* renderTarget);
		void Unbind (const RenderTarget* renderTarget);
		static void UnbindAll (const RenderTarget* renderTarget);
		void Enable (const RenderTarget* renderTarget);
		void Disable (const RenderTarget* renderTarget);
		void ReadColor (int i, const RenderTarget* renderTarget,
			Texture2D*& texture);

		// Vertex shader管理
		/*
		* vshader必须已经被应用程序创建。
		*/
		void Bind (const VertexShader* vshader);
		static void BindAll (const VertexShader* vshader);
		void Unbind (const VertexShader* vshader);
		static void UnbindAll (const VertexShader* vshader);
		void Enable (const VertexShader* vshader,
			const ShaderParameters* parameters);
		void Disable (const VertexShader* vshader,
			const ShaderParameters* parameters);

		// Pixel shader管理
		/*
		* pshader必须已经被应用程序创建。
		*/
		void Bind (const PixelShader* pshader);
		static void BindAll (const PixelShader* pshader);
		void Unbind (const PixelShader* pshader);
		static void UnbindAll (const PixelShader* pshader);
		void Enable (const PixelShader* pshader,
			const ShaderParameters* parameters);
		void Disable (const PixelShader* pshader,
			const ShaderParameters* parameters);

		void ForceBind (Movable *mov);
		void ForceBind (Renderable *renderable);

		void OnLostDevice();
		void OnRestoreDevice();

		int GetNumPdrVertexFormat () const;
		int GetNumPdrVertexBuffer () const;
		int GetNumPdrIndexBuffer () const;
		int GetNumPdrTexture1D () const;
		int GetNumPdrTexture2D () const;
		int GetNumPdrTexture3D () const;
		int GetNumPdrTextureCube () const;
		int GetNumPdrRenderTarget () const;
		int GetNumPdrVertexShader () const;
		int GetNumPdrPixelShader () const;
		int GetNumPdrMaterialPass () const;

		int GetNumGPUVertexShader () const;
		int GetNumGPUPixelShader () const;
		int GetNumGPUMaterialPass () const;

		int GetNumDrawPrimitivePerFrame () const;

		// === End resource management.

		// 全局渲染状态访问
		inline const AlphaProperty* GetAlphaProperty () const;
		inline const CullProperty* GetCullProperty () const;
		inline const DepthProperty* GetDepthProperty () const;
		inline const OffsetProperty* GetOffsetProperty () const;
		inline const StencilProperty* GetStencilProperty () const;
		inline const WireProperty* GetWireProperty () const;

		// 当前相机管理
		inline void SetCamera (Camera* camera);
		inline Camera* GetCamera ();
		inline const Camera* GetCamera () const;
		inline const HMatrix& GetViewMatrix () const;
		inline const HMatrix& GetProjectionMatrix () const;
		inline const HMatrix& GetPostProjectionMatrix () const;

		// ViewPort
		void SetViewport (const Rectf &rect);
		const Rectf &GetViewPort() const;

		/// 获得选择射线
		/**
		* 根据输入右手坐标系下(原点在左下角，y轴向上，x轴向右)的坐标(x,y)和当前
		* 的相机，获得选择射线。输入参数origin是相机的位置，direction是单位长度
		* 的方向向量，他们都是在世界坐标系中的。如果(x,y)在视口范围内，返回
		* “true”。
		*/
		bool GetPickRay (int x, int y, APoint& origin, AVector& direction) const;

		/// 获得3d点在屏幕的坐标
		Vector2f PointWorldToViewPort (const APoint &point, bool *isInBack=0);

		// 设置清空颜色，清空深度，清空模板值。
		inline void SetClearColor (const Float4& clearColor);
		inline const Float4& GetClearColor () const;
		inline void SetClearDepth (float clearDepth);
		inline float GetClearDepth () const;
		inline void SetClearStencil (unsigned int clearStencil);
		inline unsigned int GetClearStencil () const;

		// 颜色通道mask访问。
		inline void GetColorMask (bool& allowRed, bool& allowGreen,
			bool& allowBlue, bool& allowAlpha) const;

		// 设置优先渲染状态
		/*
		* 如果设置了优先渲染状态，其会凌驾于MaterialPass的渲染状态之上。取消优先
		* 渲染状态，只要传递0指针。
		*/
		inline void SetOverrideAlphaProperty (const AlphaProperty* alphaState);
		inline void SetOverrideCullProperty (const CullProperty* cullState);
		inline void SetOverrideDepthProperty (const DepthProperty* depthState);
		inline void SetOverrideOffsetProperty (const OffsetProperty* offsetState);
		inline void SetOverrideStencilProperty (const StencilProperty* stencilState);
		inline void SetOverrideWireProperty (const WireProperty* wireState);
		inline const AlphaProperty* GetOverrideAlphaProperty () const;
		inline const CullProperty* GetOverrideCullProperty () const;
		inline const DepthProperty* GetOverrideDepthProperty () const;
		inline const OffsetProperty* GetOverrideOffsetProperty () const;
		inline const StencilProperty* GetOverrideStencilProperty () const;
		inline const WireProperty* GetOverrideWireProperty () const;
		inline void SetReverseCullOrder (bool reverseCullOrder);
		inline bool GetReverseCullOrder () const;

		// 绘制整个场景可见集的入口
		void Draw (const VisibleSet& visibleSet, GlobalMaterial* globalMaterial = 0);

		// 绘制单个对象的入口
		void Draw (Renderable* renderable);
		void Draw (Renderable* renderable, const MaterialInstance* instance);

	private:
		static Mutex &GetMutex ();
		// 释放所有与此渲染器相关的资源。(渲染器析构函数内会调用，无需现显式调
		// 用资源释放函数)
		void DestroyAllVertexFormats ();
		void DestroyAllVertexBuffers ();
		void DestroyAllIndexBuffers ();
		void DestroyAllTexture1Ds ();
		void DestroyAllTexture2Ds ();
		void DestroyAllTexture3Ds ();
		void DestroyAllTextureCubes ();
		void DestroyAllRenderTargets ();
		void DestroyAllVertexShaders ();
		void DestroyAllPixelShaders ();
		void DestroyAllMaterialPasses ();

		// 渲染器构造参数
		std::string mName;
		int mWidth;
		int mHeight;
		Texture::Format mColorFormat;
		Texture::Format mDepthStencilFormat;
		int mNumMultisamples;
		Rectf mViewPort;

		// 全局渲染状态
		AlphaProperty* mDefaultAlphaProperty; // 缺省状态
		CullProperty* mDefaultCullProperty;
		DepthProperty* mDefaultDepthProperty;
		OffsetProperty* mDefaultOffsetProperty;
		StencilProperty* mDefaultStencilProperty;
		WireProperty* mDefaultWireProperty;
		const AlphaProperty* mAlphaProperty; // 当前设置的状态
		const CullProperty* mCullProperty;
		const DepthProperty* mDepthProperty;
		const OffsetProperty* mOffsetProperty;
		const StencilProperty* mStencilProperty;
		const WireProperty* mWireProperty;
		bool mReverseCullOrder;

		// 优先渲染状态
		const AlphaProperty* mOverrideAlphaProperty; // override状态
		const CullProperty* mOverrideCullProperty;
		const DepthProperty* mOverrideDepthProperty;
		const OffsetProperty* mOverrideOffsetProperty;
		const StencilProperty* mOverrideStencilProperty;
		const WireProperty* mOverrideWireProperty;

		/// 当前几何图形变换状态中的相机，相机会存储view，projection和
		/// postprojection矩阵。
		Camera* mCamera;

		// Framebuffer 清空参数
		Float4 mClearColor;
		float mClearDepth;
		unsigned int mClearStencil;

		// 后背缓冲区颜色模板值
		bool mAllowRed, mAllowGreen, mAllowBlue, mAllowAlpha;

		// 激活的资源
		typedef std::map<const VertexFormat*, PdrVertexFormat*> VertexFormatMap;
		typedef std::map<const VertexBuffer*, PdrVertexBuffer*> VertexBufferMap;
		typedef std::map<const IndexBuffer*, PdrIndexBuffer*> IndexBufferMap;
		typedef std::map<const Texture1D*, PdrTexture1D*> Texture1DMap;
		typedef std::map<const Texture2D*, PdrTexture2D*> Texture2DMap;
		typedef std::map<const Texture3D*, PdrTexture3D*> Texture3DMap;
		typedef std::map<const TextureCube*, PdrTextureCube*> TextureCubeMap;
		typedef std::map<const RenderTarget*, PdrRenderTarget*> RenderTargetMap;
		typedef std::map<const VertexShader*, PdrVertexShader*> VertexShaderMap;
		typedef std::map<const PixelShader*, PdrPixelShader*> PixelShaderMap;
		typedef std::map<const MaterialPass*, PdrMaterialPass*> MaterialPassMap;
		VertexFormatMap mVertexFormats;
		VertexBufferMap mVertexBuffers;
		IndexBufferMap mIndexBuffers;
		Texture1DMap mTexture1Ds;
		Texture2DMap mTexture2Ds;
		Texture3DMap mTexture3Ds;
		TextureCubeMap mTextureCubes;
		RenderTargetMap mRenderTargets;
		VertexShaderMap mVertexShaders;
		PixelShaderMap mPixelShaders;
		MaterialPassMap mMaterialPasses;

		VertexFormatPtr mLastVertexFormat;
		VertexBufferPtr mLastEnableVertexBuffer;
		IndexBufferPtr mLastEnableIndexBuffer;
		VertexShaderPtr mLastVertexShader;
		PixelShaderPtr mLastPixelShader;
		MaterialInstancePtr mLastMaterialInstance;

		// 渲染器管理
		typedef std::set<Renderer*> RendererSet;
		static RendererSet msRenderers;
		static Renderer *msDefaultRenderer;

		int mNumDrawPrimitivePerFrame;

		//============================================================================
		// 平台相关接口
	public:
		Renderer (RendererInput *input, int width, int height,
			Texture::Format colorFormat, Texture::Format depthStencilFormat,
			int numMultisamples);
		~Renderer ();

		static Renderer *CreateRenderer(void *ptData, void *ptDataLinuxWindow,
			void *ptDataLinuxVisual, int width, int height,
			int numMultisamples, RendererInput* &renderInput);
		static std::string GetRenderTag();

		void InitRenderStates ();

		// Pass 管理
		void Bind (const MaterialPass *pass);
		static void BindAll (const MaterialPass *pass);
		void Unbind (const MaterialPass *pass);
		static void UnbindAll (const MaterialPass *pass);
		void Enable (const Renderable* renderable,
			const MaterialInstance* instance, int index);
		void Disable (const Renderable* renderable,
			const MaterialInstance* instance, int index);

		// Renderable的渲染状态管理
		void SetAlphaProperty (const AlphaProperty* alphaState);
		void SetCullProperty (const CullProperty* cullState);
		void SetDepthProperty (const DepthProperty* depthState);
		void SetOffsetProperty (const OffsetProperty* offsetState);
		void SetStencilProperty (const StencilProperty* stencilState);
		void SetWireProperty (const WireProperty* wireState);

		// Viewport管理
		/*
		* viewport是右手坐标系。原点在屏幕左下角，y轴向上，x轴向右。
		*/
		void SetViewport (int xPosition, int yPosition, int width,
			int height);
		void GetViewport (int& xPosition, int& yPosition, int& width,
			int& height) const;
		void SetDepthRange (float zMin, float zMax);
		void GetDepthRange (float& zMin, float& zMax) const;
		void Resize (int width, int height);

		// 窗口
		void ResizeWindow (int width, int height);

		// 清空颜色，深度，和模板支持
		void ClearColorBuffer ();
		void ClearDepthBuffer ();
		void ClearStencilBuffer ();
		void ClearBuffers ();
		void ClearColorBuffer (int x, int y, int w, int h);
		void ClearDepthBuffer (int x, int y, int w, int h);
		void ClearStencilBuffer (int x, int y, int w, int h);
		void ClearBuffers (int x, int y, int w, int h);
		void DisplayColorBuffer ();

		// 设置颜色mask
		void SetColorMask (bool allowRed, bool allowGreen,
			bool allowBlue, bool allowAlpha);

		// 所有渲染接口调用都必须在函数PreDraw ()和PostDraw ()之间。通常的格式如下：
		//   if (renderer->PreDraw())
		//   {
		//       <渲染函数调用>;
		//       renderer->PostDraw();
		//   }
		bool PreDraw ();
		void PostDraw ();

		// 绘制2Dbuffers的入口
		void Draw (const unsigned char* screenBuffer, bool reflectY = false);

		// 绘制2D文字的入口
		void Draw (int x, int y, const Float4& color,
			const std::string& message);

	private:
		// 绘制3d对象的入口点函数
		void DrawPrimitive (const Renderable* renderable);

public_internal:
		/// 与平台相关信息
		/**
		* 放在公有接口里，容许资源类进行访问。
		*/
		RendererData* mData;
		const Renderable *mCurRenderable;

		PdrVertexFormat* GetResource (const VertexFormat* vformat);
		PdrVertexBuffer* GetResource (const VertexBuffer* vbuffer);
		PdrIndexBuffer* GetResource (const IndexBuffer* ibuffer);
		PdrTexture1D* GetResource (const Texture1D* texture);
		PdrTexture2D* GetResource (const Texture2D* texture);
		PdrTexture3D* GetResource (const Texture3D* texture);
		PdrTextureCube* GetResource (const TextureCube* texture);
		PdrRenderTarget* GetResource (const RenderTarget* renderTarget);
		PdrVertexShader* GetResource (const VertexShader* vshader);
		PdrPixelShader* GetResource (const PixelShader* pshader);
		PdrMaterialPass* GetResource (const MaterialPass* pass);

		// 创建的render target管理
		inline bool InTexture2DMap (const Texture2D* texture);
		inline void InsertInTexture2DMap (const Texture2D* texture,
			PdrTexture2D* platformTexture);

		class SharePdrData
		{
		public:
			SharePdrData ();
			~SharePdrData ();

			void *DataPtr;

			int DataInt;
			char ES_V_CLoc[16];
			char ES_V_TLoc[16];
			char ES_P_CLoc[16];
			char ES_P_TLoc[16];
		};
		std::map<int, SharePdrData> mSharePdrVertexShaders;
		std::map<int, SharePdrData> mSharePdrPixelShaders;
		std::map<int, SharePdrData> mSharePdrMaterialPasses;
		void ClearSharePdrData ();
		//============================================================================
	};

#include "PX2Renderer.inl"

}

#endif
