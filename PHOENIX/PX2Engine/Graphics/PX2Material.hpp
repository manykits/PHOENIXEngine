// PX2Material.hpp

#ifndef PX2MATERIAL_HPP
#define PX2MATERIAL_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2FileIO.hpp"
#include "PX2MaterialTechnique.hpp"

namespace PX2
{

	class MaterialInstance;

	/// 材质模板类
	class PX2_ENGINE_ITEM Material : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Material);

	public:
		Material();
		virtual ~Material ();

		/// 在渲染技术数组末尾插入一个渲染技术
		void InsertTechnique (MaterialTechnique* technique);

		inline int GetNumTechniques () const;
		MaterialTechnique* GetTechnique (int techniqueIndex) const;
		MaterialTechnique* GetTechnique(const std::string &techniqueName, int &index) const;

		int GetNumPasses (int techniqueIndex) const;
		MaterialPass* GetPass (int techniqueIndex, int passIndex) const;
		VertexShader* GetVertexShader (int techniqueIndex, int passIndex) const;
		PixelShader* GetPixelShader (int techniqueIndex, int passIndex) const;
		AlphaProperty* GetAlphaProperty (int techniqueIndex, int passIndex) const;
		CullProperty* GetCullProperty (int techniqueIndex, int passIndex) const;
		DepthProperty* GetDepthProperty (int techniqueIndex, int passIndex) const;
		OffsetProperty* GetOffsetProperty (int techniqueIndex, int passIndex) const;
		StencilProperty* GetStencilProperty (int techniqueIndex, int passIndex) const;
		WireProperty* GetWireProperty (int techniqueIndex, int passIndex) const;

	protected:
		std::vector<MaterialTechniquePtr> mTechniques;
	};

	PX2_REGISTER_STREAM(Material);
	typedef Pointer0<Material> MaterialPtr;
#include "PX2Material.inl"

}

#endif