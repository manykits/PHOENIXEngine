// PX2HardCamera.cpp

#include "PX2HardCamera.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2Log.hpp"

#if defined(__ANDROID__)
#include "AppPlayJNI.hpp"
#endif

using namespace PX2;

//----------------------------------------------------------------------------
HardCamera::HardCamera() :
mIsCameraOpened(false),
mObject(0)
{
}
//----------------------------------------------------------------------------
HardCamera::~HardCamera()
{
}
//----------------------------------------------------------------------------
bool HardCamera::IsCameraOpened() const
{
	return mIsCameraOpened;
}
//----------------------------------------------------------------------------
void HardCamera::OpenCamera(int type)
{
	PX2_UNUSED(type);

#if defined(__ANDROID__)
	CameraOpenStream(type);
#endif

	mIsCameraOpened = true;
}
//----------------------------------------------------------------------------
void HardCamera::CloseCamera()
{
#if defined(__ANDROID__)
	CameraCloseStream();
#endif

	mIsCameraOpened = false;
}
//----------------------------------------------------------------------------
void HardCamera::OnSetCameraFrame(int width, int height, const char *data,
	int size)
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		HardCameraCallback callback = mCallbacks[i];
		if (callback)
			callback(width, height, data, size);
	}
}
//----------------------------------------------------------------------------
bool HardCamera::AddHardCameraCallback(HardCameraCallback callback)
{
	if (IsHasHardCameraCallback(callback))
		return false;

	mCallbacks.push_back(callback);

	return false;
}
//----------------------------------------------------------------------------
bool HardCamera::IsHasHardCameraCallback(HardCameraCallback callback) const
{
	for (int i = 0; i < (int)mCallbacks.size(); i++)
	{
		if (callback == mCallbacks[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void HardCamera::ClearHardCameraCallbacks()
{
	mCallbacks.clear();
}
//----------------------------------------------------------------------------
void HardCamera::SetObj(void *object)
{
	mObject = object;
}
//----------------------------------------------------------------------------
void *HardCamera::GetObj()
{
	return mObject;
}
//----------------------------------------------------------------------------
