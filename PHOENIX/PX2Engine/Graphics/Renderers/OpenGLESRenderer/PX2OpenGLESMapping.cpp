// PX2OpenGLESMapping.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESMapping.hpp"

namespace PX2
{

#define NOT_SUPPORT 0
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                   0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                   0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                  0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                  0x8C03
#define GL_ETC1_RGB8_OES                                     0x8D64

	GLenum gOGLAlphaSrcBlend[AlphaProperty::SBM_QUANTITY] =
	{
		GL_ZERO,                        // SBM_ZERO
		GL_ONE,                         // SBM_ONE
		GL_DST_COLOR,                   // SBM_DST_COLOR
		GL_ONE_MINUS_DST_COLOR,         // SBM_ONE_MINUS_DST_COLOR
		GL_SRC_ALPHA,                   // SBM_SRC_ALPHA
		GL_ONE_MINUS_SRC_ALPHA,         // SBM_ONE_MINUS_SRC_ALPHA
		GL_DST_ALPHA,                   // SBM_DST_ALPHA
		GL_ONE_MINUS_DST_ALPHA,         // SBM_ONE_MINUS_DST_ALPHA
		GL_SRC_ALPHA_SATURATE,          // SBM_SRC_ALPHA_SATURATE
		GL_CONSTANT_COLOR,              // SBM_CONSTANT_COLOR
		GL_ONE_MINUS_CONSTANT_COLOR,    // SBM_ONE_MINUS_CONSTANT_COLOR
		GL_CONSTANT_ALPHA,              // SBM_CONSTANT_ALPHA
		GL_ONE_MINUS_CONSTANT_ALPHA     // SBM_ONE_MINUS_CONSTANT_ALPHA
	};

	GLenum gOGLAlphaDstBlend[AlphaProperty::DBM_QUANTITY] =
	{
		GL_ZERO,                        // DBM_ZERO
		GL_ONE,                         // DBM_ONE
		GL_SRC_COLOR,                   // DBM_SRC_COLOR
		GL_ONE_MINUS_SRC_COLOR,         // DBM_ONE_MINUS_SRC_COLOR
		GL_SRC_ALPHA,                   // DBM_SRC_ALPHA
		GL_ONE_MINUS_SRC_ALPHA,         // DBM_ONE_MINUS_SRC_ALPHA
		GL_DST_ALPHA,                   // DBM_DST_ALPHA
		GL_ONE_MINUS_DST_ALPHA,         // DBM_ONE_MINUS_DST_ALPHA
		GL_CONSTANT_COLOR,              // DBM_CONSTANT_COLOR
		GL_ONE_MINUS_CONSTANT_COLOR,    // DBM_ONE_MINUS_CONSTANT_COLOR
		GL_CONSTANT_ALPHA,              // DBM_CONSTANT_ALPHA
		GL_ONE_MINUS_CONSTANT_ALPHA     // DBM_ONE_MINUS_CONSTANT_ALPHA
	};

	GLenum gOGLAlphaCompare[AlphaProperty::CM_QUANTITY] =
	{
		GL_NEVER,       // CM_NEVER
		GL_LESS,        // CM_LESS
		GL_EQUAL,       // CM_EQUAL
		GL_LEQUAL,      // CM_LEQUAL
		GL_GREATER,     // CM_GREATER
		GL_NOTEQUAL,    // CM_NOTEQUAL
		GL_GEQUAL,      // CM_GEQUAL
		GL_ALWAYS       // CM_ALWAYS
	};

	GLenum gOGLDepthCompare[DepthProperty::CM_QUANTITY] = 
	{
		GL_NEVER,       // CM_NEVER
		GL_LESS,        // CM_LESS
		GL_EQUAL,       // CM_EQUAL
		GL_LEQUAL,      // CM_LEQUAL
		GL_GREATER,     // CM_GREATER
		GL_NOTEQUAL,    // CM_NOTEQUAL
		GL_GEQUAL,      // CM_GEQUAL
		GL_ALWAYS       // CM_ALWAYS
	};

