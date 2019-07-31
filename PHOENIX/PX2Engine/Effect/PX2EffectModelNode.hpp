// PX2EffectModelNode.hpp

#ifndef PX2EFFECTMODELNODE_HPP
#define PX2EFFECTMODELNODE_HPP

#include "PX2EffectPre.hpp"
#include "PX2TriMesh.hpp"
#include "PX2EffectModelNodeController.hpp"
#include "PX2Shader.hpp"
#include "PX2Node.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM EffectModelNode : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(EffectModelNode);

	public:
		EffectModelNode ();
		virtual ~EffectModelNode ();

		void SetModelFilename (const std::string &filename);
		const std::string &GetModelFilename () const;

		enum CoordinateType
		{
			CT_CLAMP,
			CT_REPEAT,
			CT_MAX_TYPE
		};
		void SetCoordinateType0 (CoordinateType type);
		CoordinateType GetCoordinateType0 () const;
		void SetCoordinateType1 (CoordinateType type);
		CoordinateType GetCoordinateType1 () const;

		void SetUVSpeed (const Float2 &uv);
		const Float2 &GetUVSpeed () const;

		void SetTextureFilename (const std::string &filename);
		const std::string &GetTextureFilename () const;

		virtual void Reset ();

public_internal:
		void UpdateModelNode (float applicationTime, float elapsedTime);

	protected:
		void CollectMeshs (Movable *movToCollect);

		EffectModelNodeControllerPtr mEMNCtrl;

		class EMNObj
		{
		public:
			Float3 InitColor;
			float InitAlpha;
			Float2 InitUV;
		};

		bool mResetFilename;
		std::string mModelFilename;

		MovablePtr mMovable;
		std::vector<TriMesh*> mMeshs;
		std::map<TriMesh*, std::vector<EMNObj> > mInitEMNObjs;		
		
		Float2 mUVSpeed;
		Float2 mUpdatedUV;

		std::string mTextureFileName;
		CoordinateType mCT0;
		CoordinateType mCT1;
	};

	PX2_REGISTER_STREAM(EffectModelNode);
	typedef Pointer0<EffectModelNode> EffectModelNodePtr;
#include "PX2EffectModelNode.inl"

}

#endif