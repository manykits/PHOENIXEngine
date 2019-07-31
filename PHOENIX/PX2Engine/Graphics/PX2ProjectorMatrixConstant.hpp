// PX2ProjectorMatrixConstant.hpp

#ifndef PX2PROJECTORMATRIXCONSTANT_HPP
#define PX2PROJECTORMATRIXCONSTANT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Projector.hpp"
#include "PX2ShaderFloat.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ProjectorMatrixConstant : public ShaderFloat
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ProjectorMatrixConstant);

	public:
		// Construction and destruction.  Set bsMatrix to 0 for the
		// bias-scale matrix that maps y' = (1-y)/2.  Set bsMatrix to 1 for the
		// bias-scale matrix that maps y' = (1-y)/2.
		ProjectorMatrixConstant (Projector* projector, bool biased);
		virtual ~ProjectorMatrixConstant ();

		Projector* GetProjector ();

		virtual void Update(const ShaderStruct *struc);

	protected:
		ProjectorMatrixConstant () {};

		ProjectorPtr mProjector;
		bool mBiased;
	};

	PX2_REGISTER_STREAM(ProjectorMatrixConstant);
	typedef Pointer0<ProjectorMatrixConstant> ProjectorMatrixConstantPtr;

}

#endif