	GLenum gOGLStencilCompare[StencilProperty::CM_QUANTITY] =
	{
		GL_NEVER,       // CM_NEVER
		GL_LESS,        // CM_LESS
		GL_EQUAL,       // CM_EQUAL
		GL_LEQUAL,      // CM_LEQUAL
		GL_GREATER,     // CM_GREATER
		GL_NOTEQUAL,    // CM_NOTEQUAL
		GL_GEQUAL,      // CM_GEQUAL
		GL_ALWAYS       // CM_ALWAYS
	};

	GLenum gOGLStencilOperation[StencilProperty::OT_QUANTITY] =
	{
		GL_KEEP,    // OT_KEEP
		GL_ZERO,    // OT_ZERO
		GL_REPLACE, // OT_REPLACE
		GL_INCR,    // OT_INCREMENT
		GL_DECR,    // OT_DECREMENT
		GL_INVERT   // OT_INVERT
	};

	GLint gOGLAttributeChannels[VertexFormat::AT_QUANTITY] =
	{
		0,  // AT_NONE (unsupported)
		1,  // AT_FLOAT1
		2,  // AT_FLOAT2
		3,  // AT_FLOAT3
		4,  // AT_FLOAT4
		1,  // AT_HALF1
		2,  // AT_HALF2
		3,  // AT_HALF3
		4,  // AT_HALF4
		4,  // AT_UBYTE4
		1,  // AT_SHORT1
		2,  // AT_SHORT2
		4   // AT_SHORT4
	};

	GLuint gOGLAttributeType[VertexFormat::AT_QUANTITY] =
	{
		0,                          // AT_NONE (unsupported)
		GL_FLOAT,                   // AT_FLOAT1
		GL_FLOAT,                   // AT_FLOAT2
		GL_FLOAT,                   // AT_FLOAT3
		GL_FLOAT,                   // AT_FLOAT4
		NOT_SUPPORT,				// AT_HALF1
		NOT_SUPPORT,				// AT_HALF2
		NOT_SUPPORT,				// AT_HALF3
		NOT_SUPPORT,				// AT_HALF4
		GL_UNSIGNED_BYTE,           // AT_UBYTE4
		GL_SHORT,                   // AT_SHORT1
		GL_SHORT,                   // AT_SHORT2
		GL_SHORT                    // AT_SHORT4
	};

	GLuint gOGLBufferLocking[Buffer::BL_QUANTITY] =
	{
		NOT_SUPPORT,	// BL_READ_ONLY
		NOT_SUPPORT,	// BL_WRITE_ONLY
		NOT_SUPPORT		// BL_READ_WRITE
	};

	GLuint gOGLBufferUsage[Buffer::BU_QUANTITY] =
	{
		GL_STATIC_DRAW,     // BU_STATIC
		GL_DYNAMIC_DRAW,    // BU_DYNAMIC
		GL_DYNAMIC_DRAW,    // BU_RENDERTARGET
		GL_DYNAMIC_DRAW,    // BU_DEPTHSTENCIL
		GL_DYNAMIC_DRAW     // BU_TEXTURE
	};

	GLint gOGLMinFilter[Shader::SF_QUANTITY] =
	{
		0,                          // SF_NONE
		GL_NEAREST,                 // SF_NEAREST
		GL_LINEAR,                  // SF_LINEAR
		GL_NEAREST_MIPMAP_NEAREST,  // SF_NEAREST_NEAREST
		GL_NEAREST_MIPMAP_LINEAR,   // SF_NEAREST_LINEAR
		GL_LINEAR_MIPMAP_NEAREST,   // SF_LINEAR_NEAREST
		GL_LINEAR_MIPMAP_LINEAR     // SF_LINEAR_LINEAR
	};

