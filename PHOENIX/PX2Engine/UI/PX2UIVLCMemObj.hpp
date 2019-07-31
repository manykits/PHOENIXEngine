// PX2UIVLCMemObj.hpp

#ifndef PX2UIVLCMEMOBJ_HPP
#define PX2UIVLCMEMOBJ_HPP

#include "PX2UIFrame.hpp"
#include "PX2VLC.hpp"
#include "PX2VLCMem.hpp"
#include "PX2ScopedCS.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM VLCMemObj : public VLCMem
	{
	public:
		VLCMemObj();
		virtual ~VLCMemObj();

		void SetMediaWidthHeight(int width, int height);
		int GetMediaWidth() const;
		int GetMediaHeight() const;
		const std::vector<char> &GetLastBuffer();

		void ShowPic(bool isShow);

		virtual void OnFormatSetup();
		virtual void OnFrameReady(const std::vector<char>* frameBuf);
		virtual void OnFrameReady(int width, int height,
			const char* buf, int size);
		virtual void OnFrameCleanUp();

		void SetTextureUpdated(bool updated);
		bool IsTextureUpdated() const;
		Texture2D *GetTex2D();

	protected:
		bool _CheckUpdateTime();

		int mFPS;
		int mLastReadyTime;

		Texture2DPtr mTex2D;
		Mutex mMutex;
		bool mIsTextureUpdated;

		std::vector<char> mLastFrameBuf;

		bool mIsShowPic;
	};
	typedef Pointer0<VLCMemObj> VLCMemObjPtr;

}

#endif