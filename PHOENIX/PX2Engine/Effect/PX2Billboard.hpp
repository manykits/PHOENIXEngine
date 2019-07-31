// PX2Billboard.hpp

#ifndef PX2BILLBOARD_HPP
#define PX2BILLBOARD_HPP

#include "PX2EffectPre.hpp"
#include "PX2Effectable.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Billboard : public Effectable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Billboard);

	public:
		Billboard ();
		virtual ~Billboard ();

		void SetDynamic (bool isDynamic);
		bool IsDynamic () const;

		void SetPivot (float anchX, float anchZ);
		void SetPivot (const Float2 &anchor);
		const Float2 &GetPivot () const;

		void SetUseTexAsSize (bool use);
		bool IsUseTexAsSize () const;

		void SetUseTrim (bool useTrim);
		bool IsUseTrim () const;

		void SetDoAlphaDisAfterStop (bool doAlphaDis);
		bool IsDoAlphaDisAfterStop () const;
		void SetDoAlphaDisAfterStopSpeed (float speed);
		float GetDoAlphaDisAfterStopSpeed () const;

		void SetSizeImmediate (float sizeX, float sizeY, float sizeZ);

		virtual const std::vector<std::string> &GetAllModuleNames_EA () const;
		virtual const std::vector<std::string> &GetAllModuleNames_EO () const;

		static std::vector<std::string> msModuleNames_EA;
		static std::vector<std::string> msModuleNames_EO;

public_internal:
		virtual void GenBuffers ();

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime1);

		bool mIsDynamic;
		Float2 mPivotPoint;
		bool mIsUseTexAsSize;
		bool mIsDoAlphaDisAfterStop;
		float mDoAlphaDisAfterStopSpeed;
		bool mIsUseTrim;
	};

#include "PX2Billboard.inl"
	PX2_REGISTER_STREAM(Billboard);
	typedef Pointer0<Billboard> BillboardPtr;

}

#endif