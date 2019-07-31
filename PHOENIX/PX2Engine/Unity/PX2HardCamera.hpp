// PX2HardCamera.hpp

#ifndef PX2HARDCAMERA_HPP
#define PX2HARDCAMERA_HPP

#include "PX2Movable.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	typedef void(*HardCameraCallback) (int width, int height, const char* buf,
		int size);

	class PX2_ENGINE_ITEM HardCamera : public Singleton<HardCamera>
	{
	public:
		HardCamera();
		virtual ~HardCamera();

		// 0 back, 1front
		void OpenCamera(int type);
		bool IsCameraOpened() const;
		void CloseCamera();
		void OnSetCameraFrame(int width, int height, const char *data, 
			int size);

		bool AddHardCameraCallback(HardCameraCallback callback);
		bool IsHasHardCameraCallback(HardCameraCallback callback) const;
		void ClearHardCameraCallbacks();

		void SetObj(void *object);
		void *GetObj();

	protected:
		bool mIsCameraOpened;
		std::vector<HardCameraCallback> mCallbacks;
		void *mObject;
	};

#define PX2_HARDCAMERA HardCamera::GetSingleton()

}

#endif