// AppPlayJNI.hpp

#ifndef APPPLAYJNI_HPP
#define APPPLAYJNI_HPP

#include <jni.h>
#include <string>

namespace appplay
{

	struct JNIMethodInfo
	{
		JNIEnv *env;
		jclass classID;
		jmethodID methodID;
	};

	class JNIHelper
	{
	public:
		static void SetJavaVM (JavaVM *vm);
		static JavaVM *GetJavaVM ();

		static jclass GetClassID (const char *className, JNIEnv *env=0);
		static bool GetStaticMethodInfo (JNIMethodInfo &methodinfo,
			const char *className, const char *methodName, 
			const char *paramCode);
		static bool GetMethodInfo (JNIMethodInfo &methodinfo,
			const char *className, const char *methodName, 
			const char *paramCode);
		static std::string JString2string (jstring str);

	private:
		static JavaVM *msJavaVM;
	};

}

extern "C"
{
	extern const char* GetPackageNameJNI();

	extern void SetKeyboardStateJNI(int open);
	extern void OpenKeyboardJNI();
	extern void CloseKeyboardJNI();
	extern void SetScreenOrientate(int ori);
	
	extern void PlatformSDKLoginJNI();
	extern void PlatformSDKLogoutJNI();
	extern void PlatformSDKSynPayJNI (
		const char *productID, const char *productName,
		float productPrice, float productOriginPrice, int count,
		const char *payDescription);
	extern void PlatformSDKASynPayJNI (
		const char *productID, const char *productName,
		float productPrice, float productOriginPrice, int count,
		const char *payDescription);
		
	extern void BluetoothSetWriteServiceID(const char *strID);
	extern void BluetoothSetWriteCharaID(const char *strID);
	extern void BluetoothSetReadServiceID(const char *strID);
	extern void BluetoothSetReadCharaID(const char *strID);
	extern void BluetoothSetBle(bool isBle);
	extern void BluetoothSetDataProtocolHex(bool isBle);
	extern bool BluetoothIsAvailable();
	extern bool BluetoothIsConnected();
	extern std::string BluetoothGetPairedDevices();
	extern void BluetoothDisConnect();
	extern void BluetoothConnect(const char *addr);
	extern void BluetoothSend(const char *buf, int length, bool withRead);
	extern void BluetoothDoDiscovery();
	extern void BluetoothCancelDiscovery();
	
	extern void WifiConnect(const char *addr, const char *password);
	extern void WifiDisConnect();
	extern void WifiDoDiscovery();
	extern void WifiCancelDiscovery();
	
	extern void VoiceStartVoiceListening();
	extern void VoiceEndVoiceListening();
	extern void VoiceEnableAutoSpeak(bool isEnable);
	extern void VoiceEnableAutoSpeakTTS(bool isEnable);
	extern void VoiceSpeak(const char *text);

	extern void CameraOpenStream(int type); // 0 back, 1 front
	extern void CameraCloseStream();

	extern void PlatformSDKStartAccelerator();
	extern void PlatformSDKRegistAccelerator();
	extern void PlatformSDKUnRegistAccelerator();
}

#endif
