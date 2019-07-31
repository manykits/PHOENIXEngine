// PX2Dx9Mapping.cpp

#ifdef PX2_USE_DX9

#include "PX2Dx9Mapping.hpp"

namespace PX2
{

BYTE gDX9AttributeType[VertexFormat::AT_QUANTITY] =
{
	255,                        // AT_NONE (unsupported)
	D3DDECLTYPE_FLOAT1,         // AT_FLOAT1
	D3DDECLTYPE_FLOAT2,         // AT_FLOAT2
	D3DDECLTYPE_FLOAT3,         // AT_FLOAT3
	D3DDECLTYPE_FLOAT4,         // AT_FLOAT4
	255,                        // AT_HPPALF1 (unsupported)
	D3DDECLTYPE_FLOAT16_2,      // AT_HPPALF2
	255,                        // AT_HPPALF3 (unsupported)
	D3DDECLTYPE_FLOAT16_4,      // AT_HPPALF4
	D3DDECLTYPE_D3DCOLOR,       // AT_UBYTE4
	255,                        // AT_SHORT1 (unsupported)
	D3DDECLTYPE_SHORT2,         // AT_SHORT2
	D3DDECLTYPE_SHORT4          // AT_SHORT4
};

BYTE gDX9AttributeUsage[VertexFormat::AU_QUANTITY] =
{
	255,                        // AU_NONE (unsupported)
	D3DDECLUSAGE_POSITION,      // AU_POSITION
	D3DDECLUSAGE_NORMAL,        // AU_NORMAL
	D3DDECLUSAGE_TANGENT,       // AU_TANGENT
	D3DDECLUSAGE_BINORMAL,      // AU_BINORMAL
	D3DDECLUSAGE_TEXCOORD,      // AU_TEXCOORD
	D3DDECLUSAGE_COLOR,         // AU_COLOR
	D3DDECLUSAGE_BLENDINDICES,  // AU_BLENDINDICES
	D3DDECLUSAGE_BLENDWEIGHT,   // AU_BLENDWEIGHT
	D3DDECLUSAGE_TESSFACTOR,    // AU_FOGCOORD
	D3DDECLUSAGE_PSIZE          // AU_PSIZE
};

DWORD gDX9BufferUsage[Buffer::BU_QUANTITY] =
{
	D3DUSAGE_WRITEONLY,         // BU_STATIC
	D3DUSAGE_DYNAMIC,           // BU_DYNAMIC
	D3DUSAGE_RENDERTARGET,      // BU_RENDERTARGET
	D3DUSAGE_DEPTHSTENCIL,      // BU_DEPTHSTENCIL
	0                           // BU_TEXTURE
};

DWORD gDX9BufferLocking[Buffer::BL_QUANTITY] =
{
	D3DLOCK_READONLY,           // BL_READ_ONLY
	0,                          // BL_WRITE_ONLY
	0                           // BL_READ_WRITE
};

D3DFORMAT gDX9TextureFormat[Texture::TF_QUANTITY] =
{
	D3DFMT_UNKNOWN,             // TF_NONE
	D3DFMT_R5G6B5,              // TF_R5G6B5
	D3DFMT_A1R5G5B5,            // TF_A1R5G5B5
	D3DFMT_A4R4G4B4,            // TF_A4R4G4B4
	D3DFMT_A8,                  // TF_A8
	D3DFMT_L8,                  // TF_L8
	D3DFMT_A8L8,                // TF_A8L8
	D3DFMT_R8G8B8,              // TF_R8G8B8
	D3DFMT_A8R8G8B8,            // TF_A8R8G8B8
	D3DFMT_A8B8G8R8,            // TF_A8B8G8R8
	D3DFMT_L16,                 // TF_L16
	D3DFMT_G16R16,              // TF_G16R16
	D3DFMT_A16B16G16R16,        // TF_A16B16G16R16
	D3DFMT_R16F,                // TF_R16F
	D3DFMT_G16R16F,             // TF_G16R16F
	D3DFMT_A16B16G16R16F,       // TF_A16B16G16R16F
	D3DFMT_R32F,                // TF_R32F
	D3DFMT_G32R32F,             // TF_G32R32F
	D3DFMT_A32B32G32R32F,       // TF_A32B32G32R32F
	D3DFMT_DXT1,                // TF_DXT1
	D3DFMT_DXT3,                // TF_DXT3
	D3DFMT_DXT5,                // TF_DXT5
	D3DFMT_D16,					// TF_D16
	D3DFMT_D24S8,               // TF_D24S8
	D3DFMT_UNKNOWN,				// TF_RGB_PVRTC_4B
	D3DFMT_UNKNOWN,				// TF_RGB_PVRTC_2B
	D3DFMT_UNKNOWN,				// TF_RGBA_PVRTC_4B
	D3DFMT_UNKNOWN,				// TF_RGBA_PVRTC_2B
	D3DFMT_UNKNOWN				// TF_RGB_ETC1
};

DWORD gDX9MinFilter[Shader::SF_QUANTITY] =
{
	D3DTEXF_NONE,               // Shader::SF_NONE
	D3DTEXF_POINT,              // Shader::SF_NEAREST
	D3DTEXF_LINEAR,             // Shader::SF_LINEAR
	D3DTEXF_POINT,              // Shader::SF_NEAREST_NEAREST
	D3DTEXF_POINT,              // Shader::SF_NEAREST_LINEAR
	D3DTEXF_LINEAR,             // Shader::SF_LINEAR_NEAREST
	D3DTEXF_LINEAR              // Shader::SF_LINEAR_LINEAR
};

DWORD gDX9MipFilter[Shader::SF_QUANTITY] =
{
	D3DTEXF_NONE,               // Shader::SF_NONE
	D3DTEXF_NONE,               // Shader::SF_NEAREST
	D3DTEXF_NONE,               // Shader::SF_LINEAR
	D3DTEXF_POINT,              // Shader::SF_NEAREST_NEAREST
	D3DTEXF_LINEAR,             // Shader::SF_NEAREST_LINEAR
	D3DTEXF_POINT,              // Shader::SF_LINEAR_NEAREST
	D3DTEXF_LINEAR              // Shader::SF_LINEAR_LINEAR
};

DWORD gDX9WrapMode[Shader::SC_QUANTITY] =
{
	0,                          // Shader::SC_NONE
	D3DTADDRESS_CLAMP,          // Shader::SC_CLAMP
	D3DTADDRESS_WRAP,           // Shader::SC_REPEAT
	D3DTADDRESS_MIRROR,         // Shader::SC_TDRORED_REPEAT
	D3DTADDRESS_BORDER,         // Shader::SC_CLAMP_BORDER
	D3DTADDRESS_CLAMP           // Shader::SC_CLAMP_EDGE
};

DWORD gDX9AlphaSrcBlend[AlphaProperty::SBM_QUANTITY] =
{
	D3DBLEND_ZERO,              // AlphaProperty::SBM_ZERO
	D3DBLEND_ONE,               // AlphaProperty::SBM_ONE
	D3DBLEND_DESTCOLOR,         // AlphaProperty::SBM_DST_COLOR
	D3DBLEND_INVDESTCOLOR,      // AlphaProperty::SBM_ONE_MINUS_DST_COLOR
	D3DBLEND_SRCALPHA,          // AlphaProperty::SBM_SRC_ALPHA
	D3DBLEND_INVSRCALPHA,       // AlphaProperty::SBM_ONE_MINUS_SRC_ALPHA
	D3DBLEND_SRCCOLOR,			// AlphaProperty::SBM_SRC_COLOR
	D3DBLEND_INVSRCCOLOR,		// AlphaProperty::SBM_ONE_MINUS_SRC_COLOR
	D3DBLEND_DESTALPHA,         // AlphaProperty::SBM_DST_ALPHA
	D3DBLEND_INVDESTALPHA,      // AlphaProperty::SBM_ONE_MINUS_DST_ALPHA
	D3DBLEND_SRCALPHASAT,       // AlphaProperty::SBM_SRC_ALPHA_SATURATE
	0,                          // AlphaProperty::SBM_CONSTANT_COLOR
	0,                          // AlphaProperty::SBM_ONE_MINUS_CONSTANT_COLOR
	0,                          // AlphaProperty::SBM_CONSTANT_ALPHA
	0                           // AlphaProperty::SBM_ONE_MINUS_CONSTANT_ALPHA
};

DWORD gDX9AlphaDstBlend[AlphaProperty::DBM_QUANTITY] =
{
	D3DBLEND_ZERO,              // AlphaProperty::DBM_ZERO
	D3DBLEND_ONE,               // AlphaProperty::DBM_ONE
	D3DBLEND_SRCCOLOR,          // AlphaProperty::DBM_SRC_COLOR
	D3DBLEND_INVSRCCOLOR,       // AlphaProperty::DBM_ONE_MINUS_SRC_COLOR
	D3DBLEND_SRCALPHA,          // AlphaProperty::DBM_SRC_ALPHA
	D3DBLEND_INVSRCALPHA,       // AlphaProperty::DBM_ONE_MINUS_SRC_ALPHA
	D3DBLEND_DESTALPHA,         // AlphaProperty::DBM_DST_ALPHA
	D3DBLEND_INVDESTALPHA,      // AlphaProperty::DBM_ONE_MINUS_DST_ALPHA
	D3DBLEND_DESTCOLOR,			// AlphaProperty::DBM_DST_COLOR
	D3DBLEND_INVDESTCOLOR,		// AlphaProperty::DBM_ONE_MINUS_DST_COLOR
	0,                          // AlphaProperty::DBM_CONSTANT_COLOR
	0,                          // AlphaProperty::DBM_ONE_MINUS_CONSTANT_COLOR
	0,                          // AlphaProperty::DBM_CONSTANT_ALPHA
	0                           // AlphaProperty::DBM_ONE_MINUS_CONSTANT_ALPHA
};

DWORD gDX9AlphaCompare[AlphaProperty::CM_QUANTITY] =
{
	D3DCMP_NEVER,               // AlphaProperty::CM_NEVER
	D3DCMP_LESS,                // AlphaProperty::CM_LESS
	D3DCMP_EQUAL,               // AlphaProperty::CM_EQUAL
	D3DCMP_LESSEQUAL,           // AlphaProperty::CM_LEQUAL
	D3DCMP_GREATER,             // AlphaProperty::CM_GREATER
	D3DCMP_NOTEQUAL,            // AlphaProperty::CM_NOTEQUAL
	D3DCMP_GREATEREQUAL,        // AlphaProperty::CM_GEQUAL
	D3DCMP_ALWAYS               // AlphaProperty::CM_ALWAYS
};

DWORD gDX9CullOrder[2] = 
{
	D3DCULL_CCW,                // CullProperty::mCCWOrder = true
	D3DCULL_CW                  // CullProperty::mCCWOrder = false
};

DWORD gDX9DepthCompare[DepthProperty::CM_QUANTITY] = 
{
	D3DCMP_NEVER,               // DepthProperty::CM_NEVER
	D3DCMP_LESS,                // DepthProperty::CM_LESS
	D3DCMP_EQUAL,               // DepthProperty::CM_EQUAL
	D3DCMP_LESSEQUAL,           // DepthProperty::CM_LEQUAL
	D3DCMP_GREATER,             // DepthProperty::CM_GREATER
	D3DCMP_NOTEQUAL,            // DepthProperty::CM_NOTEQUAL
	D3DCMP_GREATEREQUAL,        // DepthProperty::CM_GEQUAL
	D3DCMP_ALWAYS               // DepthProperty::CM_ALWAYS
};

DWORD gDX9StencilCompare[StencilProperty::CM_QUANTITY] = 
{
	D3DCMP_NEVER,               // StencilProperty::CF_NEVER
	D3DCMP_LESS,                // StencilProperty::CF_LESS
	D3DCMP_EQUAL,               // StencilProperty::CF_EQUAL
	D3DCMP_LESSEQUAL,           // StencilProperty::CF_LEQUAL
	D3DCMP_GREATER,             // StencilProperty::CF_GREATER
	D3DCMP_NOTEQUAL,            // StencilProperty::CF_NOTEQUAL
	D3DCMP_GREATEREQUAL,        // StencilProperty::CF_GEQUAL
	D3DCMP_ALWAYS               // StencilProperty::CF_ALWAYS
};

DWORD gDX9StencilOperation[StencilProperty::OT_QUANTITY] =
{
	D3DSTENCILOP_KEEP,          // StencilProperty::OT_KEEP
	D3DSTENCILOP_ZERO,          // StencilProperty::OT_ZERO
	D3DSTENCILOP_REPLACE,       // StencilProperty::OT_REPLACE
	D3DSTENCILOP_INCR,          // StencilProperty::OT_INCREMENT
	D3DSTENCILOP_DECR,          // StencilProperty::OT_DECREMENT
	D3DSTENCILOP_INVERT         // StencilProperty::OT_INVERT
};

D3DPRIMITIVETYPE gDX9PrimitiveType[Renderable::PT_MAX_QUANTITY] =
{
	D3DPT_FORCE_DWORD,          // PT_NONE (not used)
	D3DPT_POINTLIST,            // PT_POLYPOINT
	D3DPT_LINELIST,             // PT_POLYSEGMENTS_DISJOINT
	D3DPT_LINESTRIP,            // PT_POLYSEGMENTS_CONTIGUOUS
	D3DPT_FORCE_DWORD,          // PT_TRIANGLES (not used)
	D3DPT_TRIANGLELIST,         // PT_TRIMESH
	D3DPT_TRIANGLESTRIP,        // PT_TRISTRIP
	D3DPT_TRIANGLEFAN           // PT_TRIFAN
};

}

#endif