// PX2RawTerrainPage.hpp

#ifndef PX2RAWTERRAINPAGE_HPP
#define PX2RAWTERRAINPAGE_HPP

#include "PX2TerrainsPre.hpp"
#include "PX2TerrainPage.hpp"

namespace PX2
{

	class EditTerrainMaterial;

	class PX2_ENGINE_ITEM RawTerrainPage : public TerrainPage
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(RawTerrainPage);

	public:
		// size = 2^p + 1, p <= 7 (size = 3, 5, 9, 17, 33, 65, 129)
		RawTerrainPage (VertexFormat* vformat, int numVertexPage, float* heights,
			const Float2& origin, float spacing);
		virtual ~RawTerrainPage ();

		/// 将顶点高度信息保存到高程数据mHeights中
		/**
		* 在编辑器中修改顶点后将顶点高度转换到高程mHeights中
		*/
		void UpdateToHighField ();
		void UpdateHoles ();

		// 贴图
		void SetTexture (int index, Texture2D *texture); // 0-4
		void SetTexture0 (Texture2D *texture);
		void SetTexture0 (const std::string &texture0Filename);
		void SetTextureAlpha (Texture2D *texture);
		void SetTexture1 (Texture2D *texture);
		void SetTexture1 (const std::string &texture1Filename);
		void SetTexture2 (Texture2D *texture);
		void SetTexture2 (const std::string &texture2Filename);
		void SetTexture3 (Texture2D *texture);
		void SetTexture3 (const std::string &texture3Filename);
		void SetTexture4 (Texture2D *texture);
		void SetTexture4 (const std::string &texture4Filename);

		Texture2D *GetTexture(int index); //0,1,2,3,4
		Texture2D *GetTexture0 ();
		Texture2D *GetTextureAlpha ();
		Texture2D *GetTexture1 ();
		Texture2D *GetTexture2 ();
		Texture2D *GetTexture3 ();
		Texture2D *GetTexture4 ();

		void SetUV (int index, Float2 uv);
		void SetUV0 (Float2 uv);
		void SetUV1 (Float2 uv);
		void SetUV2 (Float2 uv);
		void SetUV3 (Float2 uv);
		void SetUV4 (Float2 uv);
		Float2 GetUV (int index);
		Float2 GetUV0 ();
		Float2 GetUV1 ();
		Float2 GetUV2 ();
		Float2 GetUV3 ();
		Float2 GetUV4 ();

	protected:
		Texture2DPtr mTextureAlpha;

		std::string mTexture0Filename;
		std::string mTexture1Filename;
		std::string mTexture2Filename;
		std::string mTexture3Filename;
		std::string mTexture4Filename;

		Float4 mUV01;
		Float4 mUV23;
		Float4 mUV4;
	};

	PX2_REGISTER_STREAM(RawTerrainPage);
	typedef Pointer0<RawTerrainPage> RawTerrainPagePtr;
#include "PX2RawTerrainPage.inl"

}

#endif