// PX2FramesMesh.hpp

#ifndef PX2FRAMESMESH_HPP
#define PX2FRAMESMESH_HPP

#include "PX2TriMesh.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM FramesMesh : public TriMesh
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(FramesMesh);

	public:
		FramesMesh();
		virtual ~FramesMesh();

		virtual void Play();
		virtual bool IsPlaying() const;
		virtual void Pause();

		void SetSize(float size);

		int SetTexPack(const std::string &texPackFilename);

		void SetNumDir(int numFrames);
		int GetNumDir() const;

		void SetPlayOnce(bool once);
		void SetPlayOnceTime(float playOnceTime);

		void SetWorldRunDir(const AVector &vec);

	protected:
		virtual void UpdateWorldData(double applicationTime, double elapsedTime);
		void _Cal(const std::string &texPackFilename);
		int _GetDirIndex(const AVector &dir);
		void _CalFrame();

		bool mIsNeedRecal;

		std::string mTexPackFilename;
		int mNumDir;
		int mNumAllFrames;
		int mNumFramesPerDir;
		float mSize;
		float mPlayOnceTime;
		bool mIsPlayOnce;

		bool mIsPlaying;
		float mAnimPlayedTime;
		AVector mCurDir;
	};

#include "PX2FramesMesh.inl"
	PX2_REGISTER_STREAM(FramesMesh);
	typedef Pointer0<FramesMesh> FramesMeshPtr;

}

#endif