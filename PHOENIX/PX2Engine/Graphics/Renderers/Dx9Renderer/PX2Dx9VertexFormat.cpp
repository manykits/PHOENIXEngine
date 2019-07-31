// PX2Dx9VertexFormat.cpp

#ifdef PX2_USE_DX9

#include "PX2Dx9VertexFormat.hpp"
#include "PX2Dx9Mapping.hpp"
#include "PX2Dx9RendererData.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrVertexFormat::PdrVertexFormat (Renderer* renderer,
								  const VertexFormat* vformat)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	const int numAttributes = vformat->GetNumAttributes();
	for (int i = 0; i < numAttributes; ++i)
	{
		// 获得与平台无关的顶点属性
		unsigned int streamIndex;
		unsigned int offset;
		VertexFormat::AttributeType type;
		VertexFormat::AttributeUsage usage;
		unsigned int usageIndex;
		vformat->GetAttribute(i, streamIndex, offset, type, usage,
			usageIndex);

		// 设置Dx9属性
		D3DVERTEXELEMENT9 &element = mElements[i];
		element.Stream = (WORD)streamIndex;
		element.Offset = (WORD)offset;
		element.Type = gDX9AttributeType[type];
		element.Method = D3DDECLMETHOD_DEFAULT;
		element.Usage = gDX9AttributeUsage[usage];
		element.UsageIndex = (BYTE)usageIndex;
	}

	// 最后一个DX9 element必须是D3DDECL_END().
	D3DVERTEXELEMENT9 &element = mElements[numAttributes];
	element.Stream = 0xFF;
	element.Offset = 0;
	element.Type = D3DDECLTYPE_UNUSED;
	element.Method = 0;
	element.Usage = 0;
	element.UsageIndex = 0;

	HRESULT hr = device->CreateVertexDeclaration(mElements, &mDeclaration);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to create vertex declaration: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
PdrVertexFormat::~PdrVertexFormat ()
{
	mDeclaration->Release();
}
//----------------------------------------------------------------------------
void PdrVertexFormat::Enable (Renderer* renderer)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	HRESULT hr = device->SetVertexDeclaration(mDeclaration);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to enable vertex declaration: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void PdrVertexFormat::Disable (Renderer* renderer)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	PX2_UNUSED(device);
	HRESULT hr;
	PX2_UNUSED(hr);

#ifdef PX2_PDR_DEBUG
	IDirect3DVertexDeclaration9* activeDeclaration = 0;
	hr = device->GetVertexDeclaration(&activeDeclaration);
	assertion(hr == D3D_OK, "Failed to get vertex declaration: %s\n",
		DXGetErrorString(hr));
	assertion(activeDeclaration == mDeclaration,
		"Mismatched vertex declarations\n");
	activeDeclaration->Release();
#endif

	hr = device->SetVertexDeclaration(0);
	assertion(hr == D3D_OK, "Failed to set vertex declaration: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

#endif

