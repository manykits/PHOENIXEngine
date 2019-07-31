// PX2CameraShake.hpp

#ifndef PX2CAMERASHAKE_HPP
#define PX2CAMERASHAKE_HPP

#include "PX2EffectPre.hpp"
#include "PX2Effectable.hpp"

namespace PX2
{

	struct PX2_ENGINE_ITEM CameraShakeChannel
	{
		CameraShakeChannel ();
		~CameraShakeChannel ();

		bool UseX;
		bool UseY;
		bool UseZ;
		APoint Center;
		float Strength;
		float MaxDistance;
	};

	class PX2_ENGINE_ITEM CameraShake : public Effectable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(CameraShake);

	public:
		CameraShake ();
		virtual ~CameraShake ();

		virtual void Play ();
		virtual void Pause ();

		void SetUseX (bool use);
		bool IsUseX () const;
		void SetUseY (bool use);
		bool IsUseY () const;
		void SetUseZ (bool use);
		bool IsUseZ () const;

		void SetMaxDistance (float dist);
		float GetMaxDistance () const;

		void SetEmitStrength (float strength);
		float GetEmitStrength () const;

		int GetShakeChannel () const;

		virtual const std::vector<std::string> &GetAllModuleNames_EA () const;
		virtual const std::vector<std::string> &GetAllModuleNames_EO () const;

		static int AllocChannel (float maxDist, bool useX, bool useY, bool useZ);
		static void FreeChannel (int channel);
		static void SetChannelShake (int channel, float strength, 
			const APoint &shakeObjPos);
		static float ShakeCamera (const APoint &recvCenter);

		enum { NUMSHAKEMAXCHANNELS=16 };
		static CameraShakeChannel ShakeChannels[NUMSHAKEMAXCHANNELS];

		static std::vector<std::string> msModuleNames_EA;
		static std::vector<std::string> msModuleNames_EO;

public_internal:
		virtual void GenBuffers ();
	
	protected:
		virtual void OnGetVisibleSet (Culler& culler, bool);

		bool mIsUseX;
		bool mIsUseY;
		bool mIsUseZ;
		float mMaxDistance;
		float mEmitStrength;
		int mShakeChannel;
	};

#include "PX2CameraShake.inl"
	PX2_REGISTER_STREAM(CameraShake);
	typedef Pointer0<CameraShake> CameraShakePtr;

}

#endif