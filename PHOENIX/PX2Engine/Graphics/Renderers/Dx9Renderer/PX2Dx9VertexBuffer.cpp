// PX2Dx9VertexBuffer.cpp

#ifdef PX2_USE_DX9

#include "PX2DX9VertexBuffer.hpp"
#include "PX2Dx9Mapping.hpp"
#include "PX2Dx9RendererData.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer (Renderer* renderer,
								  const VertexBuffer* vbuffer)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	UINT numBytes = (UINT)vbuffer->GetNumBytes();
	DWORD usage = gDX9BufferUsage[vbuffer->GetUsage()];

	D3DPOOL pool;
	if (vbuffer->GetUsage() == Buffer::BU_STATIC)
	{
		pool = D3DPOOL_MANAGED;
	}
	else
	{
		pool = D3DPOOL_DEFAULT;
	}

	HRESULT hr = device->CreateVertexBuffer(numBytes, usage, 0,
		pool, &mBuffer, 0);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to create vertex buffer: %s\n",
		DXGetErrorString(hr));

	void* data = Lock(Buffer::BL_WRITE_ONLY);
	memcpy(data, vbuffer->GetData(), vbuffer->GetNumBytes());
	Unlock();
}
//----------------------------------------------------------------------------
PdrVertexBuffer::~PdrVertexBuffer ()
{
	mBuffer->Release();
}
//----------------------------------------------------------------------------
void PdrVertexBuffer::Enable (Renderer* renderer, unsigned int vertexSize,
							  unsigned int streamIndex, unsigned int offset)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	HRESULT hr = device->SetStreamSource(streamIndex, mBuffer, offset,
		vertexSize);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to enable vertex buffer: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void PdrVertexBuffer::Disable (Renderer* renderer, unsigned int streamIndex)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;
	PX2_UNUSED(hr);

#ifdef PX2_PDR_DEBUG
	IDirect3DVertexBuffer9 *activeBuffer = 0;
	unsigned int activeOffset = 0, activeStride = 0;
	hr = device->GetStreamSource(streamIndex, &activeBuffer,
		&activeOffset, &activeStride);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to get stream source: %s\n",
		DXGetErrorString(hr));
	assertion(activeBuffer == mBuffer, "Mismatched vertex buffers\n");
	activeBuffer->Release();
#endif

	hr = device->SetStreamSource(streamIndex, 0, 0, 0);
	assertion(hr == D3D_OK, "Failed to set stream source: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void* PdrVertexBuffer::Lock (Buffer::Locking mode)
{
	void *videoMemory = 0;
	HRESULT hr = mBuffer->Lock(0, 0, &videoMemory, gDX9BufferLocking[mode]);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to lock vertex buffer: %s\n",
		DXGetErrorString(hr));
	return videoMemory;
}
//----------------------------------------------------------------------------
void PdrVertexBuffer::Unlock ()
{
	HRESULT hr = mBuffer->Unlock();
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to unlock vertex buffer: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

#endif