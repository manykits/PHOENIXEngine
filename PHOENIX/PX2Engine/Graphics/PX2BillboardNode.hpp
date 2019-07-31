// PX2BillboardNode.hpp

#ifndef PX2BILLBOARDNODE_HPP
#define PX2BILLBOARDNODE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Node.hpp"
#include "PX2Camera.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM BillboardNode : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(BillboardNode);

	public:
		BillboardNode (Camera* camera = 0);
		virtual ~BillboardNode ();

		inline void AlignTo (Camera* camera);

		enum BillboardAlignType
		{
			BAT_DEFAULT,
			BAT_Z,
			BAT_MAX_TYPE
		};
		void SetAlignType (BillboardAlignType type);
		BillboardAlignType GetAlignType () const;

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

		CameraPtr mCamera;
		BillboardAlignType mAlignType;
	};

	PX2_REGISTER_STREAM(BillboardNode);
	typedef Pointer0<BillboardNode> BillboardNodePtr;
#include "PX2BillboardNode.inl"

}

#endif