// PX2Effectable.hpp

#ifndef PX2EFFECTABLE_HPP
#define PX2EFFECTABLE_HPP

#include "PX2EffectPre.hpp"
#include "PX2TriMesh.hpp"
#include "PX2EffectableController.hpp"
#include "PX2TexPackData.hpp"
#include "PX2DynamicBufferManager.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Effectable : public TriMesh
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Effectable);

	public:
		virtual ~Effectable ();

		EffectableController *GetEffectableController () const;

		void SetFixedBound (bool fixed);
		bool IsFixedBound ();
		void SetFixedBoundCenter (const APoint &center);
		APoint GetFixedBoundCenter ();
		void SetFixedBoundRadius (float radius);
		float GetFixedBoundRadius ();

		void SetLocal (bool l);
		bool IsLocal () const;

		void SetEmitSizeX (float val);
		float GetEmitSizeX () const;
		void SetEmitSizeY (float val);
		float GetEmitSizeY () const;
		void SetEmitSizeZ (float val);
		float GetEmitSizeZ () const;
		void SetEmitSizeXYZ (float val);

		void SetEmitColor (const Float3 &color);
		const Float3 &GetEmitColor () const;

		void SetEmitAlpha (float alpha);
		float GetEmitAlpha () const;

		void SetEmitLife (float life);
		float GetEmitLife () const;

		enum FaceType
		{
			FT_X,
			FT_NX,
			FT_Y,
			FT_NY,
			FT_Z,
			FT_NZ,
			FT_CAMERA,
			FT_SPEEDDIR,
			FT_FREE,
			FT_CAMERA_ZUP, // for camera
			FT_CAMERA_ALIGN_EMITDIR, // 粒子发射器
			FT_MAX_TYPE
		};
		void SetFaceType (FaceType type);
		FaceType GetFaceType () const;

		enum MaterialType
		{
			MT_TEX,				// 1uv
			MT_TEX_MASK_MULTIPLY,	// 2uv
			MT_TEX_MASK_ADD,	// 2uv
			MT_MAX_TYPE
		};
		void SetMaterialType (MaterialType type);
		MaterialType GetMaterialType () const;

		void SetBackCull (bool isBackCull);
		bool IsBackCull () const;

		enum BlendMode
		{
			BM_NONE,
			BM_ALPHATEST85,
			BM_BLEND_SCRCALPHA_DSTONEMIUSSRCALPHA,
			BM_BLEND_SRCALPHA_DSTONE,
			BM_BLEND_SRCCOLOR_DSTONEMIUSSRCCOLOR,
			BM_BLEND_SRCCOLOR_DSTONE,
			BM_BLEND_SRCONE_DSTONE,
			BM_BLEND_SRCONE_DSTONEMIUSSRCALPHA,
			BM_BLEND_SRCONE_DSTONEMIUSSRCCOLOR,
			BM_MAX_MODE
		};
		void SetBlendMode (BlendMode mode);
		BlendMode GetBlendMode () const;

		// 纹理以及动画
		enum TexMode
		{
			TM_TEX,
			TM_TEXPACK_ELE,
			TM_TEX_ANIM,
			TM_TEXS_ANIM,
			TM_TEXPACK_ANIM,
			TM_MAX_MODE
		};
		void SetTexMode (TexMode mode);
		TexMode GetTexMode () const;

		// tex
		bool SetTex (const std::string &filename);
		const std::string &GetTex () const;

		// tex pack ele
		bool SetTexPack_Ele (const std::string &packFileName, const std::string &eleName);
		void GetTexPack_Ele (std::string &packFileName, std::string &eleName) const;
		
		// tex anim
		bool SetTexAnim (const std::string &filename);
		const std::string &GetTexAnim () const;
		void SetTexAnim_Repeat (const Float2 &repeat);
		const Float2 &GetTexAnim_Repeat () const; 

		// texs anim
		bool AddTexsAnim_Frame (const std::string &filename);

		// texpack anim
		bool SetTexPackAnim (const std::string &texPackFilename);
		const std::string &GetTexPackAnim () const;
		bool AddTexPackAnim_Frame (const std::string &texPackFilename,
			const std::string &eleName);

		int GetNumTexAnimTexPackAnimFrames () const;
		void ClearTexAnimTexPackAnimFrames ();

		// animFrames common
		void SetAnimInterval (float seconds);
		float GetAnimInterval () const;
		void SetAnimStartDoRandom (bool doRandom);
		bool IsAnimStartDoRandom () const;
		void SetUserNumAnimFrames (int userNumAnimFrames);
		int GetUserNumAnimFrames () const;
		int GetNumAnimFrames () const;
		void SetAnimFramesPlayOnce (bool playOnce);
		bool IsAnimFramesPlayOnce () const;

		// mask
		void SetMaskTex (const std::string &filename);
		const std::string &GetMaskTex () const;

		// uv
		void SetEmitUV0_Offset (const Float2 &offset);
		const Float2 &GetEmitUV0_Offset () const;
		void SetEmitUV0_Speed (const Float2 &speed);
		const Float2 &GetEmitUV0_Speed () const;
		void SetEmitUV0_Repeat (const Float2 &repeat);
		const Float2 &GetEmitUV0_Repeat () const;

		void SetUV1_Offset (const Float2 &offset);
		const Float2 &GetUV1_Offset () const;
		void SetUV1_Repeat (const Float2 &repeat);
		const Float2 &GetUV1_Repeat () const;
		void SetUV1_Speed (const Float2 &speed);
		const Float2 &GetUV1_Speed () const;

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

		// 获得特效所能拥有的控制器
		virtual const std::vector<std::string> &GetAllModuleNames_EA () const = 0;
		virtual const std::vector<std::string> &GetAllModuleNames_EO () const = 0;

