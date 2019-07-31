// PX2DlodNode.hpp

#ifndef PX2DLODNODE_HPP
#define PX2DLODNODE_HPP

#include "PX2SwitchNode.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM DlodNode : public SwitchNode
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(DlodNode);

	public:
		DlodNode (int numLevelsOfDetail);
		virtual ~DlodNode ();

		// Level of detail(LOD)∑√Œ 
		inline APoint& ModelCenter ();
		inline const APoint& GetModelCenter () const;
		inline const APoint& GetWorldCenter () const;

		// æ‡¿Î∑√Œ 
		inline int GetNumLevelsOfDetail () const;
		inline float GetModelMinDistance (int i) const;
		inline float GetModelMaxDistance (int i) const;
		inline float GetWorldMinDistance (int i) const;
		inline float GetWorldMaxDistance (int i) const;
		void SetModelDistance (int i, float minDist, float maxDist);

	protected:
		void SelectLevelOfDetail (const Camera* camera);

		virtual void OnGetVisibleSet (Culler& culler, bool noCull);

		APoint mModelLodCenter;
		APoint mWorldLodCenter;

		int mNumLevelsOfDetail;
		float* mModelMinDist;
		float* mModelMaxDist;
		float* mWorldMinDist;
		float* mWorldMaxDist;
	};

	PX2_REGISTER_STREAM(DlodNode);
	typedef Pointer0<DlodNode> DlodNodePtr;
#include "PX2DlodNode.inl"

}

#endif