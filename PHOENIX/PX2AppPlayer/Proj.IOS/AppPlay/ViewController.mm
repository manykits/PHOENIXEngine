// ViewController.mm

#import "ViewController.h"
#import "BabyBluetooth.h"
#include "AppPlayIOS.hpp"
#include "AppPlay.hpp"
#include "PX2Bluetooth.hpp"
#import "CTPSock.h"
#import "DFTransfer.h"
#import "DFLive.h"
#import "DFRtspMedia.h"
#import "DVUISetting.h"
#import "DFTime.h"

@interface ViewController ()
{
    // bluetooth
    BabyBluetooth *baby;
    NSMutableArray *peripheralDataArray;
    CBPeripheral *currPeripheral;
    
    // Vedio
    DFRtspMedia *rtspMedia;
    QUALITY rtsp_ql;
}
@property (strong, nonatomic) EAGLContext *context;

- (void)setupGL;
- (void)tearDownGL;

@end

ViewController *pxMainViewController = nil;

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    pxMainViewController = self;
    
    self.view.multipleTouchEnabled = TRUE;
    
    // 初始化BabyBluetooth 蓝牙库
    baby = [BabyBluetooth shareBabyBluetooth];
    [self babyDelegate];
    peripheralDataArray = [[NSMutableArray alloc]init];
    
    void *babyPtr = (__bridge void*)baby;
    void *peripheralDataArrayPtr = (__bridge void*)peripheralDataArray;
    
    AppPlay::babyPtr = babyPtr;
    AppPlay::peripheralDataArray = peripheralDataArrayPtr;
    
    // phoniex
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    
    [self setupGL];
    
    NSString *resPath = [[NSBundle mainBundle] resourcePath];
    std::string strResPath = [resPath UTF8String];
    strResPath.append("/");
    appplay::NativeCall::SetResourcePath(strResPath);
    
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    appplay::NativeCall::SetWriteablePath(strRet);

    CGSize size = [UIScreen mainScreen].applicationFrame.size;
    CGFloat scale = [UIScreen mainScreen].scale;
    appplay::NativeCall::Initlize(size.width * scale, size.height * scale);
    
    // vedio
    [CTPSock connectHost:@"192.168.1.1" Port:2222];
    rtsp_ql = QUALITY_HIGH;
    
    float sW = [[DVUISetting defaultMe] sW_Width];
    float sH = [[DVUISetting defaultMe] sH_Height];
    
    rtspMedia = [DFRtspMedia alloc];
    [rtspMedia initwithWidth:sW Height:sH];
    
    /*--- 接受以下通知 ---*/
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ctpConnected:) name:@"CTP_CONNECTED" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ctpLinkInvalid:) name:@"CTP_LINK_INVALID" object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ctpOverTime:) name:@"CTP_OVERTIME" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ctpBroken:) name:@"CTP_BROKEN" object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ctpError:) name:@"CTP_ERR" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(ctpResponds:) name:@"CTP_NOTE" object:nil];

    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(replayMedia:) name:@"UI_REPLAY" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceOrientationChanged:) name:UIDeviceOrientationDidChangeNotification object:nil];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(background:) name:@"APP_BACKGROUND" object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(foreground:) name:@"APP_FOREGROUND" object:nil];
}

//订阅一个值
-(void)setNotifiyChara:(CBCharacteristic*) chara
{
    if (chara.properties & CBCharacteristicPropertyNotify ||  chara.properties & CBCharacteristicPropertyIndicate)
    {
        [currPeripheral setNotifyValue:YES forCharacteristic:chara];
        
        [baby notify:currPeripheral characteristic:chara
               block:^(CBPeripheral *peripheral, CBCharacteristic *characteristics, NSError *error)
         {
             NSLog(@"new value %@",chara.value);
             
             NSData *valStr = chara.value;
             int length = [valStr length];
             char *buf = (char*)[valStr bytes];
             
             std::string strName(buf, length);
             PX2::PX2_BLUETOOTH.OnReceive(strName);
         }];
    }
    else
    {
        // 没有权限
        return;
    }
}

