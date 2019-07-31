// PX2ShadowMapMaterial.hpp

#ifndef PX2SHADOWMAPMATERIAL_HPP
#define PX2SHADOWMAPMATERIAL_HPP

#include "PX2GlobalMaterial.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ShadowMap_Material : public GlobalMaterial
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ShadowMap_Material);

	public:
		ShadowMap_Material();
		virtual ~ShadowMap_Material();

		virtual void Draw(Renderer* renderer, const VisibleSet& visibleSet);

	private:
		OffsetPropertyPtr mOverRideOffsetProperty;
		MaterialInstancePtr mInstanceStd;
		MaterialInstancePtr mInstanceSkinSkeleton;
		MaterialInstancePtr mInstanceTerrain;
	};

	PX2_REGISTER_STREAM(ShadowMap_Material);
	typedef Pointer0<ShadowMap_Material> ShadowMap_MaterialPtr;

}

#endif
