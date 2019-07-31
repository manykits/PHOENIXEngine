// PX2VertexColor4Material.hpp

#ifndef PX2VERTEXCOLOR4MATERIAL_HPP
#define PX2VERTEXCOLOR4MATERIAL_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2MaterialInstance.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM VertexColor4Material : public Material
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(VertexColor4Material);

	public:
		VertexColor4Material ();
		virtual ~VertexColor4Material ();

		MaterialInstance* CreateInstance () const;

		static MaterialInstance* CreateUniqueInstance ();

	private:
		static int msDx9VRegisters[1];
		static int msOglVRegisters[1];
		static int msOpenGLES2VRegisters[1];
		static int* msVRegisters[Shader::MAX_PROFILES];
		static std::string msVPrograms[Shader::MAX_PROFILES];
		static std::string msPPrograms[Shader::MAX_PROFILES];
	};

	PX2_REGISTER_STREAM(VertexColor4Material);
	typedef Pointer0<VertexColor4Material> VertexColor4MaterialPtr;

}

#endif