// PX2OpenGLESMapping.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLMAPPING_HPP
#define PX2OPENGLMAPPING_HPP

#include "PX2OpenGLESRendererPre.hpp"
#include "PX2AlphaProperty.hpp"
#include "PX2Buffer.hpp"
#include "PX2CullProperty.hpp"
#include "PX2DepthProperty.hpp"
#include "PX2Shader.hpp"
#include "PX2StencilProperty.hpp"
#include "PX2Texture.hpp"
#include "PX2VertexFormat.hpp"
#include "PX2Renderable.hpp"

namespace PX2
{
	extern GLenum gOGLAlphaSrcBlend[AlphaProperty::SBM_QUANTITY];
	extern GLenum gOGLAlphaDstBlend[AlphaProperty::DBM_QUANTITY];
	extern GLenum gOGLAlphaCompare[AlphaProperty::CM_QUANTITY];
	extern GLenum gOGLDepthCompare[AlphaProperty::CM_QUANTITY];
	extern GLenum gOGLStencilCompare[StencilProperty::CM_QUANTITY];
	extern GLenum gOGLStencilOperation[StencilProperty::OT_QUANTITY];
	extern GLint  gOGLAttributeChannels[VertexFormat::AT_QUANTITY];
	extern GLuint gOGLAttributeType[VertexFormat::AT_QUANTITY];
	extern GLuint gOGLBufferLocking[Buffer::BL_QUANTITY];
	extern GLuint gOGLBufferUsage[Buffer::BU_QUANTITY];
	extern GLint  gOGLMinFilter[Shader::SF_QUANTITY];
	extern GLuint gOGLTextureFormat[Texture::TF_QUANTITY];
	extern GLuint gOGLTextureInternalFormat[Texture::TF_QUANTITY];
	extern GLuint gOGLTextureTarget[Shader::ST_QUANTITY];
	extern GLuint gOGLTextureType[Texture::TF_QUANTITY];
	extern GLint  gOGLWrapMode[Shader::SC_QUANTITY];
	extern GLenum gOGLPrimitiveType[Renderable::PT_MAX_QUANTITY];

	// Support for binding and rebinding.  BindTexture binds the input texture
	// to the specified target and returns the previously bound texture for
	// that target.  GetBoundTexture returns the currently bound texture for
	// the specified target.
	GLuint BindTexture(Shader::SamplerType target, GLuint texture);
	GLuint GetBoundTexture(Shader::SamplerType target);
}

#endif

#endif