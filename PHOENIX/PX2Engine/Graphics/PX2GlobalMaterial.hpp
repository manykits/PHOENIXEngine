// PX2GlobalMaterial.hpp

#ifndef PX2GLOBALEFFECT_HPP
#define PX2GLOBALEFFECT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2Renderer.hpp"
#include "PX2VisibleSet.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM GlobalMaterial : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(GlobalMaterial);

	protected:
		GlobalMaterial ();
	public:
		virtual ~GlobalMaterial ();

		// 当globalMaterial指针不为空时，这个函数被Renderer::Draw(visibleSet,
		// globalMaterial)调用。
		virtual void Draw (Renderer* renderer, const VisibleSet& visibleSet) = 0;
	};

	PX2_REGISTER_STREAM(GlobalMaterial);
	typedef Pointer0<GlobalMaterial> GlobalMaterialPtr;

}

#endif
