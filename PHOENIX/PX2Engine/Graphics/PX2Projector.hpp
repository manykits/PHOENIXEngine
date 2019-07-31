// PX2Projector.hpp

#ifndef PX2PROJECTOR_HPP
#define PX2PROJECTOR_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Camera.hpp"

namespace PX2
{
	
	/// 投影器类
	/**
	* 这个类和Camera唯一区别是，你可以设置特定的深度类型。
	*/
	class PX2_ENGINE_ITEM Projector : public Camera
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Projector);

	public:
		Projector (bool isPerspective = true);
		virtual ~Projector ();

		// The 0-index matrix maps x' = (1+x)/2 and y' = (1-y)/2.  The
		// 1-index matrix maps x' = (1+x)/2 and y' = (1+y)/2.
		static const HMatrix BiasScaleMatrix[2];
	};

	PX2_REGISTER_STREAM(Projector);
	typedef Pointer0<Projector> ProjectorPtr;

}

#endif