//----------------------------------------------------------------------------
//蓝牙网关初始化和委托方法设置
-(void)babyDelegate{
    
    __weak typeof(self) weakSelf = self;
    [baby setBlockOnCentralManagerDidUpdateState:^(CBCentralManager *central) {
        if (central.state == CBCentralManagerStatePoweredOn)
        {
            // [SVProgressHUD showInfoWithStatus:@"设备打开成功，开始扫描设备"];
            // 打开设备成功
            
        }
    }];
    
    //设置扫描到设备的委托
    [baby setBlockOnDiscoverToPeripherals:^(CBCentralManager *central, CBPeripheral *peripheral, NSDictionary *advertisementData, NSNumber *rSSI)
     {
         // 搜索到设备
         std::string deviceStr;
         
         std::string strName = [peripheral.name UTF8String];
         deviceStr += strName;
         
         deviceStr += "$";
         
         NSString* uuIDNameNS = peripheral.identifier.UUIDString;
         deviceStr += [uuIDNameNS UTF8String];
         
         NSArray *peripherals = [peripheralDataArray valueForKey:@"peripheral"];
         if(![peripherals containsObject:peripheral])
         {
             NSMutableArray *indexPaths = [[NSMutableArray alloc] init];
             NSIndexPath *indexPath = [NSIndexPath indexPathForRow:peripherals.count inSection:0];
             [indexPaths addObject:indexPath];
             
             NSMutableDictionary *item = [[NSMutableDictionary alloc] init];
             [item setValue:uuIDNameNS forKey:@"uuid"];
             [item setValue:peripheral forKey:@"peripheral"];
             [item setValue:rSSI forKey:@"RSSI"];
             [item setValue:advertisementData forKey:@"advertisementData"];
             
             [peripheralDataArray addObject:item];
             
             PX2::PX2_BLUETOOTH.OnDisocveryNewDevice(deviceStr);
         }
         
         //NSLog(@"搜索到了设备:%@",peripheral.name);
         //[weakSelf insertTableView:peripheral advertisementData:advertisementData RSSI:RSSI];
     }];
    
    //设置查找设备的过滤器
    [baby setFilterOnDiscoverPeripherals:^BOOL(NSString *peripheralName, NSDictionary *advertisementData, NSNumber *RSSI)
     {
         //最常用的场景是查找某一个前缀开头的设备
         //        if ([peripheralName hasPrefix:@"Pxxxx"] ) {
         //            return YES;
         //        }
         //        return NO;
         
         //设置查找规则是名称大于0 ， the search rule is peripheral.name length > 0
         if (peripheralName.length >0)
         {
             return YES;
         }
         
         return NO;
     }];
    
    #define channelPX @"phoenix"
    [baby setBlockOnConnectedAtChannel:channelPX block:^(CBCentralManager *central, CBPeripheral *peripheral)
    {
        [baby cancelScan];
        
        currPeripheral = peripheral;
        AppPlay::BluetoothSetConnected(true);
        PX2::PX2_BLUETOOTH.OnConnected();
    }];
    
    //设置设备连接失败的委托
    [baby setBlockOnFailToConnectAtChannel:channelPX block:^(CBCentralManager *central, CBPeripheral *peripheral, NSError *error) {
        currPeripheral = nil;
        PX2::PX2_BLUETOOTH.OnConnectFailed();
        AppPlay::BluetoothSetConnected(false);
    }];
    
    //设置设备断开连接的委托
    [baby setBlockOnDisconnectAtChannel:channelPX block:^(CBCentralManager *central, CBPeripheral *peripheral, NSError *error)
     {
         currPeripheral = nil;
         PX2::PX2_BLUETOOTH.OnDisConnected();
         AppPlay::BluetoothSetConnected(false);
    }];
    
    
    //设置发现设备的Services的委托
    [baby setBlockOnDiscoverServicesAtChannel:channelPX block:^(CBPeripheral *peripheral, NSError *error)
     {
    }];
    
    //设置发现设service的Characteristics的委托
    [baby setBlockOnDiscoverCharacteristicsAtChannel:channelPX block:^(CBPeripheral *peripheral, CBService *service, NSError *error)
     {
         NSLog(@"===service name:%@",service.UUID);
         
         std::string writeServiceID = PX2::PX2_BLUETOOTH.GetWriteServiceID();
         std::string writeCharaID = PX2::PX2_BLUETOOTH.GetWriteCharaID();
         std::string readServiceID = PX2::PX2_BLUETOOTH.GetReadServiceID();
         std::string readCharaID = PX2::PX2_BLUETOOTH.GetReadServiceID();
         
         NSString *uuidStrNS = service.UUID.UUIDString;
         std::string uuidStr = [uuidStrNS UTF8String];
         
         if ([service.UUID isEqual:[CBUUID UUIDWithString: [NSString stringWithUTF8String:writeServiceID.c_str()]]])
         {
             for (CBCharacteristic *c in service.characteristics)
             {
                 NSString *uuidStrNSCharas = service.UUID.UUIDString;
                 NSLog(@"charateristic name is :%@", c.UUID.UUIDString);
                 
                 if ([c.UUID isEqual:[CBUUID UUIDWithString: [NSString stringWithUTF8String:writeCharaID.c_str()]]])
                 {
                     AppPlay::writeCharacteristic = (__bridge void*)c;
                 }
             }
         }
         
         if ([service.UUID isEqual:[CBUUID UUIDWithString: [NSString stringWithUTF8String:readServiceID.c_str()]]])
         {
             for (CBCharacteristic *c in service.characteristics)
             {
                 NSLog(@"chara uuID name is :%@",c.UUID);
                 
                // [self setNotifiyChara:c];
                 CBUUID *uuIDTarget = [CBUUID UUIDWithString: [NSString stringWithUTF8String:readCharaID.c_str()]];
                 NSLog(@"target uuID name is :%@", uuIDTarget);
                 
                // if ([c.UUID isEqual:uuIDTarget])
                 {
                    [self setNotifiyChara:c];
                 }
             }
         }
    }];
    //设置读取characteristics的委托
    [baby setBlockOnReadValueForCharacteristicAtChannel:channelPX block:^(CBPeripheral *peripheral, CBCharacteristic *characteristics, NSError *error)
     {
         NSLog(@"characteristic name:%@ value is:%@",characteristics.UUID,characteristics.value);
    }];
    
    //设置发现characteristics的descriptors的委托
    [baby setBlockOnDiscoverDescriptorsForCharacteristicAtChannel:channelPX block:^(CBPeripheral *peripheral, CBCharacteristic *characteristic, NSError *error)
    {
        NSLog(@"===characteristic name:%@",characteristic.service.UUID);
        for (CBDescriptor *d in characteristic.descriptors) {
            NSLog(@"CBDescriptor name is :%@",d.UUID);
        }
    }];
    
    //设置读取Descriptor的委托
    [baby setBlockOnReadValueForDescriptorsAtChannel:channelPX block:^(CBPeripheral *peripheral, CBDescriptor *descriptor, NSError *error)
     {
        NSLog(@"Descriptor name:%@ value is:%@",descriptor.characteristic.UUID, descriptor.value);
    }];
    
    //读取rssi的委托
    [baby setBlockOnDidReadRSSI:^(NSNumber *RSSI, NSError *error)
     {
        NSLog(@"setBlockOnDidReadRSSI:RSSI:%@",RSSI);
    }];

    //扫描选项->CBCentralManagerScanOptionAllowDuplicatesKey:忽略同一个Peripheral端的多个发现事件被聚合成一个发现事件
    NSDictionary *scanForPeripheralsWithOptions = @{CBCentralManagerScanOptionAllowDuplicatesKey:@YES};
    /*连接选项->
     CBConnectPeripheralOptionNotifyOnConnectionKey :当应用挂起时，如果有一个连接成功时，如果我们想要系统为指定的peripheral显示一个提示时，就使用这个key值。
     CBConnectPeripheralOptionNotifyOnDisconnectionKey :当应用挂起时，如果连接断开时，如果我们想要系统为指定的peripheral显示一个断开连接的提示时，就使用这个key值。
     CBConnectPeripheralOptionNotifyOnNotificationKey:
     当应用挂起时，使用该key值表示只要接收到给定peripheral端的通知就显示一个提
     */
    NSDictionary *connectOptions = @{CBConnectPeripheralOptionNotifyOnConnectionKey:@YES,
                                     CBConnectPeripheralOptionNotifyOnDisconnectionKey:@YES,
                                     CBConnectPeripheralOptionNotifyOnNotificationKey:@YES};
    
    [baby setBabyOptionsAtChannel:channelPX scanForPeripheralsWithOptions:scanForPeripheralsWithOptions connectPeripheralWithOptions:connectOptions scanForPeripheralsWithServices:nil discoverWithServices:nil discoverWithCharacteristics:nil];
}

