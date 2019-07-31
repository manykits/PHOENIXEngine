// PX2EffectModel.hpp

#ifndef PX2EFFECTMODEL_HPP
#define PX2EFFECTMODEL_HPP

#include "PX2EffectPre.hpp"
#include "PX2Effectable.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM EffectModel : public Effectable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(EffectModel);

	public:
		EffectModel ();
		virtual ~EffectModel ();

		virtual void Reset ();

		enum ModelType
		{
			MT_SPHERE,
			MT_CYLINDEROPEN,
			MT_MODEL,
			MT_MAX_TYPE
		};
		void SetModelType (ModelType modelType);
		ModelType GetModelType () const;

		void SetRadiusSample (int radiusSample);
		int GetRadiusSample () const;

		void SetZSample (int zSample);
		int GetZSample () const;

		enum HeightAlphaType
		{
			HAT_NORMAL,
			HAT_H2L_APPEAR,
			HAT_H2L_DISAPPEAR,
			HAT_H2L_APPEAR_DISAPPEAR,
			HAT_H2L_DISAPPEAR_APPEAR,
			HAT_MAX_TYPE
		};
		void SetHeightAlphaType (HeightAlphaType type);
		HeightAlphaType GetHeightAlphaType () const;

		void SetModelFilename (const std::string &filename);
		const std::string &GetModelFilename () const;

		virtual const std::vector<std::string> &GetAllModuleNames_EA () const;
		virtual const std::vector<std::string> &GetAllModuleNames_EO () const;

		static std::vector<std::string> msModuleNames_EA;
		static std::vector<std::string> msModuleNames_EO;

public_internal:
		virtual void GenBuffers ();

	protected:
		void GenMesh ();

		ModelType mModelType;
		int mRadiusSample;
		int mZSample;
		HeightAlphaType mHeightAlphaType;

		std::vector<Float2> mInitUVs;
		bool mIsNeedReGenMesh;

		std::string mModelFilename;
	};

	PX2_REGISTER_STREAM(EffectModel);
#include "PX2EffectModel.inl"
	typedef Pointer0<EffectModel> EffectModelPtr;

}

#endif