// PX2MaterialPass.hpp

#ifndef PX2MATERIALPASS_HPP
#define PX2MATERIALPASS_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2VertexShader.hpp"
#include "PX2PixelShader.hpp"
#include "PX2AlphaProperty.hpp"
#include "PX2CullProperty.hpp"
#include "PX2DepthProperty.hpp"
#include "PX2OffsetProperty.hpp"
#include "PX2StencilProperty.hpp"
#include "PX2WireProperty.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM MaterialPass : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(MaterialPass);

	public:
		MaterialPass ();
		virtual ~MaterialPass ();

		// 设置渲染状态
		inline void SetVertexShader (VertexShader* vshader);
		inline void SetPixelShader (PixelShader* pshader);
		inline void SetAlphaProperty (AlphaProperty* alphaState);
		inline void SetCullProperty (CullProperty* cullState);
		inline void SetDepthProperty (DepthProperty* depthState);
		inline void SetOffsetProperty (OffsetProperty* offsetState);
		inline void SetStencilProperty (StencilProperty* stencilState);
		inline void SetWireProperty (WireProperty* wireState);

		// 成员访问
		inline VertexShader* GetVertexShader () const;
		inline PixelShader* GetPixelShader () const;
		inline AlphaProperty* GetAlphaProperty () const;
		inline CullProperty* GetCullProperty () const;
		inline DepthProperty* GetDepthProperty () const;
		inline OffsetProperty* GetOffsetProperty () const;
		inline StencilProperty* GetStencilProperty () const;
		inline WireProperty* GetWireProperty () const;

		// pdr
	public:
		void *PdrPointer;

	protected:
		VertexShaderPtr mVShader;
		PixelShaderPtr mPShader;
		AlphaPropertyPtr mAlphaProperty;
		CullPropertyPtr mCullProperty;
		DepthPropertyPtr mDepthProperty;
		OffsetPropertyPtr mOffsetProperty;
		StencilPropertyPtr mStencilProperty;
		WirePropertyPtr mWireProperty;
	};

	PX2_REGISTER_STREAM(MaterialPass);
	typedef Pointer0<MaterialPass> MaterialPassPtr;
#include "PX2MaterialPass.inl"

}

#endif