-(void)viewDidAppear:(BOOL)animated{
    NSLog(@"viewDidAppear");
}

-(void)viewWillDisappear:(BOOL)animated{
    NSLog(@"viewWillDisappear");
}

- (void)dealloc
{    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    // Dispose of any resources that can be recreated.
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
}

- (void)update
{
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    appplay::NativeCall::OnOdle();
}


#pragma mark -->> CTP__Note
-(void)ctpConnected:(NSNotification*)note{

    // 连接上，开始播放
    [rtspMedia play:(QUALITY_HIGH)];
}

-(void)ctpLinkInvalid:(NSNotification*)note{
    
    if ([[DVUISetting defaultMe] UIIndex] != 2) {
        NSString *tmpTitle = NSLocalizedString(@"CONNECT_FIRST_TIP", nil);
        NSString *tmpMessage = NSLocalizedString(@"CONNECT_OPERATION", nil);
        NSString *tmpCancel = NSLocalizedString(@"OK", nil);
        UIAlertView * alert = [[UIAlertView alloc] initWithTitle:tmpTitle message:tmpMessage delegate:self cancelButtonTitle:tmpCancel otherButtonTitles:nil, nil];
        [alert show];
    }
}


-(void)ctpOverTime:(NSNotification*)note{
    [rtspMedia stop];
    UIAlertView * alertview = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"TEMP_TIPS", nil)
                                                         message:NSLocalizedString(@"CONNECT_OVERTIMG", nil)
                                                        delegate:self
                                               cancelButtonTitle:NSLocalizedString(@"NO_CONNECT", nil)
                                               otherButtonTitles:NSLocalizedString(@"RESET_CONNECT", nil), nil];
    alertview.tag = 555;
    [alertview show];
}


