// PX2Renderers.hpp

#ifndef PX2RENDERERS_HPP
#define PX2RENDERERS_HPP

#include "PX2Renderer.hpp"

#ifdef PX2_USE_DX9
#include "PX2Dx9IndexBuffer.hpp"
#include "PX2Dx9Mapping.hpp"
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
#include "PX2OpenGLESIndexBuffer.hpp"
#include "PX2OpenGLESPixelShader.hpp"
#include "PX2OpenGLESRendererData.hpp"
#include "PX2OpenGLESShader.hpp"
#include "PX2OpenGLESTexture1D.hpp"
#include "PX2OpenGLESTexture2D.hpp"
#include "PX2OpenGLESTexture3D.hpp"
#include "PX2OpenGLESTextureCube.hpp"
#include "PX2OpenGLESRenderTarget.hpp"
#include "PX2OpenGLESVertexBuffer.hpp"
#include "PX2OpenGLESVertexFormat.hpp"
#include "PX2OpenGLESVertexShader.hpp"
#endif

#endif
