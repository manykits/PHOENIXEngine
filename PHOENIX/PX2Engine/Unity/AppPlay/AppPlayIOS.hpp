// AppPlayIOS.hpp

#ifndef APPPLAYIOS_HPP
#define APPPLAYIOS_HPP

#include "PX2CorePre.hpp"

#if defined (__IOS__)

class AppPlay
{
public:
    static void Init();
    
	static void SetKeyboardStateIOS(int open, const char *openstr);
	static void OpenKeyboardIOS(const char *openstr);
	static void CloseKeyboardIOS();
	static void SetScreenOrientate(int ori);
	
	static void PlatformSDKLoginIOS();
	static void PlatformSDKLogoutIOS();
	static void PlatformSDKSynPayIOS (
		const char *productID, const char *productName,
		float productPrice, float productOriginPrice, int count,
		const char *payDescription);
	static void PlatformSDKASynPayIOS (
		const char *productID, const char *productName,
		float productPrice, float productOriginPrice, int count,
		const char *payDescription);
		
	static void BluetoothSetWriteServiceID(const char *strID);
	static void BluetoothSetWriteCharaID(const char *strID);
	static void BluetoothSetReadServiceID(const char *strID);
	static void BluetoothSetReadCharaID(const char *strID);
	static void BluetoothSetBle(bool isBle);
	static void BluetoothSetDataProtocolHex(bool hex);
	static bool BluetoothIsAvailable();
    static void BluetoothSetConnected(bool isConnected);
	static bool BluetoothIsConnected();
	static const char *BluetoothGetPairedDevices();
	static void BluetoothDisConnect();
	static void BluetoothConnect(const char *addr);
	static void BluetoothSend(const char *buf, int length, bool withRead);
	static void BluetoothDoDiscovery();
	static void BluetoothCancelDiscovery();
	
	static void WifiConnect(const char *addr, const char *password);
	static void WifiDisConnect();
    static bool WifiIsConnected();
	static void WifiDoDiscovery();
	static void WifiCancelDiscovery();
	
	static void VoiceStartVoiceListening();
	static void VoiceEndVoiceListening();
	static void VoiceEnableAutoSpeak(bool isEnable);
	static void VoiceSpeak(const char *text);

	static void CameraOpenStream(int type); // 0 back, 1 front
	static void CameraCloseStream();
    
    static void *peripheralDataArray;
    static void *babyPtr;
    static void *writeCharacteristic;
    static bool IsConnected;
};

#endif

#endif