	GLuint gOGLTextureInternalFormat[Texture::TF_QUANTITY] =
	{
		0,                                  // TF_NONE
		GL_RGB,                             // TF_R5G6B5
		GL_RGBA,                            // TF_A1R5G5B5
		GL_RGBA,                            // TF_A4R4G4B4
		GL_ALPHA,	                        // TF_A8
		GL_LUMINANCE,						// TF_L8
		GL_LUMINANCE_ALPHA,					// TF_A8L8
		GL_RGB,								// TF_R8G8B8
		GL_RGBA,							// TF_A8R8G8B8
		NOT_SUPPORT,                        // TF_A8B8G8R8
		NOT_SUPPORT,						// TF_L16
		NOT_SUPPORT,                        // TF_G16R16
		NOT_SUPPORT,                        // TF_A16B16G16R16
		NOT_SUPPORT,						// TF_R16F
		NOT_SUPPORT,                        // TF_G16R16F
		NOT_SUPPORT,						// TF_A16B16G16R16F
		NOT_SUPPORT,                        // TF_R32F
		NOT_SUPPORT,                        // TF_G32R32F
		NOT_SUPPORT,						// TF_A32B32G32R32F
		NOT_SUPPORT,						// TF_DXT1
		NOT_SUPPORT,						// TF_DXT3
		NOT_SUPPORT,						// TF_DXT5
		GL_DEPTH_COMPONENT,					// TF_D16
		NOT_SUPPORT,						// TF_D24S8
		GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, // TF_RGB_PVRTC_4B
		GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, // TF_RGB_PVRTC_2B
		GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, // TF_RGBA_PVRTC_4B
		GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG, // TF_RGBA_PVRTC_2B
		GL_ETC1_RGB8_OES                     // TF_RGB_ETC1
	};

	GLuint gOGLTextureFormat[Texture::TF_QUANTITY] =
	{
		0,                                  // TF_NONE
		GL_RGB,                             // TF_R5G6B5
		GL_RGBA,                            // TF_A1R5G5B5
		GL_RGBA,                            // TF_A4R4G4B4
		GL_ALPHA,                           // TF_A8
		GL_LUMINANCE,                       // TF_L8
		GL_LUMINANCE_ALPHA,                 // TF_A8L8
		GL_RGB,								// TF_R8G8B8
		GL_RGBA,							// TF_A8R8G8B8
		GL_RGBA,                            // TF_A8B8G8R8
		GL_LUMINANCE,                       // TF_L16
		NOT_SUPPORT,                        // TF_G16R16
		GL_RGBA,                            // TF_A16B16G16R16
		NOT_SUPPORT,                        // TF_R16F
		NOT_SUPPORT,                        // TF_G16R16F
		GL_RGBA,                            // TF_A16B16G16R16F
		NOT_SUPPORT,                        // TF_R32F
		NOT_SUPPORT,                        // TF_G32R32F
		GL_RGBA,                            // TF_A32B32G32R32F
		NOT_SUPPORT,						// TF_DXT1
		NOT_SUPPORT,						// TF_DXT3
		NOT_SUPPORT,						// TF_DXT5
		GL_DEPTH_COMPONENT,					// TF_D16
		NOT_SUPPORT,						// TF_D24S8
		GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG, // TF_RGB_PVRTC_4B
		GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG, // TF_RGB_PVRTC_2B
		GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG, // TF_RGBA_PVRTC_4B
		GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG,  // TF_RGBA_PVRTC_2B
		GL_ETC1_RGB8_OES                      // TF_RGB_ETC1
	};

	GLuint gOGLTextureTarget[Shader::ST_QUANTITY] =
	{
		0,                  // ST_NONE
		NOT_SUPPORT,      // ST_1D
		GL_TEXTURE_2D,      // ST_2D
		NOT_SUPPORT,      // ST_3D
		GL_TEXTURE_CUBE_MAP // ST_CUBE
	};

