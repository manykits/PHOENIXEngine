// org_appplay_platformsdk_PlatformSDKNatives.cpp

#include "org_appplay_platformsdk_PlatformSDKNatives.h"
#include "AppPlay.hpp"
#include "AppPlayJNI.hpp"
using namespace appplay;

#ifdef __cplusplus
extern "C"
{
#endif

JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_SetPlatformSDK
  (JNIEnv *, jclass, jstring)
  {
	  
  }

JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_OnLoginSuc
  (JNIEnv *, jclass, jint, jstring, jstring, jstring)
  {
	  
  }

JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_OnLoginCancel
  (JNIEnv *, jclass)
  {
	  
  }

  
JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_OnLoginFailed
  (JNIEnv *, jclass)
  {
	  
  }

  
JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_OnGuestOfficialSuc
  (JNIEnv *, jclass)
  {
	  
  }

  
JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_OnPaySuc
  (JNIEnv *, jclass, jstring, jboolean)
  {
	  
  }


JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_OnPayCancel
  (JNIEnv *, jclass, jstring, jboolean)
  {
	  
  }


JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_OnPayFailed
  (JNIEnv *, jclass, jstring, jboolean)
  {
	  
  }


JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_OnPaySMSSent
  (JNIEnv *, jclass, jstring, jboolean)
  {
	  
  }


JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_OnPayRequestSubmitted
  (JNIEnv *, jclass, jstring, jboolean)
  {
	  
  }


JNIEXPORT void JNICALL Java_org_appplay_platformsdk_PlatformSDKNatives_OnPayError
  (JNIEnv *, jclass, jstring, jint, jboolean)
  {
	  
  }
  
#ifdef __cplusplus
}
#endif