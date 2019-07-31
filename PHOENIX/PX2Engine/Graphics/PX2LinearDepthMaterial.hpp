// PX2LinearDepthMaterial.hpp

#ifndef PX2LINEARDEPTHMATERIAL_HPP
#define PX2LINEARDEPTHMATERIAL_HPP

#include "PX2GlobalMaterial.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM LinearDepth_Material : public GlobalMaterial
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(LinearDepth_Material);

	public:
		LinearDepth_Material();
		virtual ~LinearDepth_Material();

		virtual void Draw(Renderer* renderer, const VisibleSet& visibleSet);

	private:
		OffsetPropertyPtr mOverRideOffsetProperty;
		MaterialInstancePtr mInstanceStd;
		MaterialInstancePtr mInstanceSkinSkeleton;
		MaterialInstancePtr mInstanceTerrain;
	};

	PX2_REGISTER_STREAM(LinearDepth_Material);
	typedef Pointer0<LinearDepth_Material> LinearDepth_MaterialPtr;

}

#endif