-(void)ctpBroken:(NSNotification*)note{
    /*--- 开启网络提示 ---*/
    //disconnectTips = [[DFDisconnectTips alloc] init];
    //[disconnectTips show];
    
}

-(void)ctpError:(NSNotification*)note{
    NSString * str = [note object];
    NSLog(@"CTP错误:%@",str);
}

-(void)ctpResponds:(NSNotification*)note{
    NSDictionary * dict = [note object];
    NSNumber * cmd = dict[@"CMD"];
    NSString * parm = dict[@"PARM"];
    int index = [parm intValue];
    switch ([cmd intValue])
    {
        case 29:{
            NSLog(@"CTP Enable:%@",parm);
            NSArray * parms = [parm componentsSeparatedByString:@" "];
            if ([parms[0] isEqual:@"-1"]) {
                /*--- 设备已被占用 ---*/
                NSString * tmpTitle = NSLocalizedString(@"HOLD_FACILITY", nil);
                NSString *tmpCancel = NSLocalizedString(@"OK", nil);
                NSString *tmpOther = NSLocalizedString(@"TRY_AGAIN", nil);
                UIAlertView * alert = [[UIAlertView alloc] initWithTitle:tmpTitle message:nil delegate:self cancelButtonTitle:tmpCancel otherButtonTitles:tmpOther, nil];
                alert.tag = 110;
                [alert show];
                
            }else if ([parms[0] isEqual:@"-2"]){
                /*--- 设备处于USB模式 ---*/
                UIAlertView * alert = [[UIAlertView alloc] initWithTitle:nil message:NSLocalizedString(@"DEVICE_TIPS", nil) delegate:self cancelButtonTitle:NSLocalizedString(@"OK", nil) otherButtonTitles:nil, nil];
                [alert show];
                
            }else{
                if ([[DVUISetting defaultMe] UIIndex] == 1) {
                    /*--- 允许开启RTSP ---*/
                    [DFTime delaySec:1.0 perform:^()
                    {
                        [rtspMedia play:rtsp_ql];
                    }];
                }
                
                
            }
        }break;
            
        case 30:{
            NSLog(@"设备关闭了WiFi信号.");
            /*--- 关闭RTSP流 ---*/
            [rtspMedia stop];
            
            UIAlertView * alert = [[UIAlertView alloc] initWithTitle:nil message:NSLocalizedString(@"DEVICE_CLOSE_WIFI", nil) delegate:self cancelButtonTitle:NSLocalizedString(@"OK", nil) otherButtonTitles:nil, nil];
            [alert show];
        }break;
    }
}

