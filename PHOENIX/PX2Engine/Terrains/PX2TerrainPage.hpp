// PX2TerrainPage.hpp

#ifndef PX2TERRAINPAGE_HPP
#define PX2TERRAINPAGE_HPP

#include "PX2TerrainsPre.hpp"
#include "PX2TriMesh.hpp"
#include "PX2Jungler.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TerrainPage : public TriMesh
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(TerrainPage);

	public:
		// size = 2^p + 1, p <= 7 (size = 3, 5, 9, 17, 33, 65, 129)
		TerrainPage(int numVertexPage, float* heights, const Float2& origin,
			float spacing);
		virtual ~TerrainPage ();

		// 高程访问
		inline int GetSize () const;
		inline const float* GetHeights () const;
		inline const Float2& GetOrigin () const;
		inline float GetMinElevation () const;
		inline float GetMaxElevation () const;
		inline float GetSpacing () const;
		inline float GetWidth () const;

		void ClearHoles ();
		void AddHole (int index);
		void RemoveHole (int index);
		virtual void UpdateHoles ();

		/// 获得高度
		/**
		* 如果方位不再地形页中，返回MAX_REAL
		*/
		float GetHeight (float x, float y) const;
		int GetJunglerNum (); 
		Jungler *GetJungler (int i);

		void SetJunglerFrequency (float fre);
		void SetJunglerStrength (float strength);

	protected:
		inline float GetX (int x) const;
		inline float GetY (int y) const;
		inline float GetHeight (int index) const; //< iIndex表网格顶点索引
		virtual void OnGetVisibleSet (Culler& culler, bool noCull);
		bool IsHole (int index);

		// 高程参数及数据
		int mNumVertexPage, mNumVertexPageM1;
		float* mHeights;
		Float2 mOrigin;
		float mSpacing;
		float mInvSpacing;
		std::vector<int> mHoleIndexs;

		// Junglers
		std::vector<JunglerPtr> mJunglers;
		std::map<Texture2D*, JunglerPtr> mJunglersMap;

public_internal:
		void AddJunglers (Texture2D *tex, std::vector<JObj> objs);
		void RemoveJunglers (Texture2D *tex, APoint center, float radius, int num);
		void RemoveJunglerPoints (Jungler *jungler, APoint center, float radius, int num);
		std::vector<JObj> mAddingJObjs;
	};

	PX2_REGISTER_STREAM(TerrainPage);
	typedef Pointer0<TerrainPage> TerrainPagePtr;
#include "PX2TerrainPage.inl"

}

#endif