public_internal:
		int GetAnimStartRandomIndex () const;
		virtual void GenBuffers ();
		void SetBufferEverGenerated (bool gen);
		bool IsBufferEverGenerated () const;
		const TexPackElement &GetTexPackElement () const;
		const std::vector<TexPackElement> &GetTexPackAnim_Frames () const;

		virtual void UpdateWorldData(double applicationTime, double elapsedTime);

	protected:
		Effectable ();
		int GetUV (int startRandomIndex, float age, float &uBegin,
			float &uEnd, float &vBegin, float &vEnd);
		void _SetBlendMode(BlendMode mode);
		
		bool mIsNeedUpdate;

		bool mIsFixedBound;
		APoint mFixedCenter;
		float mFixedBoundRadius;
		bool mIsLocal;
		float mEmitSizeX;
		float mEmitSizeY;
		float mEmitSizeZ;
		Float3 mEmitColor;
		float mEmitAlpha;
		float mEmitLife;
		FaceType mFaceType;
		MaterialType mMtlType;
		BlendMode mBlendMode;

		TexMode mTexMode;		
		std::string mTexFilename;		// TM_TEX TM_TEX_ANIM TM_TEXPACK_ELE TM_TEXPACK_ANIM
		std::string mTexPackEleName;
		TexPackElement mTexPackEle;
		Float2 mTexAnim_Repeat;
		float mAnimInterval;
		bool mIsAnimStartDoRandom;
		int mUserNumAnimFrames;
		std::vector<TexPackElement> mTexPackAnim_Frames;
		std::string mTexMaskFilename;
		Float2 mEmitUV0_Offset;
		Float2 mEmitUV0_Repeat;
		Float2 mEmitUV0_Speed;
		Float2 mUV1_Offset;
		Float2 mUV1_Repeat;
		Float2 mUV1_Speed;

		EffectableControllerPtr mEffectableCtrl;

		bool mIsAnimFramesPlayOnce;
		CoordinateType mCoordinateType0;
		CoordinateType mCoordinateType1;

		bool mIsBackCull;

		bool mIsBufferEverGenerated;
	};

	PX2_REGISTER_STREAM(Effectable);
	typedef Pointer0<Effectable> EffectablePtr;
#include "PX2Effectable.inl"

}

#endif