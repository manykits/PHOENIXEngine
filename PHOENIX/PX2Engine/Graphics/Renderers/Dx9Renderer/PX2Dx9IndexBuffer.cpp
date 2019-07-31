// PX2Dx9IndexBuffer.cpp

#ifdef PX2_USE_DX9

#include "PX2Dx9IndexBuffer.hpp"
#include "PX2Dx9Mapping.hpp"
#include "PX2Dx9RendererData.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrIndexBuffer::PdrIndexBuffer (Renderer* renderer,
								const IndexBuffer* ibuffer)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	UINT numBytes = (UINT)ibuffer->GetNumBytes();
	DWORD usage = gDX9BufferUsage[ibuffer->GetUsage()];
	D3DFORMAT format =
		(ibuffer->GetElementSize() == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32);

	D3DPOOL pool;
	if (ibuffer->GetUsage() == Buffer::BU_STATIC)
	{
		pool = D3DPOOL_MANAGED;
	}
	else
	{
		pool = D3DPOOL_DEFAULT;
	}

	HRESULT hr = device->CreateIndexBuffer(numBytes, usage, format,
		pool, &mBuffer, 0);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to create index buffer: %s\n",
		DXGetErrorString(hr));

	void* data = Lock(Buffer::BL_WRITE_ONLY);
	memcpy(data, ibuffer->GetData(), ibuffer->GetNumBytes());
	Unlock();
}
//----------------------------------------------------------------------------
PdrIndexBuffer::~PdrIndexBuffer ()
{
	mBuffer->Release();
}
//----------------------------------------------------------------------------
void PdrIndexBuffer::Enable (Renderer* renderer)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	HRESULT hr = device->SetIndices(mBuffer);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to enable index buffer: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void PdrIndexBuffer::Disable (Renderer* renderer)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;
	PX2_UNUSED(hr);

#ifdef PX2_PDR_DEBUG
	// 检测需要被取消激活的数据，是否匹配。
	IDirect3DIndexBuffer9 *activeBuffer = 0;
	hr = device->GetIndices(&activeBuffer);
	assertion(hr == D3D_OK, "Failed to get indices: %s\n",
		DXGetErrorString(hr));
	assertion(activeBuffer == mBuffer, "Mismatched index buffers\n");
	activeBuffer->Release();
#endif

	hr = device->SetIndices(0);
	assertion(hr == D3D_OK, "Failed to set indices: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void* PdrIndexBuffer::Lock (Buffer::Locking mode)
{
	void *videoMemory = 0;
	HRESULT hr = mBuffer->Lock(0, 0, &videoMemory, gDX9BufferLocking[mode]);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to lock index buffer: %s\n",
		DXGetErrorString(hr));
	return videoMemory;
}
//----------------------------------------------------------------------------
void PdrIndexBuffer::Unlock ()
{
	HRESULT hr = mBuffer->Unlock();
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to unlock index buffer: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

#endif
