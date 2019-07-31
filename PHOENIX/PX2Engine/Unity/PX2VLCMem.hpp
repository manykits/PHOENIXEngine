// PX2VLCMen.hpp

#ifndef PX2VLCMEN_HPP
#define PX2VLCMEN_HPP

#include "PX2VLCPre.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	enum
	{
		DEF_PIXEL_BYTES = 4,
		original_media_width = 0,
		original_media_height = 0
	};

	class PX2_ENGINE_ITEM VLCMem
	{
	public:
		VLCMem();
		virtual ~VLCMem();

		void SetDesireWidth(int dWidth);
		void SetDesireHeight(int dHeight);

		virtual void OnFormatSetup();
		virtual void OnFrameReady(const std::vector<char>* frameBuf) = 0;
		virtual void OnFrameReady(int width, int height, 
			const char* buf, int size) = 0;
		virtual void OnFrameCleanUp() = 0;

	public_internal:
		static unsigned _FormatCallback(void **opaque, char *chroma,
			unsigned *width, unsigned *height,
			unsigned *pitches, unsigned *lines);
		static void _CleanUpCallback(void *opaque);

		static void* _LockCallback(void *opaque, void **planes);
		static void _UnLockCallback(void *opaque, void *picture, void *const *planes);
		static void _DisplayCallback(void *opaque, void *picture);

	protected:
		unsigned FormatCallback(char *chroma,
			unsigned *width, unsigned *height,
			unsigned *pitches, unsigned *lines);
		void CleanUpCallback();

		void* LockCallback(void **planes);
		void UnLockCallback(void *picture, void *const *planes);
		void DisplayCallback(void *picture);

		std::vector<char> mFrameBuf;
		int mDesireWidth;
		int mDesireHeight;
		unsigned mMediaWidth;
		unsigned mMediaHeight;
	};
	typedef Pointer0<VLCMem> VLCMemPtr;

}

#endif