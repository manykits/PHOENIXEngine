// PX2Terrain.hpp

#ifndef PX2TERRAIN_HPP
#define PX2TERRAIN_HPP

#include "PX2TerrainsPre.hpp"
#include "PX2Node.hpp"
#include "PX2Camera.hpp"
#include "PX2TerrainPage.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Terrain : public Node
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Terrain);

	public:
		virtual ~Terrain ();

		virtual void OnChildDetach(Movable* child);

		// members
		inline int GetRowQuantity () const;
		inline int GetColQuantity () const;
		inline int GetNumVertexPage () const;
		inline float GetSpacing () const;
		float GetHeight (float x, float y);
		AVector GetNormal (float x, float y);

		TerrainPage* GetPage (int row, int col);
		TerrainPage* GetCurrentPage (float x, float y);
		bool GetPageIndex (int &outRow, int &outCol, TerrainPage *page);
		TerrainPagePtr ReplacePage (int row, int col, TerrainPage* newPage);

		void UpdateHoles ();

		Shine *GetTerrainShine ();

		// 在radius，随机生成num个植被
		void AddJunglers (Texture2D *tex, APoint center, float radius, int num, 
			float width, float height, float lower);
		void RemoveJunglers (Texture2D *tex, APoint center, float radius, int num);

		void SetJunglerFrequency (float fre);
		float GetJunglerFrequency ();
		void SetJunglerStrength (float strength);
		float GetJunglerStrength ();

	protected:
		Terrain();

		int mRowFrom, mRowTo;
		int mColFrom, mColTo;
		int mNumVertexPage;
		float mMinElevation, mMaxElevation, mSpacing;
		std::map<std::pair<int, int>, TerrainPagePtr> mPages;
		VertexFormatPtr mVFormatEdit;

		ShinePtr mTerrainShine;
		float mJunglerFrequency;
		float mJunglerStrength;
	};

	PX2_REGISTER_STREAM(Terrain);
	typedef Pointer0<Terrain> TerrainPtr;
#include "PX2Terrain.inl"

}

#endif