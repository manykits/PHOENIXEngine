// PX2Jungler.hpp

#ifndef PX2JUNGLER_HPP
#define PX2JUNGLER_HPP

#include "PX2TerrainsPre.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2Texture2D.hpp"
#include "PX2MaterialInstance.hpp"
#include "PX2TriMesh.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM JObj
	{
	public:
		JObj () 
		{
			Width = 1.0f;
			Height = 1.0f;
		}
		~JObj () {}

		APoint Pos;
		AVector Normal;
		float Width;
		float Height;
	};

	class PX2_ENGINE_ITEM Jungler : public TriMesh
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Jungler);

	public:
		enum JunglerType
		{
			JT_PLANE,
			JT_CROSS,
			JT_MAX_TYPE
		};
		Jungler (Texture2D *tex, int maxNum=500, JunglerType type=JT_PLANE);
		virtual ~Jungler ();

		MaterialInstance *GetMtlInst ();
		Texture2D *GetTexture ();

		void Add (std::vector<JObj> objs);
		void Remove (std::vector<int> indexs);
		int GetNum ();
		const APoint &GetPos (int i);

		void SetJunglerFrequency (float fre);
		void SetJunglerStrength (float strength);

	protected:
		Jungler ();
		void ReGenerate ();

		static int msJunVertxNum[Jungler::JT_MAX_TYPE];
		static int msJunIndexNum[Jungler::JT_MAX_TYPE];

		JunglerType mJunglerType;
		int mMaxNum;
		MaterialInstancePtr mMtlInst;
		Texture2DPtr mTexture;
		std::vector<APoint> mPoses;
		std::vector<AVector> mNormals;
		std::vector<float> mWidths;
		std::vector<float> mHeights;
	};

	PX2_REGISTER_STREAM(Jungler);
	typedef Pointer0<Jungler> JunglerPtr;
#include "PX2Jungler.inl"

}

#endif