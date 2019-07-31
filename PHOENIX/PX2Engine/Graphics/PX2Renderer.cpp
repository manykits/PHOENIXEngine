// PX2Renderer.cpp

#include "PX2Renderer.hpp"
#include "PX2GlobalMaterial.hpp"
#include "PX2Canvas.hpp"

#ifdef PX2_USE_DX9
#include "PX2Dx9IndexBuffer.hpp"
#include "PX2Dx9PixelShader.hpp"
#include "PX2Dx9RenderTarget.hpp"
#include "PX2Dx9Shader.hpp"
#include "PX2Dx9Texture1D.hpp"
#include "PX2Dx9Texture2D.hpp"
#include "PX2Dx9Texture3D.hpp"
#include "PX2Dx9TextureCube.hpp"
#include "PX2Dx9VertexBuffer.hpp"
#include "PX2Dx9VertexFormat.hpp"
#include "PX2Dx9VertexShader.hpp"
#endif

#ifdef PX2_USE_OPENGLES
#include "Renderers/OpenGLESRenderer/PX2OpenGLESIndexBuffer.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESPixelShader.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESRendererData.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESShader.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESTexture1D.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESTexture2D.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESTexture3D.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESTextureCube.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESRenderTarget.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESVertexBuffer.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESVertexFormat.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESVertexShader.hpp"
#include "Renderers/OpenGLESRenderer/PX2OpenGLESMaterialPass.hpp"
#endif

#ifdef PX2_USE_OPENGL
#include "Renderers/OpenGLRenderer/PX2OpenGLIndexBuffer.hpp"
#include "Renderers/OpenGLRenderer/PX2OpenGLRenderTarget.hpp"
#include "Renderers/OpenGLRenderer/PX2OpenGLShader.hpp"
#include "Renderers/OpenGLRenderer/PX2OpenGLTexture1D.hpp"
#include "Renderers/OpenGLRenderer/PX2OpenGLTexture2D.hpp"
#include "Renderers/OpenGLRenderer/PX2OpenGLTexture3D.hpp"
#include "Renderers/OpenGLRenderer/PX2OpenGLTextureCube.hpp"
#include "Renderers/OpenGLRenderer/PX2OpenGLVertexBuffer.hpp"
#include "Renderers/OpenGLRenderer/PX2OpenGLVertexFormat.hpp"
#include "Renderers/OpenGLRenderer/PX2OpenGLVertexShader.hpp"
#include "Renderers/OpenGLRenderer/PX2OpenGLPixelShader.hpp"
#endif

using namespace PX2;

std::set<Renderer*> Renderer::msRenderers;
Renderer *Renderer::msDefaultRenderer = 0;