	GLuint gOGLTextureTargetBinding[Shader::ST_QUANTITY] =
	{
		0,                          // ST_NONE
		0,							// ST_1D
		GL_TEXTURE_BINDING_2D,      // ST_2D
		0,							// ST_3D
		GL_TEXTURE_BINDING_CUBE_MAP // ST_CUBE
	};

	GLuint gOGLTextureType[Texture::TF_QUANTITY] =
	{
		0,                              // TF_NONE
		GL_UNSIGNED_SHORT_5_6_5,		// TF_R5G6B5
		GL_UNSIGNED_SHORT_5_5_5_1,		// TF_A1R5G5B5
		GL_UNSIGNED_SHORT_4_4_4_4,		// TF_A4R4G4B4
		GL_UNSIGNED_BYTE,               // TF_A8
		GL_UNSIGNED_BYTE,               // TF_L8
		GL_UNSIGNED_BYTE,               // TF_A8L8
		GL_UNSIGNED_BYTE,               // TF_R8G8B8
		GL_UNSIGNED_BYTE,               // TF_A8R8G8B8
		GL_UNSIGNED_BYTE,               // TF_A8B8G8R8
		GL_UNSIGNED_SHORT,              // TF_L16
		GL_UNSIGNED_SHORT,              // TF_G16R16
		GL_UNSIGNED_SHORT,              // TF_A16B16G16R16
		NOT_SUPPORT,					// TF_R16F
		NOT_SUPPORT,					// TF_G16R16F
		NOT_SUPPORT,					// TF_A16B16G16R16F
		GL_FLOAT,                       // TF_R32F
		GL_FLOAT,                       // TF_G32R32F
		GL_FLOAT,                       // TF_A32B32G32R32F
		GL_NONE,                        // TF_DXT1 (not needed)
		GL_NONE,                        // TF_DXT3 (not needed)
		GL_NONE,                        // TF_DXT5 (not needed)
		GL_UNSIGNED_SHORT,				// TF_D16
		NOT_SUPPORT,					// TF_D24S8
		GL_NONE,						// TF_RGB_PVRTC_4B
		GL_NONE,						// TF_RGB_PVRTC_2B
		GL_NONE,						// TF_RGBA_PVRTC_4B
		GL_NONE,						// TF_RGBA_PVRTC_2B
		GL_NONE							// TF_RGB_ETC1
	};

	GLint gOGLWrapMode[Shader::SC_QUANTITY] =
	{
		NOT_SUPPORT,           // SC_NONE
		GL_CLAMP_TO_EDGE,           // SC_CLAMP
		GL_REPEAT,          // SC_REPEAT
		GL_MIRRORED_REPEAT, // SC_MIRRORED_REPEAT
		GL_REPEAT, // SC_CLAMP_BORDER
		GL_CLAMP_TO_EDGE,   // SC_CLAMP_EDGE
	};

	GLenum gOGLPrimitiveType[Renderable::PT_MAX_QUANTITY] =
	{
		0,                  // PT_NONE (not used)
		GL_POINTS,          // PT_POLYPOINT
		GL_LINES,           // PT_POLYSEGMENTS_DISJOINT
		GL_LINE_STRIP,      // PT_POLYSEGMENTS_CONTIGUOUS
		GL_TRIANGLES,       // PT_TRIANGLES (not used)
		GL_TRIANGLES,       // PT_TRIMESH
		GL_TRIANGLE_STRIP,  // PT_TRISTRIP
		GL_TRIANGLE_FAN     // PT_TRIFAN
	};

	GLuint BindTexture(Shader::SamplerType target, GLuint texture)
	{
		GLint current = 0;
		glGetIntegerv(gOGLTextureTargetBinding[target], &current);
		glBindTexture(gOGLTextureTarget[target], texture);
		return (GLuint)current;
	}
	//----------------------------------------------------------------------------
	GLuint GetBoundTexture(Shader::SamplerType target)
	{
		GLint current = 0;
		glGetIntegerv(gOGLTextureTargetBinding[target], &current);
		return (GLuint)current;
	}
	//----------------------------------------------------------------------------
}

#endif