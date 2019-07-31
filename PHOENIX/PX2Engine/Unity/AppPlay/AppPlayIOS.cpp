// AppPlayIOS.mm

#include "AppPlayIOS.hpp"

#if defined (__IOS__)

#import <UIKit/UIAlertView.h>

@interface InputFinish : NSObject <UIAlertViewDelegate>
@end
//----------------------------------------------------------------------------

static UIAlertView *input = nil;
void AppPlay::SetKeyboardStateIOS (int open)
{
        if (open>0) 
		{
            AppPlay::OpenKeyboardIOS();
        }
		else 
		{
            AppPlay::CloseKeyboardIOS();
        }
}

    void AppPlay::OpenKeyboardIOS()
	{
        if (input == nil)
        {
            input = [[UIAlertView alloc] initWithTitle:nil message:nil delegate:[[InputFinish alloc] init] cancelButtonTitle:@"Cancel" otherButtonTitles:@"Ok", nil];
            input.alertViewStyle = UIAlertViewStylePlainTextInput;
        }
    }

    void AppPlay::CloseKeyboardIOS()
	{

    }
    

    @implementation InputFinish
    
    -(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
    {
        if (buttonIndex != 0)
        {
            UITextField *textfield = [alertView textFieldAtIndex:0];
            std::string text = [textfield.text UTF8String];
        }
    }
    
    @end
     
	
	void AppPlay::SetScreenOrientate(int ori)
	{

	}
	
	void AppPlay::PlatformSDKLoginIOS()
	{

	}

	void AppPlay::PlatformSDKLogoutIOS()
	{

	}

	void AppPlay::PlatformSDKSynPayIOS (
		const char *productID, const char *productName,
		float productPrice, float productOriginPrice, int count,
		const char *payDescription)
	{

	}

	void AppPlay::PlatformSDKASynPayIOS (
		const char *productID, const char *productName,
		float productPrice, float productOriginPrice, int count,
		const char *payDescription)
	{

	}

	void AppPlay::BluetoothSetWriteServiceID(const char *strID)
	{

	}

	void AppPlay::BluetoothSetWriteCharaID(const char *strID)
	{

	}

	void AppPlay::BluetoothSetReadServiceID(const char *strID)
	{

	}

	void AppPlay::BluetoothSetReadCharaID(const char *strID)
	{

	}
	
	void AppPlay::BluetoothSetBle(bool isBle)
	{

	}

	void AppPlay::BluetoothSetDataProtocolHex(bool isBle)
	{

	}

	bool AppPlay::BluetoothIsAvailable()
	{


		return false;
	}

	bool AppPlay::BluetoothIsConnected()
	{


		return false;
	}

	static std::string pairedDevicesStr = "";
	const char *AppPlay::BluetoothGetPairedDevices()
	{
		return pairedDevicesStr.c_str();
	}

	void AppPlay::BluetoothDisConnect()
	{

	}

	void AppPlay::BluetoothConnect(const char *addr)
	{

	}

	void AppPlay::BluetoothSend(const char *buf, int length, bool withRead)
	{

	}

	void AppPlay::BluetoothDoDiscovery()
	{

	}

	void AppPlay::BluetoothCancelDiscovery()
	{

	}
	
	void AppPlay::WifiConnect(const char *addr, const char *password)
	{

	}
	
	void AppPlay::WifiDisConnect()
	{

	}
	
	void AppPlay::WifiDoDiscovery()
	{

	}
	
	void AppPlay::WifiCancelDiscovery()
	{

	}
	
	void AppPlay::VoiceStartVoiceListening()
	{

	}
	
	void AppPlay::VoiceEndVoiceListening()
	{

	}

	void AppPlay::VoiceEnableAutoSpeak(bool isEnable)
	{

	}

	void AppPlay::VoiceSpeak(const char *text)
	{

	}
	
	void AppPlay::CameraOpenStream(int type)
	{

	}
	
    void AppPlay::CameraCloseStream()
	{

	}
}

#endif
//----------------------------------------------------------------------------