#pragma mark -->> 从其他界面返回，开启播放
-(void)replayMedia:(NSNotification*)note{
    int n_int = [[note object] intValue];
    
    /*--- 重置横竖屏UI ---*/
    [self resetOrientationForUI];
    [[DVUISetting defaultMe] setUIIndex:1];
    
    
    if (n_int == 1) {
        /*--- 从FTP界面退出,退出成功后开启RTSP ---*/
        [CTPSock FtpExit];
    }
    
    if (n_int == 2)
    {
        if (![CTPSock ctpIsLinked]) {
            //重连设备（之后会自动重启RTSP）
            [CTPSock connectHost:@"192.168.1.1" Port:2222];
        }else{
            /*---重启RTSP ---*/
            [rtspMedia play:rtsp_ql];
        }
    }
}


#pragma mark -->> /*--- 横屏重置屏幕size ---*/
-(void)deviceOrientationChanged:(NSNotification*)note{
    
    /*--- RTSP是否在播放 ---*/
    BOOL isPlay = [rtspMedia isPlaying];
    int uiIndex = [[DVUISetting defaultMe] UIIndex];

    [self resetOrientationForUI];
}

-(void)resetOrientationForUI{
    
    //取得當前Device的方向，來當作判斷敘述。
    float sW = [[DVUISetting defaultMe] sW_Width];
    UIDevice *device = [UIDevice currentDevice];
    switch (device.orientation) {
        case UIDeviceOrientationFaceUp:{
            //NSLog(@"螢幕朝上平躺");
        
        }break;
            
        case UIDeviceOrientationFaceDown:{
            
            //NSLog(@"螢幕朝下平躺");
        }break;
            
            //系統無法判斷目前Device的方向，有可能是斜置
        case UIDeviceOrientationUnknown:{
            //NSLog(@"未知方向");
            
        }break;
            
        case UIDeviceOrientationLandscapeLeft:{
            //NSLog(@"螢幕向左橫置");
            
        }break;
        case UIDeviceOrientationLandscapeRight:{
            //NSLog(@"螢幕向右橫置");
        
        }break;
        case UIDeviceOrientationPortrait:{
            //NSLog(@"螢幕直立");

        }break;
        case UIDeviceOrientationPortraitUpsideDown:{
            //NSLog(@"螢幕直立，上下顛倒");
            
        }break;
    }
}

#pragma mark -->> 活跃（前台）
-(void)foreground:(NSNotification*)note{
 
    [DFTime delaySec:1.0 perform:^(){
        if ([[DVUISetting defaultMe] UIIndex] == 1) {
            [CTPSock connectHost:@"192.168.1.1" Port:2222];
        }
    }];
    
}

#pragma mark -->> 不活跃（后台）
-(void)background:(NSNotification*)note{
    /*--- 关闭FTP下载 ---*/
    [rtspMedia stop];
    [CTPSock cutOffLink];
    
    /*--- 取消倒数UI ---*/
}

#pragma mark ModifyViewDelegate
-(void)confirmSSID:(NSString *)ssid Psw:(NSString *)psw{
    [CTPSock ApSSID:ssid];
    [CTPSock ApPassword:psw?:@"0"];
}

@end