//----------------------------------------------------------------------------
void Renderer::Initialize (int width, int height, Texture::Format colorFormat,
						   Texture::Format depthStencilFormat,
						   int numMultisamples)
{
	assertion(width > 0, "Width must be positive\n");

	assertion(height > 0, "Height must be positive\n");

	assertion(depthStencilFormat == Texture::TF_D24S8,
		"Only 24-bit depth and 8-bit stencil are currently supported\n");

	assertion(
		numMultisamples == 0 || numMultisamples == 2 || numMultisamples == 4,
		"The number of multisamples can be only 0, 2, or 4\n");

	mWidth = width;
	mHeight = height;
	mColorFormat = colorFormat;
	mDepthStencilFormat = depthStencilFormat;
	mNumMultisamples = numMultisamples;
	mDefaultAlphaProperty = new0 AlphaProperty();
	mDefaultCullProperty = new0 CullProperty();
	mDefaultDepthProperty = new0 DepthProperty();
	mDefaultOffsetProperty = new0 OffsetProperty();
	mDefaultStencilProperty = new0 StencilProperty();
	mDefaultWireProperty = new0 WireProperty();
	mAlphaProperty = mDefaultAlphaProperty;
	mCullProperty = mDefaultCullProperty;
	mDepthProperty = mDefaultDepthProperty;
	mOffsetProperty = mDefaultOffsetProperty;
	mStencilProperty = mDefaultStencilProperty;
	mWireProperty = mDefaultWireProperty;
	mReverseCullOrder = false;
	mOverrideAlphaProperty = 0;
	mOverrideCullProperty = 0;
	mOverrideDepthProperty = 0;
	mOverrideOffsetProperty = 0;
	mOverrideStencilProperty = 0;
	mOverrideWireProperty = 0;
	mCamera = 0;
	mClearColor = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	mClearDepth = 1.0f;
	mClearStencil = 0;
	mAllowRed = true;
	mAllowGreen = true;
	mAllowBlue = true;
	mAllowAlpha = true;

	mNumDrawPrimitivePerFrame = 0;

	GetMutex().Enter();
	msRenderers.insert(this);
	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Terminate ()
{
	delete0(mDefaultAlphaProperty);
	delete0(mDefaultCullProperty);
	delete0(mDefaultDepthProperty);
	delete0(mDefaultOffsetProperty);
	delete0(mDefaultStencilProperty);
	delete0(mDefaultWireProperty);

	// 释放所有资源
	DestroyAllVertexFormats();
	DestroyAllVertexBuffers();
	DestroyAllIndexBuffers();
	DestroyAllTexture1Ds();
	DestroyAllTexture2Ds();
	DestroyAllTexture3Ds();
	DestroyAllTextureCubes();
	DestroyAllRenderTargets();
#ifdef PX2_USE_OPENGLES
	DestroyAllMaterialPasses();
#endif
	DestroyAllVertexShaders();
	DestroyAllPixelShaders();

	ClearSharePdrData();

	GetMutex().Enter();
	msRenderers.erase(this);
	GetMutex().Leave();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void Renderer::SetDefaultRenderer (Renderer *renderer)
{
	msDefaultRenderer = renderer;
}
//----------------------------------------------------------------------------
Renderer *Renderer::GetDefaultRenderer ()
{
	return msDefaultRenderer;
}
//----------------------------------------------------------------------------
void Renderer::SetName(const std::string &name)
{
	mName = name;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 顶点格式管理
//----------------------------------------------------------------------------
void Renderer::Bind (const VertexFormat* vformat)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (mVertexFormats.find(vformat) == mVertexFormats.end())
	{
		PdrVertexFormat *pdrVFormat = new0 PdrVertexFormat(this, vformat);
		mVertexFormats[vformat] = pdrVFormat;
		((VertexFormat*)vformat)->PdrPointer = pdrVFormat;
	}
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const VertexFormat* vformat)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Bind(vformat);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const VertexFormat* vformat)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	VertexFormatMap::iterator iter = mVertexFormats.find(vformat);
	if (iter != mVertexFormats.end())
	{
		PdrVertexFormat* dxVFormat = iter->second;
		delete0(dxVFormat);
		mVertexFormats.erase(iter);
		((VertexFormat*)vformat)->PdrPointer = 0;
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const VertexFormat* vformat)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Unbind(vformat);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const VertexFormat* vformat)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrVertexFormat* pdrVFormat = 0;

	if (1 == (int)msRenderers.size())
	{
		if (vformat->PdrPointer)
		{
			pdrVFormat = (PdrVertexFormat*)vformat->PdrPointer;
		}
		else
		{
			pdrVFormat = new0 PdrVertexFormat(this, vformat);
			mVertexFormats[vformat] = pdrVFormat;
			((VertexFormat*)vformat)->PdrPointer = pdrVFormat;
		}
	}
	else
	{
		VertexFormatMap::iterator iter = mVertexFormats.find(vformat);
		if (iter != mVertexFormats.end())
		{
			pdrVFormat = iter->second;
		}
		else
		{
			pdrVFormat = new0 PdrVertexFormat(this, vformat);
			mVertexFormats[vformat] = pdrVFormat;
		}
	}

	pdrVFormat->Enable(this);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const VertexFormat* vformat)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrVertexFormat* pdrVFormat = 0;

	if (1 == (int)msRenderers.size())
	{
		pdrVFormat = (PdrVertexFormat*)vformat->PdrPointer;
		pdrVFormat->Disable(this);
	}
	else
	{
		VertexFormatMap::iterator iter = mVertexFormats.find(vformat);
		if (iter != mVertexFormats.end())
		{
			pdrVFormat = iter->second;
			pdrVFormat->Disable(this);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Vertex buffer管理
//----------------------------------------------------------------------------
void Renderer::Bind (const VertexBuffer* vbuffer)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (mVertexBuffers.find(vbuffer) == mVertexBuffers.end())
	{
		PdrVertexBuffer *pdrVBuffer = new0 PdrVertexBuffer(this, vbuffer);
		mVertexBuffers[vbuffer] = pdrVBuffer;
		((VertexBuffer*)vbuffer)->PdrPointer = pdrVBuffer;
	}
}
//----------------------------------------------------------------------------
bool Renderer::IsBind (const VertexBuffer* vbuffer)
{
	if (1 == (int)msRenderers.size())
	{
		return 0 != vbuffer->PdrPointer;
	}
	else
	{
		return mVertexBuffers.find(vbuffer) != mVertexBuffers.end();
	}
}
//----------------------------------------------------------------------------
bool Renderer::IsOneBind (const VertexBuffer* vbuffer)
{
	bool hasBind = false;

	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			if (renderer->IsBind(vbuffer))
			{
				hasBind = true;
			}
		}
	}

	GetMutex().Leave();

	return hasBind;
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const VertexBuffer* vbuffer)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Bind(vbuffer);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const VertexBuffer* vbuffer)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	VertexBufferMap::iterator iter = mVertexBuffers.find(vbuffer);
	if (iter != mVertexBuffers.end())
	{
		PdrVertexBuffer* pdrVBuffer = iter->second;
		delete0(pdrVBuffer);
		mVertexBuffers.erase(iter);
		((VertexBuffer*)vbuffer)->PdrPointer = 0;
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const VertexBuffer* vbuffer)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Unbind(vbuffer);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const VertexBuffer* vbuffer,
					   unsigned int streamIndex, unsigned int offset)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrVertexBuffer* pdrVBuffer = 0;

	if (1 == (int)msRenderers.size())
	{
		if (vbuffer->PdrPointer)
		{
			pdrVBuffer = (PdrVertexBuffer*)vbuffer->PdrPointer;
		}
		else
		{
			pdrVBuffer = new0 PdrVertexBuffer(this, vbuffer);
			mVertexBuffers[vbuffer] = pdrVBuffer;
			((VertexBuffer*)vbuffer)->PdrPointer = pdrVBuffer;
		}
	}
	else
	{
		VertexBufferMap::iterator iter = mVertexBuffers.find(vbuffer);
		if (iter != mVertexBuffers.end())
		{
			pdrVBuffer = iter->second;
		}
		else
		{
			pdrVBuffer = new0 PdrVertexBuffer(this, vbuffer);
			mVertexBuffers[vbuffer] = pdrVBuffer;
		}
	}

	pdrVBuffer->Enable(this, vbuffer->GetElementSize(), streamIndex, offset);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const VertexBuffer* vbuffer, unsigned int streamIndex)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrVertexBuffer* pdrVBuffer = 0;

	if (1 == (int)msRenderers.size())
	{
		pdrVBuffer = (PdrVertexBuffer*)vbuffer->PdrPointer;
		pdrVBuffer->Disable(this, streamIndex);
	}
	else
	{
		VertexBufferMap::iterator iter = mVertexBuffers.find(vbuffer);
		if (iter != mVertexBuffers.end())
		{
			pdrVBuffer = iter->second;
			pdrVBuffer->Disable(this, streamIndex);
		}
	}
}
//----------------------------------------------------------------------------
void* Renderer::Lock (const VertexBuffer* vbuffer, Buffer::Locking mode)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrVertexBuffer* pdrVBuffer = 0;

	if (1 == (int)msRenderers.size())
	{
		if (vbuffer->PdrPointer)
		{
			pdrVBuffer = (PdrVertexBuffer*)vbuffer->PdrPointer;
		}
		else
		{
			pdrVBuffer = new0 PdrVertexBuffer(this, vbuffer);
			mVertexBuffers[vbuffer] = pdrVBuffer;
			((VertexBuffer*)vbuffer)->PdrPointer = pdrVBuffer;
		}
	}
	else
	{
		VertexBufferMap::iterator iter = mVertexBuffers.find(vbuffer);
		if (iter != mVertexBuffers.end())
		{
			pdrVBuffer = iter->second;
		}
		else
		{
			pdrVBuffer = new0 PdrVertexBuffer(this, vbuffer);
			mVertexBuffers[vbuffer] = pdrVBuffer;
		}
	}

	return pdrVBuffer->Lock(mode);
}
//----------------------------------------------------------------------------
void Renderer::Unlock (const VertexBuffer* vbuffer)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrVertexBuffer* pdrVBuffer = 0;

	if (1 == (int)msRenderers.size())
	{
		pdrVBuffer = (PdrVertexBuffer*)vbuffer->PdrPointer;
		pdrVBuffer->Unlock();
	}
	else
	{
		VertexBufferMap::iterator iter = mVertexBuffers.find(vbuffer);
		if (iter != mVertexBuffers.end())
		{
			pdrVBuffer = iter->second;
			pdrVBuffer->Unlock();
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::Update (const VertexBuffer* vbuffer)
{
#ifdef PX2_USE_OPENGLES
	Lock(vbuffer, Buffer::BL_WRITE_ONLY); // lock is updated
#else
	int numBytes = vbuffer->GetNumBytes();
	char* srcData = vbuffer->GetData();
	void* trgData = Lock(vbuffer, Buffer::BL_WRITE_ONLY);
	if (trgData)
		memcpy(trgData, srcData, numBytes);
	Unlock(vbuffer);
#endif

}
//----------------------------------------------------------------------------
void Renderer::UpdateAll (const VertexBuffer* vbuffer)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Update(vbuffer);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Index buffer管理
//----------------------------------------------------------------------------
void Renderer::Bind (const IndexBuffer* ibuffer)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (mIndexBuffers.find(ibuffer) == mIndexBuffers.end())
	{
		PdrIndexBuffer *pdrIBuffer = new0 PdrIndexBuffer(this, ibuffer);
		mIndexBuffers[ibuffer] = pdrIBuffer;
		((IndexBuffer*)ibuffer)->PdrPointer = pdrIBuffer;
	}
}
//----------------------------------------------------------------------------
bool Renderer::IsBind (const IndexBuffer* ibuffer)
{
	if (1 == (int)msRenderers.size())
	{
		return 0 != ibuffer->PdrPointer;
	}
	else
	{
		return mIndexBuffers.find(ibuffer) != mIndexBuffers.end();
	}
}
//----------------------------------------------------------------------------
bool Renderer::IsOneBind (const IndexBuffer* ibuffer)
{
	bool hasBind = false;

	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			if (renderer->IsBind(ibuffer))
			{
				hasBind = true;
			}
		}
	}

	GetMutex().Leave();

	return hasBind;
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const IndexBuffer* ibuffer)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Bind(ibuffer);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const IndexBuffer* ibuffer)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	IndexBufferMap::iterator iter = mIndexBuffers.find(ibuffer);
	if (iter != mIndexBuffers.end())
	{
		PdrIndexBuffer* pdrIBuffer = iter->second;
		delete0(pdrIBuffer);
		mIndexBuffers.erase(iter);
		((IndexBuffer*)ibuffer)->PdrPointer = 0;
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const IndexBuffer* ibuffer)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Unbind(ibuffer);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const IndexBuffer* ibuffer)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrIndexBuffer* pdrIBuffer = 0;

	if (1 == (int)msRenderers.size())
	{
		if (ibuffer->PdrPointer)
		{
			pdrIBuffer = (PdrIndexBuffer*)ibuffer->PdrPointer;
		}
		else
		{
			pdrIBuffer = new0 PdrIndexBuffer(this, ibuffer);
			mIndexBuffers[ibuffer] = pdrIBuffer;
			((IndexBuffer*)ibuffer)->PdrPointer = pdrIBuffer;
		}
	}
	else
	{
		IndexBufferMap::iterator iter = mIndexBuffers.find(ibuffer);
		if (iter != mIndexBuffers.end())
		{
			pdrIBuffer = iter->second;
		}
		else
		{
			pdrIBuffer = new0 PdrIndexBuffer(this, ibuffer);
			mIndexBuffers[ibuffer] = pdrIBuffer;
		}
	}

	pdrIBuffer->Enable(this);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const IndexBuffer* ibuffer)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrIndexBuffer* pdrIBuffer = 0;

	if (1 == (int)msRenderers.size())
	{
		pdrIBuffer = (PdrIndexBuffer*)ibuffer->PdrPointer;
		pdrIBuffer->Disable(this);
	}
	else
	{
		IndexBufferMap::iterator iter = mIndexBuffers.find(ibuffer);
		if (iter != mIndexBuffers.end())
		{
			pdrIBuffer = iter->second;
			pdrIBuffer->Disable(this);
		}
	}
}
//----------------------------------------------------------------------------
void* Renderer::Lock (const IndexBuffer* ibuffer, Buffer::Locking mode)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrIndexBuffer* pdrIBuffer = 0;

	if (1 == (int)msRenderers.size())
	{
		if (ibuffer->PdrPointer)
		{
			pdrIBuffer = (PdrIndexBuffer*)ibuffer->PdrPointer;
		}
		else
		{
			pdrIBuffer = new0 PdrIndexBuffer(this, ibuffer);
			mIndexBuffers[ibuffer] = pdrIBuffer;
			((IndexBuffer*)ibuffer)->PdrPointer = pdrIBuffer;
		}
	}
	else
	{
		IndexBufferMap::iterator iter = mIndexBuffers.find(ibuffer);
		if (iter != mIndexBuffers.end())
		{
			pdrIBuffer = iter->second;
		}
		else
		{
			pdrIBuffer = new0 PdrIndexBuffer(this, ibuffer);
			mIndexBuffers[ibuffer] = pdrIBuffer;
		}
	}

	return pdrIBuffer->Lock(mode);
}
//----------------------------------------------------------------------------
void Renderer::Unlock (const IndexBuffer* ibuffer)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (1 == (int)msRenderers.size())
	{
		PdrIndexBuffer* pdrIBuffer = (PdrIndexBuffer*)ibuffer->PdrPointer;
		pdrIBuffer->Unlock();
	}
	else
	{
		IndexBufferMap::iterator iter = mIndexBuffers.find(ibuffer);
		if (iter != mIndexBuffers.end())
		{
			PdrIndexBuffer* pdrIBuffer = iter->second;
			pdrIBuffer->Unlock();
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::Update (const IndexBuffer* ibuffer)
{
#ifdef PX2_USE_OPENGLES
	Lock(ibuffer, Buffer::BL_WRITE_ONLY); // lock is updated
#else
	int numBytes = ibuffer->GetNumBytes();
	char* srcData = ibuffer->GetData();
	void* trgData = Lock(ibuffer, Buffer::BL_WRITE_ONLY);
	if (trgData)
		memcpy(trgData, srcData, numBytes);
	Unlock(ibuffer);
#endif
}
//----------------------------------------------------------------------------
void Renderer::UpdateAll (const IndexBuffer* ibuffer)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Update(ibuffer);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Texture1D 管理
//----------------------------------------------------------------------------
void Renderer::Bind (const Texture1D* texture)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (mTexture1Ds.find(texture) == mTexture1Ds.end())
	{
		mTexture1Ds[texture] = new0 PdrTexture1D(this, texture);
	}
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const Texture1D* texture)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Bind(texture);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const Texture1D* texture)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Texture1DMap::iterator iter = mTexture1Ds.find(texture);
	if (iter != mTexture1Ds.end())
	{
		PdrTexture1D* pdrTexture = iter->second;
		delete0(pdrTexture);
		mTexture1Ds.erase(iter);
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const Texture1D* texture)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Unbind(texture);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const Texture1D* texture, int textureUnit)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Texture1DMap::iterator iter = mTexture1Ds.find(texture);
	PdrTexture1D* pdrTexture;
	if (iter != mTexture1Ds.end())
	{
		pdrTexture = iter->second;
	}
	else
	{
		pdrTexture = new0 PdrTexture1D(this, texture);
		mTexture1Ds[texture] = pdrTexture;
	}

	pdrTexture->Enable(this, textureUnit);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const Texture1D* texture, int textureUnit)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Texture1DMap::iterator iter = mTexture1Ds.find(texture);
	if (iter != mTexture1Ds.end())
	{
		PdrTexture1D* pdrTexture = iter->second;
		pdrTexture->Disable(this, textureUnit);
	}
}
//----------------------------------------------------------------------------
void* Renderer::Lock (const Texture1D* texture, int level,
					  Buffer::Locking mode)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Texture1DMap::iterator iter = mTexture1Ds.find(texture);
	PdrTexture1D* pdrTexture;
	if (iter != mTexture1Ds.end())
	{
		pdrTexture = iter->second;
	}
	else
	{
		pdrTexture = new0 PdrTexture1D(this, texture);
		mTexture1Ds[texture] = pdrTexture;
	}

	return pdrTexture->Lock(level, mode);
}
//----------------------------------------------------------------------------
void Renderer::Unlock (const Texture1D* texture, int level)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Texture1DMap::iterator iter = mTexture1Ds.find(texture);
	if (iter != mTexture1Ds.end())
	{
		PdrTexture1D* pdrTexture = iter->second;
		pdrTexture->Unlock(level);
	}
}
//----------------------------------------------------------------------------
void Renderer::Update (const Texture1D* texture, int level)
{
	int numBytes = texture->GetNumLevelBytes(level);
	char* srcData = texture->GetData(level);
	void* trgData = Lock(texture, level, Buffer::BL_WRITE_ONLY);
	if (trgData)
		memcpy(trgData, srcData, numBytes);
	Unlock(texture, level);
}
//----------------------------------------------------------------------------
void Renderer::UpdateAll (const Texture1D* texture, int level)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Update(texture, level);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Texture2D 管理
//----------------------------------------------------------------------------
void Renderer::Bind (const Texture2D* texture)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (mTexture2Ds.find(texture) == mTexture2Ds.end())
	{
		PdrTexture2D *pdrTexture = new0 PdrTexture2D(this, texture);
		mTexture2Ds[texture] = pdrTexture;
		((Texture2D*)texture)->PdrPointer = pdrTexture;
	}
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const Texture2D* texture)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		renderer->Bind(texture);
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const Texture2D* texture)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Texture2DMap::iterator iter = mTexture2Ds.find(texture);
	if (iter != mTexture2Ds.end())
	{
		PdrTexture2D* pdrTexture = iter->second;
		delete0(pdrTexture);
		mTexture2Ds.erase(iter);
		((Texture2D*)texture)->PdrPointer = 0;
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const Texture2D* texture)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Unbind(texture);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const Texture2D* texture, int textureUnit)
{
	if (!texture)
		return;

#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrTexture2D* pdrTexture = 0;

	if (1 == (int)msRenderers.size())
	{
		if (texture->PdrPointer)
		{
			pdrTexture = (PdrTexture2D*)texture->PdrPointer;
		}
		else
		{
			pdrTexture = new0 PdrTexture2D(this, texture);
			mTexture2Ds[texture] = pdrTexture;
			((Texture2D*)texture)->PdrPointer = pdrTexture;
		}
	}
	else
	{
		Texture2DMap::iterator iter = mTexture2Ds.find(texture);
		if (iter != mTexture2Ds.end())
		{
			pdrTexture = iter->second;
		}
		else
		{
			pdrTexture = new0 PdrTexture2D(this, texture);
			mTexture2Ds[texture] = pdrTexture;
		}
	}

	pdrTexture->Enable(this, textureUnit);
}
//----------------------------------------------------------------------------
void Renderer::Enable (const Texture2D* texture, int textureUnit, int loc)
{
	if (!texture)
		return;

#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrTexture2D* pdrTexture = 0;

	if (1 == (int)msRenderers.size())
	{
		if (texture->PdrPointer)
		{
			pdrTexture = (PdrTexture2D*)texture->PdrPointer;
		}
		else
		{
			pdrTexture = new0 PdrTexture2D(this, texture);
			mTexture2Ds[texture] = pdrTexture;
			((Texture2D*)texture)->PdrPointer = pdrTexture;
		}
	}
	else
	{
		Texture2DMap::iterator iter = mTexture2Ds.find(texture);
		if (iter != mTexture2Ds.end())
		{
			pdrTexture = iter->second;
		}
		else
		{
			pdrTexture = new0 PdrTexture2D(this, texture);
			mTexture2Ds[texture] = pdrTexture;
		}
	}

	pdrTexture->Enable(this, textureUnit, loc);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const Texture2D* texture, int textureUnit)
{
	if (!texture)
		return;

#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (1 == (int)msRenderers.size())
	{
		PdrTexture2D* pdrTexture = (PdrTexture2D*)texture->PdrPointer;
		pdrTexture->Disable(this, textureUnit);
	}
	else
	{
		Texture2DMap::iterator iter = mTexture2Ds.find(texture);
		if (iter != mTexture2Ds.end())
		{
			PdrTexture2D* pdrTexture = iter->second;
			pdrTexture->Disable(this, textureUnit);
		}
	}
}
//----------------------------------------------------------------------------
void* Renderer::Lock (const Texture2D* texture, int level,
					  Buffer::Locking mode)
{
	if (!texture)
		return 0;

#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrTexture2D* pdrTexture = 0;

	if (1 == (int)msRenderers.size())
	{
		if (texture->PdrPointer)
		{
			pdrTexture = (PdrTexture2D*)texture->PdrPointer;
		}
		else
		{
			pdrTexture = new0 PdrTexture2D(this, texture);
			mTexture2Ds[texture] = pdrTexture;
			((Texture2D*)texture)->PdrPointer = pdrTexture;
		}
	}
	else
	{
		Texture2DMap::iterator iter = mTexture2Ds.find(texture);
		if (iter != mTexture2Ds.end())
		{
			pdrTexture = iter->second;
		}
		else
		{
			pdrTexture = new0 PdrTexture2D(this, texture);
			mTexture2Ds[texture] = pdrTexture;
		}
	}

	return pdrTexture->Lock(level, mode);
}
//----------------------------------------------------------------------------
void Renderer::Unlock (const Texture2D* texture, int level)
{
	if (!texture)
		return;

#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (1 == (int)msRenderers.size())
	{
		PdrTexture2D* pdrTexture = (PdrTexture2D*)texture->PdrPointer;
		pdrTexture->Unlock(level);
	}
	else
	{
		Texture2DMap::iterator iter = mTexture2Ds.find(texture);
		if (iter != mTexture2Ds.end())
		{
			PdrTexture2D* pdrTexture = iter->second;
			pdrTexture->Unlock(level);
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::Update (const Texture2D* texture, int level)
{
	if (!texture)
		return;

	//if (texture->IsDataNull())
	//	return;

#ifdef PX2_USE_OPENGLES
	Lock(texture, level, Buffer::BL_WRITE_ONLY); // lock is updated
#else

	int numBytes = texture->GetNumLevelBytes(level);
	char* srcData = texture->GetData(level);
	void* trgData = Lock(texture, level, Buffer::BL_WRITE_ONLY);
	if (trgData)
		memcpy(trgData, srcData, numBytes);
	Unlock(texture, level);
#endif
}
//----------------------------------------------------------------------------
void Renderer::UpdateAll (const Texture2D* texture, int level)
{
	if (!texture)
		return;

	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Update(texture, level);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Texture3D 管理
//----------------------------------------------------------------------------
void Renderer::Bind (const Texture3D* texture)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (mTexture3Ds.find(texture) == mTexture3Ds.end())
	{
		mTexture3Ds[texture] = new0 PdrTexture3D(this, texture);
	}
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const Texture3D* texture)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		renderer->Bind(texture);
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const Texture3D* texture)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Texture3DMap::iterator iter = mTexture3Ds.find(texture);
	if (iter != mTexture3Ds.end())
	{
		PdrTexture3D* pdrTexture = iter->second;
		delete0(pdrTexture);
		mTexture3Ds.erase(iter);
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const Texture3D* texture)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		renderer->Unbind(texture);
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const Texture3D* texture, int textureUnit)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Texture3DMap::iterator iter = mTexture3Ds.find(texture);
	PdrTexture3D* pdrTexture;
	if (iter != mTexture3Ds.end())
	{
		pdrTexture = iter->second;
	}
	else
	{
		pdrTexture = new0 PdrTexture3D(this, texture);
		mTexture3Ds[texture] = pdrTexture;
	}

	pdrTexture->Enable(this, textureUnit);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const Texture3D* texture, int textureUnit)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Texture3DMap::iterator iter = mTexture3Ds.find(texture);
	if (iter != mTexture3Ds.end())
	{
		PdrTexture3D* pdrTexture = iter->second;
		pdrTexture->Disable(this, textureUnit);
	}
}
//----------------------------------------------------------------------------
void* Renderer::Lock (const Texture3D* texture, int level,
					  Buffer::Locking mode)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	Texture3DMap::iterator iter = mTexture3Ds.find(texture);
	PdrTexture3D* pdrTexture;
	if (iter != mTexture3Ds.end())
	{
		pdrTexture = iter->second;
	}
	else
	{
		pdrTexture = new0 PdrTexture3D(this, texture);
		mTexture3Ds[texture] = pdrTexture;
	}

	return pdrTexture->Lock(level, mode);
}
//----------------------------------------------------------------------------
void Renderer::Unlock (const Texture3D* texture, int level)
{
	Texture3DMap::iterator iter = mTexture3Ds.find(texture);
	if (iter != mTexture3Ds.end())
	{
		PdrTexture3D* pdrTexture = iter->second;
		pdrTexture->Unlock(level);
	}
}
//----------------------------------------------------------------------------
void Renderer::Update (const Texture3D* texture, int level)
{
	int numBytes = texture->GetNumLevelBytes(level);
	char* srcData = texture->GetData(level);
	void* trgData = Lock(texture, level, Buffer::BL_WRITE_ONLY);
	if (trgData)
		memcpy(trgData, srcData, numBytes);
	Unlock(texture, level);
}
//----------------------------------------------------------------------------
void Renderer::UpdateAll (const Texture3D* texture, int level)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		renderer->Update(texture, level);
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// TextureCube 管理
//----------------------------------------------------------------------------
void Renderer::Bind (const TextureCube* texture)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (mTextureCubes.find(texture) == mTextureCubes.end())
	{
		mTextureCubes[texture] = new0 PdrTextureCube(this, texture);
	}
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const TextureCube* texture)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		renderer->Bind(texture);
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const TextureCube* texture)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	TextureCubeMap::iterator iter = mTextureCubes.find(texture);
	if (iter != mTextureCubes.end())
	{
		PdrTextureCube* pdrTexture = iter->second;
		delete0(pdrTexture);
		mTextureCubes.erase(iter);
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const TextureCube* texture)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		renderer->Unbind(texture);
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const TextureCube* texture, int textureUnit)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	TextureCubeMap::iterator iter = mTextureCubes.find(texture);
	PdrTextureCube* pdrTexture;
	if (iter != mTextureCubes.end())
	{
		pdrTexture = iter->second;
	}
	else
	{
		pdrTexture = new0 PdrTextureCube(this, texture);
		mTextureCubes[texture] = pdrTexture;
	}

	pdrTexture->Enable(this, textureUnit);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const TextureCube* texture, int textureUnit)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	TextureCubeMap::iterator iter = mTextureCubes.find(texture);
	if (iter != mTextureCubes.end())
	{
		PdrTextureCube* pdrTexture = iter->second;
		pdrTexture->Disable(this, textureUnit);
	}
}
//----------------------------------------------------------------------------
void* Renderer::Lock (const TextureCube* texture, int face, int level,
					  Buffer::Locking mode)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	TextureCubeMap::iterator iter = mTextureCubes.find(texture);
	PdrTextureCube* pdrTexture;
	if (iter != mTextureCubes.end())
	{
		pdrTexture = iter->second;
	}
	else
	{
		pdrTexture = new0 PdrTextureCube(this, texture);
		mTextureCubes[texture] = pdrTexture;
	}

	return pdrTexture->Lock(face, level, mode);
}
//----------------------------------------------------------------------------
void Renderer::Unlock (const TextureCube* texture, int face, int level)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	TextureCubeMap::iterator iter = mTextureCubes.find(texture);
	if (iter != mTextureCubes.end())
	{
		PdrTextureCube* pdrTexture = iter->second;
		pdrTexture->Unlock(face, level);
	}
}
//----------------------------------------------------------------------------
void Renderer::Update (const TextureCube* texture, int face, int level)
{
	int numBytes = texture->GetNumLevelBytes(level);
	char* srcData = texture->GetData(face, level);
	void* trgData = Lock(texture, face, level, Buffer::BL_WRITE_ONLY);
	if (trgData)
		memcpy(trgData, srcData, numBytes);
	Unlock(texture, face, level);
}
//----------------------------------------------------------------------------
void Renderer::UpdateAll (const TextureCube* texture, int face, int level)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		renderer->Update(texture, face, level);
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Render target 管理
//----------------------------------------------------------------------------
void Renderer::Bind (const RenderTarget* renderTarget)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (mRenderTargets.find(renderTarget) == mRenderTargets.end())
	{
		PdrRenderTarget *pdrRenderTarget = new0 PdrRenderTarget(this,
			renderTarget);
		((RenderTarget*)renderTarget)->PdrPointer = pdrRenderTarget;
		mRenderTargets[renderTarget] = pdrRenderTarget;
	}
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const RenderTarget* renderTarget)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		renderer->Bind(renderTarget);
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const RenderTarget* renderTarget)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	RenderTargetMap::iterator iter = mRenderTargets.find(renderTarget);
	if (iter != mRenderTargets.end())
	{
		((RenderTarget*)renderTarget)->PdrPointer = 0;
		PdrRenderTarget* pdrRenderTarget = iter->second;
		delete0(pdrRenderTarget);
		mRenderTargets.erase(iter);
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const RenderTarget* renderTarget)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		renderer->Unbind(renderTarget);
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const RenderTarget* renderTarget)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrRenderTarget* pdrRenderTarget = 0;

	if (1 == (int)msRenderers.size())
	{
		if (renderTarget->PdrPointer)
		{
			pdrRenderTarget = (PdrRenderTarget*)renderTarget->PdrPointer;
		}
		else
		{
			pdrRenderTarget = new0 PdrRenderTarget(this, renderTarget);
			((RenderTarget*)renderTarget)->PdrPointer = pdrRenderTarget;
			mRenderTargets[renderTarget] = pdrRenderTarget;
		}
	}
	else
	{
		RenderTargetMap::iterator iter = mRenderTargets.find(renderTarget);
		if (iter != mRenderTargets.end())
		{
			pdrRenderTarget = iter->second;
		}
		else
		{
			pdrRenderTarget = new0 PdrRenderTarget(this, renderTarget);
			mRenderTargets[renderTarget] = pdrRenderTarget;
		}
	}

	pdrRenderTarget->Enable(this);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const RenderTarget* renderTarget)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrRenderTarget* pdrRenderTarget = 0;

	if (1 == (int)msRenderers.size())
	{
		pdrRenderTarget = (PdrRenderTarget*)renderTarget->PdrPointer;
	}
	else
	{
		RenderTargetMap::iterator iter = mRenderTargets.find(renderTarget);
		if (iter != mRenderTargets.end())
		{
			pdrRenderTarget = iter->second;
		}
	}

	pdrRenderTarget->Disable(this);
}
//----------------------------------------------------------------------------
void Renderer::ReadColor (int i, const RenderTarget* renderTarget,
						  Texture2D*& texture)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	RenderTargetMap::iterator iter = mRenderTargets.find(renderTarget);
	if (iter != mRenderTargets.end())
	{
		PdrRenderTarget* pdrRenderTarget = iter->second;
		pdrRenderTarget->ReadColor(i, this, texture);
	}
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Vertex shader 管理
//----------------------------------------------------------------------------
void Renderer::Bind (const VertexShader* vshader)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (mVertexShaders.find(vshader) == mVertexShaders.end())
	{
		PdrVertexShader *pdrVShader = new0 PdrVertexShader(this, vshader);
		mVertexShaders[vshader] = pdrVShader;
		((VertexShader*)vshader)->PdrPointer = pdrVShader;
	}
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const VertexShader* vshader)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		renderer->Bind(vshader);
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const VertexShader* vshader)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	VertexShaderMap::iterator iter = mVertexShaders.find(vshader);
	if (iter != mVertexShaders.end())
	{
		PdrVertexShader* pdrVShader = iter->second;
		delete0(pdrVShader);
		mVertexShaders.erase(iter);
		((VertexShader*)vshader)->PdrPointer = 0;
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const VertexShader* vshader)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Unbind(vshader);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const VertexShader* vshader,
					   const ShaderParameters* parameters)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrVertexShader* pdrVShader = 0;

	if (1 == (int)msRenderers.size())
	{
		if (vshader->PdrPointer)
		{
			pdrVShader = (PdrVertexShader*)vshader->PdrPointer;
		}
		else
		{
			pdrVShader = new0 PdrVertexShader(this, vshader);
			mVertexShaders[vshader] = pdrVShader;
			((VertexShader*)vshader)->PdrPointer = pdrVShader;
		}
	}
	else
	{
		VertexShaderMap::iterator iter = mVertexShaders.find(vshader);
		if (iter != mVertexShaders.end())
		{
			pdrVShader = iter->second;
		}
		else
		{
			pdrVShader = new0 PdrVertexShader(this, vshader);
			mVertexShaders[vshader] = pdrVShader;
		}
	}

	pdrVShader->Enable(this, vshader, parameters);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const VertexShader* vshader,
						const ShaderParameters* parameters)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (1 == (int)msRenderers.size())
	{
		PdrVertexShader* pdrVShader = (PdrVertexShader*)vshader->PdrPointer;
		pdrVShader->Disable(this, vshader, parameters);
	}
	else
	{
		VertexShaderMap::iterator iter = mVertexShaders.find(vshader);
		if (iter != mVertexShaders.end())
		{
			PdrVertexShader* pdrVShader = iter->second;
			pdrVShader->Disable(this, vshader, parameters);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Pixel shader 管理
//----------------------------------------------------------------------------
void Renderer::Bind (const PixelShader* pshader)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (mPixelShaders.find(pshader) == mPixelShaders.end())
	{
		PdrPixelShader *pdrPShader = new0 PdrPixelShader(this, pshader);
		mPixelShaders[pshader] = pdrPShader;
		((PixelShader*)pshader)->PdrPointer = pdrPShader;
	}
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const PixelShader* pshader)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Bind(pshader);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const PixelShader* pshader)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PixelShaderMap::iterator iter = mPixelShaders.find(pshader);
	if (iter != mPixelShaders.end())
	{
		PdrPixelShader* pdrPShader = iter->second;
		delete0(pdrPShader);
		mPixelShaders.erase(iter);
		((PixelShader*)pshader)->PdrPointer = 0;
	}
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const PixelShader* pshader)
{
	GetMutex().Enter();

	RendererSet::iterator iter = msRenderers.begin();
	RendererSet::iterator end = msRenderers.end();
	for (/**/; iter != end; ++iter)
	{
		Renderer* renderer = *iter;
		if (renderer)
		{
			renderer->Unbind(pshader);
		}
	}

	GetMutex().Leave();
}
//----------------------------------------------------------------------------
void Renderer::Enable (const PixelShader* pshader,
					   const ShaderParameters* parameters)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	PdrPixelShader* pdrPShader = 0;

	if (1 == (int)msRenderers.size())
	{
		if (pshader->PdrPointer)
		{
			pdrPShader = (PdrPixelShader*)pshader->PdrPointer;
		}
		else
		{
			pdrPShader = new0 PdrPixelShader(this, pshader);
			mPixelShaders[pshader] = pdrPShader;
			((PixelShader*)pshader)->PdrPointer = pdrPShader;
		}
	}
	else
	{
		PixelShaderMap::iterator iter = mPixelShaders.find(pshader);
		if (iter != mPixelShaders.end())
		{
			pdrPShader = iter->second;
		}
		else
		{
			pdrPShader = new0 PdrPixelShader(this, pshader);
			mPixelShaders[pshader] = pdrPShader;
		}
	}

	pdrPShader->Enable(this, pshader, parameters);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const PixelShader* pshader,
						const ShaderParameters* parameters)
{
#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	if (1 == (int)msRenderers.size())
	{
		PdrPixelShader *pdrPShader = (PdrPixelShader*)pshader->PdrPointer;
		pdrPShader->Disable(this, pshader, parameters);
	}
	else
	{
		PixelShaderMap::iterator iter = mPixelShaders.find(pshader);
		if (iter != mPixelShaders.end())
		{
			PdrPixelShader* pdrPShader = iter->second;
			pdrPShader->Disable(this, pshader, parameters);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Node Bind
//----------------------------------------------------------------------------
void Renderer::ForceBind (Renderable *renderable)
{
	if (!renderable)
		return;

#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	renderable->OnForceBind();

	VertexFormat *vf = renderable->GetVertexFormat();
	VertexBuffer *vb = renderable->GetVertexBuffer();
	IndexBuffer *ib = renderable->GetIndexBuffer();

	if (vf)
		Bind(vf);

	if (vb)
		Bind(vb);

	if (ib)
		Bind(ib);

	MaterialInstance *mtlInst = renderable->GetMaterialInstance();
	if (!mtlInst)
		return;

	Material *mtl = mtlInst->GetMaterial();
	ShaderParameters *shaderParam = mtlInst->GetPixelParameters(0);
	MaterialPass *mtlPass = mtl->GetPass(0, 0);

	Bind(mtlPass->GetVertexShader());
	Bind(mtlPass->GetPixelShader());

	for (int i=0; i<shaderParam->GetNumTextures(); i++)
	{
		const Texture2D *tex2D = DynamicCast<Texture2D>(
			shaderParam->GetTexture(i));
		if (tex2D)
		{
			Bind(tex2D);
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::ForceBind (Movable *mov)
{
	if (!mov)
		return;

#if defined PX2_USE_OPENGL
	PreDraw();
#endif

	mov->Update(0.0f);

	Node *node = DynamicCast<Node>(mov);
	Renderable *renderable = DynamicCast<Renderable>(mov);
	if (node)
	{
		for (int i=0; i<node->GetNumChildren(); i++)
		{
			Movable *child = node->GetChild(i);

			if (child)
				ForceBind(child);
		}
	}
	else if (renderable)
	{
		ForceBind(renderable);
	}
}
//----------------------------------------------------------------------------
template <typename Resource, typename PdrResource>
void RecreateResources (Renderer* renderer,	std::map<const Resource *, PdrResource *>& resmap)
{
	typename std::map<const Resource *, PdrResource *>::iterator iter = resmap.begin();
	typename std::map<const Resource *, PdrResource *>::iterator end = resmap.end();
	for (; iter != end; ++iter)
	{
		renderer->Bind(iter->first);
	}
}

template <typename Resource, typename PdrResource>
void DestroyResources (Renderer* renderer,	std::map<const Resource *, PdrResource *>& resmap)
{
	PX2_UNUSED(renderer);

	typename std::map<const Resource *, PdrResource *>::iterator iter = resmap.begin();
	typename std::map<const Resource *, PdrResource *>::iterator end = resmap.end();
	for (; iter != end; ++iter)
	{
		delete0(iter->second);
		((Resource *)iter->first)->PdrPointer = 0;
	}
}
//----------------------------------------------------------------------------
void Renderer::OnLostDevice()
{
	DestroyResources(this, mVertexFormats);
	DestroyResources(this, mVertexBuffers);
	DestroyResources(this, mIndexBuffers);
	DestroyResources(this, mTexture1Ds);
	DestroyResources(this, mTexture2Ds);
	DestroyResources(this, mTexture3Ds);
	DestroyResources(this, mTextureCubes);
	DestroyResources(this, mRenderTargets);
	DestroyResources(this, mVertexShaders);
	DestroyResources(this, mPixelShaders);

#ifdef PX2_USE_OPENGLES
	DestroyResources(this, mMaterialPasses);
#endif

	mVertexFormats.clear();
	mVertexBuffers.clear();
	mIndexBuffers.clear();
	mTexture1Ds.clear();
	mTexture2Ds.clear();
	mTexture3Ds.clear();
	mTextureCubes.clear();
	mRenderTargets.clear();
	mVertexShaders.clear();
	mPixelShaders.clear();

	ClearSharePdrData();

#ifdef PX2_USE_OPENGLES
	mMaterialPasses.clear();
#endif
}
//----------------------------------------------------------------------------
void Renderer::OnRestoreDevice()
{
	InitRenderStates();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Viewport
//----------------------------------------------------------------------------
void Renderer::SetViewport (const Rectf &rect)
{
	mViewPort = rect;

	SetViewport((int)rect.Left, (int)rect.Bottom, (int)rect.Width(),
		(int)rect.Height());
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 计算选择射线
//----------------------------------------------------------------------------
bool Renderer::GetPickRay (int x, int y, APoint& origin, AVector& direction)
const
{
	if (!mCamera)
	{
		return false;
	}

	// Get the current viewport and test whether (x,y) is in it.
	int viewX, viewY, viewW, viewH;
	GetViewport(viewX, viewY, viewW, viewH);
	if (x < viewX || x > viewX + viewW || y < viewY || y > viewY + viewH)
	{
		return false;
	}

	// Get the [0,1]^2-normalized coordinates of (x,y).
	float r = ((float)(x - viewX))/(float)viewW;
	float u = ((float)(y - viewY))/(float)viewH;

	// Get the relative coordinates in [rmin,rmax]x[umin,umax].
	float rBlend = (1.0f - r)*mCamera->GetRMin() + r*mCamera->GetRMax();
	float uBlend = (1.0f - u)*mCamera->GetUMin() + u*mCamera->GetUMax();

	if (mCamera->IsPerspective())
	{
		origin = mCamera->GetPosition();
		direction = mCamera->GetDMin()*mCamera->GetDVector() +
			rBlend*mCamera->GetRVector() + uBlend*mCamera->GetUVector();
		direction.Normalize();
	}
	else
	{
		origin = mCamera->GetPosition() + rBlend*mCamera->GetRVector() +
			uBlend*mCamera->GetUVector();
		direction = mCamera->GetDVector();
		direction.Normalize();
	}

	return true;
}
//----------------------------------------------------------------------------
Vector2f Renderer::PointWorldToViewPort (const  APoint &point, bool *isInBack)
{
	HMatrix matProjView = GetProjectionMatrix() * GetViewMatrix();
	HPoint hPoint(point.X(), point.Y(), point.Z(), point.W());
	HPoint tempPoint = matProjView * hPoint;

	if (isInBack)
	{
		if (tempPoint.Z() <= 0)
			*isInBack = true;
		else
			*isInBack = false;
	}

	float wInv = 1.0f / tempPoint.W();

	//投影坐标范围为[-1,1]要变成[0,1]
	Vector2f screenPoint;
	screenPoint.X() = (1.0f + tempPoint.X()*wInv)/2.0f;
	screenPoint.Y() = (1.0f + tempPoint.Y()*wInv)/2.0f;

	//投影坐标范围为[0,1]要变成视口内坐标
	int viewX, viewY, viewW, viewH;
	GetViewport(viewX, viewY, viewW, viewH);

	screenPoint.X() = viewX + screenPoint.X()*viewW;
	screenPoint.Y() = viewY + screenPoint.Y()*viewH;

	return screenPoint;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 绘制入口点
//----------------------------------------------------------------------------
void Renderer::Draw (const VisibleSet& visibleSet, GlobalMaterial* globalMaterial)
{
	if (!globalMaterial)
	{
		const int numVisible = visibleSet.GetNumVisible();
		for (int i = 0; i < numVisible; ++i)
		{
			Renderable *renderable = visibleSet.GetVisible(i);
			const MaterialInstance *instance = renderable->GetMaterialInstance();

			Draw(renderable, instance);
		}
	}
	else
	{
		globalMaterial->Draw(this, visibleSet);
	}
}
//----------------------------------------------------------------------------
void Renderer::Draw (Renderable* renderable)
{
	const MaterialInstance* instance = renderable->GetMaterialInstance();
	Draw(renderable, instance);
}
//----------------------------------------------------------------------------
void Renderer::Draw (Renderable* renderable,
					 const MaterialInstance* instance)
{
	Renderable::PrimitiveType pt = renderable->GetPrimitiveType();
	if (Renderable::PT_NONE == pt)
	{ // Canvas Binder
		Canvas *canvas = DynamicCast<Canvas>(renderable->GetParent());
		if (canvas)
		{
			canvas->Draw(this);
		}
	}
	else
	{
		mCurRenderable = renderable;

		const VertexFormat* vformat = renderable->GetVertexFormat();
		const VertexBuffer* vbuffer = renderable->GetVertexBuffer();
		const IndexBuffer* ibuffer = renderable->GetIndexBuffer();

		renderable->OnDraw(this);

		Enable(vbuffer);

		Enable(vformat);

		if (ibuffer) Enable(ibuffer);

		const int numPasses = instance->GetNumPasses();
		for (int i = 0; i < numPasses; ++i)
		{
			// 激活Pass
			Enable(renderable, instance, i);

			// 绘制几何图形
			DrawPrimitive(renderable);

			//		// 取消激活Pass
			//		Disable(renderable, instance, i);
			//
			//#ifdef PX2_RESET_STATE_AFTER_DRAW
			//		// 恢复到缺省渲染状态
			//		SetAlphaProperty(mDefaultAlphaProperty);
			//		SetCullProperty(mDefaultCullProperty);
			//		SetDepthProperty(mDefaultDepthProperty);
			//		SetOffsetProperty(mDefaultOffsetProperty);
			//		SetStencilProperty(mDefaultStencilProperty);
			//		SetWireProperty(mDefaultWireProperty);
			//#endif
		}
	}
}
//----------------------------------------------------------------------------
Mutex &Renderer::GetMutex ()
{
	static Mutex mx;
	return mx;
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllVertexFormats ()
{
	VertexFormatMap::iterator iter = mVertexFormats.begin();
	VertexFormatMap::iterator end = mVertexFormats.end();
	for (/**/; iter != end; ++iter)
	{
		PdrVertexFormat* pdrVFormat = iter->second;
		delete0(pdrVFormat);
	}
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllVertexBuffers ()
{
	VertexBufferMap::iterator iter = mVertexBuffers.begin();
	VertexBufferMap::iterator end = mVertexBuffers.end();
	for (/**/; iter != end; ++iter)
	{
		PdrVertexBuffer* pdrVBuffer = iter->second;
		delete0(pdrVBuffer);
	}
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllIndexBuffers ()
{
	IndexBufferMap::iterator iter = mIndexBuffers.begin();
	IndexBufferMap::iterator end = mIndexBuffers.end();
	for (/**/; iter != end; ++iter)
	{
		PdrIndexBuffer* pdrIBuffer = iter->second;
		delete0(pdrIBuffer);
	}
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllTexture1Ds ()
{
	Texture1DMap::iterator iter = mTexture1Ds.begin();
	Texture1DMap::iterator end = mTexture1Ds.end();
	for (/**/; iter != end; ++iter)
	{
		PdrTexture1D* pdrTexture = iter->second;
		delete0(pdrTexture);
	}
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllTexture2Ds ()
{
	Texture2DMap::iterator iter = mTexture2Ds.begin();
	Texture2DMap::iterator end = mTexture2Ds.end();
	for (/**/; iter != end; ++iter)
	{
		PdrTexture2D* pdrTexture = iter->second;
		delete0(pdrTexture);
	}
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllTexture3Ds ()
{
	Texture3DMap::iterator iter = mTexture3Ds.begin();
	Texture3DMap::iterator end = mTexture3Ds.end();
	for (/**/; iter != end; ++iter)
	{
		PdrTexture3D* pdrTexture = iter->second;
		delete0(pdrTexture);
	}
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllTextureCubes ()
{
	TextureCubeMap::iterator iter = mTextureCubes.begin();
	TextureCubeMap::iterator end = mTextureCubes.end();
	for (/**/; iter != end; ++iter)
	{
		PdrTextureCube* pdrTexture = iter->second;
		delete0(pdrTexture);
	}
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllRenderTargets ()
{
	RenderTargetMap::iterator iter = mRenderTargets.begin();
	RenderTargetMap::iterator end = mRenderTargets.end();
	for (/**/; iter != end; ++iter)
	{
		PdrRenderTarget* pdrRenderTarget = iter->second;
		delete0(pdrRenderTarget);
	}
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllMaterialPasses ()
{
#ifdef PX2_USE_OPENGLES
	MaterialPassMap::iterator iter = mMaterialPasses.begin();
	MaterialPassMap::iterator end = mMaterialPasses.end();
	for (/**/; iter != end; ++iter)
	{
		PdrMaterialPass* pdrMaterialPass = iter->second;
		delete0(pdrMaterialPass);
	}
#endif
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllVertexShaders ()
{
	VertexShaderMap::iterator iter = mVertexShaders.begin();
	VertexShaderMap::iterator end = mVertexShaders.end();
	for (/**/; iter != end; ++iter)
	{
		PdrVertexShader* pdrVShader = iter->second;
		delete0(pdrVShader);
	}
}
//----------------------------------------------------------------------------
void Renderer::DestroyAllPixelShaders ()
{
	PixelShaderMap::iterator iter = mPixelShaders.begin();
	PixelShaderMap::iterator end = mPixelShaders.end();
	for (/**/; iter != end; ++iter)
	{
		PdrPixelShader* pdrPShader = iter->second;
		delete0(pdrPShader);
	}
}
//----------------------------------------------------------------------------
PdrVertexFormat* Renderer::GetResource (const VertexFormat* vformat)
{
	VertexFormatMap::iterator iter = mVertexFormats.find(vformat);
	if (iter != mVertexFormats.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
PdrVertexBuffer* Renderer::GetResource (const VertexBuffer* vbuffer)
{
	VertexBufferMap::iterator iter = mVertexBuffers.find(vbuffer);
	if (iter != mVertexBuffers.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
PdrIndexBuffer* Renderer::GetResource (const IndexBuffer* ibuffer)
{
	IndexBufferMap::iterator iter = mIndexBuffers.find(ibuffer);
	if (iter != mIndexBuffers.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
PdrTexture1D* Renderer::GetResource (const Texture1D* texture)
{
	Texture1DMap::iterator iter = mTexture1Ds.find(texture);
	if (iter != mTexture1Ds.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
PdrTexture2D* Renderer::GetResource (const Texture2D* texture)
{
	Texture2DMap::iterator iter = mTexture2Ds.find(texture);
	if (iter != mTexture2Ds.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
PdrTexture3D* Renderer::GetResource (const Texture3D* texture)
{
	Texture3DMap::iterator iter = mTexture3Ds.find(texture);
	if (iter != mTexture3Ds.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
PdrTextureCube* Renderer::GetResource (const TextureCube* texture)
{
	TextureCubeMap::iterator iter = mTextureCubes.find(texture);
	if (iter != mTextureCubes.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
PdrRenderTarget* Renderer::GetResource (const RenderTarget* renderTarget)
{
	RenderTargetMap::iterator iter = mRenderTargets.find(renderTarget);
	if (iter != mRenderTargets.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
PdrVertexShader* Renderer::GetResource (const VertexShader* vshader)
{
	VertexShaderMap::iterator iter = mVertexShaders.find(vshader);
	if (iter != mVertexShaders.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
PdrPixelShader* Renderer::GetResource (const PixelShader* pshader)
{
	PixelShaderMap::iterator iter = mPixelShaders.find(pshader);
	if (iter != mPixelShaders.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
PdrMaterialPass* Renderer::GetResource (const MaterialPass* pass)
{
	MaterialPassMap::iterator iter = mMaterialPasses.find(pass);
	if (iter != mMaterialPasses.end())
	{
		return iter->second;
	}
	return 0;
}
//----------------------------------------------------------------------------
Renderer::SharePdrData::SharePdrData ()
	:
DataPtr(0),
DataInt(0)
{
	for (int i=0; i<16; i++)
	{
		ES_V_CLoc[i] = 0;
		ES_V_TLoc[i] = 0;
		ES_P_CLoc[i] = 0;
		ES_P_TLoc[i] = 0;
	}
}
//----------------------------------------------------------------------------
Renderer::SharePdrData::~SharePdrData ()
{
}
//----------------------------------------------------------------------------
