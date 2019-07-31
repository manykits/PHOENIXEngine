// PX2ProjectorWorldPositionConstant.hpp

#ifndef PX2PROJECTORWORLDPOSITIONCONSTANT_HPP
#define PX2PROJECTORWORLDPOSITIONCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Projector.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ProjectorWorldPositionConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ProjectorWorldPositionConstant);

	public:
		ProjectorWorldPositionConstant (Projector* projector);
		virtual ~ProjectorWorldPositionConstant ();

		Projector* GetProjector ();

		virtual void Update(const ShaderStruct *struc);

	protected:
		ProjectorPtr mProjector;
	};

	PX2_REGISTER_STREAM(ProjectorWorldPositionConstant);
	typedef Pointer0<ProjectorWorldPositionConstant>
		ProjectorWorldPositionConstantPtr;

}

#endif