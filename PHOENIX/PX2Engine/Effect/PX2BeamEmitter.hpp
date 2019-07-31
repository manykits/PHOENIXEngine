// PX2BeamEmitter.hpp

#ifndef PX2BEAMEMITTER_HPP
#define PX2BEAMEMITTER_HPP

#include "PX2EffectPre.hpp"
#include "PX2Effectable.hpp"
#include "PX2BeamObject.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM BeamEmitter : public Effectable
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(BeamEmitter);

	public:
		BeamEmitter();
		~BeamEmitter();

		void SetNumMaxBeams (int num); 
		int GetNumMaxBeams () const;

		void SetEmitRate (float rate);
		float GetEmitRate () const;

		void SetEmitStartPos (const APoint &startPos);
		const APoint &GetEmitStartPos () const;
		void SetEmitEndPos (const APoint &endPos);
		const APoint &GetEmitEndPos () const;

		// 在发射器IsLocal是false时，用此可发出点是本地的
		void SetStartPosUseLocal (bool startUseLocal);
		bool IsStartPosUseLocal () const;

		void SetSimpleLineWave (bool simpleLineWave);
		bool IsSimpleLineWave () const;
		void SetSimpleLineDoAlphaDisAfterStopSpeed (float speed);
		float GetSimpleLineDoAlphaDisAfterStopSpeed () const;

		enum WaveType
		{
			WT_RANDOM,
			WT_SIN,
			WT_LINE,
			WT_MAX_TYPE
		};

		void SetWaveTypeUp (WaveType type);
		WaveType GetWaveTypeUp () const;
		void SetWaveTypeExtend (WaveType type);
		WaveType GetWaveTypeExtend () const;

		void SetNumLowFrequency (int numLow);
		int GetNumLowFrequency () const;
		void SetNumHighFrequency (int numHigh);
		int GetNumHighFrequency () const;

		void SetLowFrequencyRangeUp (const Float2 &range);
		void SetLowFrequencyRangeExtend (const Float2 &range);
		const Float2 &GetLowFrequencyRangeUp () const;
		const Float2 &GetLowFrequencyRangeExtend () const;

		void SetHighFrequencyRangeUp (const Float2 &range);
		void SetHighFrequencyRangeExtend (const Float2 &range);
		const Float2 &GetHighFrequencyRangeUp () const;
		const Float2 &GetHighFrequencyRangeExtend () const;

		virtual const std::vector<std::string> &GetAllModuleNames_EA () const;
		virtual const std::vector<std::string> &GetAllModuleNames_EO () const;

		static std::vector<std::string> msModuleNames_EA;
		static std::vector<std::string> msModuleNames_EO;

	public_internal:
		virtual void GenBuffers ();

	protected:
		void UpdateMaxNumVertexs ();
		void FillBeamVert (VertexBufferAccessor &vba, unsigned short *ib,
			int startVB, const BeamObject &beamObj);

		int mNumMaxBeams;
		float mEmitRate;

		APoint mEmitStartPos;
		APoint mEmitEndPos;
		bool mIsStartUseLocal;

		bool mIsSimpleLineWave;
		float mDoAlphaDisAfterStopSpeed;

		WaveType mWaveTypeUp;
		WaveType mWaveTypeExtend;

		int mNumLowFrequency; // 为Low的线段,顶点数量为(mNumLowFrequency+1)
		int mNumHighFrequency; // 为Low顶点之间的线段数,中间的顶点数量为(mNumHighFrequency-1)

		Float2 mLowRangeUp;
		Float2 mLowRangeExtend;
		Float2 mHighRangeUp;
		Float2 mHighRangeExtend;
	};

#include "PX2BeamEmitter.inl"
	PX2_REGISTER_STREAM(BeamEmitter);
	typedef Pointer0<BeamEmitter> BeamEmitterPtr;

}

#endif