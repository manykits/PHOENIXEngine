// AppPlayIOS.mm

#include "AppPlayIOS.hpp"
#include "PX2IMEDispatcher.hpp"

#if defined (__IOS__)
#import <UIKit/UIAlertView.h>
#import "BabyBluetooth.h"

//----------------------------------------------------------------------------
@interface InputFinishDelegate : NSObject <UIAlertViewDelegate>
{
}
@end
@implementation InputFinishDelegate
-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (buttonIndex != 0)
    {
        UITextField *textfield = [alertView textFieldAtIndex:0];
        std::string text = [textfield.text UTF8String];
        
        PX2::IMEDispatcher::GetSingleton().DispathInsertText(text.c_str(), text.length());
    }
}
@end
//----------------------------------------------------------------------------
void *AppPlay::peripheralDataArray = 0;
void *AppPlay::babyPtr = 0;
void *AppPlay::writeCharacteristic = 0;
bool AppPlay::IsConnected = false;
//----------------------------------------------------------------------------
void AppPlay::Init()
{    
}
//----------------------------------------------------------------------------
static InputFinishDelegate *delegateFinish = [[InputFinishDelegate alloc]init];
static UIAlertView *input = nil;
void AppPlay::SetKeyboardStateIOS (int open, const char *openstr)
{
    if (open>0)
    {
        AppPlay::OpenKeyboardIOS(openstr);
    }
    else
    {
        AppPlay::CloseKeyboardIOS();
    }
}
//----------------------------------------------------------------------------
void AppPlay::OpenKeyboardIOS(const char *str)
{
    if (input == nil)
    {
        input = [[UIAlertView alloc] initWithTitle:@" " message:nil delegate:delegateFinish cancelButtonTitle:@"取消" otherButtonTitles:@"确定", nil];
        input.alertViewStyle = UIAlertViewStylePlainTextInput;
    }
        
    UITextField *textfield = [input textFieldAtIndex:0];
    textfield.text = [NSString stringWithUTF8String:str];
    [input show];
}
//----------------------------------------------------------------------------
void AppPlay::CloseKeyboardIOS()
{
    [input setHidden:true];
}
//----------------------------------------------------------------------------
void AppPlay::SetScreenOrientate(int ori)
{
    UIDeviceOrientation deviceOrientation = [UIDevice currentDevice].orientation;
}
//----------------------------------------------------------------------------
void AppPlay::PlatformSDKLoginIOS()
{
}
//----------------------------------------------------------------------------
void AppPlay::PlatformSDKLogoutIOS()
{
}
//----------------------------------------------------------------------------
void AppPlay::PlatformSDKSynPayIOS (
		const char *productID, const char *productName,
		float productPrice, float productOriginPrice, int count,
		const char *payDescription)
{

}
//----------------------------------------------------------------------------
void AppPlay::PlatformSDKASynPayIOS (
		const char *productID, const char *productName,
		float productPrice, float productOriginPrice, int count,
		const char *payDescription)
{

}
//----------------------------------------------------------------------------
void AppPlay::BluetoothSetWriteServiceID(const char *strID)
{
}
//----------------------------------------------------------------------------
void AppPlay::BluetoothSetWriteCharaID(const char *strID)
{
}
//----------------------------------------------------------------------------
void AppPlay::BluetoothSetReadServiceID(const char *strID)
{
}
//----------------------------------------------------------------------------
void AppPlay::BluetoothSetReadCharaID(const char *strID)
{
}
//----------------------------------------------------------------------------
void AppPlay::BluetoothSetBle(bool isBle)
{
    PX2_UNUSED(isBle);
}
//----------------------------------------------------------------------------
void AppPlay::BluetoothSetDataProtocolHex(bool hex)
{
    PX2_UNUSED(hex);
}
//----------------------------------------------------------------------------
bool AppPlay::BluetoothIsAvailable()
{
    return false;
}
//----------------------------------------------------------------------------
void AppPlay::BluetoothSetConnected(bool isConnected)
{
    IsConnected = isConnected;
    
    if (!IsConnected)
    {
        writeCharacteristic = 0;
    }
}
//----------------------------------------------------------------------------
CBPeripheral *connectPeripheral = nil;
//----------------------------------------------------------------------------
bool AppPlay::BluetoothIsConnected()
{
    if (nil == connectPeripheral)
        return false;
    
    if(connectPeripheral.state != CBPeripheralStateConnected)
    {
        return false;
    }
    
    return true;
}
//----------------------------------------------------------------------------
static std::string pairedDevicesStr = "";
const char *AppPlay::BluetoothGetPairedDevices()
{
    return pairedDevicesStr.c_str();
}
//----------------------------------------------------------------------------
void AppPlay::BluetoothDisConnect()
{
    BabyBluetooth *baby = (__bridge BabyBluetooth*)babyPtr;
    NSMutableArray *mutArray = (__bridge NSMutableArray*)peripheralDataArray;
    
    if (nil != connectPeripheral)
    {
        //[baby cancelAllPeripheralsConnection];
        //[baby cancelPeripheralConnection:connectPeripheral];
        baby.having(connectPeripheral).stop(1);
        
        connectPeripheral = nil;
        writeCharacteristic = 0;
    }
}
//----------------------------------------------------------------------------
#define channelPX @"phoenix"
//----------------------------------------------------------------------------
void AppPlay::BluetoothConnect(const char *addr)
{
    BabyBluetooth *baby = (__bridge BabyBluetooth*)babyPtr;
    NSMutableArray *mutArray = (__bridge NSMutableArray*)peripheralDataArray;
    
    std::string strAddr = addr;
    NSString *deviceStrNS=[NSString stringWithUTF8String:addr];
    
    NSArray *uuidArray = [mutArray valueForKey:@"uuid"];
    NSUInteger index = [uuidArray indexOfObject :deviceStrNS];
    NSDictionary *item = [mutArray objectAtIndex:index];
    
    CBPeripheral *peripheral = [item objectForKey:@"peripheral"];
    NSDictionary *advertisementData = [item objectForKey:@"advertisementData"];
    NSNumber *RSSI = [item objectForKey:@"RSSI"];
    NSString *deviceStr = [item objectForKey:@"uuid"];
    
    connectPeripheral = peripheral;
    
    baby.having(peripheral).channel(channelPX).
    connectToPeripherals().discoverServices().discoverCharacteristics().readValueForCharacteristic().
    discoverDescriptorsForCharacteristic().readValueForDescriptors().begin();
}
//----------------------------------------------------------------------------
void AppPlay::BluetoothSend(const char *buf, int length, bool withRead)
{
    BabyBluetooth *baby = (__bridge BabyBluetooth*)babyPtr;
    NSMutableArray *mutArray = (__bridge NSMutableArray*)peripheralDataArray;
    
    if (0 != writeCharacteristic)
    {
        CBCharacteristic *charac = (__bridge CBCharacteristic*)writeCharacteristic;
        
        NSData *datas=[NSData dataWithBytes:buf length:length];
        CBCharacteristicWriteType t = CBCharacteristicWriteWithoutResponse;
        
        if (nil!=connectPeripheral && nil!=charac)
            [connectPeripheral writeValue:datas forCharacteristic:charac type:t];
    }
}
//----------------------------------------------------------------------------
void AppPlay::BluetoothDoDiscovery()
{
    BabyBluetooth *baby = (__bridge BabyBluetooth*)babyPtr;
    NSMutableArray *mutArray = (__bridge NSMutableArray*)peripheralDataArray;
    
    [mutArray removeAllObjects];

    //设置委托后直接可以使用，无需等待CBCentralManagerStatePoweredOn状态。
    baby.scanForPeripherals().begin();
    //baby.scanForPeripherals().begin().stop(10);
}
//----------------------------------------------------------------------------
void AppPlay::BluetoothCancelDiscovery()
{
    BabyBluetooth *baby = (__bridge BabyBluetooth*)babyPtr;
    NSMutableArray *mutArray = (__bridge NSMutableArray*)peripheralDataArray;
    
    [baby cancelScan];
}
//----------------------------------------------------------------------------
void AppPlay::WifiConnect(const char *addr, const char *password)
{
}
//----------------------------------------------------------------------------
void AppPlay::WifiDisConnect()
{
}
//----------------------------------------------------------------------------
void AppPlay::WifiDoDiscovery()
{
}
//----------------------------------------------------------------------------
void AppPlay::WifiCancelDiscovery()
{
}
//----------------------------------------------------------------------------
bool AppPlay::WifiIsConnected()
{
    return false;
}
//----------------------------------------------------------------------------
void AppPlay::VoiceStartVoiceListening()
{
}
//----------------------------------------------------------------------------
void AppPlay::VoiceEndVoiceListening()
{
}
//----------------------------------------------------------------------------
void AppPlay::VoiceEnableAutoSpeak(bool isEnable)
{
}
//----------------------------------------------------------------------------
void AppPlay::VoiceSpeak(const char *text)
{
}
//----------------------------------------------------------------------------
void AppPlay::CameraOpenStream(int type)
{
}
//----------------------------------------------------------------------------
void AppPlay::CameraCloseStream()
{
}
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------
