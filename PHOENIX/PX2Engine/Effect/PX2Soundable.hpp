// PX2Soundable.hpp

#ifndef PX2SOUNDABLE_HPP
#define PX2SOUNDABLE_HPP

#include "PX2EffectPre.hpp"
#include "PX2SoundSystem.hpp"
#include "PX2Effectable.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Soundable : public Effectable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Soundable);

	public:
		Soundable (const std::string &filename="Data/engine/ling.ogg");
		virtual ~Soundable ();

		enum SourceType
		{
			ST_SOUND,
			ST_MUSIC,
			ST_MAX_TYPE
		};
		void SetSourceType (SourceType st);
		SourceType GetSourceType () const;
		void SetMusicChannel (int channel);
		int GetMusicChannel () const;

		virtual void Reset ();
		virtual void Play ();
		void PlaySound ();
		virtual void Pause ();

		void SetIngoreStopSound (bool stop);
		bool IsIngoreStopSound () const;

		void SetSoundFilename (const std::string &filename);
		const std::string &GetSoundFilename () const;

		void Set3D (bool is3D);
		bool Is3D () const;
		void SetVolume (float volume);
		float GetVolume () const;
		void SetVelocity (const AVector &velocity);
		const AVector &GetVelocity () const;
		void SetDistance (float minDistance, float maxDistance);
		void SetMinDistance (float minDistance);
		void SetMaxDistance (float maxDistance);
		float GetMinDistance () const;
		float GetMaxDistance () const;
		void SetLoop (bool isLoop);
		bool IsLoop () const;

		void SetDistanceUseX (bool use);
		bool IsDistanceUseX () const;
		void SetDistanceUseY (bool use);
		bool IsDistanceUseY () const;
		void SetDistanceUseZ (bool use);
		bool IsDistanceUseZ () const;

		virtual const std::vector<std::string> &GetAllModuleNames_EA () const;
		virtual const std::vector<std::string> &GetAllModuleNames_EO () const;

		static std::vector<std::string> msModuleNames_EA;
		static std::vector<std::string> msModuleNames_EO;

public_internal:
		virtual void GenBuffers ();

	protected:
		virtual void UpdateWorldData (double applicationTime, double elapsedTime);
		virtual void OnGetVisibleSet (Culler& culler, bool);

		SourceType mSourceType;
		int mMusicChannel;
		bool mIngoreStopSound;
		std::string mFilename;
		bool mIsLoop;
		bool mIs3D;
		float mVolume;
		float mMinDistance; 
		float mMaxDistance;
		AVector mVelocity;

		bool mDistanceUseX;
		bool mDistanceUseY;
		bool mDistanceUseZ;

		SoundPtr mSound;
	};

	PX2_REGISTER_STREAM(Soundable);
	typedef Pointer0<Soundable> SoundablePtr;
#include "PX2Soundable.inl"

}

#endif