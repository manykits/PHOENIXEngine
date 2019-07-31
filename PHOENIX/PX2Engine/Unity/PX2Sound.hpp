// PX2Sound.hpp

#ifndef PX2SOUND_HPP
#define PX2SOUND_HPP

#include "PX2UnityPre.hpp"
#include "PX2APoint.hpp"
#include "PX2AVector.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Sound
	{
	public:
		Sound();
		virtual ~Sound();

		// 操作
		virtual bool IsPlaying() = 0;
		virtual void SetVolume(float volume) = 0;
		virtual void SetPaused(bool paused) = 0;
		virtual void Stop() = 0;
		void MinusNumPlaySameTime();

		// 方位
		/*
		* 以下方法仅对3dSound有效
		*/
		virtual void SetPosition(const APoint &position) = 0;
		virtual void SetVelocity(const AVector &velocity) = 0;
		virtual void SetDistance(float minDistance, float maxDistance) = 0;

		std::string Filename;
		
		virtual bool Update(float elapsedSeconds);
		float Life;
		float Age;
	};
	typedef Pointer0<Sound> SoundPtr;

}

#endif